#include "main_win.h"
#include "gui_app.h"
#include "login_win.h"
#include "calendar_win.h"
#include "call_win.h"
#include "chart_win.h"
#include "gauge_win.h"

//����ͼƬ
LV_IMG_DECLARE(logo_img);
LV_IMG_DECLARE(date_img);
LV_IMG_DECLARE(chart_img);
LV_IMG_DECLARE(gauge_img);
LV_IMG_DECLARE(call_img);
LV_IMG_DECLARE(weixin_img);//ͼƬ����

//��������
LV_FONT_DECLARE(my_font);//����������ֻ������ԭ���ĸ�����

//��������
#define TABLE_COL_CNT   		3   //���������
#define TABLE_ROW_CNT   		3   //����Ŀ���
#define SPLIT_LINE_WIDTH		200 //ˮƽ�ָ��ߵĿ���
const lv_point_t SPLIT_LINE_POINTS[2] = {{0,0},{SPLIT_LINE_WIDTH,0}};
const char * const BTNM_MAP[] = {
	"    "LV_SYMBOL_AUDIO"\nAUDIO","  "LV_SYMBOL_BELL"\nBELL","\n",
	"   "LV_SYMBOL_CALL"\nCALL","  "LV_SYMBOL_GPS"\nGPS",
	""
};
const char * const BTNM_TITLE[] = {"audio btn","bell btn","call btn","gps btn"};
const char * const MBOX_MAP[] ={"Yes","No",""};
//�������ÿһ�е�����
const char * const TABLE_CELL_VALUE[TABLE_ROW_CNT][TABLE_COL_CNT] = {
  {"Name","Sex","Age"},//��һ��,��Ϊ������
  {"fish","man","25"},
  {"xjy","woman","26"}
};

lv_obj_t *  main_win;
static lv_obj_t * quit_btn;
static lv_obj_t * btnm;
static lv_obj_t * quit_mbox = NULL;
static lv_obj_t * calendar_item_btn,* call_item_btn,* chart_item_btn;


//��������
void quit_mbox_create(lv_obj_t * parent);

//�¼��ص�����
static void event_handler(lv_obj_t * obj,lv_event_t event)
{
	uint16_t btn_id;
	if(obj==quit_btn)
	{
		if(event==LV_EVENT_RELEASED)
		{
			quit_mbox_create(lv_obj_get_parent(obj));//�Ƿ��˳�?
		}
	}
	else if(obj==btnm)
	{
		if(event==LV_EVENT_VALUE_CHANGED)
		{
			btn_id = *((uint16_t*)lv_event_get_data());//��ȡ����ť�еİ�ťid
			toast(BTNM_TITLE[btn_id],1000);
		}
	}
	else if(obj==quit_mbox)
	{
		if(event==LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(quit_mbox);//�ر��˳��Ի���
			quit_mbox = NULL;
			btn_id = *((uint16_t*)lv_event_get_data());//��ȡ�˳��Ի����еİ�ťid
			if(btn_id==0)//�����Yes��ť
			{
				lv_obj_del(main_win);//ɾ��������
				login_win_create(lv_scr_act());//���ص���¼����
			}
		}
	}
	else if(obj==calendar_item_btn)
	{
		//���б����ֹ���ʱ,��LV_EVENT_CLICKED�����������¼����LV_EVENT_RELEASED��һЩ,
		//��Ϊ�����б�ʱ,LV_EVENT_CLICKED�¼���������󴥷�,��LV_EVENT_RELEASED���󴥷�
		if(event==LV_EVENT_CLICKED) 
			calendar_win_create(lv_scr_act());//��ת��calendar����
	}
	else if(obj==call_item_btn)
	{
		if(event==LV_EVENT_CLICKED)
			call_win_create(lv_scr_act());//��ת��call����
	}
	else if(obj==chart_item_btn)
	{
		if(event==LV_EVENT_CLICKED)
			chart_win_create(lv_scr_act());//��ת��chart����
	}
}

