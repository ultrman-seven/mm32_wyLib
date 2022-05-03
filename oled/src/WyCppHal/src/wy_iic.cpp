#include "common.h"

namespace IIC
{
    struct iicInitStruct
    {
    };

    class IIC_Object
    {
    private:
        uint8_t slaveAdd;

        void ack(bool a);
        void start(void);
        void end(void);
        /* data */
    public:
        IIC_Object() = default;
        IIC_Object(iicInitStruct &i) { init(i); }

        void init(iicInitStruct &i);
        void sendByte(uint8_t dat);
        void getByte(void);

        template <typename dataType>
        void sendData(dataType dat);
    };

} // namespace IIC

using namespace IIC;
