#ifndef __WY_LIB_CPP_UART_HPP__
#define __WY_LIB_CPP_UART_HPP__
namespace UART
{
    struct InitStruct
    {
        uint32_t bode;
        uint8_t uartIdx;
        GPIO_TypeDef *RxPort, *TxPort;
        uint16_t RxPin, TxPin;
        uint8_t RxAF, TxAF;
        /* data */
    };

    class UART_Object
    {
    private:
        UART_TypeDef *uart;
        uint8_t nvicChannel;
        DMA_Channel_TypeDef *dmaChannel;
        void (**rxFun)(void) = nullptr;
        /* data */
    public:
        UART_Object() = default;
        UART_Object(InitStruct &s);

        void setNVIC(void) { setNVIC(1, true, nullptr); }
        void setNVIC(uint8_t priority) { setNVIC(priority, true, nullptr); }
        void setNVIC(uint8_t priority, bool open) { setNVIC(priority, open, nullptr); }
        void setNVIC(uint8_t priority, bool open, void (*f)(void));
        void setRxFunction(void (*f)(void)) { *(this->rxFun) = f; };

        void setDMA(uint32_t add, uint16_t size, char mode) { setDMA(add, size, 1, mode, false, nullptr); }
        void setDMA(uint32_t add, uint16_t size, char mode, bool interrupt) { setDMA(add, size, 1, mode, interrupt, nullptr); }
        void setDMA(uint32_t add, uint16_t size, uint8_t priority, char mode, bool interrupt) { setDMA(add, size, priority, mode, interrupt, nullptr); }
        void setDMA(uint32_t add, uint16_t size, uint8_t priority, char mode, bool interrupt, void (*f)(void));
        void setDMA_InterruptFunction(void (*f)(void));
        void DMA_On(bool set) {DMA_Cmd(this->dmaChannel, set ? ENABLE : DISABLE); }

        void sendByte(uint8_t dat);
        void sendByte(uint8_t *dat, uint8_t len);

        uint8_t receiveByte(void) { return UART_ReceiveData(this->uart); }
    };

} // namespace UART

#endif /* __WY_LIB_CPP_UART_HPP__ */
