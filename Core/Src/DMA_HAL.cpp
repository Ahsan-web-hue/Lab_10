#include "DMA_HAL.h"
#include "dma.h"
#include "main.h"
#include <cstring>

bool DMA_HAL::is_init = false;

ISR<UART_IR> UART_IR::ISR_List;

void DMA_HAL::init()
{
    if (is_init)
        return;

    is_init = true;
    MX_DMA_Init();
}




UART::UART(UART_HandleTypeDef *huart)
{
    Instance = huart;
}

UART::~UART()
{

}

HAL_StatusTypeDef UART::write(uint8_t *data,
                              uint16_t size,
                              uint32_t timeout)
{
    return HAL_UART_Transmit(
            Instance,
            data,
            size,
            timeout);
}

//HAL_StatusTypeDef UART::read(uint8_t *data,
//                             uint16_t size)
//{
//    return HAL_UART_Receive(
//            Instance,
//            data,
//            size);
//
//}

UART_IR::UART_IR(UART_HandleTypeDef *huart)
    : UART(huart),
      is_tx_done(true)
{

#if USE_HAL_UART_REGISTER_CALLBACKS != 1
#endif
	_buffer = new CircularBuffer<uint8_t>(sizeof(_read_buffer)*2);

//    huart->TxCpltCallback = UART_IR::TxCpltCallback;
//    huart->RxEventCallback = UART_IR::RxEventCallback;

    ISR_List.add(this);
}

UART_IR::~UART_IR()
{
	if(_buffer!=NULL){
		delete _buffer;
	}
    ISR_List.remove(this);
}

HAL_StatusTypeDef UART_IR::write(uint8_t *pData,
                                 uint16_t Size)
{
    if(!is_tx_done)
    {
        return HAL_BUSY;
    }

    is_tx_done = false;

    return HAL_UART_Transmit_IT(
            Instance,
            pData,
            Size);
}
//HAL_StatusTypeDef UART_IR::read(uint8_t *pData,
//                                uint16_t Size)
//{
//	uint16_t RxLen = 0;
//
//	    return HAL_UARTEx_ReceiveToIdle(
//	        Instance,
//	        pData,
//	        Size,
//	        &RxLen,
//	        HAL_MAX_DELAY
//	    );

//}
uint16_t UART_IR::read(uint8_t *pData,uint16_t Size){
	return _buffer->pull(pData,Size);
}

HAL_StatusTypeDef UART_IR::start_read(void)
{
	memset(_read_buffer,0,sizeof(_read_buffer));

//   return read(pData,Size);
	return HAL_UARTEx_ReceiveToIdle_IT(Instance,_read_buffer,sizeof(_read_buffer));
}

void UART_IR::TxCpltCallback(UART_HandleTypeDef *huart)
{
    for(uint16_t i = 0; i < ISR_List.size(); i++) // check UART list
    {
        UART_IR *obj = ISR_List.get(i); // Current UART

        if(obj != nullptr && obj->Instance == huart) // check UART is initilaze and not empty
        {
            obj->is_tx_done = true;
        }
    }

}


void UART_IR::RxEventCallback(UART_HandleTypeDef *huart, uint16_t Pos)
{
    for (uint16_t i = 0; i < ISR_List.size(); i++)
    {
        if (ISR_List.get(i)->Instance == huart)
        {
            ISR_List.get(i)->put(0, Pos);
        }
    }
}
void UART_IR::put(uint16_t index,uint16_t size){
	_buffer->put(&_read_buffer[index],size);
	start_read();
}



UART_DMA::UART_DMA(UART_HandleTypeDef *huart)
    : UART_IR(huart)
{
    DMA_HAL::init();
}

UART_DMA::~UART_DMA()
{
}

HAL_StatusTypeDef UART_DMA::write(uint8_t *pData,
                                  uint16_t Size)
{
    if (!is_tx_done)
        return HAL_BUSY;

    is_tx_done = false;

    HAL_StatusTypeDef ret =
        HAL_UART_Transmit_DMA(Instance,
                              pData,
                              Size);

    if (ret != HAL_OK)
        is_tx_done = true;

    return ret;
}

HAL_StatusTypeDef UART_DMA::start_read()
{
    memset(_read_buffer, 0, RX_SIZE_BUFFER);

    return HAL_UARTEx_ReceiveToIdle_DMA(
        Instance,
        _read_buffer,
        RX_SIZE_BUFFER);
}

void UART_DMA::put(uint16_t index,
                   uint16_t size)
{
    (void)index;

    static uint16_t last_size = 0;
    static uint16_t next_index = 0;

    uint16_t bytes = size - last_size;

    _buffer->put(&_read_buffer[next_index], bytes);

    next_index += bytes;

    if (next_index >= RX_SIZE_BUFFER)
        next_index -= RX_SIZE_BUFFER;

    if (size >= RX_SIZE_BUFFER)
    {
        last_size = 0;
        return;
    }

    last_size = size;
}
