#include "i2c.h"
#include "stm32f0xx_i2c.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#if 0
void i2c_gpio_config()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void i2c_init()
{
    I2C_InitTypeDef I2C_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0xA0;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;
    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);
}

void I2C_Write(unsigned char addr, unsigned char data)
{
    I2C_AcknowledgeConfig(I2C1,ENABLE); //ʹ��Ӧ��
    I2C_GenerateSTART(I2C1,ENABLE); //����һ����ʼλ
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;}//�ȴ�EV5
    I2C_Send7bitAddress(I2C1,0xA0,I2C_Direction_Transmitter); //���ʹӵ�ַ
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;} //�ȴ�EV6
    I2C_SendData(I2C1,addr); //����Ҫд�����ݵĵ�ַ
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //�ȴ�EV8
    I2C_SendData(I2C1,data); //����Ҫд�������
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //�ȴ�EV8
    I2C_GenerateSTOP(I2C1,ENABLE); //����ֹͣλ
}

unsigned char I2C_Read(unsigned char nAddr)
{
    I2C_AcknowledgeConfig(I2C1,ENABLE); //ʹ��Ӧ��
    I2C_GenerateSTART(I2C1,ENABLE); //����һ����ʼλ
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;} //�ȴ�EV5
    I2C_Send7bitAddress(I2C1,0xA0,I2C_Direction_Transmitter); //����һ��αдָ��
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){;}//�ȴ�EV6
    I2C_SendData(I2C1,nAddr);//���Ͷ���ַ
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)){;} //�ȴ�EV8

    I2C_GenerateSTART(I2C1,ENABLE); //����һ����ʼλ
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)){;} //�ȴ�EV5
    I2C_Send7bitAddress(I2C1,0xA0,I2C_Direction_Receiver); //����һ����ָ��
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){;} //�ȴ�EV6
    I2C_AcknowledgeConfig(I2C1,DISABLE); //Ӧ��ʹ�ܹر�
    I2C_GenerateSTOP(I2C1,ENABLE); //����һ��ֹͣλ
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)){;} //�ȴ�EV7
    return I2C_ReceiveData(I2C1); //���ض���������
}
#endif