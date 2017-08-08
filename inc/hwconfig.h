/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"

#define WS751_UART                      USART1

#define WS751_UART_CLK		       				RCC_APB2Periph_USART1

#define WS751_UART_TX_PIN         			GPIO_Pin_9
#define WS751_UART_TX_GPIO_PORT   			GPIOA                       
#define WS751_UART_TX_GPIO_CLK    			RCC_AHBPeriph_GPIOA
#define WS751_UART_TX_SOURCE            GPIO_PinSource9
#define WS751_UART_TX_AF                GPIO_AF_1
	 
#define WS751_UART_RX_PIN        			  GPIO_Pin_10                 
#define WS751_UART_RX_GPIO_PORT  			  GPIOA                       
#define WS751_UART_RX_GPIO_CLK   			  RCC_AHBPeriph_GPIOA
#define WS751_UART_RX_SOURCE            GPIO_PinSource10
#define WS751_UART_RX_AF                GPIO_AF_1
#define WS751_UART_IRQn                 USART1_IRQn

#define DEBUG_COM                       USART2
#define DEBUG_COM_CLK                   RCC_APB1Periph_USART2

#define DEBUG_COM_TX_PIN                GPIO_Pin_2
#define DEBUG_COM_TX_GPIO_PORT          GPIOA
#define DEBUG_COM_TX_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define DEBUG_COM_TX_SOURCE             GPIO_PinSource2
#define DEBUG_COM_TX_AF                 GPIO_AF_1

#define DEBUG_COM_RX_PIN                GPIO_Pin_3
#define DEBUG_COM_RX_GPIO_PORT          GPIOA
#define DEBUG_COM_RX_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define DEBUG_COM_RX_SOURCE             GPIO_PinSource3
#define DEBUG_COM_RX_AF                 GPIO_AF_1
#define DEBUG_COM_IRQn                  USART2_IRQn


#define AT                              "AT\r\n"
#define AT_UART_ECHO_OFF                "AT+UART_ECHO_OFF"
#define AT_WIFISTATUS                   "AT+WIFISTATUS"
#define ATBLES_INIT                     "AT+BLES_INIT=BLENAME"
#define AT_BLESTATUS                    "AT+BLESTATUS"
#define AT_STARTBLE_BCST                "AT+STARTBLE_BCST"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
