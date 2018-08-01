#include "hal_ble.h"
#include "hal_spi.h"

/* Read operation */
#define LL_OP_READ      		(0x8000)
#define WL1600_OP_READ      (0x8000)

#define WL1600_ENABLE()  						(BLE_SPI_CE_SetHigh())   
#define WL1600_DISABLE()  					(BLE_SPI_CE_SetLow())
#define WL1600_SPI_SELECT()   			(BLE_SPI_CS_Enable_())
#define WL1600_SPI_DESELECT()   		(BLE_SPI_CS_Disable_())


static void wl1600_init_regs(void);
static u8 wl1600_check_regs(void);
static void wl1600_config_gpio(void);
void write_ll_reg(u16 reg, u8 val);
u8 read_ll_reg(u16 reg);
void wl1600_write_reg(u8 reg, u8 val);
u8 wl1600_read_reg(u8 reg);

/*
 * Default WL1600 register settings.
 */
#define REG_END_MARK    (255)
const u8 wl1600_regs[][2] = {   //小米5能收到
    {0,  0x6F},
    {1,  0xE8},
    {2,  0x56},
    {3,  0x0D},
    {4,  0xC4}, // C4
    {8,  0x25}, // 05 | 25
    {9,  0x41},
    {10, 0x57},
    {11, 0x3B},
    {16, 0x84},
    {17, 0x7A}, // 7B | 5A
    {18, 0x08},
    {19, 0x00},
    {27, 0xB4},
    {34, 0x12},
    {35, 0x00},
    {45, 0x06},
    {46, 0x01},
    {47, 0x15},
    {49, 0x5B},
    {50, 0x15},
    {51, 0x14},
    {52, 0x1A},
    {53, 0x00},
    {64, 0x78},
    {66, 0x60},
    {67, 0xA0},
    {68, 0x80},
    {94, 0x00},
		
		
    {255, 255} 
};

/**
 * Initialize WL1600 registers.
 */
void wl1600_init_regs(void)
{
    u8 i;

    for (i = 0; wl1600_regs[i][0] != REG_END_MARK; ++i) {
        wl1600_write_reg(wl1600_regs[i][0], wl1600_regs[i][1]);
    }
}

/**
 * Check if WL1600 register values match default settings.
 *
 * @return REG_END_MARK if register values are correct; otherwise, the address
 * of the register with incorrect value is returned.
 */
u8 wl1600_check_regs(void)
{
    u8 i;

    for (i = 0; wl1600_regs[i][0] != REG_END_MARK; ++i) {
        if (wl1600_read_reg(wl1600_regs[i][0]) != wl1600_regs[i][1]) {
            return (i);
        }
    }
    
    return (REG_END_MARK);
}

/**
 * Configure GPIO for WL1600.
 */
static void wl1600_config_gpio(void)
{
/*
#if (USE_SH77 != 1)  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    CONFIG_GPIO_OUTPUT_OUT_PP(WL1600_CSN_PORT, WL1600_CSN_PIN);
    CONFIG_GPIO_OUTPUT_OUT_PP(WL1600_CE_PORT, WL1600_CE_PIN);
    CONFIG_GPIO_INPUT_FLOATING(WL1600_PKT_FLAG_PORT, WL1600_PKT_FLAG_PIN);
#endif  */
    WL1600_DISABLE();
    WL1600_SPI_DESELECT();
}

/**
 * Initialize WL1600 and communication interface.
 */
void wl1600_init(void)
{
    wl1600_config_gpio();
    //spi_init();

    /* Reset WL1600 */
    WL1600_DISABLE();
	
#if (USE_SH77 == 1) 
    delay_10us();
    delay_10us();
#else
    delay_us(20);
#endif
	
    WL1600_ENABLE();
	
#if (USE_SH77 == 1) 
     delay_ms(1);
#else
    delay_us(1000);
#endif
    
    /* Configure WL1600 */
    wl1600_init_regs();
}



/**
 * Write value to specific register.
 *
 * @param reg The register address.
 * @param val The register value.
 */
void write_ll_reg(u16 reg, u8 val)
{
    WL1600_SPI_SELECT();
    //spi_read_write_byte(reg >> 8);    //高8bit
    //spi_read_write_byte(reg);         //低8bit
		BLE_SPI_WriteRead(reg);
    //spi_read_write_byte(val);
		BLE_SPI_WriteRead(val);
    WL1600_SPI_DESELECT();    
}

/**
 * Read value from specific register.
 *
 * @param reg The register address.
 *
 * @return The register address.
 */
u8 read_ll_reg(u16 reg)
{
    u8 val_ll;
    WL1600_SPI_SELECT();
    //spi_read_write_byte((reg | LL_OP_READ) >> 8);   //高8bit
    //spi_read_write_byte(reg | LL_OP_READ);         //低8bit
		BLE_SPI_WriteRead(reg | LL_OP_READ);
    //val_ll = spi_read_byte();
		val_ll = BLE_SPI_WriteRead(0);
    WL1600_SPI_DESELECT(); 
    return (val_ll);   
}

/**
 * Write value to specific register.
 *
 * @param reg The register address.
 * @param val The register value.
 */
void wl1600_write_reg(u8 reg, u8 val)
{
    WL1600_SPI_SELECT();
    delay_us(5);
    //spi_read_write_byte((reg | RF_BASE_ADDR) >> 8);  //高8bit
    //spi_read_write_byte(reg | RF_BASE_ADDR);        //低8bit
		BLE_SPI_WriteRead(reg | RF_BASE_ADDR);
    //spi_read_write_byte(val);
		BLE_SPI_WriteRead(val);
    delay_us(5);
    WL1600_SPI_DESELECT();     
}

/**
 * Read value from specific register.
 *
 * @param reg The register address.
 *
 * @return The register address.
 */
u8 wl1600_read_reg(u8 reg)
{
    u8 val;
    
    WL1600_SPI_SELECT();
    delay_us(5);
    //spi_read_write_byte(((reg + RF_BASE_ADDR) | WL1600_OP_READ) >> 8);  //高8bit
    //spi_read_write_byte((reg + RF_BASE_ADDR) | WL1600_OP_READ);         //低8bit
		BLE_SPI_WriteRead((reg + RF_BASE_ADDR) | WL1600_OP_READ);
    //val = spi_read_byte();
		val = BLE_SPI_WriteRead(0);
    delay_us(5);
    WL1600_SPI_DESELECT();
    
    return (val);
}
