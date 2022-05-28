#include "w25q.hpp"
//#define  sFLASH_ID                       0xEF3015     //W25X16
//#define  sFLASH_ID                       0xEF4015	    //W25Q16
#define sFLASH_ID 0XEF4017 // W25Q64
//#define  sFLASH_ID                       0XEF4018    //W25Q128

//#define SPI_FLASH_PageSize            4096
#define PageSize 256
#define SectorSize 0x1000
#define SPI_FLASH_PerWritePageSize 256

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define WriteEnable 0x06
#define WriteDisable 0x04
#define ReadStatusReg 0x05
#define WriteStatusReg 0x01
#define ReadData 0x03
#define FastReadData 0x0B
#define FastReadDual 0x3B
#define PageProgram 0x02
#define BlockErase64 0xD8
#define BlockErase32 0x52
#define SectorErase 0x20
#define ChipErase 0xC7
#define PowerDown 0xB9
#define ReleasePowerDown 0xAB
#define DeviceID 0xAB
#define ManufactDeviceID 0x90
#define JedecDeviceID 0x9F

#define WIP_Flag 0x01 /* Write In Progress (WIP) flag */
#define Dummy_Byte 0xFF
/*命令定义-结尾*******************************/

using namespace w25q;

void W25Q::writeSector(uint16_t sectorNum, uint8_t *dat, uint16_t datLen)
{
    uint8_t pageCnt = datLen / (PageSize + 1);
    // uint8_t dummyPage = 16 - pageCnt - 1;
    this->erase(sectorNum);

    sectorNum *= 16;
    while (pageCnt--)
    {
        this->writePage(sectorNum, dat, PageSize);
        dat += PageSize;
        datLen -= PageSize;
        sectorNum ++;
    }
    this->writePage(sectorNum, dat, datLen);
}
void W25Q::writePage(uint16_t pageNum, uint8_t *dat, uint16_t datLen)
{
    u32_split address;
    address.val = pageNum * PageSize;

    uint16_t dummyLen = PageSize - datLen;
    this->enable();
    while (this->isBusy())
        ;
    this->cs->reset();
    this->spi->writeRead(PageProgram);
    this->spi->writeRead(address.unit[2]);
    this->spi->writeRead(address.unit[1]);
    this->spi->writeRead(address.unit[0]);
    while (datLen--)
        this->spi->writeRead(*dat++);
    while (dummyLen--)
        this->spi->writeRead(0xff);
    this->cs->set();
    while (this->isBusy())
        ;
}

void W25Q::read(uint32_t add, uint8_t *buf, uint16_t len)
{
    u32_split address;
    address.val = add;

    this->cs->reset();
    this->spi->writeRead(ReadData);
    this->spi->writeRead(address.unit[2]);
    this->spi->writeRead(address.unit[1]);
    this->spi->writeRead(address.unit[0]);
    while (len--)
        *buf++ = this->spi->writeRead();
    this->cs->set();
}

void W25Q::write(uint16_t pageNum, uint8_t *dat, uint16_t len)
{
    u32_split address;
    address.val = pageNum * 256;
    uint16_t len_left = len, cnt = 0;

    do
    {
        this->erase(pageNum / 16 + cnt++);
        len_left -= SectorSize;
    } while (len_left >= SectorSize);

    this->enable();
    while (this->isBusy())
        ;

    cnt = 0;
    while (len--)
    {
        if (cnt == 0)
        {
            this->cs->reset();
            this->spi->writeRead(PageProgram);
            this->spi->writeRead(address.unit[2]);
            this->spi->writeRead(address.unit[1]);
            this->spi->writeRead(address.unit[0]);
            address.val += 256;
        }

        this->spi->writeRead(*dat++);

        if (++cnt == 256)
        {
            cnt = 0;
            this->cs->set();
            while (this->isBusy())
                ;
        }
    }
}
void W25Q::erase(void)
{
    this->cs->reset();
    this->enable();
    while (this->isBusy())
        ;
    this->spi->writeRead(ChipErase);
    this->spi->writeRead();
    this->spi->writeRead();
    this->spi->writeRead();
    this->cs->set();
    while (this->isBusy())
        ;
}
void W25Q::erase(uint16_t count, uint8_t mode)
{
    u32_split address;
    this->enable();
    while (this->isBusy())
        ;

    this->cs->reset();
    switch (mode)
    {
    case 1:
        address.val = 0x8000 * count;
        this->spi->writeRead(BlockErase32);
        break;
    case 2:
        address.val = 0x10000 * count;
        this->spi->writeRead(BlockErase64);
        break;

    case 0:
    default:
        address.val = 0x1000 * count;
        this->spi->writeRead(SectorErase);
        break;
    }

    this->spi->writeRead(address.unit[2]);
    this->spi->writeRead(address.unit[1]);
    this->spi->writeRead(address.unit[0]);

    this->cs->set();

    while (this->isBusy())
        ;
}
void W25Q::enable(void)
{
    this->cs->reset();
    this->spi->writeRead(WriteEnable);
    this->cs->set();
}
bool W25Q::isBusy(void)
{
    uint8_t s;
    this->cs->reset();
    this->spi->writeRead(ReadStatusReg);
    s = this->spi->writeRead();
    this->cs->set();
    return s & 0x01;
}
void W25Q::disable(void)
{
    this->cs->reset();
    this->spi->writeRead(WriteDisable);
    this->cs->set();
}
uint32_t W25Q::getID(void)
{
    u32_split dat;

    this->cs->reset();
    // this->spi->sendOneByte(JedecDeviceID);
    this->spi->writeRead(JedecDeviceID);
    dat.unit[2] = this->spi->writeRead();
    dat.unit[1] = this->spi->writeRead();
    dat.unit[0] = this->spi->writeRead();
    this->cs->set();

    dat.unit[3] = 0;
    return dat.val;
}
