#include "wy_headfile.h"

char USART_SEND_BUF[USART_SEND_LEN]; // 发送器，存放待发送的数据
char UsartRecData[USART_REC_LEN]; 	 // 接收到的数据，存放接收到的完整数据

float Variable[NUM_MAX];			 // 存放要发送的变量


/*****************************************************
*@Func:	按照协议发送数据
******
*@para:
******
*@desc: 默认的协议为：@ + 数据 + 回车符
******************************************************/	
void USART_SendStr_AsDeal(USART_TypeDef* USARTx, char* Uart_Str)
{
    char* p = Uart_Str;
    USART_SendData(USARTx, DATAHEAD);               //发送首检验符
    while(*p != '\0')
    {
        USART_SendData(USARTx, *p++); // 发送下一位数据
    }
    USART_SendData(USARTx, DATATAIL1);               //发送尾检验符1
    USART_SendData(USARTx, DATATAIL2);               //发送尾检验符2
}

void USART_SendStr(USART_TypeDef* USARTx, char* Uart_Str)
{
    char* p = Uart_Str;
    while(*p != '\0')
    {
        USART_SendData(USARTx, *p++); // 发送下一位数据
    }
}

void USART_SendInt(USART_TypeDef* USARTx, int Uart_Int)
{
    char Uart_str[10];
    sprintf((char*)Uart_str, "%5d", Uart_Int); // （u16) 打印到(char)。
    USART_SendStr(USARTx, Uart_str);
}


void USART_SendFloat(USART_TypeDef* USARTx, float Uart_Float)
{
    char Uart_str[10];
    sprintf((char*)Uart_str, "%7.2f", Uart_Float); // （u16) 打印到(char)。
    USART_SendStr(USARTx, Uart_str);
}

/*****************************************************
*@Func:	蓝牙串口初始化
******
*@para:
******
*@desc: 
******************************************************/	
void Blue_Init(u8 ocp_nvc, u8 fow_nvc, u32 bound) 
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//使能UART4时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能GPIOC时钟

    //USART4_TX   PC10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				// PC10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			// 复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);					// 初始化Pc10

    //USART4_RX	  PC11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				// PC11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	// 浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);					// 初始化Pc11

//    //Usart3 NVIC 配置
//    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ocp_nvc ; 	// 抢占优先级3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = fow_nvc;			// 子优先级1
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					// IRQ通道使能
//    NVIC_Init(&NVIC_InitStructure);									// 根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;					// 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;	// 字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;			// 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					// 收发模式

    USART_Init(UART4, &USART_InitStructure); 										// 初始化串口4
//    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);								// 开启串口接受中断
    USART_Cmd(UART4, ENABLE);                    									// 使能串口4
}



/*****************************************************
*@Func:	借助蓝牙发送变量给名优科创串口助手
******
*@para:
******
*@desc: 
******************************************************/		
void USART_SendToKeYou(USART_TypeDef* USARTx)
{
    u8 i;
    float temp = 0.0;
    u8 ch;
	
	/*这里放置需要发送的变量，注意变量的个数不能超过设定的最大发送个数NUM_MAX*/
    Variable[0] =  UlCurDistance;
//	Variable[1] =  gyrox;
//	Variable[2] =  gyroy;
//	Variable[3] =  gyroz;
//	Variable[4] =  aacx;
//	Variable[5] =  aacy;
//	Variable[6] =  aacz;
//	Variable[7] =  Temp;
//	Variable[8] =  pitch;
//	Variable[9] =  roll;
//	Variable[10] =  yaw;
//	Variable[11] =  tempI;
//	Variable[12] =  bias;
//	Variable[13] =  angle_accl;
//	Variable[14] = angle_cur;
//	Variable[15] = Balance_pid.errIntg;


	/*这是与名优科创串口助手之间的通信协议*/
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0x11);
	USART_SendData(USARTx, 0xaa);
	USART_SendData(USARTx, 0x55);
	USART_SendData(USARTx, 0xaa);
	USART_SendData(USARTx, 0xff);
	USART_SendData(USARTx, 0x01);
	USART_SendData(USARTx, NUM_MAX);

    for(i = 0; i < NUM_MAX; i++)// 逐位发送(stm32采用的是大端存储数据的方式、即高字节存放在高地址中)
    {
        temp = Variable[i];

		ch = BYTE0(temp);
		USART_SendData(USARTx, ch);
		
        ch = BYTE1(temp);
        USART_SendData(USARTx, ch);

        ch = BYTE2(temp);
        USART_SendData(USARTx, ch);

        ch = BYTE3(temp);
        USART_SendData(USARTx, ch);
    }
	
    USART_SendData(USARTx, 0x01);// 尾校验符
}

