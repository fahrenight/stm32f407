
#include "stm32f4xx.h"

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */
uint32_t systemClock;

void RCC_Config(void);

int main(void)
{
	RCC_Config();
	SystemCoreClockUpdate();

	systemClock = SystemCoreClock;

  while (1)
  {

  }
}

void RCC_Config(void){  //REGISTER SEVIYESINDE PLL ILE FREKANS AYARLAMA
	RCC->CR |= 1<<16;
	while(!(RCC->CR & ~(1<<17)));
	RCC->CR |= 1<<19;

	//PLL Settings.
	RCC->PLLCFGR = 0x00000000; //PLL Register fill 0s
	RCC->PLLCFGR |= 0x00000004; // PLL M = 4
	RCC->PLLCFGR |= 0x00002A00; //PLL N=168
	RCC->PLLCFGR |= 0x00020000; //PLL P = 2
	RCC->PLLCFGR |= 1<<22; // PLLSRC = 1 --> HSE

	//PLLON
	RCC->CR |= 1<<24; // PLL activated
	while(!(RCC->CR & ~(1<<25))); //Wait PLL Flag SET

	//System Clock Switch Select
	RCC->CFGR |= 0x00000002; //System clock has selected as 10: PLL selected as system clock
	while(!(RCC->CFGR & ~(1<<3))); //PLL 10 checking flag
}
