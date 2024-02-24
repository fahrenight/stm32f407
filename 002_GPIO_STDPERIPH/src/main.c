
#include "stm32f4xx.h"

GPIO_InitTypeDef GPIO_InitStruct; // GPIO T�r�nden bir nesne olu�turduk
uint32_t time = 3360000;
int counter=-1; // STM Studio Viewer de g�r�nmedi�i i�in global bir de�i�ken olarak tan�mlad�m 4 ledin yan���n� g�zlemlemek i�in


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
	  //GPIO_LED_button_toggle(); // Butona bast���m�zda yanan uygulama
	  //GPIO_LED_button_counter(); // Bir counter ile LED lerimizi yak�p ST studio da inceleyece�imiz foksiyon
	  GPIO_7segment();
  }
}


void GPIO_Config(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); // 7 segment uygulamas� i�in GPIO E aktif hale getirildi

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; // Buraya buton eklenece�i i�in dijital giri� olarak se�ildi
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; // GPIO_A_0 a buton eklendi
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ; //Burada kullan�lacak diren� pull down direnciydi ve push pull yolunu kullanmam�z gerekiyor.
	//Bu �zelli�e �ematik �zerinden ula�abiliriz.
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;//Buton Pull down ba�land��� i�in
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; //Kullan�lan butonun h�z�n� se�tik

	GPIO_Init(GPIOA, &GPIO_InitStruct); // Yapt���m�z tan�mlar� GPIO portunun ba�lat�lmas� i�in kulland�k ve y�kledik

	// B�ylece buraya kadar buton tan�mlamas� ve �zelliklerini se�mi� olduk.
	// �imdi tekrar port a�mam�z laz�m. Bunun i�in D Portuna ba�l� LED leri kullanaca��z ve konfigurasyonlar�n� yapaca��z.
	// Kod bak�m�ndan daha d�zenli olsun diye �st tarafta tan�mlamas� yap�ld�.

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //LEDler i�in farketmez. OpenDrain se�ildi�i i�in push pull ayar� yap�lmaya da gerek kalmad�

	GPIO_Init(GPIOD, &GPIO_InitStruct); //Yapt���m�z ayarlar� D portuna y�klemi� olduk.
	//Art�k buradan sonra GPIO portundan veri okumaya ge�ebiliriz.

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
	// 7 segment i�in kullanaca��m�z gnd hari� 8 biti GPIO pinlerine y�klenmek �zere �zellik kazand�r�yoruz
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
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){ // butona bas�l�p bas�lmad���n� kontrol edece�iz
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_PinSource0)); // Butona hala bas�l�p bas�lmad���n� kontrol et. Bas�ld��� s�rece d�ng�de kal
		delay(time); //Freq=168MHZ ise 1 sn 168m yapar o halde 0,2ms i�in 3.360.000 olmas� gerekiyor

		GPIO_ToggleBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_12); //12 ve 13. pini toggle et
		GPIO_ToggleBits(GPIOD,GPIO_Pin_14 | GPIO_Pin_15); // 14 ve 15i yak

	}
}


void GPIO_LED_button_counter(){ // Mod 4 e g�re ledleri s�ras�yla yak�p s�ras�yla s�nd�ren ve 20ye geldi�i zaman t�m led leri s�nd�r�p en ba�tan ba�layan kod yap�s�
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
