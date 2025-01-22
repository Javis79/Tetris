#include "menu.h"
#include "oled.h"
#include "klaw.h"
#include "tsi.h"
#include "gpio.h"
#include "gpioa.h"
#include "time.h"
#include "spi.h"
#include "area.h"

#define MAX_HIGH_SCORES 5

static uint32_t high_scores[MAX_HIGH_SCORES] = {0};
static uint8_t level = 0;
static int gravitation = 0;
static int min_gravitation = 0;

//settery i gettery
void set_high_score(int i,uint32_t value) {
    high_scores[i] = value;
}

uint32_t get_high_score(int i) {
    return high_scores[i];
}

void set_level(uint8_t value) {
    level = value;
}

uint8_t get_level(void) {
    return level;
}

void set_gravitation(int value) {
    gravitation = value;
}

int get_gravitation(void) {
    return gravitation;
}
void set_min_gravitation(int value) {
    min_gravitation = value;
}

int get_min_gravitation(void) {
    return min_gravitation;
}


// Wprowadzenie
void introduction(void) {
		SysTick_Config(SystemCoreClock / 1000);
		Klaw_Init();
    Klaw_S2_4_Int();
    TSI_Init();
    spi_init(SPI_MODE_0, 1000000, SPI_SIDE_MASTER);
    oled_init();
    clear_oled_buffer(); // Czyszczenie bufora przed wprowadzeniem
    oled_clear_display();
		oled_update_screen();

    oled_draw_string(15, 10, "TETRIS");
    oled_draw_string(45, 25, "by");
    oled_draw_string(70, 0, "KRYSTIAN");
    oled_draw_string(90, 0, "BAJERSKI");
		oled_update_screen();

    // Odczekanie trzech sekund i wyczyszczenie ekranu
    while (get_c_time() < 300) {
        if (get_sec_OK()) {
            set_c_time(get_c_time() + 1);
            set_sec_OK(0);
        }
    }
    set_c_time(0);
    clear_oled_buffer();
    oled_clear_display();
		clear_oled_buffer();
}

// Funkcja aktualizujaca tablice najlepszych wyników
void update_high_scores(uint32_t current_score) {
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (current_score > high_scores[i]) {
            // Przesun wyniki w dól
            for (int j = MAX_HIGH_SCORES - 1; j > i; j--) {
                set_high_score(j, get_high_score(j - 1));
            }
            // Wstaw nowy wynik
            set_high_score(i, current_score);
            break;
        }
    }
}

// Funkcja wyswietlajaca najlepsze wyniki
void display_high_scores(void) {
    char buffer[20];
    oled_draw_string(40, 0, "Top");
	  oled_draw_string(50, 0, "Scores:");
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        snprintf(buffer, sizeof(buffer), "%d. %u", i + 1, get_high_score(i));
        oled_draw_string(50 + 10 * (i + 1), 0, buffer); // Wyswietl kolejne wyniki
    }
}


// Funkcja do wyboru poziomu trudnosci
void choose_level(void){
		clear_oled_buffer();
		oled_clear_display();
		oled_update_screen();
		clear_oled_buffer();
		oled_draw_string(10, 10,"Choose");
		oled_draw_string(20, 10,"Level");
		oled_draw_string(30, 0,"1.Easy");
		oled_draw_string(40, 0,"2.Medium");
		oled_draw_string(50, 0,"3.Hard");
		oled_update_screen();
		set_S2_press(0);
		set_S3_press(0);
		set_S4_press(0);
		while(1){
			if(get_S2_press()){
				set_gravitation(700000);
				set_min_gravitation(700000);
				set_S2_press(0);
				set_level(1);
				break;
			}
			else if(get_S3_press()){
				set_gravitation(350000);
				set_min_gravitation(350000);
				set_S3_press(0);
				set_level(2);
				break;
			}
			else if(get_S4_press()){
				set_gravitation(175000);
				set_min_gravitation(175000);
				set_S4_press(0);
				set_level(3);
				break;
			}
		}	
		DELAY(500)
		clear_oled_buffer();
		oled_clear_display();
		oled_update_screen();
		clear_oled_buffer();
	}

	
// Funckja do wyswietlania ekranu konca gry oraz tabelii najlepszych wynikow
void game_over_screen(void){
    oled_draw_string(10, 15, "Game");
    oled_draw_string(20, 15, "Over");
		oled_draw_string(30, 0, "P:");
		update_high_scores(get_score());
    display_high_scores();
		oled_update_screen();

		while(get_c_time()<300)		
		{
			if(get_sec_OK())
			{
				set_c_time(get_c_time()+1);			
				set_sec_OK(0);
			}
		}
		set_c_time(0);
		oled_clear_display();
		oled_update_screen();
		clear_oled_buffer();
}
