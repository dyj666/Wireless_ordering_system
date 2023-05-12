#include "main_win.h"
#include "gui_app.h"
#include "login_win.h"
#include "calendar_win.h"
#include "call_win.h"
#include "chart_win.h"
#include "gauge_win.h"

//申明图片
LV_IMG_DECLARE(logo_img);
LV_IMG_DECLARE(date_img);
LV_IMG_DECLARE(chart_img);
LV_IMG_DECLARE(gauge_img);
LV_IMG_DECLARE(call_img);
LV_IMG_DECLARE(weixin_img);//图片申明

//申明字体
LV_FONT_DECLARE(my_font);//此字体里面只有正点原子四个汉字

//变量定义
#define TABLE_COL_CNT   		3   //表格的列数
#define TABLE_ROW_CNT   		3   //表格的宽度
#define SPLIT_LINE_WIDTH		200 //水平分割线的宽度
const lv_point_t SPLIT_LINE_POINTS[2] = {{0,0},{SPLIT_LINE_WIDTH,0}};
const char * const BTNM_MAP[] = {
	"    "LV_SYMBOL_AUDIO"\nAUDIO","  "LV_SYMBOL_BELL"\nBELL","\n",
	"   "LV_SYMBOL_CALL"\nCALL","  "LV_SYMBOL_GPS"\nGPS",
	""
};
const char * const BTNM_TITLE[] = {"audio btn","bell btn","call btn","gps btn"};
const char * const MBOX_MAP[] ={"Yes","No",""};
//定义表格每一行的内容
const char * const TABLE_CELL_VALUE[TABLE_ROW_CNT][TABLE_COL_CNT] = {
  {"Name","Sex","Age"},//第一行,作为标题行
  {"fish","man","25"},
  {"xjy","woman","26"}
};

lv_obj_t *  main_win;
static lv_obj_t * quit_btn;
static lv_obj_t * btnm;
static lv_obj_t * quit_mbox = NULL;
static lv_obj_t * calendar_item_btn,* call_item_btn,* chart_item_btn;


//函数申明
void quit_mbox_create(lv_obj_t * parent);

//事件回调函数
static void event_handler(lv_obj_t * obj,lv_event_t event)
{
	uint16_t btn_id;
	if(obj==quit_btn)
	{
		if(event==LV_EVENT_RELEASED)
		{
			quit_mbox_create(lv_obj_get_parent(obj));//是否退出?
		}
	}
	else if(obj==btnm)
	{
		if(event==LV_EVENT_VALUE_CHANGED)
		{
			btn_id = *((uint16_t*)lv_event_get_data());//获取矩阵按钮中的按钮id
			toast(BTNM_TITLE[btn_id],1000);
		}
	}
	else if(obj==quit_mbox)
	{
		if(event==LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(quit_mbox);//关闭退出对话框
			quit_mbox = NULL;
			btn_id = *((uint16_t*)lv_event_get_data());//获取退出对话框中的按钮id
			if(btn_id==0)//点击了Yes按钮
			{
				lv_obj_del(main_win);//删除主界面
				login_win_create(lv_scr_act());//返回到登录界面
			}
		}
	}
	else if(obj==calendar_item_btn)
	{
		//当列表出现滚轮时,用LV_EVENT_CLICKED来处理松手事件会比LV_EVENT_RELEASED好一些,
		//因为滑动列表时,LV_EVENT_CLICKED事件不会出现误触发,而LV_EVENT_RELEASED会误触发
		if(event==LV_EVENT_CLICKED) 
			calendar_win_create(lv_scr_act());//跳转到calendar窗体
	}
	else if(obj==call_item_btn)
	{
		if(event==LV_EVENT_CLICKED)
			call_win_create(lv_scr_act());//跳转到call窗体
	}
	else if(obj==chart_item_btn)
	{
		if(event==LV_EVENT_CLICKED)
			chart_win_create(lv_scr_act());//跳转到chart窗体
	}
}

