#include "main.h"
#include "stdio.h"
#define TXBUFFERSIZE   150
#define RXBUFFERSIZE   150
__IO uint8_t TxCount = 0; 
__IO uint16_t RxCount = 0;
__IO uint8_t TxBuffer[TXBUFFERSIZE];
__IO uint8_t RxBuffer[RXBUFFERSIZE];
uint8_t *Array_Buffer;
__IO bool Start_Parser = true;
__IO uint8_t Rx_CLength = 4;
__IO uint8_t NbrOfDataToTransfer = 0;

static void USART_Config(void);
static void NVIC_Config(void);
static void Send_AT(char *command);
static __IO uint32_t TimingDelay;
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);
int main(void)
{ 
  /* NVIC configuration */
  NVIC_Config();
	
  
  /* USART configuration */
  USART_Config();
	
	/* Enable the EVAL_COM1 Transmoit interrupt: this interrupt is generated when the 
  EVAL_COM1 transmit data register is empty */  
  USART_ITConfig(WS751_UART, USART_IT_RXNE, ENABLE);
  USART_ITConfig(DEBUG_COM, USART_IT_RXNE, ENABLE);
  if(SysTick_Config(SystemCoreClock/1000))
  {
	  while(1);
  }
	uint8_t cmd_state=0;
  while(1)
  {
		if(Start_Parser)
		{
			for(cmd_state=0;cmd_state<=2;cmd_state++)
			{	 
				Send_AT("");
			  switch(cmd_state)
				{
					case 0:
					Send_AT("AT\r\n");
				  Delay(500);
				  break;
					case 1:
					Delay(300);
					Send_AT("AT+WIFISTATUS\r\n");
					Delay(300);
	   			break;
					case 2:
					Delay(500);			
		      Send_AT("AT+BLES_INIT=BLENAME\r\n");
					Delay(500);
					break;
					default:
					break;
				}	 
			}
			Start_Parser = false;
		}		
  }
}


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
static void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = WS751_UART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Enable the USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_COM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief Configure the USART Device
  * @param  None
  * @retval None
  */
static void USART_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(WS751_UART_TX_GPIO_CLK | WS751_UART_RX_GPIO_CLK, ENABLE);

  /* Enable USART clock */
  RCC_APB2PeriphClockCmd(WS751_UART_CLK, ENABLE);
  RCC_APB1PeriphClockCmd(DEBUG_COM_CLK, ENABLE);

  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(WS751_UART_TX_GPIO_PORT, WS751_UART_TX_SOURCE, WS751_UART_TX_AF);
  GPIO_PinAFConfig(DEBUG_COM_TX_GPIO_PORT, DEBUG_COM_TX_SOURCE, DEBUG_COM_TX_AF);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(WS751_UART_RX_GPIO_PORT, WS751_UART_RX_SOURCE, WS751_UART_RX_AF);
  GPIO_PinAFConfig(DEBUG_COM_RX_GPIO_PORT, DEBUG_COM_RX_SOURCE, DEBUG_COM_RX_AF);

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = WS751_UART_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(WS751_UART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = WS751_UART_RX_PIN;
  GPIO_Init(WS751_UART_RX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = DEBUG_COM_TX_PIN;
  GPIO_Init(DEBUG_COM_TX_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = DEBUG_COM_RX_PIN;
  GPIO_Init(DEBUG_COM_RX_GPIO_PORT, &GPIO_InitStructure);

/* USARTx configured as follow:
  - BaudRate = 152000 baud  
  - Word Length = 8 Bits
  - Two Stop Bit
  - Odd parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* USART configuration */
  USART_Init(WS751_UART, &USART_InitStructure);
  /* Enable USART */
  USART_Cmd(WS751_UART, ENABLE);
  /* USART configuration */
  USART_Init(DEBUG_COM, &USART_InitStructure);
  /* Enable USART */
  USART_Cmd(DEBUG_COM, ENABLE);

}
static void Send_AT (char *command)
{
	
	  memset((uint8_t*)TxBuffer,0,150);
    NbrOfDataToTransfer = strlen(command);
    memcpy((uint8_t*)TxBuffer, command, NbrOfDataToTransfer);
    USART_ITConfig(WS751_UART, USART_IT_TXE, ENABLE);	
	  USART_ClearITPendingBit(WS751_UART, USART_IT_TXE);
}
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

	  

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif



