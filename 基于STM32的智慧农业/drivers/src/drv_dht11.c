#include "drv_dht11.h"

#define DT GPIO_Pin_3

#define  DHT11_DQ_HIGH     GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define  DHT11_DQ_LOW      GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define  DHT11_DQ_READ     GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3)

void DHT11_IO_OUT(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}

void DHT11_IO_IN(void)
{
	 GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN ;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	//GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
}
 void DHT11_Rst(void) 
 { 
  DHT11_IO_OUT(); //SET OUTPUT
  DHT11_DQ_LOW; //����DQ
  drv_systick_delayms(20);//��������18ms
  DHT11_DQ_HIGH; //DQ=1
	 drv_systick_delayus(30);
   //��������20~40us
 }
 
 u8 DHT11_Check(void) 
 { 
 u8 retry=0;
 DHT11_IO_IN();//SET INPUT? ? ? ???
while (DHT11_DQ_READ&&retry<100)
  {
		retry++;
		drv_systick_delayus(1);
  }
 if(retry>=100)return 1;
 else retry=0;
  while (!DHT11_DQ_READ&&retry<100)//DHT11���ͺ���ٴ�����40~80us
  {
  retry++;
  drv_systick_delayus(1);
 }
 if(retry>=100)return 1; 
 return 0;
 }
 
 u8 DHT11_Read_Bit(void) 
 {
 u8 retry=0;
   while(DHT11_DQ_READ&&retry<100)//�ȴ���Ϊ�͵�ƽ
 {
 retry++;
drv_systick_delayus(1);
  }
  retry=0;
 while(!DHT11_DQ_READ&&retry<100)//�ȴ���ߵ�ƽ
  {
 retry++;
 drv_systick_delayus(1);
  }
  drv_systick_delayus(40);//�ȴ�40us
  if(DHT11_DQ_READ)return 1;
  else return 0; 
 }
 
 u8 DHT11_Read_Byte(void) 
 { 
 u8 i,dat;
 dat=0;
  for (i=0;i<8;i++)
 {
 dat<<=1;
  dat|=DHT11_Read_Bit();
 } 
  return dat;
 }
 
 
 u8 DHT11_Read_Data(u8 *temp,u8 *humi) 
 { 
 u8 buf[5];
  u8 i;
 DHT11_Rst();
  if(DHT11_Check()==0)
  {
  for(i=0;i<5;i++)//��ȡ40λ����
 {
  buf[i]=DHT11_Read_Byte();
  }
 if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
 {
  *humi=buf[0];
  *temp=buf[2];
  }
  }
  else return 1;
 return 0; 
 }
 
 u8 DHT11_Init(void)
 { 
 GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��PG�˿�ʱ��
  GPIO_InitStructure.GPIO_Pin = DT; //PG11�˿�����
 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��IO��
		GPIO_SetBits(GPIOA,DT); //PG11 ����� 
		DHT11_Rst(); //��λDHT11
 return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
 }

