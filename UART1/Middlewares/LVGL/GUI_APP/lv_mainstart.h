#ifndef __LV_MAINSTART_H
#define __LV_MAINSTART_H

#include "lvgl.h"

typedef struct my_gui_t						//定义结构体
{
	lv_obj_t *lig;
	lv_obj_t *tem;
	lv_obj_t *axi;
}My_Gui_T;

extern My_Gui_T my_gui_switch;		//外部申明可调用
extern My_Gui_T my_gui_label;

void my_sensor_gui(void);					//开关任务



#endif

