#include <stdint.h>
#include "blocks.h"

// Poprawione definicje bloczków, aby linia miala 4 kwadraty
static const uint8_t TETRIS_BLOCK_I[1][4][2] = { // Linia pozioma (1x4)
    {
        {0, 0}, {1, 0}, {2, 0}, {3, 0} // Cztery kwadraty obok siebie
    }
};

const uint8_t (*get_TETRIS_BLOCK_I(void))[4][2] {
    return TETRIS_BLOCK_I;
}


static const uint8_t TETRIS_BLOCK_O[2][4][2] = { // Kwadrat 2x2
    {
        {0, 0}, {1, 0}
    },
    {
        {0, 1}, {1, 1}
    }
};

const uint8_t (*get_TETRIS_BLOCK_O(void))[4][2] {
    return TETRIS_BLOCK_O;
}


static const uint8_t TETRIS_BLOCK_T[2][4][2] = { // Kapelusz 3x2
    {
        {0, 0}, {1, 0}, {2, 0}
    },
    {
        {1, 1}
    }
};

const uint8_t (*get_TETRIS_BLOCK_T(void))[4][2] {
    return TETRIS_BLOCK_T;
}


static const uint8_t TETRIS_BLOCK_L[2][4][2] = { // L z podstawa o dlugosci 3 i nózka o dlugosci 1
    {
        {0, 0}, {1, 0}, {2, 0} // Podstawa
    },
    {
        {2, 1} // Nózka
    }
};

const uint8_t (*get_TETRIS_BLOCK_L(void))[4][2] {
    return TETRIS_BLOCK_L;
}


static const uint8_t TETRIS_BLOCK_J[2][4][2] = { // Odwrócone L z podstawa o dlugosci 3 i nózka o dlugosci 1
    {
        {0, 0}, {1, 0}, {2, 0} // Podstawa
    },
    {
        {0, 1} // Nózka
    }
};

const uint8_t (*get_TETRIS_BLOCK_J(void))[4][2] {
    return TETRIS_BLOCK_J;
}


static const uint8_t TETRIS_BLOCK_S[2][4][2] = { // S
    {
        {0, 0}, {1, 0}
    },
    {
        {1, 1}, {2, 1}
    }
};

const uint8_t (*get_TETRIS_BLOCK_S(void))[4][2] {
    return TETRIS_BLOCK_S;
}


static const uint8_t TETRIS_BLOCK_Z[2][4][2] = { // Z
    {
				{1, 0}
		},
		{
        {0, 1}, {1, 1}
    }
};

const uint8_t (*get_TETRIS_BLOCK_Z(void))[4][2] {
    return TETRIS_BLOCK_Z;
}

