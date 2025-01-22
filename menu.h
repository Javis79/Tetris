#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include "oled.h"
#include "klaw.h"

// Funkcje menu

void introduction(void);
void update_high_scores(uint32_t current_score);
void display_high_scores(void);
void game_over_screen(void);

// Settery i gettery
void set_high_score(int i,uint32_t value);
uint32_t get_high_score(int i);

void set_level(uint8_t value);
uint8_t get_level(void);

void set_gravitation(int value);
int get_gravitation(void);

void set_min_gravitation(int value);
int get_min_gravitation(void);


#endif // MENU_H
