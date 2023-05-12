#ifndef __CHART_WIN_H__
#define __CHART_WIN_H__
#include "sys.h"
#include "lvgl.h"



#define POINT_COUNT   	7  //????????????
extern lv_coord_t sales_volume[POINT_COUNT];


//º¯ÊýÉêÃ÷
void chart_win_create(lv_obj_t * parent);



#endif

