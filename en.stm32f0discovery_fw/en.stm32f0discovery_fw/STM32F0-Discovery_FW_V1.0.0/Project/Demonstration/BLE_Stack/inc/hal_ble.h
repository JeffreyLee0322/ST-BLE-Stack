#ifndef HAL_BLE_H
#define HAL_BLE_H

#include "userConfig.h"


/*
 * Convert RF channel to WL1600 PLL channel number.
 */
#define RF_CHN_TO_PLL_CHN(_x_)  ((_x_) * 2) 
#define RF_BASE_ADDR		0x800
/*
 * Macros to control WL1600.
 * Macros name their functions.
 */
#define ENABLE_BLE_SYNC_WORD() \
    wl1600_write_reg(73, 0xD6)

#define DISABLE_BLE_SYNC_WORD() \
    wl1600_write_reg(73, 0x00)

/* 调整VCO参考电流 */
#define WRITE_TX_VCO() \
    wl1600_write_reg(52, 0x1A)

/* 调整VCO参考电流 */
#define WRITE_RX_VCO() \
    wl1600_write_reg(52, 0x1E)

#define ENABLE_PACK_LEN() \
    wl1600_write_reg(82, 0x31)

#define DISABLE_PACK_LEN() \
    wl1600_write_reg(82, 0x11)

#define ENABLE_PKT_FLAG_INTR() \
    wl1600_write_reg(33, 0x02)

/*#if (USE_SH77 == 1)
#define POLL_PKT_FLAG_INTR_STAT() \
    WL1600_PKT_FLAG_PIN
#else*/
#define POLL_PKT_FLAG_INTR_STAT() \
    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)
//#endif

/* Currently no effect */
#define ACK_PKT_FLAG_INTR_STAT() \
    wl1600_write_reg(37, 0x02)

#define RESET_FIFO_WR_PTR() \
    wl1600_write_reg(104, 0x80)

#define READ_FIFO_WR_PTR() \
    wl1600_read_reg(104)

#define RESET_FIFO_RD_PTR() \
    wl1600_write_reg(105, 0x80)

#define READ_FIFO_RD_PTR() \
    wl1600_read_reg(104)

#define READ_FIFO() \
    wl1600_read_reg(100)

#define READ_FIFO_LEN(_buf_, _len_) \
    wl1600_read_fifo(100, _buf_, _len_)

#define WRITE_FIFO(_x_) \
    wl1600_write_reg(100, _x_)

#define WRITE_FIFO_LEN(_buf_, _len_) \
    wl1600_write_fifo(100, _buf_, _len_)

#define ENABLE_RX() \
    wl1600_write_reg_bit(15, 7, 1)

#define DISABLE_RX() \
    wl1600_write_reg_bit(15, 7, 0)

#define ENABLE_TX() \
    wl1600_write_reg_bit(14, 0, 1)

#define DISABLE_TX() \
    wl1600_write_reg_bit(14, 0, 0)

#define SET_1600_SLEEP_MODE() \
    wl1600_write_reg_bit(70, 6, 1)
    
#define SET_1600_IDLE_MODE() \
    wl1600_write_reg_bit(70, 6, 0)

/*
extern void wl1600_init_regs(void);
extern u8 wl1600_check_regs(void);
extern void wl1600_init(void);
extern void wl1600_write_reg(u8 reg, u8 val);
extern void write_ll_reg(u16 reg, u8 val);
extern u8 read_ll_reg(u16 reg);
extern void wl1600_write_fifo(u8 reg, u8 *buf, u8 len);
extern u8 wl1600_read_reg(u8 reg);
extern void wl1600_read_fifo(u8 reg, u8 *buf, u8 len);
extern void wl1600_write_reg_bit(u8 reg, u8 bits, u8 set);
extern u8 wl1600_read_reg_bit(u8 reg, u8 bits);
extern void wl1600_set_rf_channel(u8 chn);*/

void wl1600_init(void);

#endif

