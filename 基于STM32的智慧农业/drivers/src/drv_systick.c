#include "drv_systick.h"
static uint8_t fs_us=0; // US��Ҫ�ǵĴ���
static uint16_t fs_ms=0;//ms��Ҫ�ƵĴ���
/**********************************************************
��������drv_systick_init

�������ܣ��δ�ʱ���ĳ�ʼ��

���������systick ϵͳʱ��

�����������

����˵����	ʱ��Դʹ��8����

**********************************************************/
void drv_systick_init(uint8_t systick)
{
	SysTick->CTRL &= ~0x05; //8��Ƶ���رն�ʱ��
	fs_us = systick/8;  //��1US��Ҫ��21��
	fs_ms = fs_us*1000;
}

/**********************************************************
��������drv_systick_delayms

�������ܣ�MS��ʱ����

���������systick ϵͳʱ��

�����������

����˵����	

**********************************************************/
void drv_systick_delayms(uint16_t ms)
{
	uint32_t temp=0;
	uint8_t  systick_flg=0;
	while(systick_flg == 0)
	{
		if(ms > 798)
		{
			SysTick->LOAD = fs_ms*798;//װ��ֵ
			ms = ms - 798;
		}
		else
		{

			SysTick->LOAD = fs_ms*ms;//װ��ֵ
			systick_flg =1;

		}
		SysTick->VAL = 0;//��ǰֵ
		SysTick->CTRL |= 0x01;//������ʱ����
		do
		{
			temp = SysTick->CTRL;//��ȡ������־
		}
		while (!(temp&(0x01<<16)));//�ȴ���ʱʱ�䵽
		SysTick->CTRL &= ~0x01;//�رն�ʱ��
	}
	
}
/**********************************************************
��������drv_systick_delayus

�������ܣ�us��ʱ����

���������systick ϵͳʱ��

�����������

����˵����	

**********************************************************/
void drv_systick_delayus(uint16_t us)
{
	uint32_t temp=0;
	SysTick->LOAD = fs_us*us;//װ��ֵ
	SysTick->VAL = 0;//��ǰֵ
	SysTick->CTRL |= 0x01;//������ʱ����
	do
	{
		temp = SysTick->CTRL;//��ȡ������־
	}
	while (!(temp&(0x01<<16)));//�ȴ���ʱʱ�䵽
	SysTick->CTRL &= ~0x01;//�رն�ʱ��
}

