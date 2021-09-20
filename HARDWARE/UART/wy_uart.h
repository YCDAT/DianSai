#ifndef __WY_UART_H
#define __WY_UART_H
#include "stm32f10x.h"

#define DATAHEAD 0x40
#define DATATAIL1 0x0d
#define DATATAIL2 0x0a
#define USART_SEND_LEN 100

#define NUM_MAX  1	// 每次发送给名优科创串口助手的数据最大个数，可以改，越大发的越慢


#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))

extern char USART_SEND_BUF[USART_SEND_LEN];
extern char UsartRecData[USART_REC_LEN];

void USART_SendStr_AsDeal(USART_TypeDef* USARTx, char* Uart_Str);
void USART_SendStr(USART_TypeDef* USARTx, char* Uart_Str);
void USART_SendInt(USART_TypeDef* USARTx, int Uart_Int);
void USART_SendFloat(USART_TypeDef* USARTx, float Uart_Float);
void Blue_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound);
void USART_SendToKeYou(USART_TypeDef* USARTx);

#endif
