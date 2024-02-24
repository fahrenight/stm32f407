
#include "stm32f4xx.h"

GPIO_InitTypeDef GPIO_InitStruct;
EXTI_InitTypeDef EXTI_InitStruct;
NVIC_InitTypeDef NVIC_InitStruct;

void GPIO_Config();
void EXTI_Config();
void Delay(uint32_t time);
void EXTI1_IRQHandler();

uint32_t time = 3200000;

int main(void)
{
	GPIO_Config();
	EXTI_Config();
while(1)
  {
		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
		Delay(time);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
		Delay(time);
  }
}


void GPIO_Config(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOA,&GPIO_InitStruct);


}


void EXTI_Config(){ //EXTI içinde Delay kullanýlmaz ))
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);



	EXTI_InitStruct.EXTI_Line = EXTI_Line1 | EXTI_Line2; // pin ve pin2 olan 2 tane butondan kesme alacaðýz
	EXTI_InitStruct.EXTI_LineCmd = ENABLE; // secilen hatlari aktif duruma getirdik
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; //secilen hatlarin bir interrupt olacagini belirttik
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; // yukselen kenar ile interrupt yaptik
	//Buraya kadar interrupt clock hattý acildi line 1 ve line 2 hatlari da acilip nasýl calisaacagi belirtildi fakat bunlari hangi hat üzerinde yapacagini belirtmedik.
	//Artik burada hangi hat üzerinde yapacagimizi da belirtmeliyiz.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1 | EXTI_PinSource2);//Hangiportta kullanacagimizi da belirttik
	//Artik IT leri yukleyebiliriz.

	EXTI_Init(&EXTI_InitStruct);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //1bit on hazirlik icin yeterli olacagindan dolayi bunu kullandik
//Bu fonksiyonda cortex M3 ve M4 mikrdenetleyicilerde 4 bit ayrilir. Group1 = preemption =1bit,subpriority=3bit.
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct); // Böylece EXTI1 hattýný 0. oncelige koymus olduk. preemptionpriority kýsmýnda bu durumdan bahsettiysek
//Subpriority de bundan bahsetmeye gerek yok. O yuzden 0 yazdik.

	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1; //Onceligi 0-15 arasinda bir deger yazabiliriz fakat 0 dolu oldugu icin 1-15 arasi bir deger yazmaliyiz
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;

	NVIC_Init(&NVIC_InitStruct);
}



void Delay(uint32_t time){
	while(time--);
}

void EXTI1_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line1) != RESET){
			GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_IRQHandler(){
	if(EXTI_GetITStatus(EXTI_Line2) != RESET){
		GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}





