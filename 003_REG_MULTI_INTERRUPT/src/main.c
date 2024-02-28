
#include "stm32f4xx.h"

volatile uint32_t time = 0;


void CLK_Config(void);
void GPIO_Config(void);
void EXTI_Config(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void delay(uint32_t time);

int main(void)
{

	CLK_Config();
	GPIO_Config();
	EXTI_Config();
  while (1)
  {
	  if(time > 0)
		  delay(time);
	  GPIOD->ODR = 0x0000F000;
  }
}



void CLK_Config(void){
	RCC->CR |= 0x00030000; // HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x00020000)); //HSEON Ready Flag waiting
	RCC->CR |= 0x00080000; //CSS Enable
	RCC->PLLCFGR |= 0x00400000; // PLL seleceted as HSE
	RCC->PLLCFGR |= 0x00000004; // PLL M = 4
	RCC->PLLCFGR |= 0x00005A00; // PLL N = 168
	RCC ->PLLCFGR |= 0x00000000; // PLL P = 0

	RCC->CFGR |= 0x00000000; // AHB1 prescaler = 1
	RCC->CFGR |= 0x00080000; // APB2 prescaler = 2
	RCC->CFGR |= 0x00001400; //APB1 prescaler = 4

	RCC->CIR |= 0x00080000; //HSERDY flag clear
	RCC->CIR |= 0x00800000; //CSS Flag Clear
}

void GPIO_Config(void){

	RCC->AHB1ENR = 0x00000009; //AHB1 clock bus activated for GPIOA and GPIOD activate

	GPIOD->MODER = 0x55000000; //GPIOD output pin12,13,14,15
	GPIOD->OTYPER = 0x00000000; // Push Pull
	GPIOD->OSPEEDR = 0xFF000000; //High Speed 100MHz
	GPIOD->PUPDR = 0x00000000; //No pull up-No pull down
}


void EXTI_Config(void){
	RCC->APB2ENR |= 0x00004000; //SYSCGF activated (EXTI)

	SYSCFG->EXTICR[0] = 0x00000000;

	NVIC_EnableIRQ(EXTI0_IRQn); //EXTI0 activated for 0s pins
	NVIC_EnableIRQ(EXTI1_IRQn); // EXTI1 is activated for 1st pins
	NVIC_EnableIRQ(EXTI2_IRQn); //EXTI2 is activated for 2nd pins

	NVIC_SetPriority(EXTI0_IRQn,0); // 0. pinlerden gelen iinterrupt daha önceliklidir
	NVIC_SetPriority(EXTI1_IRQn,1); //2.sýrada 1.pinlerden gelen interruptlar
	NVIC_SetPriority(EXTI2_IRQn,2); //3. sýrada ise 2. pinlerden gelen interruptlar alýnýr.

	EXTI->IMR = 0x00000007; //Interrupt olarak seçilecek pinleri SET yap
	EXTI->RTSR |= 0x00000007;
}

void EXTI0_IRQHandler(void){
	if(EXTI->PR & 0x00000001){
		GPIOD->ODR = 0x00001000; // Only GPIOD Pin 12 LED is turn on
		time = 16800000;
		EXTI->PR = 0x00000001; //
	}
}

void EXTI1_IRQHandler(void){
	if(EXTI->PR & 0x00000002){
		GPIOD->ODR = 0x00002000;
		time = 16800000;
		EXTI->PR = 0x00000002;
	}
}

void EXTI2_IRQHandler(void){
	if(EXTI->PR & 0x00000004){
		GPIOD->ODR=0x00004000;
		time = 16800000;
		EXTI->PR = 0x00000004;
	}
}


void delay(uint32_t time){
	while(time--);
}
