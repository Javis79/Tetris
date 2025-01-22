#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#include "gpio.h"
#include "font.h"
#define OLED_WIDTH 132
#define OLED_HEIGHT 64

// Deklaracje setterów i getterów
void set_oled_buffer(uint16_t index, uint8_t value);
uint8_t get_oled_buffer(uint16_t index);

void oled_init(void);
void oled_send_command(uint8_t command);
void oled_send_data(uint8_t data);
void oled_fill_display(uint8_t color);
void oled_clear_display(void);
void oled_draw_string(uint8_t x, uint8_t y, const char *str);
void oled_draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void oled_draw_char(uint8_t x, uint8_t y, char c);
void oled_draw_tetris_block(uint8_t x, uint8_t y, const uint8_t block_bitmap[][4][2], uint8_t rows); 
void oled_update_screen(void);
void clear_oled_buffer(void);

#endif // OLED_H
