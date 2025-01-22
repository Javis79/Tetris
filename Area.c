#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "klaw.h"
#include "oled.h"
#include "blocks.h"
#include "area.h"
#include "tsi.h"
#include "gpio.h"
#include "gpioa.h"
#include "time.h"
#include "spi.h"
#include "menu.h"
#include <string.h>

#define GAME_AREA_WIDTH 18 // Szerokosc obszaru gry w kwadratach
#define GAME_AREA_HEIGHT 12 // Wysokosc obszaru gry w kwadratach
#define NUM_BLOCKS (sizeof(blocks) / sizeof(blocks[0]))

static uint8_t game_area[GAME_AREA_HEIGHT][GAME_AREA_WIDTH] = {0}; // Obszar gry (0 - puste, 1 - zajete)

static TetrisBlock current_block;
static TetrisBlock next_block;
static uint8_t random_index = 0;
static uint8_t next_random_index = 0;
static int game_over = 0; // Flaga oznaczajaca koniec gry
static uint8_t slider_position = 0;
static uint8_t last_slider_position = 0; // Ostatnia pozycja slidera
static uint8_t slider_value = 0;
static int first_block = 0;

static uint32_t score = 0;             // Zliczanie punktów
static uint32_t last_displayed_score = 0; // Przechowuje ostatnio wyswietlony wynik
static char score_text[20];
static uint8_t cleared_lines = 0;
static uint8_t initialized = 0;

////////////////////////////////////////
//settery i gettery
void set_game_area(uint8_t y, uint8_t x, uint8_t value) {
    if (x < GAME_AREA_WIDTH && y < GAME_AREA_HEIGHT) {
        game_area[y][x] = value;
    }
}

uint8_t get_game_area(uint8_t y, uint8_t x) {
    if (x < GAME_AREA_WIDTH && y < GAME_AREA_HEIGHT) {
        return game_area[y][x];
    }
    return 0; // Wartosc domyslna w przypadku blednych indeksów
}
void set_current_block(TetrisBlock block) {
    current_block = block;
}

TetrisBlock get_current_block(void) {
    return current_block;
}

void set_cleared_lines(uint8_t value) {
    cleared_lines = value;
}

uint8_t get_cleared_lines(void) {
    return cleared_lines;
}

void set_initialized(uint8_t value) {
    initialized = value;
}

uint8_t get_initialized(void) {
    return initialized;
}

void set_random_index(uint8_t value) {
    random_index = value;
}

uint8_t get_random_index(void) {
    return random_index;
}

void set_next_random_index(uint8_t value) {
    next_random_index = value;
}

uint8_t get_next_random_index(void) {
    return next_random_index;
}


void set_game_over(int value) {
    game_over = value;
}

int get_game_over(void) {
    return game_over;
}

void set_slider(uint8_t value) {
    slider_position = value;
}

uint8_t get_slider(void) {
    return slider_position;
}

void set_slider_value(uint8_t value) {
    slider_value = value;
}

uint8_t get_slider_value(void) {
    return slider_value;
}

void set_last_slider_position(uint8_t value) {
    last_slider_position = value;
}

uint8_t get_last_slider_position(void) {
    return last_slider_position;
}

void set_score(uint32_t value) {
    score = value;
}

uint32_t get_score(void) {
    return score;
}

void set_last_displayed_score(uint32_t value) {
    last_displayed_score = value;
}

uint32_t get_last_displayed_score(void) {
    return last_displayed_score;
}

////////////////////////////////////////////

struct BlockInfo {
    const uint8_t (*bitmap)[4][2];
    uint8_t rows;
};

static struct BlockInfo blocks[7];

void initialize_blocks(void) {
    blocks[0].bitmap = get_TETRIS_BLOCK_I();
    blocks[0].rows = 1;

    blocks[1].bitmap = get_TETRIS_BLOCK_O();
    blocks[1].rows = 2;

    blocks[2].bitmap = get_TETRIS_BLOCK_T();
    blocks[2].rows = 2;

    blocks[3].bitmap = get_TETRIS_BLOCK_L();
    blocks[3].rows = 2;

    blocks[4].bitmap = get_TETRIS_BLOCK_J();
    blocks[4].rows = 2;

    blocks[5].bitmap = get_TETRIS_BLOCK_S();
    blocks[5].rows = 2;

    blocks[6].bitmap = get_TETRIS_BLOCK_Z();
    blocks[6].rows = 2;
}

