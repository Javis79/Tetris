#ifndef TETRIS_GAME_AREA_H
#define TETRIS_GAME_AREA_H

#include <stdint.h>
#include "blocks.h"
#include "klaw.h"

#define GAME_AREA_WIDTH 18 // Szerokosc obszaru gry w kwadratach
#define GAME_AREA_HEIGHT 12 // Wysokosc obszaru gry w kwadratach


// Struktura reprezentujaca spadajacy klocek
typedef struct {
    const uint8_t (*block_bitmap)[4][2]; // Oryginalna bitmapa
    uint8_t rows;                       // Liczba rzedów
    uint8_t bitmap[4][4][2];            // Edytowalna kopia bitmapy
    uint8_t x;                          // Pozycja X
    uint8_t y;                          // Pozycja Y
} TetrisBlock;


// Funkcje do zarzadzania obszarem gry
void spawn_block_random(void);
void draw_next_block(void);
int check_collision(int dx, int dy);
void move_block(int dx, int dy);
void handle_touch_input(void);
void draw_game_area(void);
void display_score(void);
void game_loop(void);
void lock_block(void);
void clear_lines(void);
void rotate_block(void);
void handle_button_input_2(void);
void handle_button_input_3(void);
void handle_button_input_4(void);
void update_display(void);
void initialize_blocks(void);
void reset_game(void);
void choose_level(void);
uint8_t get_initialized(void);
void set_initialized(uint8_t value);


// Deklaracje setterów i getterów
void set_game_area(uint8_t x, uint8_t y, uint8_t value);
uint8_t get_game_area(uint8_t x, uint8_t y);

void set_current_block(TetrisBlock block);
TetrisBlock get_current_block(void);

void set_cleared_lines(uint8_t value);
uint8_t get_cleared_lines(void);

void set_random_index(uint8_t value);
uint8_t get_random_index(void);

void set_next_random_index(uint8_t value);
uint8_t get_next_random_index(void);

void set_game_over(int value);
int get_game_over(void);

void set_slider(uint8_t value);
uint8_t get_slider(void);

void set_slider_value(uint8_t value);
uint8_t get_slider_value(void);

void set_last_slider_position(uint8_t value);
uint8_t get_last_slider_position(void);

void set_score(uint32_t value);
uint32_t get_score(void);

void set_last_displayed_score(uint32_t value);
uint32_t get_last_displayed_score(void);


#endif // TETRIS_GAME_AREA_H
