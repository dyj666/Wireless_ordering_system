#include "login_win.h"
#include "gui_app.h"
#include "main_win.h"
#include <string.h>

//�궨��
#define LOGIN_ACCOUNT		"123456"	//��¼�ֻ��� 
#define LOGIN_PASSWORD	"abcdef"	//��¼���� 

//��������
static lv_obj_t * login_win = NULL;
static lv_obj_t * account_ta = NULL;
static lv_obj_t * password_ta = NULL;
static lv_obj_t * kb = NULL;
static lv_obj_t * login_btn = NULL;


//����
LV_IMG_DECLARE(weixin_img);//ͼƬ����
void kb_login_create(lv_obj_t * bind_ta);


//�¼��ص�����
static void event_handler(lv_obj_t * obj,lv_event_t event)
{
  if(obj==account_ta||obj==password_ta)
  {
    if(event==LV_EVENT_RELEASED)//�����¼�
		{
			lv_ta_set_cursor_type(account_ta,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ع��
			lv_ta_set_cursor_type(password_ta,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ع��
      kb_login_create(obj);//��������
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
	else if(obj==login_btn)
	{
		if(event==LV_EVENT_RELEASED)
		{
			// main_win_create(lv_scr_act());//��ת��������
			const char * phone = lv_ta_get_text(account_ta);//��ȡ����ĵ绰����
			const char * password = lv_ta_get_text(password_ta);//��ȡ���������
			if((strcmp(phone,LOGIN_ACCOUNT)==0)&&(strcmp(password,LOGIN_PASSWORD)==0))
			{ 
				//��֤�ɹ�
				lv_obj_del(login_win);//ɾ��������¼����
				login_win = NULL;
				kb = NULL;
				main_win_create(lv_scr_act());//��ת��������
			}
			else
			{
				//��֤ʧ��,������ʾ
				toast("#FF0000 Tip#\nPhone:"LOGIN_ACCOUNT"\nPassword:"LOGIN_PASSWORD,3000);
			}
		}
	}
}

//��������
void kb_login_create(lv_obj_t * bind_ta)
{
	if(kb==NULL)//������̲�����,���ȴ�������
	{
		kb = lv_kb_create(login_win,NULL);
		lv_kb_set_cursor_manage(kb,true);//�Թ����й���
		lv_obj_set_event_cb(kb,event_handler);//�����¼��ص�����
  }
	lv_kb_set_mode(kb,bind_ta==account_ta?LV_KB_MODE_NUM:LV_KB_MODE_TEXT);//����󶨵��ǵ绰�����,�򵯳����ּ���,���򵯳��ı�����
	lv_kb_set_ta(kb,bind_ta);//�������
}

//������¼����
void login_win_create(lv_obj_t * parent)
{
	uint16_t parent_width,parent_height;
	
	//��ȡ������Ŀ�͸�
	parent_width = lv_obj_get_width(parent);
	parent_height = lv_obj_get_height(parent);
	//������¼����
	login_win = lv_cont_create(parent,NULL);//��������ģ�ⴰ��
	lv_obj_set_size(login_win,parent_width,parent_height);//���ô���Ĵ�С
	lv_cont_set_layout(login_win,LV_LAYOUT_OFF);//�����޲��ַ�ʽ
	//����һ������
	lv_obj_t * title_label = lv_label_create(login_win,NULL);
	lv_label_set_recolor(title_label,true);//ʹ���ı��ػ�ɫ����
	lv_label_set_text(title_label,"#ff0000 Lo##0000ff g##00ff00 in#");//����"Login"����
	lv_obj_align(title_label,NULL,LV_ALIGN_IN_TOP_MID,0,10);//���ö��뷽ʽ
	//����һ��logoͼƬ
	lv_obj_t * logo_img = lv_img_create(login_win,NULL);
	lv_img_set_src(logo_img,&weixin_img);//����ͼƬԴ
	lv_obj_align(logo_img,title_label,LV_ALIGN_OUT_BOTTOM_MID,0,20);//���ö��뷽ʽ
	//����һ���˺������
	account_ta = lv_ta_create(login_win,NULL);
	lv_obj_set_size(account_ta,parent_width*0.7f,30);//���ô�С
	lv_obj_align(account_ta,logo_img,LV_ALIGN_OUT_BOTTOM_MID,0,20);//���ö��뷽ʽ
	lv_ta_set_text(account_ta,"");//���Ĭ�ϵ��ı�����
	lv_ta_set_placeholder_text(account_ta,"Input account");//����placeholder��ʾ�ı�
	lv_ta_set_one_line(account_ta,true);//ʹ�ܵ���ģʽ
	lv_obj_set_event_cb(account_ta,event_handler);//�����¼��ص�����
	//����һ�����������
	password_ta = lv_ta_create(login_win,account_ta);//��account_ta�Ͻ��п���
	lv_ta_set_pwd_mode(password_ta,true);//ʹ�����뱣��ģʽ
	lv_obj_align(password_ta,account_ta,LV_ALIGN_OUT_BOTTOM_MID,0,10);
	lv_ta_set_cursor_type(password_ta,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ع��
	lv_ta_set_placeholder_text(password_ta,"Input password");//����placeholder��ʾ�ı�
	//����һ����½��ť
	login_btn = lv_btn_create(login_win,NULL);
	lv_obj_set_size(login_btn,parent_width*0.7f,40);//���ô�С
	lv_obj_align(login_btn,password_ta,LV_ALIGN_OUT_BOTTOM_MID,0,20);//���ö��뷽ʽ
	lv_obj_t * btn_label = lv_label_create(login_btn,NULL);
	lv_label_set_text(btn_label,"login");//���ð�ť�ϵı���
	lv_obj_set_event_cb(login_btn,event_handler);//�����¼��ص�����
}















