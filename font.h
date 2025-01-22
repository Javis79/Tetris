#ifndef FONT_H
#define FONT_H

#include <stdint.h>

// Struktura czcionki
typedef struct {
    const uint8_t *table;  // Wskaznik do danych bitmap
    uint16_t Width;        // Szerokosc znaku w pikselach
    uint16_t Height;       // Wysokosc znaku w pikselach
} sFONT;

// Deklaracja zewnetrznej czcionki 8x8
const uint8_t (*get_Font8_Table(void))[8];
const sFONT* get_Font8(void);
#endif // FONT_H
