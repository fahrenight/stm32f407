
#include "stm32f4xx.h"


extern uint32_t SystemCoreClock;

uint32_t systemClock;

void RCC_config(void)
{


	RCC->CR  &= 0x00000083; // Reset value of RCC register: 0x0000XX83 where X is undefined
	RCC->CR &= ~(16<<0); // Ba�lang��ta a��k olarak konfigure edilen HSIEN biti kapat�ld�.
	//Now we want to set HSE ON. HSE ON is the 16th pin of RCC register. We have to set the 16th pin of CR
	RCC->CR &= ~(16<<1);
	RCC->CR |= 1<<16;
	/*RCC_CR nin 16. baca�� olan HSEON pinini set ettik. Fakat bu kadar� yetmez ��nk� 17. bitte
	bulunan HSERDY flagi 6 cycles sonra aya�a kalkacakt�r. ve biz bu pinin aktif olup olmad���n� kontrol ederek kodumuzu
	kontrol etmeliyiz.*/
	while(!(RCC->CR & (1<<17))); /*��erisi TRUE olmad��� s�rece beklemesini istedi�imize g�re bu �ekilde bir kullan�m
yapmal�y�z*/
	RCC->CR |= (1<<19); // CSSON bit is activated. E�er bir osilator ar�zas� tespit edilirse CSS taraf�ndan pasif edilir
	RCC->CR |= (1<<24);
	while(!(RCC->CR & (1<<25)));

	RCC->PLLCFGR &= ~(31 << 0); // Burada yap�lan i�lem 63=111111 say�s�n� tersleyerek 0 bit sola kayd�r�yoruz.





}



int main(void)
{
	RCC_config();

//SONU� OLARAK BURADA HSE 168 MHZ G�RECE��Z



  while (1)
  {

  }
}
