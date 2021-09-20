#ifndef __WY_OLED_H
#define __WY_OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	   
  
//ʹ��4��SPI���нӿ�ʱʹ��
//	GND --- �ӵض˿�
//	VCC --- ��3.3V��Դ�˿�
//��D0  --- CLKʱ���ź�(��ͬ�������SCL)
//��D1  --- ���ݶ˿�(��ͬ�������SDA)
//��RES --- ��λ�˿�(��ͬ�������RST)
//��DC  --- ����/����ѡ������(0д����1д����)
//��CS  --- Ƭѡ����(�͵�ƽ��Ч��Ҳ��������Ҫ�ӵ͵�ƽ�����Ӹ�����Ҳ�ǿ�������ʹ�õ�)
#define  GPIO_OLED_SCLK_Pin     GPIO_Pin_0							/* D0 */
#define  GPIO_OLED_PIN_Pin     	GPIO_Pin_1							/* D1 */
#define  GPIO_OLED_RES_Pin     	GPIO_Pin_2							/* RES */
#define  GPIO_OLED_DC_Pin     	GPIO_Pin_3							/* DC */

#define OLED_SCLK PCout(0)
#define OLED_SDIN PCout(1)
#define OLED_RST PCout(2) 	
#define OLED_DC  PCout(3)
		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(char x,char y,char chr,char size,char mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(char x,char y,const char *p,char size);
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
#endif  
	 







 

