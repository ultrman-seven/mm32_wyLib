#ifndef C7A23A25_2EEC_4976_ACA7_6108629DC386
#define C7A23A25_2EEC_4976_ACA7_6108629DC386
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
        /* data */
    public:
        UART_Object() = default;
        UART_Object(InitStruct &s);
        void setNVIC(void) { setNVIC(1, true); };
        void setNVIC(uint8_t priority) { setNVIC(priority, true); };
        void setNVIC(uint8_t priority, bool open);
        void setDMA(uint32_t add, uint16_t size, uint8_t priority, char mode){setDMA(add, size, priority, mode, false, nullptr);}
        void setDMA(uint32_t add, uint16_t size, uint8_t priority, char mode, bool interrupt) { setDMA(add, size, priority, mode, interrupt, nullptr); }
        void setDMA(uint32_t add, uint16_t size, uint8_t priority, char mode, bool interrupt, void (*f)(void));
        void setDMA_InterruptFunction(void (*f)(void));
        void sendByte(uint8_t dat);
        void sendByte(uint8_t *dat, uint8_t len);
    };

} // namespace UART

#endif /* C7A23A25_2EEC_4976_ACA7_6108629DC386 */
