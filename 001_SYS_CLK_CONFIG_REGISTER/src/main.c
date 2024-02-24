
#include "stm32f4xx.h"


extern uint32_t SystemCoreClock;

uint32_t systemClock;

void RCC_config(void)
{


	RCC->CR  &= 0x00000083; // Reset value of RCC register: 0x0000XX83 where X is undefined
	RCC->CR &= ~(16<<0); // Baþlangýçta açýk olarak konfigure edilen HSIEN biti kapatýldý.
	//Now we want to set HSE ON. HSE ON is the 16th pin of RCC register. We have to set the 16th pin of CR
	RCC->CR &= ~(16<<1);
	RCC->CR |= 1<<16;
	/*RCC_CR nin 16. bacaðý olan HSEON pinini set ettik. Fakat bu kadarý yetmez çünkü 17. bitte
	bulunan HSERDY flagi 6 cycles sonra ayaða kalkacaktýr. ve biz bu pinin aktif olup olmadýðýný kontrol ederek kodumuzu
	kontrol etmeliyiz.*/
	while(!(RCC->CR & (1<<17))); /*Ýçerisi TRUE olmadýðý sürece beklemesini istediðimize göre bu þekilde bir kullaným
yapmalýyýz*/
	RCC->CR |= (1<<19); // CSSON bit is activated. Eðer bir osilator arýzasý tespit edilirse CSS tarafýndan pasif edilir
	RCC->CR |= (1<<24);
	while(!(RCC->CR & (1<<25)));

	RCC->PLLCFGR &= ~(31 << 0); // Burada yapýlan iþlem 63=111111 sayýsýný tersleyerek 0 bit sola kaydýrýyoruz.





}



int main(void)
{
	RCC_config();

//SONUÇ OLARAK BURADA HSE 168 MHZ GÖRECEÐÝZ



  while (1)
  {

  }
}
