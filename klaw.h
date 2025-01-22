#ifndef KLAW_H
#define KLAW_H

#include "MKL05Z4.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "tsi.h"

#define S1_MASK	(1<<9)		// Maska dla klawisza S1
#define S2_MASK	(1<<10)		// Maska dla klawisza S2
#define S3_MASK	(1<<11)		// Maska dla klawisza S3
#define S4_MASK	(1<<12)		// Maska dla klawisza S4
#define S1	9							// Numer bitu dla klawisza S1
#define S2	10						// Numer bitu dla klawisza S2
#define S3	11						// Numer bitu dla klawisza S3
#define S4	12						// Numer bitu dla klawisza S4

void Klaw_Init(void);
void Klaw_S2_4_Int(void);
void PORTA_IRQHandler(void);
void SysTick_Handler(void);

//Deklracaje setterów i getterów przycisków
void set_S2_press(uint8_t value);
uint8_t get_S2_press(void);

void set_S3_press(uint8_t value);
uint8_t get_S3_press(void);

void set_S4_press(uint8_t value);
uint8_t get_S4_press(void);

// Deklaracje setterów i getterów systickowych
void set_c_time(uint32_t value);
uint32_t get_c_time(void);

void set_sec(uint8_t value);
uint8_t get_sec(void);

void set_sec_OK(uint8_t value);
uint8_t get_sec_OK(void);

#endif  /* KLAW_H */
