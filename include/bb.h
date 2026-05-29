#ifndef BB_H
#define BB_H

#include <stdint.h>

typedef uint64_t bb;

#define RF(rank, file) ((rank) * 8 + (file))
#define BIT(sq) (1ULL << (sq))
#define LSB(b) lsb(b)

int lsb(bb b);
void bb_inicializar_hash(void);

extern bb HASH_WHITE_PAWN[64];
extern bb HASH_WHITE_KNIGHT[64];
extern bb HASH_WHITE_BISHOP[64];
extern bb HASH_WHITE_ROOK[64];
extern bb HASH_WHITE_QUEEN[64];
extern bb HASH_WHITE_KING[64];
extern bb HASH_BLACK_PAWN[64];
extern bb HASH_BLACK_KNIGHT[64];
extern bb HASH_BLACK_BISHOP[64];
extern bb HASH_BLACK_ROOK[64];
extern bb HASH_BLACK_QUEEN[64];
extern bb HASH_BLACK_KING[64];
extern bb HASH_COLOR;
extern bb HASH_CASTLE[16];
extern bb HASH_EP[8];

#endif
