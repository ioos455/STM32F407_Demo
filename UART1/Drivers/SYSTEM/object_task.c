#include "object_task.h"
#include "lv_mainstart.h"
#include "queue.h"
#include "stdio.h"

// 定义单条日志最大长度、队列能存多少条日志
#define LOG_STR_MAX_LEN    64
#define LOG_QUEUE_LEN      8

SemaphoreHandle_t iic_mutex;					//iic互斥信号量

/****************任务句柄**************/
TaskHandle_t LigTask_Handler = NULL;
TaskHandle_t TemTask_Handler = NULL;
TaskHandle_t AxiTask_Handler = NULL;
TaskHandle_t LogTask_Handle = NULL;

/**************队列句柄**************/
QueueHandle_t log_queue = NULL;



void Send_Log(const char *log_str);

/**
 * @brief light传感器任务
 * @param NULL
 * @retval void
*/
void lig_task(void *pvParameters)
{
	char tmp_buf[32];
	while(1)
	{
		
		Lig = Lig_Get_Val();
	
		if(Lig <=5)
		{
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET);//过暗开启LED
		}else if(Lig >5)
		{
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);	//过亮关闭LED
		}


		
		sprintf(tmp_buf, "[光照] 数值：%u", Lig);				// 数值转字符串
		Send_Log(tmp_buf);  														// 送入日志队列
		vTaskDelay(210);
	}
}

/**
 * @brief temperature传感器任务
 * @param NULL
 * @retval void
*/
void Tem_task(void *pvParameters)
{
	char tmp_buf[32];
	while(1)
	{
		Tem = Tem_Get_Val();

		sprintf(tmp_buf, "[温度] 数值：%.2f", Tem);			// 数值转字符串
		Send_Log(tmp_buf);  														// 送入日志队列
		vTaskDelay(320);
	}
}

/**
 * @brief AXI传感器任务
 * @param NULL
 * @retval void
*/
void Axi_task(void *pvParameters)
{
	char tmp_buf[48];
	while(1)
	{
		BaseType_t ret = xSemaphoreTake(iic_mutex, portMAX_DELAY);		//获取互斥信号量返回值
		if(ret == pdPASS)
		{	
			QMA_Read_Axis(&imu_val);
			xSemaphoreGive(iic_mutex);
		}
		
		sprintf(tmp_buf, "[加速度] X:%d Y:%d Z:%d",
						imu_val.x_data, imu_val.y_data, imu_val.z_data);			// 数值转字符串
		Send_Log(tmp_buf);  																					// 送入日志队列
		vTaskDelay(100);
	}

}

/**
 * @brief 打印任务
 * @param void *pvParameters
 * @retval void
*/
void Log_Task(void *pvParameters)
{
    char recv_buf[LOG_STR_MAX_LEN];
    BaseType_t ret;

    while(1)
    {
        // 阻塞等待队列里有日志，没数据(读取失败)直接休眠，释放CPU
        ret = xQueueReceive(log_queue, recv_buf, portMAX_DELAY);
        if(ret == pdTRUE)
        {
            printf("%s\r\n", recv_buf);
        }
    }
}

// 把字符串送入日志队列
void Send_Log(const char *log_str)
{
    if(log_queue != NULL)
    {
        // 等待10ms，队列满了直接丢弃这条日志，不阻塞采集任务
        xQueueSend(log_queue, log_str, pdMS_TO_TICKS(10));
    }
}

/**
 * @brief		简单if实现lvgl打印任务，不放在各自的任务里面
						因为会卡死，直接在最高优先级任务进行刷新
 * @param  	TaskHandle_t task_handler 需要执行任务的句柄
 * @retval  void
*/
void Printf_Sensor(TaskHandle_t task_handler)
{
	if(task_handler == LigTask_Handler)
	{
		if(eTaskGetState(task_handler) == eSuspended)
		{
									/***改变LCD的显示值***/
			lv_label_set_text_fmt(my_gui_label.lig, "Lig:");
		}
		else lv_label_set_text_fmt(my_gui_label.lig, "Lig:%d", Lig);
	}
	else	if(task_handler == TemTask_Handler)
	{
		if(eTaskGetState(task_handler) == eSuspended)
		{
									/***改变LCD的显示值***/
			lv_label_set_text_fmt(my_gui_label.tem, "Tem:");			
			//编译器自动生成指令，把 4 字节 float 拷贝、扩展成 8 字节 double，需强转为double类型才能正常显示
		}
		else lv_label_set_text_fmt(my_gui_label.tem, "Tem:%f", (double)Tem);	
	}
		else	if(task_handler == AxiTask_Handler)
	{
		if(eTaskGetState(task_handler) == eSuspended)
		{
					/***改变LCD的显示值***/
		lv_label_set_text_fmt(my_gui_label.axi, "x: y: z:");		
		}
		else lv_label_set_text_fmt(my_gui_label.axi, "x:%d y:%d z:%d", 
											imu_val.x_data,imu_val.y_data,imu_val.z_data);	
	}

}


/**
 * @brief 初始化创建三个任务
 * @param NULL
 * @retval void
*/
void object_task_init(void)
{
	iic_mutex = xSemaphoreCreateMutex();												//创建互斥信号量
  log_queue = xQueueCreate(LOG_QUEUE_LEN, LOG_STR_MAX_LEN);		// 创建日志消息队列
	
	/**********分别创建三个任务*************/
	    xTaskCreate((TaskFunction_t )lig_task,
                (const char*    )"lig_task",
                (uint16_t       )LIG_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LIG_TASK_PRIO,
                (TaskHandle_t*  )&LigTask_Handler);
		
	    xTaskCreate((TaskFunction_t )Tem_task,
                (const char*    )"Tem_task",
                (uint16_t       )TEM_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )TEM_TASK_PRIO,
                (TaskHandle_t*  )&TemTask_Handler);

	    xTaskCreate((TaskFunction_t )Axi_task,
                (const char*    )"Axi_task",
                (uint16_t       )AXI_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )AXI_TASK_PRIO,
                (TaskHandle_t*  )&AxiTask_Handler);
								
	    xTaskCreate((TaskFunction_t )Log_Task,
                (const char*    )"LogTask",
                (uint16_t       )Log_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Log_TASK_PRIO,
                (TaskHandle_t*  )&LogTask_Handle);
								
			
	/********挂起任务*********/							
	vTaskSuspend(LigTask_Handler);
	vTaskSuspend(TemTask_Handler);
	vTaskSuspend(AxiTask_Handler);
								

}