//创建退出确认的消息对话框
void quit_mbox_create(lv_obj_t * parent)
{
	if(quit_mbox)//不要重复创建
		return;
	quit_mbox = lv_mbox_create(parent,NULL);
	lv_obj_set_size(quit_mbox,lv_obj_get_width(parent)*0.7f,lv_obj_get_height(parent)/2);
	lv_mbox_set_text(quit_mbox,"Tip\nAre you to quit?");
	lv_mbox_add_btns(quit_mbox,(const char**)MBOX_MAP);
	lv_obj_set_drag(quit_mbox,true);//设置对话框可以被拖拽
	lv_obj_align(quit_mbox,NULL,LV_ALIGN_CENTER,0,0);
	lv_obj_set_event_cb(quit_mbox,event_handler);
}

//首页选项卡初始化
void home_tab_init(lv_obj_t * parent)
{
	//创建列表
	lv_obj_t * list = lv_list_create(parent,NULL);
  lv_obj_set_size(list,lv_obj_get_width(parent)*0.9f,lv_obj_get_height(parent)-160);
	lv_obj_align(list,NULL,LV_ALIGN_IN_TOP_MID,0,30);
	lv_list_set_sb_mode(list,LV_SB_MODE_AUTO);
	//添加列表项1,有2行
  calendar_item_btn = lv_list_add_btn(list,&date_img,"#FFFFFF Calendar#\n#666666 This is the date#");
	lv_obj_t * item_label = lv_list_get_btn_label(calendar_item_btn);
	lv_label_set_recolor(item_label,true);
	lv_obj_set_event_cb(calendar_item_btn,event_handler);//设置事件回调函数
	//添加列表项2
  call_item_btn = lv_list_add_btn(list,&call_img,"Call");
	lv_obj_set_event_cb(call_item_btn,event_handler);//设置事件回调函数
	//添加列表项3
  chart_item_btn = lv_list_add_btn(list,&chart_img,"Chart");
	lv_obj_set_event_cb(chart_item_btn,event_handler);//设置事件回调函数
	
}


//设置选项卡初始化
void setting_tab_init(lv_obj_t * parent)
{
	uint16_t parent_width = lv_obj_get_width(parent);//获取父对象的宽度
	
  //创建头像
	lv_obj_t * head_img = lv_img_create(parent,NULL);
	lv_img_set_src(head_img,&weixin_img);
	lv_obj_set_pos(head_img,20,20);
	//创建用户名和id
	lv_obj_t * name_id_label = lv_label_create(parent,NULL);
	lv_label_set_recolor(name_id_label,true);
	lv_label_set_text(name_id_label,"#FFFFFF dyjyyds#\n#BBBBBB ID:18606181495#");
	lv_obj_align(name_id_label,head_img,LV_ALIGN_OUT_RIGHT_MID,20,0);
	//创建一根水平分割横线
	lv_obj_t * split_line = lv_line_create(parent,NULL);
	lv_obj_set_size(split_line,SPLIT_LINE_WIDTH,2);
	lv_line_set_points(split_line,SPLIT_LINE_POINTS,2);
	lv_obj_align(split_line,NULL,LV_ALIGN_IN_TOP_MID,0,100);
	
	//创建一个退出按钮
	quit_btn = lv_btn_create(parent,NULL);
	lv_obj_set_width(quit_btn,parent_width/2);
	lv_obj_align(quit_btn,split_line,LV_ALIGN_OUT_BOTTOM_MID,0,250);
	lv_obj_t * quit_label = lv_label_create(quit_btn,NULL);
	lv_label_set_recolor(quit_label,true);
	lv_label_set_text(quit_label,"#FF0000 Quit#");
	lv_obj_set_event_cb(quit_btn,event_handler);
	
}


//创建主界面
void main_win_create(lv_obj_t * parent)
{
	//为了减少复杂性,我们这里就直接把tabview作为main_win窗体了
	main_win = lv_tabview_create(parent,NULL);
	lv_tabview_set_btns_pos(main_win,LV_TABVIEW_BTNS_POS_BOTTOM);//选项卡按钮位于底部
	//添加tab1选项卡
  lv_obj_t * home_tab = lv_tabview_add_tab(main_win,LV_SYMBOL_HOME" Home");
  home_tab_init(home_tab);
  //添加tab1选项卡
  lv_obj_t * setting_tab = lv_tabview_add_tab(main_win,LV_SYMBOL_SETTINGS" Setting");
  setting_tab_init(setting_tab);
}










