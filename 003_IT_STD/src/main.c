
#include "stm32f4xx.h"

GPIO_InitTypeDef GPIO_InitStruct; // GPIO structerlarýný çektik
EXTI_InitTypeDef EXTI_InitStruct; // Interrupt structerlarý çekildi
NVIC_InitTypeDef NVIC_InitStruct; // NVIC structerlarýný çektik

void GPIO_Config();
void EXTI_Config();
void Delay(uint32_t time);
void EXTI0_IRQHandler();

uint32_t time = 3200000; //200 ms delay komutu

int main(void)
{
	GPIO_Config(); //GPIO ayarlamalarýný yaparak baþlatýldý
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//GPIO pertlarýný açmak için AHB1 clock hattýný aktif hale getiriyoruz
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//Buton için A portunun RCC clocklarý açýldý
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE); //Interrupt için istenen fonksiyon EXTI yani external interrupt fakat bu
	//iþlem syscfg içinde olduðu için bu þekilde kullanýlýr
	//NVIC aktif etmeye gerek yok çünkü NVIC iþlemcinin içinde ve clock hatlarýna dahil deðildir. O yüzden onun clock hattýný aktif
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
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN; // Buton pull down baðlý olduðu için

	GPIO_Init(GPIOD,&GPIO_InitStruct);
	// GPIO HATLARI AKTÝF EDÝLDÝ VE NASIL ÇALIÞILACAÐI BÝLDÝRÝLDÝ
}


void EXTI_Config(){
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA,GPIO_PinSource0); // A portunun 0. pinini interrupt olarak kullanacaðýmýzý belirttik.
	//Böylece artýk bu hattan gelen data mikroiþlemciyi kesmeye uðratacaðýný mikroiþlemciye belirtmiþ olduk

	EXTI_InitStruct.EXTI_Line = EXTI_Line0; //Portlarýnýn 0. pinleri Line 0a baðlýdýr o yüzden 0. pin seçmek istediðimiz için
	//Line0 oalrak tanýmlama yaptýk.
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; // yükselen kenarda interrupt a girmesini istediðimizi belirttik.
	//Böylece Butona basýldýðý anda yükselen kenarda kesmeye gidecek

	EXTI_Init(&EXTI_InitStruct); // Interrrupta yükleme yaptýk

	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; //EXTI0_IRQn çünkü interrupt line 0 ý kullanýyoruz. Bu ayný zamanda interrupt öncelik sýralamasýnda 6. sýrada yer alan interrupttýr
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;//Kullanacaðýmýz interrupt için bir öncelik sýralamasýna sokuyoruz.
// Bu ifade birden fazla interrupt kullanýlacaðý zman kaçýncý sýrada kullanacaðýmýzý belirtiyoruz
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; //Birden fazla interrupt kullanýlacaksa ona göre bir sýraalama yapmamýzý
	//istiyor. Fakat 1 interrupt kullanacaðýmýz için önemli deðil.

	NVIC_Init(&NVIC_InitStruct);
}


void Delay(uint32_t time){
	while(time--);


}

void EXTI0_IRQHandler(){
	// ýnterrupt a girilip girilmediði bu fonksiyonun içine girilmesiyle alaklýdýr. Interrupt flag ini kontrol ederek bu
	//fonksiyona girilip girilmediði anlaþýlabilir.
	if(EXTI_GetITStatus(EXTI_Line0) != RESET){//Bu eXTI flagi reset deðilse yani set edilmiþse kesinlikle interrrupt a girilmiþ dmektir
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		Delay(time);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		Delay(time);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		Delay(time);
		GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,RESET);
		Delay(3*time);
	}

	EXTI_ClearITPendingBit(EXTI_Line0); //IT flagini temizleyerek tekrar kullanabilmenin önü açýldý
}
