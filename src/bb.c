#include "bb.h"

#include "utils.h"

#if defined(_MSC_VER)
#include <intrin.h>
#endif

int lsb(bb b) {
    if (b == 0) {
        return 0;
    }
#if defined(_MSC_VER)
    unsigned long index;
    _BitScanForward64(&index, b);
    return (int)index;
#else
    return __builtin_ctzll(b);
#endif
}

bb HASH_WHITE_PAWN[64];
bb HASH_WHITE_KNIGHT[64];
bb HASH_WHITE_BISHOP[64];
bb HASH_WHITE_ROOK[64];
bb HASH_WHITE_QUEEN[64];
bb HASH_WHITE_KING[64];
bb HASH_BLACK_PAWN[64];
bb HASH_BLACK_KNIGHT[64];
bb HASH_BLACK_BISHOP[64];
bb HASH_BLACK_ROOK[64];
bb HASH_BLACK_QUEEN[64];
bb HASH_BLACK_KING[64];
bb HASH_COLOR;
bb HASH_CASTLE[16];
bb HASH_EP[8];

static int hash_inicializado;

static void preencher(bb *tabela, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        tabela[i] = ((bb)prng() << 32) | prng();
    }
}

void bb_inicializar_hash(void) {
    if (hash_inicializado) {
        return;
    }

    prng_seed(0xC0FFEE42U);
    preencher(HASH_WHITE_PAWN, 64);
    preencher(HASH_WHITE_KNIGHT, 64);
    preencher(HASH_WHITE_BISHOP, 64);
    preencher(HASH_WHITE_ROOK, 64);
    preencher(HASH_WHITE_QUEEN, 64);
    preencher(HASH_WHITE_KING, 64);
    preencher(HASH_BLACK_PAWN, 64);
    preencher(HASH_BLACK_KNIGHT, 64);
    preencher(HASH_BLACK_BISHOP, 64);
    preencher(HASH_BLACK_ROOK, 64);
    preencher(HASH_BLACK_QUEEN, 64);
    preencher(HASH_BLACK_KING, 64);
    HASH_COLOR = ((bb)prng() << 32) | prng();
    preencher(HASH_CASTLE, 16);
    preencher(HASH_EP, 8);
    hash_inicializado = 1;
}
