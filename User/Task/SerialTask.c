#include "cmsis_os2.h"
#include "sdm02.h"
#include "usart.h"

void StartSerialTask(void *argument)
{
    Sdm02_Init(&sdm02_devices[0], &huart1, 1);
    Sdm02_Init(&sdm02_devices[1], &huart2, 2);

    for(;;)
    {
        osDelay(10);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART1) 
  {
    Sdm02_RxHandler(&sdm02_devices[0], sdm02_devices[0].rx_buffer);
    /* HAL 会自动重新启动 DMA，无需我们手动调用 */
  }
  else if (huart->Instance == USART2) 
  {
    Sdm02_RxHandler(&sdm02_devices[1], sdm02_devices[1].rx_buffer);
    /* HAL 会自动重新启动 DMA，无需我们手动调用 */
  }
} 