// Funkcja inicjalizujaca nowy klocek
void spawn_block_random(void) {
    if (!get_initialized()) {
        srand(SysTick->VAL); // Inicjalizacja generatora losowego
        set_initialized(1);
    }
		
		if(first_block == 0){
    set_random_index((uint8_t)((unsigned int)rand() % NUM_BLOCKS)); // Wybierz losowy klocek
		first_block = 1;
		}
		else{
		set_random_index(get_next_random_index());
		}
		set_next_random_index((uint8_t)((unsigned int)rand() % NUM_BLOCKS)); //Wybierz nastepny losowy klocek
    current_block.block_bitmap = blocks[get_random_index()].bitmap;
    current_block.rows = blocks[get_random_index()].rows;
    current_block.x = 0; // Poczatkowa pozycja X
    current_block.y = GAME_AREA_HEIGHT / 2; // Poczatkowa pozycja Y
		next_block.block_bitmap = blocks[get_next_random_index()].bitmap;
		next_block.rows = blocks[get_next_random_index()].rows;
		next_block.x = 20; // Na stale pozycja nastepnego klocka
		next_block.y = 9;
		
	
    // Kopiuje dane bitmapy do edytowalnej tablicy
    for (uint8_t i = 0; i < current_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            current_block.bitmap[i][j][0] = current_block.block_bitmap[i][j][0];
            current_block.bitmap[i][j][1] = current_block.block_bitmap[i][j][1];
        }
    }
		
		for (uint8_t i = 0; i < next_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            next_block.bitmap[i][j][0] = next_block.block_bitmap[i][j][0];
            next_block.bitmap[i][j][1] = next_block.block_bitmap[i][j][1];
        }
    }

    // Sprawdz, czy nowy klocek koliduje z istniejacymi
    if (check_collision(0, 0)) {
        set_game_over(1); // Ustaw flage konca gry
    }
}


// Funkcja sprawdzajaca kolizje
int check_collision(int dx, int dy) {
    for (uint8_t i = 0; i < current_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            uint8_t block_x = (uint8_t)(current_block.x + current_block.bitmap[i][j][0] + dx);
            uint8_t block_y = (uint8_t)(current_block.y + current_block.bitmap[i][j][1] + dy);

            if (block_x >= GAME_AREA_WIDTH || block_y >= GAME_AREA_HEIGHT) {
                return 1; // Kolizja ze sciana lub podloga
            }
            if (game_area[block_y][block_x]) {
                return 1; // Kolizja z innym klockiem
            }
        }
    }
    return 0; // Brak kolizji
}

// Funkcja przesuwajaca klocek
void move_block(int dx, int dy) {
    if (!check_collision(dx, dy)) {
        current_block.x += dx; // Zamiana osi
        current_block.y += dy; // Zamiana osi
    }
}

// Funkcja rysujaca obszar gry
void draw_game_area(void) {
    clear_oled_buffer();

    // Rysowanie postawionych klocków na planszy
    for (uint8_t y = 0; y < GAME_AREA_HEIGHT; y++) {
        for (uint8_t x = 0; x < GAME_AREA_WIDTH; x++) {
            if (game_area[y][x]) {
                for (uint8_t dx = 0; dx < 5; dx++) {
                    for (uint8_t dy = 0; dy < 5; dy++) {
                        oled_draw_pixel(x * 5 + dx, y * 5 + dy, 1);
                    }
                }
            }
        }
    }

    // Rysowanie aktualnego klocka
    for (uint8_t i = 0; i < current_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            uint8_t x = current_block.x + current_block.bitmap[i][j][0];
            uint8_t y = current_block.y + current_block.bitmap[i][j][1];
            for (uint8_t dx = 0; dx < 5; dx++) {
                for (uint8_t dy = 0; dy < 5; dy++) {
                    oled_draw_pixel(x * 5 + dx, y * 5 + dy, 1);
                }	
            }
        }
    }
		
		// Rysowanie nastepnego klocka
    for (uint8_t i = 0; i < next_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            uint8_t x = next_block.x + next_block.bitmap[i][j][0];
            uint8_t y = next_block.y + next_block.bitmap[i][j][1];
            for (uint8_t dx = 0; dx < 5; dx++) {
                for (uint8_t dy = 0; dy < 5; dy++) {
                    oled_draw_pixel(x * 5 + dx, y * 5 + dy, 1);
                }	
            }
        }
    }
		
		//Rysowanie krawedzi gry
		for(uint8_t i = 0; i <61; i++){
			oled_draw_pixel(90, i,1);
		}
		for(uint8_t i = 0; i<90; i ++){
			oled_draw_pixel(i, 0,1);
		}
		for(uint8_t i = 0; i<90; i ++){
			oled_draw_pixel(i, 60,1);
		}
		
    // Przeslanie bufora na ekran
		display_score(); // Dopisanie wyniku na ekran
		oled_update_screen();

}

