#include "klaw.h"

static uint8_t S2_press=0;	
static uint8_t S3_press=0;
static uint8_t S4_press=0;	//Przyciski

static uint32_t c_time=0;				// Licznik czasu
static uint8_t sec=0;			// Licznik przerwan
static uint8_t sec_OK=0;

//gettery i settery dla przycisków
void set_S2_press(uint8_t value) {
    S2_press = value;
}

uint8_t get_S2_press(void) {
    return S2_press;
}
void set_S3_press(uint8_t value) {
    S3_press = value;
}

uint8_t get_S3_press(void) {
    return S3_press;
}
void set_S4_press(uint8_t value) {
    S4_press = value;
}

uint8_t get_S4_press(void) {
    return S4_press;
}

//gettery i settery dla SysTickowych zmiennych
void set_c_time(uint32_t value) {
    c_time = value;
}

uint32_t get_c_time(void) {
    return c_time;
}

void set_sec(uint8_t value) {
    sec = value;
}

uint8_t get_sec(void) {
    return sec;
}

void set_sec_OK(uint8_t value) {
    sec_OK = value;
}

uint8_t get_sec_OK(void) {
    return sec_OK;
}

//inicjalizacja klawiatury
void Klaw_Init(void)
{
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;		// Wlaczenie portu A
	PORTA->PCR[S1] |= PORT_PCR_MUX(1);
	PORTA->PCR[S2] |= PORT_PCR_MUX(1);
	PORTA->PCR[S3] |= PORT_PCR_MUX(1);
	PORTA->PCR[S4] |= PORT_PCR_MUX(1);
	PORTA->PCR[S1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[S2] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[S3] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTA->PCR[S4] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}
void Klaw_S2_4_Int(void)
{
	PORTA -> PCR[S2] |= PORT_PCR_IRQC(0xa);		//0x8 - poziom "0"; 0x9 - zbocze narastające; 0xa - zbocze opadające; 0xb - obydwa zbocza
	PORTA -> PCR[S3] |= PORT_PCR_IRQC(0xa);		
	PORTA -> PCR[S4] |= PORT_PCR_IRQC(0xa);
	NVIC_SetPriority(PORTA_IRQn, 3); 
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	NVIC_EnableIRQ(PORTA_IRQn);
}

void PORTA_IRQHandler(void)	// Podprogram obs³ugi przerwania od klawiszy S2, S3 i S4
{
	uint32_t buf;
	buf=PORTA->ISFR & (S2_MASK | S3_MASK | S4_MASK);

	switch(buf)
	{
		case S2_MASK:	DELAY(100)
									if(!(PTA->PDIR&S2_MASK))		// Minimalizacja drgañ zestyków
									{
										DELAY(100)
										if(!(PTA->PDIR&S2_MASK))	// Minimalizacja drgañ zestyków (c.d.)
										{
											if(!get_S2_press())
											{
												set_S2_press(1);
											}
										}
									}
									break;
		case S3_MASK:	DELAY(100)
									if(!(PTA->PDIR&S3_MASK))
									{
										DELAY(100)
										if(!(PTA->PDIR&S3_MASK))	// Minimalizacja drgañ zestyków (c.d.)
										{
											if(!get_S3_press())
											{
												set_S3_press(1);
											}
										}
									}
									break;
		case S4_MASK:	DELAY(100)
									if(!(PTA->PDIR&S4_MASK))		// Minimalizacja drgañ zestyków
									{
										DELAY(100)
										if(!(PTA->PDIR&S4_MASK))	// Minimalizacja drgañ zestyków (c.d.)
										{
											if(!get_S4_press())
											{
												set_S4_press(1);
											}
										}
									}
									break;							
		default:			break;
	}	
	PORTA->ISFR |=  S2_MASK | S3_MASK | S4_MASK;	// Kasowanie wszystkich bitów ISF
	NVIC_ClearPendingIRQ(PORTA_IRQn);
}


void SysTick_Handler(void)	// Podprogram obslugi przerwania od SysTick'a
{ 
	sec+=1;
	if(sec==10)
	{
		sec=0;
		sec_OK=1;		
	}
}

