#ifndef __SDM02_H
#define __SDM02_H

#include "main.h"

// SDM02设备结构体
typedef struct {
    UART_HandleTypeDef *huart;           // 串口句柄
    uint8_t rx_buffer[16];               // 接收缓冲区
    uint16_t distance;                   // 距离值
    uint8_t data_ready;                  // 数据就绪标志
    uint8_t device_id;                   // 设备ID
} SDM02_Device_t;

// 最大设备数量
#define SDM02_MAX_DEVICES 4

// 函数声明
void Sdm02_Init(SDM02_Device_t *dev, UART_HandleTypeDef *huart, uint8_t device_id);
void Sdm02_RxHandler(SDM02_Device_t *dev, uint8_t* data);
uint16_t Sdm02_GetDistance(SDM02_Device_t *dev);
uint8_t Sdm02_DataReady(SDM02_Device_t *dev);

// 全局设备数组（可选，如果需要全局访问）
extern SDM02_Device_t sdm02_devices[SDM02_MAX_DEVICES];

#endif /* __SDM02_H */