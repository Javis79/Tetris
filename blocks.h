#ifndef TETRIS_BLOCKS_H
#define TETRIS_BLOCKS_H

#include <stdint.h>

#define BLOCK_SIZE 4 // Kwadrat o wymiarze 4x4

// Deklaracje bloczków
static const uint8_t TETRIS_BLOCK_I[1][4][2];
static const uint8_t TETRIS_BLOCK_O[2][4][2];
static const uint8_t TETRIS_BLOCK_T[2][4][2];
static const uint8_t TETRIS_BLOCK_L[2][4][2];
static const uint8_t TETRIS_BLOCK_J[2][4][2];
static const uint8_t TETRIS_BLOCK_S[2][4][2];
static const uint8_t TETRIS_BLOCK_Z[2][4][2];

const uint8_t (*get_TETRIS_BLOCK_I(void))[4][2];
const uint8_t (*get_TETRIS_BLOCK_O(void))[4][2];
const uint8_t (*get_TETRIS_BLOCK_T(void))[4][2];
const uint8_t (*get_TETRIS_BLOCK_L(void))[4][2];
const uint8_t (*get_TETRIS_BLOCK_J(void))[4][2];
const uint8_t (*get_TETRIS_BLOCK_S(void))[4][2];
const uint8_t (*get_TETRIS_BLOCK_Z(void))[4][2];

#endif // TETRIS_BLOCKS_H
