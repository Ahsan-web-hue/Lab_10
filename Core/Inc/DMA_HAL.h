#ifndef INC_DMA_HAL_H_
#define INC_DMA_HAL_H_

#include "CircularBuffer.h"
#include "ISR_List.h"
#include "stm32f1xx_hal.h"

#define RX_SIZE_BUFFER 64

class DMA_HAL
{
public:
    static void init();

private:
    static bool is_init;
};

class UART
{
public:
    UART(UART_HandleTypeDef *huart);
    virtual ~UART();

    HAL_StatusTypeDef write(uint8_t *data,
                            uint16_t size,
                            uint32_t timeout = 1000);

    HAL_StatusTypeDef read(uint8_t *data,
                           uint16_t size,
                           uint32_t timeout = 1000);

protected:
    UART_HandleTypeDef *Instance;
};

class UART_IR : public UART
{
public:
    UART_IR(UART_HandleTypeDef *huart);
    virtual ~UART_IR();

    virtual HAL_StatusTypeDef write(uint8_t *pData,
                                    uint16_t Size);

    virtual HAL_StatusTypeDef start_read();

    uint16_t read(uint8_t *pData,
                  uint16_t Size);

protected:
    bool is_tx_done;

    uint8_t _read_buffer[RX_SIZE_BUFFER];

    CircularBuffer<uint8_t> *_buffer;

    // Make virtual so DMA class can override
    virtual void put(uint16_t index,
                     uint16_t size);

    static ISR<UART_IR> ISR_List;

private:
    static void TxCpltCallback(UART_HandleTypeDef *huart);

    static void RxEventCallback(UART_HandleTypeDef *huart,
                                uint16_t Pos);
};

class UART_DMA : public UART_IR
{
public:
    UART_DMA(UART_HandleTypeDef *huart);
    virtual ~UART_DMA();

    HAL_StatusTypeDef write(uint8_t *pData,
                            uint16_t Size) override;

    HAL_StatusTypeDef start_read() override;

protected:
    void put(uint16_t index,
             uint16_t size) override;
};

#endif
