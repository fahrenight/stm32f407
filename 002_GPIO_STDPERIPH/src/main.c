
#include "stm32f4xx.h"

GPIO_InitTypeDef GPIO_InitStruct; // GPIO Türünden bir nesne oluþturduk
uint32_t time = 3360000;
int counter=-1; // STM Studio Viewer de görünmediði için global bir deðiþken olarak tanýmladým 4 ledin yanýþýný gözlemlemek için


void GPIO_Config();
void delay (uint32_t time);
void GPIO_LED_button_toggle();
void GPIO_LED_button_counter();
void GPIO_7segment();


int main(void)
{
	GPIO_Config();



  while (1)
  {
	  delay(time);
	  //GPIO_LED_button_toggle(); // Butona bastýðýmýzda yanan uygulama
	  //GPIO_LED_button_counter(); // Bir counter ile LED lerimizi yakýp ST studio da inceleyeceðimiz foksiyon
	  GPIO_7segment();
  }
}


void GPIO_Config(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // 7 segment uygulamasý için GPIO E aktif hale getirildi

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // Buraya buton ekleneceði için dijital giriþ olarak seçildi
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; // GPIO_A_0 a buton eklendi
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ; //Burada kullanýlacak direnç pull down direnciydi ve push pull yolunu kullanmamýz gerekiyor.
	//Bu özelliðe þematik üzerinden ulaþabiliriz.
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;//Buton Pull down baðlandýðý için
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; //Kullanýlan butonun hýzýný seçtik

	GPIO_Init(GPIOA, &GPIO_InitStruct); // Yaptýðýmýz tanýmlarý GPIO portunun baþlatýlmasý için kullandýk ve yükledik

	// Böylece buraya kadar buton tanýmlamasý ve özelliklerini seçmiþ olduk.
	// Þimdi tekrar port açmamýz lazým. Bunun için D Portuna baðlý LED leri kullanacaðýz ve konfigurasyonlarýný yapacaðýz.
	// Kod bakýmýndan daha düzenli olsun diye üst tarafta tanýmlamasý yapýldý.

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //LEDler için farketmez. OpenDrain seçildiði için push pull ayarý yapýlmaya da gerek kalmadý

	GPIO_Init(GPIOD, &GPIO_InitStruct); //Yaptýðýmýz ayarlarý D portuna yüklemiþ olduk.
	//Artýk buradan sonra GPIO portundan veri okumaya geçebiliriz.

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOB, &GPIO_InitStruct);


	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	// 7 segment için kullanacaðýmýz gnd hariç 8 biti GPIO pinlerine yüklenmek üzere özellik kazandýrýyoruz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void delay (uint32_t time){
	while(time--);
}


void GPIO_LED_button_toggle(){
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){ // butona basýlýp basýlmadýðýný kontrol edeceðiz
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_PinSource0)); // Butona hala basýlýp basýlmadýðýný kontrol et. Basýldýðý sürece döngüde kal
		delay(time); //Freq=168MHZ ise 1 sn 168m yapar o halde 0,2ms için 3.360.000 olmasý gerekiyor

		GPIO_ToggleBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_12); //12 ve 13. pini toggle et
		GPIO_ToggleBits(GPIOD,GPIO_Pin_14 | GPIO_Pin_15); // 14 ve 15i yak

	}
}


void GPIO_LED_button_counter(){ // Mod 4 e göre ledleri sýrasýyla yakýp sýrasýyla söndüren ve 20ye geldiði zaman tüm led leri söndürüp en baþtan baþlayan kod yapýsý
	int var=20;//;

	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0));
		delay(time);

		if(counter == var){
			counter = -1;
			GPIO_WriteBit(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15,RESET);
			delay(time);
		}
		else{
			counter++;
			switch(counter % 4){
			case 1:
				GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
				break;
			case 2:
				GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
				break;
			case 3:
				GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
				break;
			case 0:
				GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
				break;
			default:
				break;
			}
		}

	}



}


void GPIO_7segment(){




	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)){
		while(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0));
		delay(time);
		counter++;
	}


		//A= PA0	B= PA1	C= PA2	D= PA3	E=PA4	F= PA5	G= PA6	DP= PA7
		switch(counter){
		case 0:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
			GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);
			break;
		}
		case 1:{
			GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
			break;
		}
		case 2:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_4);
			GPIO_ResetBits(GPIOA, GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_7);
			break;
		}
		case 3:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 );
			GPIO_ResetBits(GPIOA, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7);
			break;
		}
		case 4:{
			GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
			break;
		}
		case 5:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 |  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,  GPIO_Pin_1 |  GPIO_Pin_4 | GPIO_Pin_7);
			break;
		}
		case 6:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
			break;
		}
		case 7:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
			GPIO_ResetBits(GPIOA,  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
			break;
		}
		case 8:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,  GPIO_Pin_7);
			break;
		}
		case 9:{
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6);
			GPIO_ResetBits(GPIOA,  GPIO_Pin_4 | GPIO_Pin_7);
			break;
		}
		case 10:{
			GPIO_SetBits(GPIOA, GPIO_Pin_7 );
			GPIO_ResetBits(GPIOA,  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
			break;
		}
		default:
			break;
			//A= PA0	B= PA1	C= PA2	D= PA3	E=PA4	F= PA5	G= PA6	DP= PA7
		}


	if (counter == 11){
		counter = 0;
	}
}
