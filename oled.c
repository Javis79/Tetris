#include "oled.h"
#include "spi.h"
#include "font.h"
#include "blocks.h"

#define OLED_RST_PIN 5 // PTB5
#define OLED_DC_PIN 7  // PTB7
#define OLED_CS_PIN 6  // PTB6
#define OLED_WIDTH 132
#define OLED_HEIGHT 64

static uint8_t oled_buffer[OLED_WIDTH * (OLED_HEIGHT / 8)]; // Bufor ekranu

void set_oled_buffer(uint16_t index, uint8_t value) {
    if (index < (sizeof(oled_buffer))) {
        oled_buffer[index] = value;
    }
}
uint8_t get_oled_buffer(uint16_t index) {
    if (index < (sizeof(oled_buffer))) {
        return oled_buffer[index];
    }
    return 0; // Wartosc domyslna w przypadku nieprawidlowego indeksu
}

void clear_oled_buffer(void) {
    for (uint16_t i = 0; i < sizeof(oled_buffer); i++) {
        set_oled_buffer(i, 0); // Ustaw kazda komórke na 0
    }
}

//Funkcja do przesylania komend
void oled_send_command(uint8_t command) {
    gpio_clear(OLED_DC_PIN);
    gpio_clear(OLED_CS_PIN);
    spi_master_write(command);
    gpio_set(OLED_CS_PIN);
}

//Funkcja do przesylania danych
void oled_send_data(uint8_t data) {
    gpio_set(OLED_DC_PIN);
    gpio_clear(OLED_CS_PIN);
    spi_master_write(data);
    gpio_set(OLED_CS_PIN);
}

//Funkcja do wypelnienia ekranu, w jakim stopniu ma byc wypelniony
void oled_fill_display(uint8_t color) {
    for (uint16_t i = 0; i < sizeof(oled_buffer); i++) {
        oled_buffer[i] = color; // Ustawienie wartosci dla kazdego bajtu bufora
    }
		oled_update_screen();
}

//funkcja do czyszczenia ekranu
void oled_clear_display(void) {
    oled_fill_display(0x00);
	    // Wyczysc bufor ekran
}

//updtade wyswietlacza przez bufor
void oled_update_screen(void) {
    for (uint8_t page = 0; page < (OLED_HEIGHT / 8); page++) {
        oled_send_command(0xB0 + page);    // Ustaw strone
        oled_send_command(0x00);          // Ustaw kolumne (nizsze bity)
        oled_send_command(0x10);          // Ustaw kolumne (wyzsze bity)
        for (uint8_t column = 0; column < OLED_WIDTH; column++) {
            oled_send_data(get_oled_buffer(page * OLED_WIDTH + column)); // Wyslij dane z bufora
        }
    }
		clear_oled_buffer();
}

void oled_draw_char(uint8_t x, uint8_t y, char c) {
    const sFONT *font = get_Font8(); // Pobranie wskaznika do struktury czcionki
    const uint8_t (*font_table)[8] = (const uint8_t (*)[8])font->table;

    if (c < 32 || c > 127) {
        c = '?'; // Zamieniamy nieobslugiwane znaki na '?'
    }

    const uint8_t *char_bitmap = font_table[c - 32];

    for (uint8_t i = 0; i < font->Width; i++) { // Przejdz przez kolumny znaku
        uint8_t column = char_bitmap[i];
        for (uint8_t j = 0; j < font->Height; j++) { // Przejdz przez wiersze znaku
            uint8_t pixel = (column >> j) & 0x01; // Pobierz bit (piksel)
            oled_draw_pixel(x + i, y + (font->Height - 1 - j), pixel); // Odwróc wiersze
        }
    }
}


void oled_draw_string(uint8_t x, uint8_t y, const char *str) {
    const sFONT *font = get_Font8(); // Pobranie wskaznika do struktury czcionki

    while (*str) {
        oled_draw_char(x, y, *str); // Rysuj pojedynczy znak
        y += font->Width;          // Przesun pozycje w prawo
        if (x + font->Width > OLED_WIDTH) {
            x = 0;                 // Nowa linia
            y += font->Height;
            if (y + font->Height > OLED_HEIGHT) {
                return;            // Wyjscie, jesli nie ma miejsca na ekranie
            }
        }
        str++;
    }
}



void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color) {
    if (x >= OLED_WIDTH || y >= OLED_HEIGHT) {
        return; // Wyjscie, jesli piksel jest poza zakresem
    }

    uint16_t index = (y / 8) * OLED_WIDTH + x; // Indeks w buforze
    uint8_t bit_position = y % 8;

    if (color) {
        oled_buffer[index] |= (1 << bit_position); // Ustawienie bitu
    } else {
        oled_buffer[index] &= ~(1 << bit_position); // Wyczyszczenie bitu
    }

    // Wyslij zmodyfikowany bajt do wyswietlacza
}

//Funckja do rysowania bloczkow tetrisa
void oled_draw_tetris_block(uint8_t x, uint8_t y, const uint8_t block_bitmap[][4][2], uint8_t rows) {
    for (uint8_t i = 0; i < rows; i++) { // Iteruj przez wiersze bloczka
        for (uint8_t j = 0; j < 4; j++) { // Iteruj przez kolumny bloczka
            uint8_t dx = block_bitmap[i][j][0]; // Pozycja x kwadratu
            uint8_t dy = block_bitmap[i][j][1]; // Pozycja y kwadratu

            // Sprawdz, czy dane pozycje sa prawidlowe (ignoruj wartosci spoza zakresu)
            if ((dx == 0 && dy == 0) && !(i == 0 && j == 0)) {
                continue; // Ignoruj pozycje 0,0, jesli nie jest explicite w bitmapie
            }

            if (x + dx * 3 >= OLED_WIDTH || y + dy * 3 >= OLED_HEIGHT) {
                continue; // Ignoruj piksele poza ekranem
            }

            // Rysuj kwadrat 3x3
            for (uint8_t bx = 0; bx < 5; bx++) {
                for (uint8_t by = 0; by < 5; by++) {
                    oled_draw_pixel(x + dx * 5 + bx, y + dy * 5 + by, 1);
                }
            }
        }
    }
}





void oled_init() {
    gpio_init_pin(OLED_RST_PIN);
    gpio_init_pin(OLED_DC_PIN);
    gpio_init_pin(OLED_CS_PIN);

    gpio_clear(OLED_RST_PIN);
    for (volatile int i = 0; i < 10000; i++);
    gpio_set(OLED_RST_PIN);

    oled_send_command(0xAE);
    oled_send_command(0xD5);
    oled_send_command(0x80);
    oled_send_command(0xA8);
    oled_send_command(0x3F);
    oled_send_command(0xD3);
    oled_send_command(0x00);
    oled_send_command(0x40);
    oled_send_command(0x8D);
    oled_send_command(0x14);
    oled_send_command(0x20);
    oled_send_command(0x00);
    oled_send_command(0xA0);
    oled_send_command(0xC8);
    oled_send_command(0xDA);
    oled_send_command(0x12);
    oled_send_command(0x81);
    oled_send_command(0x7F);
    oled_send_command(0xD9);
    oled_send_command(0xF1);
    oled_send_command(0xDB);
    oled_send_command(0x40);
    oled_send_command(0xA4);
    oled_send_command(0xA6);
    oled_send_command(0xAF);
		
		oled_clear_display();
}