void display_score(void) {
    // Przygotowanie tekstu wyniku
		// Sprawdzenie czy wynik obecny rozni sie od poprzedniego, jesli nie rzutuje wyniku od nowa
		if(get_score()!=get_last_displayed_score()){
		if(get_score()-get_last_displayed_score()>get_level()*50){ // zabezpieczenie
		set_score(get_last_displayed_score() + get_level());
		}
		set_last_displayed_score(get_score());
    snprintf(score_text, sizeof(score_text)," %u", get_score()); // Rzutowanie wyniku na tekst
		}
    // Wyswietlanie wyniku na dole ekranu
		oled_draw_string(120, 0, "P:");
    oled_draw_string(120, 20, score_text); // Pozycjonowanie na dole
		
}

// Funkcja blokujaca klocek na planszy
void lock_block(void) {
    for (uint8_t i = 0; i < current_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            uint8_t block_x = current_block.x + current_block.bitmap[i][j][0];
            uint8_t block_y = current_block.y + current_block.bitmap[i][j][1];
            set_game_area(block_y,block_x, 1); // Oznacz pole jako zajete
        }
    }
}

void clear_lines(void) {
		uint32_t clearedLines = 0;
    for (uint8_t x = GAME_AREA_WIDTH-1; x > 0; x--) { // Iteracja przez wszystkie wiersze planszy
        int is_full = 1; // Zakladamy, ze linia jest pelna

        // Sprawdzenie, czy linia jest pelna
        for (uint8_t y = 0; y < GAME_AREA_HEIGHT; y++) {
            if (get_game_area(y,x) == 0) { // Jesli jakiekolwiek pole jest puste, linia nie jest pelna
                is_full = 0;
                break;
            }
        }
				
        if (is_full) {
            // Usun pelna linie i przesun wszystkie linie powyzej w dól
            for (uint8_t ty = 0; ty < GAME_AREA_HEIGHT; ty++) {
										set_game_area(ty,x,0);
            }
						
						for (uint8_t tx = x; tx > 0; tx--) {
                for (uint8_t ty = 0; ty < GAME_AREA_HEIGHT; ty++) {
								set_game_area(ty,tx, get_game_area(ty,tx-1)); // Przesuniecie linii powyzej w dól
                }
            }

            // Wyczyszczenie górnej linii
            for (uint8_t ty = 0; ty < GAME_AREA_HEIGHT; ty++) {
                set_game_area(ty,0,0);
            }
						x++;
						clearedLines++; // Dodatkowe punkty za kilka wyczyszczonych linii na raz
						set_score(get_score()+get_level()*clearedLines);
            // Po przesunieciu musimy ponownie sprawdzic te sama linie, poniewaz moze byc wypelniona po przesunieciu          
        }
    }
}


// Funkcja obslugujaca przesuwanie klocka za pomoca pola dotykowego
void handle_touch_input(void) {
    set_slider(TSI_ReadSlider());

    if (get_slider() != get_last_slider_position()) {
        if (get_slider() > 50 && current_block.x + 1 < GAME_AREA_WIDTH) {
            move_block(0, 1); // Przesun w prawo

        } else if (get_slider() > 0 && current_block.x > 0) {
            move_block(0, -1); // Przesun w lewo
        }
				set_last_slider_position(get_slider());
				set_slider_value(1);
    }
		else{
		set_last_slider_position(get_slider());
		set_slider_value(0);
		}

}

