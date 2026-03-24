#include "sdm02.h"
#include "usart.h"

SDM02_Device_t sdm02_devices[SDM02_MAX_DEVICES];

void Sdm02_Init(SDM02_Device_t *dev, UART_HandleTypeDef *huart, uint8_t device_id)
{
    // 初始化设备结构体
    dev->huart = huart;
    dev->device_id = device_id;
    dev->distance = 0;
    dev->data_ready = 0;
    
    // 清空接收缓冲区
    memset(dev->rx_buffer, 0, sizeof(dev->rx_buffer));
    
    // 启用空闲中断
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    // 启动DMA接收
    HAL_UARTEx_ReceiveToIdle_DMA(huart, dev->rx_buffer, sizeof(dev->rx_buffer));
}

// 串口接收中断处理函数（需要在串口中断中调用）
void Sdm02_RxHandler(SDM02_Device_t *dev, uint8_t* data)
{
    if(data[0] == 0x5C)
    {
        uint8_t calculated_checksum = ~(dev->rx_buffer[1] + dev->rx_buffer[2]);
        
        // 验证校验和
        if (calculated_checksum == dev->rx_buffer[3]) 
        {
            // 校验通过，提取距离值
            dev->distance = (dev->rx_buffer[2] << 8) | dev->rx_buffer[1];
        }
        
        // 重置接收状态
        memset(dev->rx_buffer, 0, sizeof(dev->rx_buffer));
        
        // 重新启动DMA接收
        HAL_UARTEx_ReceiveToIdle_DMA(dev->huart, dev->rx_buffer, sizeof(dev->rx_buffer));
    }
}

