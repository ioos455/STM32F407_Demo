#ifndef __OBJECT_TASK_H__
#define __OBJECT_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

#include "gpio.h"
#include "LightSensor.h"
#include "QMA6100P.h"
#include "Temperature.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LIG_TASK_PRIO     19          /* 任务优先级 */
#define LIG_STK_SIZE      128         /* 任务堆栈大小 */
extern TaskHandle_t LigTask_Handler;  /* 任务句柄 */
void lig_task(void *pvParameters);    /* 任务函数 */

#define TEM_TASK_PRIO     19          /* 任务优先级 */
#define TEM_STK_SIZE      128         /* 任务堆栈大小 */
extern TaskHandle_t TemTask_Handler;  /* 任务句柄 */
void Tem_task(void *pvParameters);    /* 任务函数 */

#define AXI_TASK_PRIO     19          /* 任务优先级 */
#define AXI_STK_SIZE      128         /* 任务堆栈大小 */
extern TaskHandle_t AxiTask_Handler;  /* 任务句柄 */
void Axi_task(void *pvParameters);    /* 任务函数 */


#define Log_TASK_PRIO     18     	    /* 任务优先级 */
#define Log_STK_SIZE      128        	/* 任务堆栈大小 */
extern TaskHandle_t LogTask_Handle; 	/* 任务句柄 */
void Log_Task(void *pvParameters);    /* 任务函数 */

void object_task_init(void);
void Printf_Sensor(TaskHandle_t task_handler);

#endif
