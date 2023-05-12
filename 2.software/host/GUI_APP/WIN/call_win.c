#include "call_win.h"
#include "gui_app.h"
#include <stdio.h>
#include <string.h>


const char * call_num;
const char * const Check_MAP[] ={"Ok",""};


static lv_obj_t * num_ta = NULL;
static lv_obj_t * kb = NULL;
static lv_obj_t * call_btn = NULL;
static lv_obj_t * win;
static lv_obj_t * check_mbox = NULL;


void kb_call_create(lv_obj_t * bind_ta);
void check_mbox_create(lv_obj_t * parent);

extern void delay_ms(u16 nms);
//�¼��ص�����
static void event_handler(lv_obj_t * obj,lv_event_t event)
{
	if(obj==num_ta)
  {
    if(event==LV_EVENT_RELEASED)//�����¼�
		{
			lv_ta_set_cursor_type(num_ta,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ع��
      kb_call_create(obj);//��������
		}
  }
	else if(obj==kb)
	{
		lv_kb_def_event_cb(kb,event);//���ü��̵�Ĭ���¼��ص�����
		if(event==LV_EVENT_APPLY||event==LV_EVENT_CANCEL)//����˼����ϵġ̻��ߡ�����
    {
      lv_obj_del(kb);//ɾ������
      kb = NULL;
    }
	}
	else if(obj==call_btn)
	{
		if(event==LV_EVENT_RELEASED)
		{
			call_num = lv_ta_get_text(num_ta);
			memcpy(num_send,call_num,3);
			send_flag = 1;
			if(kb != NULL)//������̴���,��ɾ������
			{
				lv_obj_del(kb);//ɾ������
				kb = NULL;
			}
			check_mbox_create(lv_obj_get_parent(obj));
		}
	}
	else if(obj==check_mbox)
	{
		if(event==LV_EVENT_VALUE_CHANGED)
		{
			lv_obj_del(check_mbox);//�ر��˳��Ի���
			check_mbox = NULL;
			lv_obj_del(win);//ɾ�����н���
		}
	}
}

//��������
void kb_call_create(lv_obj_t * bind_ta)
{
	if(kb==NULL)//������̲�����,���ȴ�������
	{
		kb = lv_kb_create(win,NULL);
		lv_kb_set_cursor_manage(kb,true);//�Թ����й���
		lv_obj_set_event_cb(kb,event_handler);//�����¼��ص�����
  }
	lv_kb_set_mode(kb,LV_KB_MODE_NUM);//�������ּ���
	lv_kb_set_ta(kb,bind_ta);//�������
}

//�����˳�ȷ�ϵ���Ϣ�Ի���
void check_mbox_create(lv_obj_t * parent)
{
	if(check_mbox)//��Ҫ�ظ�����
		return;
	check_mbox = lv_mbox_create(parent,NULL);
	lv_obj_set_size(check_mbox,lv_obj_get_width(parent)*0.7f,lv_obj_get_height(parent)/2);
	if(strlen(call_num) != 0)
		lv_mbox_set_text(check_mbox,"Hey man\nmessage send success");
	else
		lv_mbox_set_text(check_mbox,"What's up man?\nyou call nothing");
	lv_mbox_add_btns(check_mbox,(const char**)Check_MAP);
	lv_obj_set_drag(check_mbox,true);//���öԻ�����Ա���ק
	lv_obj_align(check_mbox,NULL,LV_ALIGN_CENTER,0,0);
	lv_obj_set_event_cb(check_mbox,event_handler);
}

void call_win_create(lv_obj_t * parent)
{
	uint16_t parent_width,parent_height;
	
	win = general_win_create(parent,"CALL");
	// lv_obj_set_event_cb(win,event_handler);//�����¼��ص�����

	//��ȡ������Ŀ�͸�
	parent_width = lv_obj_get_width(win);
	parent_height = lv_obj_get_height(win);

	//����һ�����������
	num_ta = lv_ta_create(win,NULL);
	lv_obj_set_size(num_ta,parent_width*0.7f,50);//���ô�С
	lv_obj_align(num_ta,NULL,LV_ALIGN_IN_TOP_MID,0,60);//���ö��뷽ʽ
	lv_ta_set_text(num_ta,"");//���Ĭ�ϵ��ı�����
	lv_ta_set_placeholder_text(num_ta,"Input num");//����placeholder��ʾ�ı�
	lv_ta_set_one_line(num_ta,true);//ʹ�ܵ���ģʽ
	lv_obj_set_event_cb(num_ta,event_handler);//�����¼��ص�����
	//����һ��ȷ�ϰ�ť
	call_btn = lv_btn_create(win,NULL);
	lv_obj_set_size(call_btn,parent_width*0.7f,40);//���ô�С
	lv_obj_align(call_btn,num_ta,LV_ALIGN_OUT_BOTTOM_MID,0,20);//���ö��뷽ʽ
	lv_obj_t * btn_label = lv_label_create(call_btn,NULL);
	lv_label_set_text(btn_label,"call num");//���ð�ť�ϵı���
	lv_obj_set_event_cb(call_btn,event_handler);//�����¼��ص�����
}



