#include "delay.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#if 0
static unsigned char fac_us=0;//us延时倍乘数
static unsigned short fac_ms=0;//ms延时倍乘数
//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(unsigned char SYSCLK)
{
    SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟 HCLK/8
    fac_us=SYSCLK/8;
    fac_ms=(unsigned short)fac_us*1000;
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(unsigned short nms)
{
    unsigned int temp;
    SysTick->LOAD=(unsigned int)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
    SysTick->VAL =0x00; //清空计数器
    SysTick->CTRL=0x01 ; //开始倒数
    do
    {
    temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器
}
//延时nus
//nus为要延时的us数.
void delay_us(unsigned int nus)
{
    unsigned int temp;
    SysTick->LOAD=nus*fac_us; //时间加载
    SysTick->VAL=0x00; //清空计数器
    SysTick->CTRL=0x01 ; //开始倒数
    do
    {
    temp=SysTick->CTRL;
    }
    while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器
}
#endif