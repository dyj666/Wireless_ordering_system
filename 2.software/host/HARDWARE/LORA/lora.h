#ifndef _LORA_H_
#define _LORA_H_




#include "sys.h"
#include <stdio.h>




extern _Bool rx_flag;


extern u8 rx_data[10];






void Lora_Init(void);
void SendString(char *s);
void Sendhex(u8 *array,u8 len);









#endif

