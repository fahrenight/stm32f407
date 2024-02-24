
#include "stm32f4xx.h"

GPIO_InitTypeDef GPIO_InitStruct; // GPIO structerlar�n� �ektik
EXTI_InitTypeDef EXTI_InitStruct; // Interrupt structerlar� �ekildi
NVIC_InitTypeDef NVIC_InitStruct; // NVIC structerlar�n� �ektik

void GPIO_Config();
void EXTI_Config();
void Delay(uint32_t time);
void EXTI0_IRQHandler();

uint32_t time = 3200000; //200 ms delay komutu

int main(void)
{
	GPIO_Config(); //GPIO ayarlamalar�n� yaparak ba�lat�ld�
	EXTI_Config();
	while (1)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_12);
		Delay(time);
		GPIO_SetBits(GPIOD,GPIO_Pin_13);
		Delay(time);
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
		Delay(time);
	}
}

void GPIO_Config(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//GPIO pertlar�n� a�mak i�in AHB1 clock hatt�n� aktif hale getiriyoruz
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//Buton i�in A portunun RCC clocklar� a��ld�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //Interrupt i�in istenen fonksiyon EXTI yani external interrupt fakat bu
	//i�lem syscfg i�inde oldu�u i�in bu �ekilde kullan�l�r
	//NVIC aktif etmeye gerek yok ��nk� NVIC i�lemcinin i�inde ve clock hatlar�na dahil de�ildir. O y�zden onun clock hatt�n� aktif
	//etmeye gerek yok

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; // Buton pull down ba�l� oldu�u i�in

	GPIO_Init(GPIOD,&GPIO_InitStruct);
	// GPIO HATLARI AKT�F ED�LD� VE NASIL �ALI�ILACA�I B�LD�R�LD�
}


void EXTI_Config(){
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,GPIO_PinSource0); // A portunun 0. pinini interrupt olarak kullanaca��m�z� belirttik.
	//B�ylece art�k bu hattan gelen data mikroi�lemciyi kesmeye u�rataca��n� mikroi�lemciye belirtmi� olduk

	EXTI_InitStruct.EXTI_Line = EXTI_Line0; //Portlar�n�n 0. pinleri Line 0a ba�l�d�r o y�zden 0. pin se�mek istedi�imiz i�in
	//Line0 oalrak tan�mlama yapt�k.
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; // y�kselen kenarda interrupt a girmesini istedi�imizi belirttik.
	//B�ylece Butona bas�ld��� anda y�kselen kenarda kesmeye gidecek

	EXTI_Init(&EXTI_InitStruct); // Interrrupta y�kleme yapt�k

	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; //EXTI0_IRQn ��nk� interrupt line 0 � kullan�yoruz. Bu ayn� zamanda interrupt �ncelik s�ralamas�nda 6. s�rada yer alan interruptt�r
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//Kullanaca��m�z interrupt i�in bir �ncelik s�ralamas�na sokuyoruz.
// Bu ifade birden fazla interrupt kullan�laca�� zman ka��nc� s�rada kullanaca��m�z� belirtiyoruz
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //Birden fazla interrupt kullan�lacaksa ona g�re bir s�raalama yapmam�z�
	//istiyor. Fakat 1 interrupt kullanaca��m�z i�in �nemli de�il.

	NVIC_Init(&NVIC_InitStruct);
}


void Delay(uint32_t time){
	while(time--);


}

void EXTI0_IRQHandler(){
	// �nterrupt a girilip girilmedi�i bu fonksiyonun i�ine girilmesiyle alakl�d�r. Interrupt flag ini kontrol ederek bu
	//fonksiyona girilip girilmedi�i anla��labilir.
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){//Bu eXTI flagi reset de�ilse yani set edilmi�se kesinlikle interrrupt a girilmi� dmektir
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		Delay(time);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		Delay(time);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		Delay(time);
		GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,RESET);
		Delay(3*time);
	}

	EXTI_ClearITPendingBit(EXTI_Line0); //IT flagini temizleyerek tekrar kullanabilmenin �n� a��ld�
}
