/*  Benjamin DELPY `gentilkiwi`
    https://blog.gentilkiwi.com
    benjamin@gentilkiwi.com
    Licence : https://creativecommons.org/licenses/by/4.0/
*/
#include "trf7970a.h"

void TRF7970A_init()
{
    TRF_CS_DISABLE(); // just in case
    
    TRF_DISABLE();
    TIMER_delay_Milliseconds(10);
    TRF_ENABLE();
    TIMER_delay_Milliseconds(10);
    // 6.11 TRF7970A Initialization
    TRF7970A_SPI_DirectCommand(TRF79X0_SOFT_INIT_CMD);
    __no_operation();
    __no_operation();
    TRF7970A_SPI_DirectCommand(TRF79X0_IDLE_CMD);
    TIMER_delay_Milliseconds(1);
    TRF7970A_SPI_DirectCommand(TRF79X0_RESET_FIFO_CMD);
    TRF7970A_SPI_Write_SingleRegister(TRF79X0_CHIP_STATUS_CTRL_REG, 0x00);
    TRF7970A_clearIrqStatus();

    TRF_IRQ_ENABLE();
}

void TRF7970A_SPI_Send_raw(const uint8_t *pcbData, uint8_t cbData)
{
    TRF_CS_ENABLE();
    
    
    spi_write_blocking(PIKO_SPI, pcbData, cbData);
    
    
    TRF_CS_DISABLE();
}

void __time_critical_func(TRF7970A_SPI_DirectCommand_internal)(uint8_t CommandCode_Preparred) // be careful, optimization make multiple direct commands too fast, a cycle between can help...
{
    TRF_CS_ENABLE();
    spi_write_blocking(PIKO_SPI, &CommandCode_Preparred, 1);
    TRF_CS_DISABLE();
}

uint8_t __time_critical_func(TRF7970A_SPI_Read_SingleRegister_internal)(uint8_t Register_Prepared)
{
    uint8_t buffer[2] = {Register_Prepared, };

    TRF_CS_ENABLE();
    spi_write_read_blocking(PIKO_SPI, buffer, buffer, sizeof(buffer));

    TRF_CS_DISABLE();

    return buffer[1];
}

void __time_critical_func(TRF7970A_SPI_Write_SingleRegister_internal)(uint8_t Register_Prepared, const uint8_t Value)
{
    uint8_t buffer[2] = {Register_Prepared, Value};
    
    TRF_CS_ENABLE();
    spi_write_blocking(PIKO_SPI, buffer, sizeof(buffer));
    TRF_CS_DISABLE();
}

void __time_critical_func(TRF7970A_SPI_Read_ContinuousRegister_internal)(uint8_t Register_Prepared, uint8_t *pbData, uint8_t cbData)
{
    TRF_CS_ENABLE();

    spi_write_blocking(PIKO_SPI, &Register_Prepared, 1);

    spi_read_blocking(PIKO_SPI, 0x00, pbData, cbData);


    TRF_CS_DISABLE();
}

void __time_critical_func(TRF7970A_SPI_Write_Packet_TYPED)(const uint8_t *pcbData, uint8_t cbData, const uint8_t type)
{
    uint16_t ui16TotalLength = cbData;
    uint8_t buffer[5] = {
        MK_DC(TRF79X0_RESET_FIFO_CMD),
        type,
        MK_WC(TRF79X0_TX_LENGTH_BYTE1_REG),
        (uint8_t) ((ui16TotalLength & 0x0ff0) >> 4),    // in TRF79X0_TX_LENGTH_BYTE1_REG
        (ui16TotalLength & 0x0f) << 4,                  // in TRF79X0_TX_LENGTH_BYTE2_REG
    };
        


    TRF_CS_ENABLE();

    spi_write_blocking(PIKO_SPI, buffer, sizeof(buffer));

    spi_write_blocking(PIKO_SPI, pcbData, cbData);

    TRF_CS_DISABLE();
}

uint8_t __time_critical_func(TRF7970A_SPI_waitIrq)()
{
    while(!g_irq_TRF)
    {
        __low_power_mode_0();
    }
    g_irq_TRF = false;

    return TRF7970A_getIrqStatus();
}