//�����˳�ȷ�ϵ���Ϣ�Ի���
void quit_mbox_create(lv_obj_t * parent)
{
	if(quit_mbox)//��Ҫ�ظ�����
		return;
	quit_mbox = lv_mbox_create(parent,NULL);
	lv_obj_set_size(quit_mbox,lv_obj_get_width(parent)*0.7f,lv_obj_get_height(parent)/2);
	lv_mbox_set_text(quit_mbox,"Tip\nAre you to quit?");
	lv_mbox_add_btns(quit_mbox,(const char**)MBOX_MAP);
	lv_obj_set_drag(quit_mbox,true);//���öԻ�����Ա���ק
	lv_obj_align(quit_mbox,NULL,LV_ALIGN_CENTER,0,0);
	lv_obj_set_event_cb(quit_mbox,event_handler);
}

//��ҳѡ���ʼ��
void home_tab_init(lv_obj_t * parent)
{
	//�����б�
	lv_obj_t * list = lv_list_create(parent,NULL);
  lv_obj_set_size(list,lv_obj_get_width(parent)*0.9f,lv_obj_get_height(parent)-160);
	lv_obj_align(list,NULL,LV_ALIGN_IN_TOP_MID,0,30);
	lv_list_set_sb_mode(list,LV_SB_MODE_AUTO);
	//�����б���1,��2��
  calendar_item_btn = lv_list_add_btn(list,&date_img,"#FFFFFF Calendar#\n#666666 This is the date#");
	lv_obj_t * item_label = lv_list_get_btn_label(calendar_item_btn);
	lv_label_set_recolor(item_label,true);
	lv_obj_set_event_cb(calendar_item_btn,event_handler);//�����¼��ص�����
	//�����б���2
  call_item_btn = lv_list_add_btn(list,&call_img,"Call");
	lv_obj_set_event_cb(call_item_btn,event_handler);//�����¼��ص�����
	//�����б���3
  chart_item_btn = lv_list_add_btn(list,&chart_img,"Chart");
	lv_obj_set_event_cb(chart_item_btn,event_handler);//�����¼��ص�����
	
}


//����ѡ���ʼ��
void setting_tab_init(lv_obj_t * parent)
{
	uint16_t parent_width = lv_obj_get_width(parent);//��ȡ������Ŀ���
	
  //����ͷ��
	lv_obj_t * head_img = lv_img_create(parent,NULL);
	lv_img_set_src(head_img,&weixin_img);
	lv_obj_set_pos(head_img,20,20);
	//�����û�����id
	lv_obj_t * name_id_label = lv_label_create(parent,NULL);
	lv_label_set_recolor(name_id_label,true);
	lv_label_set_text(name_id_label,"#FFFFFF dyjyyds#\n#BBBBBB ID:18606181495#");
	lv_obj_align(name_id_label,head_img,LV_ALIGN_OUT_RIGHT_MID,20,0);
	//����һ��ˮƽ�ָ����
	lv_obj_t * split_line = lv_line_create(parent,NULL);
	lv_obj_set_size(split_line,SPLIT_LINE_WIDTH,2);
	lv_line_set_points(split_line,SPLIT_LINE_POINTS,2);
	lv_obj_align(split_line,NULL,LV_ALIGN_IN_TOP_MID,0,100);
	
	//����һ���˳���ť
	quit_btn = lv_btn_create(parent,NULL);
	lv_obj_set_width(quit_btn,parent_width/2);
	lv_obj_align(quit_btn,split_line,LV_ALIGN_OUT_BOTTOM_MID,0,250);
	lv_obj_t * quit_label = lv_label_create(quit_btn,NULL);
	lv_label_set_recolor(quit_label,true);
	lv_label_set_text(quit_label,"#FF0000 Quit#");
	lv_obj_set_event_cb(quit_btn,event_handler);
	
}


//����������
void main_win_create(lv_obj_t * parent)
{
	//Ϊ�˼��ٸ�����,���������ֱ�Ӱ�tabview��Ϊmain_win������
	main_win = lv_tabview_create(parent,NULL);
	lv_tabview_set_btns_pos(main_win,LV_TABVIEW_BTNS_POS_BOTTOM);//ѡ���ťλ�ڵײ�
	//����tab1ѡ�
  lv_obj_t * home_tab = lv_tabview_add_tab(main_win,LV_SYMBOL_HOME" Home");
  home_tab_init(home_tab);
  //����tab1ѡ�
  lv_obj_t * setting_tab = lv_tabview_add_tab(main_win,LV_SYMBOL_SETTINGS" Setting");
  setting_tab_init(setting_tab);
}









