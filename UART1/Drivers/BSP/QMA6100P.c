#include "i2c.h"
#include "QMA6100P.h"


#define QMA6100P_ADDR     						0x12				//QMA6100P配置为原理图情况的地址	
#define QMA6100P_L_DATA_L     				0x01        //QMA数据寄存器地址
#define QMA6100P_L_DATA_H     				0x02        //QMA数据寄存器地址
#define QMA6100P_M_DATA_L     				0x03        //QMA数据寄存器地址
#define QMA6100P_M_DATA_H     				0x04        //QMA数据寄存器地址
#define QMA6100P_H_DATA_L     				0x05        //QMA数据寄存器地址
#define QMA6100P_H_DATA_H     				0x06        //QMA数据寄存器地址

#define QMA6100P_RANGE_REGISTER     	0x0F        //QMA量程滤波器寄存器地址
#define QMA6100P_OUTPUT_DATA_RATE     0x10        //QMA输出速率和滤波器极性寄存器地址
#define QMA6100P_PM_REGISTER 					0x11				//是否唤醒等
#define QMA6100P_ID										0x00        //ID

Qma6100p_axis_t imu_val = {0};
uint8_t buf[6]= {0}; // 一次性读取6个字节（0x01~0x06）

/* QMA6100P初始化，输出寄存器配置，使得QMA6100P输出数据,手册的初始化流程 */
void QMA6100P_Init(void)
{
    uint8_t data = 0;
    uint8_t status = 0;

    // 1. 触发软件复位
    data = 0xB6;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x36,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    HAL_Delay(1); // 手册要求延时1ms

    // 2. 停止软件复位
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x36,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // 3. 等待OTP加载完成（读0x33直到bit0和bit2为1）
    //【重点修改：加重试次数，杜绝死循环】
    uint8_t retry = 0;
    do {
        if(HAL_I2C_Mem_Read(&hi2c1, (QMA6100P_ADDR << 1), 0x33,I2C_MEMADD_SIZE_8BIT, &status, 1, 50) != HAL_OK)
        {
            //I2C读失败直接跳出，防止总线卡死反复读
            retry++;
            if(retry >=15) break;
            continue;
        }
        retry++;
        if(retry >=15) break;
    } while ((status & 0x05) != 0x05);

    // 4. 设置唤醒模式（进入Active）
    data = 0x80;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x11,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // 5. 设置主时钟51KHz
    data = 0x84;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x11,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // 6. 配置ANA寄存器
    data = 0x20;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x4A,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    data = 0x01;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x56,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    data = 0x80;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x5F,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    HAL_Delay(1); // 手册要求延时1ms
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x5F,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // 7. 配置量程±2g（0x0F=0x01）
    data = 0x01;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x0F,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    // 8. 配置输出速率100Hz（0x10=0x00）
    data = 0x00;
    HAL_I2C_Mem_Write(&hi2c1, (QMA6100P_ADDR << 1), 0x10,
                      I2C_MEMADD_SIZE_8BIT, &data, 1, 100);

    HAL_Delay(100); // 等待传感器稳定
}




/**
 * @brief 简易兼容freertos，防止死等卡死
 * @param 无
 * @retval ID or OXFF
*/
uint8_t QMA6100P_Read_ID(void)
{
	uint8_t ID;
	if(HAL_I2C_Mem_Read(&hi2c1, (QMA6100P_ADDR << 1), QMA6100P_ID, 
										I2C_MEMADD_SIZE_8BIT,&ID, 1, 30) == HAL_OK)
	{
		return ID;
	}
	else
	{
		return 0XFF;
	}
}

/************ 合成16位数据（低字节在前，高字节在后）**************/

void QMA_Read_Axis(Qma6100p_axis_t *axis)
{
    if(HAL_I2C_Mem_Read(&hi2c1,(QMA6100P_ADDR<<1),QMA6100P_L_DATA_L,
                        I2C_MEMADD_SIZE_8BIT,buf,6,200)==HAL_OK)
    {
        axis->x_data = (short)(((buf[1]<<8)|buf[0])>>2);
        axis->y_data = (short)(((buf[3]<<8)|buf[2])>>2);
        axis->z_data = (short)(((buf[5]<<8)|buf[4])>>2);
    }
    else
    {
        axis->x_data=axis->y_data=axis->z_data=0xFF;
    }
}




