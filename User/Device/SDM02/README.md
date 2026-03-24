# 一、Cubemx配置

随便打开一个串口，选择Asynchronous，然后初始化串口参数如下：

| 波特率 | 字节长度 | 校验位 | 停止位 |
| :----: | :------: | :----: | :----: |
| 115200 |    8    |   0   |   1   |

然后其它的默认设置就可以

# 二、SDM02的相关参数

参考image中的官方pdf文件

# 三、相关使用代码说明

## 1.关于串口的使用

hal库串口的初始化需要使用如下代码

```
HAL_UART_Receive_IT(&huart2, &rxdata, sizeof(rxdata));
```

***@param huart Pointer to a UART_HandleTypeDef structure that contains the configuration information for the specified UART module.
@param pData Pointer to data buffer (u8 or u16 data elements).
@param Size Amount of data elements (u8 or u16) to be received.***

第一个参数是串口结构体句柄

第二个是传入接收变量

第三个是接收变量的类型（u8或者u16）

## 2.数据包解析

该设备（SDM02）一帧的长度为四个字节

| 包头 | 数据位低八位 | 数据位高八位 | 校验位 |
| ---- | ------------ | ------------ | ------ |
| 0x5C | 0x*XX*     | 0x*XX*     |        |

其中校验位计算的公式是

两个数据位之和取反

```
uint8_t calculated_checksum = ~(tof_rx_buffer[1] + tof_rx_buffer[2]);
```

## 3.代码使用说明

### 1.第一次使用串口，需要如下代码

```apache
HAL_UART_Receive_IT(&huart2, &rxdata, sizeof(rxdata));
```

### 2.在串口的中断回调函数添加以下代码

```apache
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        /* 处理接收到的数据 */
        Sdm02_RxHandler(rxdata);
        HAL_UART_Receive_IT(&huart2, &rxdata, sizeof(rxdata));
    }
}

```

### 3.使用全局变量读即可（sdm02_distance）
