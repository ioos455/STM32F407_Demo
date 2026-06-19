#ifndef __QMA6100P_H__
#define __QMA6100P_H__

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

typedef struct Qma6100p_axis
{
	short x_data ;
	short y_data ;
	short z_data ;
}Qma6100p_axis_t;

extern Qma6100p_axis_t imu_val;			//加速度值

void QMA6100P_Init(void);
uint8_t QMA6100P_Read_ID(void);
void QMA_Read_Axis(Qma6100p_axis_t *axis);

#endif

