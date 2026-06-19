#include "lv_mainstart.h"
#include "lvgl.h"

#include "object_task.h"

#include <stdio.h>


My_Gui_T my_gui_switch;
My_Gui_T my_gui_label;

lv_obj_t *task_all;
void task_on(void);


static void all_even_cb(lv_event_t * e)							//回调函数
{
	lv_event_code_t code = lv_event_get_code(e);			// 获取事件类型
	lv_obj_t *tar = lv_event_get_target(e);						// 获取触发事件的控件

	if(code == LV_EVENT_VALUE_CHANGED)								// 判断是否为开关状态切换事件
	{
		if(tar == my_gui_switch.lig)							// 判断是否为光敏开关
		{
			if(lv_obj_has_state(tar , LV_STATE_CHECKED))	// 判断开关是否打开
			{
				vTaskResume(LigTask_Handler);
			}
			else 
			{
				vTaskSuspend(LigTask_Handler);
//				lv_label_set_text_fmt(my_gui_label.lig, "Lig:");						//初始化显示
			}
		}
		if(tar == my_gui_switch.tem)							// 判断是否为温度开关
		{
			if(lv_obj_has_state(tar , LV_STATE_CHECKED))	// 判断开关是否打开
			{
				vTaskResume(TemTask_Handler);
			}
			else 
			{
				vTaskSuspend(TemTask_Handler);
//				lv_label_set_text_fmt(my_gui_label.tem, "Tem:");	
			}
		}
		if(tar == my_gui_switch.axi)							// 判断是否为六轴加速度开关
		{
			if(lv_obj_has_state(tar , LV_STATE_CHECKED))	// 判断开关是否打开
			{
				vTaskResume(AxiTask_Handler);
			}
			else 
			{
				vTaskSuspend(AxiTask_Handler);
//				lv_label_set_text_fmt(my_gui_label.axi, "Axi:");
			}
		}
		
		/************************总的任务开关**************************/
				if(tar == task_all)							// 判断是否为六轴加速度开关
		{
			if(lv_obj_has_state(tar , LV_STATE_CHECKED))	// 判断开关是否打开
			{
				vTaskResume(AxiTask_Handler);
				vTaskResume(TemTask_Handler);
				vTaskResume(LigTask_Handler);
			}
			else 
			{
				vTaskSuspend(AxiTask_Handler);
				vTaskSuspend(TemTask_Handler);
				vTaskSuspend(LigTask_Handler);
			}
		}
	}

}


/**
 * @brief 创建三个sensor开关
 * @param 无
 * @retval 无
*/
void my_switch_gui(void)
{
	my_gui_switch.lig = lv_switch_create(lv_scr_act());		//分别创建三个开关，父对象为滑动屏幕
	my_gui_switch.tem = lv_switch_create(lv_scr_act());
	my_gui_switch.axi = lv_switch_create(lv_scr_act());
	
	lv_obj_set_style_bg_color(my_gui_switch.tem, lv_color_hex(0xF44336),LV_STATE_DEFAULT);	//红
	lv_obj_set_style_bg_color(my_gui_switch.lig, lv_color_hex(0xFFC107),LV_STATE_DEFAULT);	//黄
	lv_obj_set_style_bg_color(my_gui_switch.axi, lv_color_hex(0x9C27B0),LV_STATE_DEFAULT);	//紫
	
	lv_obj_align(my_gui_switch.tem, LV_ALIGN_CENTER, 0, -120);		//tem修改位置到中间
	lv_obj_align(my_gui_switch.lig, LV_ALIGN_CENTER, 200, -120);
	lv_obj_align(my_gui_switch.axi, LV_ALIGN_CENTER, -200, -120);
}


/**
 * @brief 创建三个sensor标签
 * @param 无
 * @retval 无
*/
void my_label_gui(void)
{
	my_gui_label.lig = lv_label_create(lv_scr_act());							//分别创建三个标签，父对象为滑动屏幕
	my_gui_label.tem = lv_label_create(lv_scr_act());
	my_gui_label.axi = lv_label_create(lv_scr_act());
	
	lv_obj_align(my_gui_label.tem, LV_ALIGN_CENTER, 0, -80);			//位置修改
	lv_obj_align(my_gui_label.lig, LV_ALIGN_CENTER, 200, -80);
	lv_obj_align(my_gui_label.axi, LV_ALIGN_CENTER, -200, -80);
	
	lv_label_set_text_fmt(my_gui_label.tem, "Tem:");							//初始化显示
	lv_label_set_text_fmt(my_gui_label.lig, "Lig:");
	lv_label_set_text_fmt(my_gui_label.axi, "Axi:");
}

void my_sensor_gui(void)
{
/***********************************开关部件**************************************************/

	my_switch_gui();
	task_on();
	lv_obj_add_event_cb(my_gui_switch.lig, all_even_cb ,LV_EVENT_VALUE_CHANGED ,NULL);			//创建回调函数
	lv_obj_add_event_cb(my_gui_switch.tem, all_even_cb ,LV_EVENT_VALUE_CHANGED ,NULL);
	lv_obj_add_event_cb(my_gui_switch.axi, all_even_cb ,LV_EVENT_VALUE_CHANGED ,NULL);
	
/***********************************标签部件**************************************************/	
	my_label_gui();
}

void task_on(void)
{
	task_all = lv_switch_create(lv_scr_act());
	
	lv_obj_align(task_all, LV_ALIGN_CENTER, 0, 0);		
	
	lv_obj_add_event_cb(task_all , all_even_cb ,LV_EVENT_VALUE_CHANGED, NULL);
	
	
}