void rotate_block(void) {
    uint8_t rotated_bitmap[4][2]; // Tymczasowa tablica
    uint8_t max_dim = 4;          // Maksymalny wymiar klocka
    uint8_t temp_bitmap[4][4][2]; // Tymczasowa kopia dla rotacji
		
    // Obracaj kazdy rzad bitmapy
    for (uint8_t i = 0; i < current_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            rotated_bitmap[j][0] = current_block.bitmap[i][j][1]; // x' = y
            rotated_bitmap[j][1] = max_dim - 1 - current_block.bitmap[i][j][0]; // y' = max - x
            temp_bitmap[i][j][0] = rotated_bitmap[j][0];
            temp_bitmap[i][j][1] = rotated_bitmap[j][1];
        }
    }

    // Sprawdz kolizje dla obróconego klocka
    int collision = 0;
    for (uint8_t i = 0; i < current_block.rows; i++) {
        for (uint8_t j = 0; j < 4; j++) {
            uint8_t x = current_block.x + temp_bitmap[i][j][0];
            uint8_t y = current_block.y + temp_bitmap[i][j][1];
            if (x >= GAME_AREA_WIDTH || y >= GAME_AREA_HEIGHT || game_area[y][x]) {
                collision = 1; // Kolizja wykryta
                break;
            }
        }
        if (collision) break;
    }

    if (!collision) {
        // Zapisz obrócone wspólrzedne
        for (uint8_t i = 0; i < current_block.rows; i++) {
            for (uint8_t j = 0; j < 4; j++) {
                current_block.bitmap[i][j][0] = temp_bitmap[i][j][0];
                current_block.bitmap[i][j][1] = temp_bitmap[i][j][1];
            }
        }
    }
    // Kolizja nie powoduje cofniecia obrotu, ale zatrzymuje dalsze dzialania
}

void handle_button_input_2(void) {
    if (get_S2_press()) {
			  rotate_block();           // Obróc klocek
				move_block(-1,0);
        set_S2_press(0);
    }
}
void handle_button_input_3(void) {
    if (get_S3_press()) {
				set_gravitation(get_gravitation()*3/4);
        set_S3_press(0);
    }
}

void handle_button_input_4(void) {
    if (get_S4_press()) {
				set_gravitation(get_min_gravitation());
        set_S4_press(0);
    }
}


void reset_game(void) {
    // Reset planszy gry
    memset(game_area, 0, sizeof(game_area)); // Wyzerowanie planszy

    // Reset klocka
    TetrisBlock empty_block = {0};
    set_current_block(empty_block);
		first_block=0;

    // Reset zmiennych gry
    set_gravitation(0); // Resetowanie grawitacji do wartosci poczatkowej
    set_game_over(0);         // Resetowanie flagi konca gry
    set_slider(0);            // Resetowanie suwaka
    set_slider_value(0);      // Resetowanie wartosci suwaka
    set_last_slider_position(0); // Resetowanie ostatniej pozycji suwaka

    // Reset wyniku
    set_score(0);
    set_last_displayed_score(0);
		set_level(0);


    // Czyszczenie wyswietlacza
    clear_oled_buffer();
    oled_clear_display();
}


// Funkcja glówna gry


void game_loop(void) {
		//Wywolanie ekranu tytulowego
		introduction();
		clear_oled_buffer();
		oled_clear_display();
		oled_update_screen();
		clear_oled_buffer();
		//Wywolanie ekranu wyboru poziomu trudnosci
		choose_level();
		//Inicjalizacja bloczkow
		initialize_blocks();
		//Rysowanie obszaru gry oraz jej elementow
    draw_game_area();
		snprintf(score_text,sizeof(score_text), " %u", score); // Tworzenie tekstu wyniku
    spawn_block_random(); //Generowanie losowego bloczku
		
    while (1) {
				if(get_game_over()){
				break;
				}			
				handle_touch_input();
        handle_button_input_2(); // Obsluga wejscia przycisku S2, S3, S4
				handle_button_input_3();
				handle_button_input_4();
        draw_game_area(); // Aktualizacja planszy i bufora
			  clear_lines(); // Usuwanie pelnych linii

        if (!check_collision(1, 0)) {
					if(!get_slider_value() && !get_S2_press())
            move_block(1, 0); // Przesuwanie klocka w dól
        } else {
            lock_block(); // Zablokowanie klocka
            spawn_block_random(); // Generowanie nowego losowego klocka
        }

        // Opóznienie symulujace grawitacje
        for (volatile int i = 0; i < get_gravitation(); i++);
    }
		DELAY(200)
    oled_clear_display();
		oled_draw_string(30, 20, score_text);
		game_over_screen();
		reset_game();
		DELAY(500)
		}
