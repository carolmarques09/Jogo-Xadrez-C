#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "bb.h"
#include "board.h"

#define MAX_MOVES 256

typedef struct {
    unsigned char src;
    unsigned char dst;
    unsigned char promotion;
} Move;

typedef struct {
    unsigned char piece;
    unsigned char capture;
    unsigned char castle;
    bb ep;
} Undo;

void criarMovimento(Board *board, Move *move);
void fazerMovimentoNulo(Board *board, Undo *undo);
void desfazerMovimentoNulo(Board *board, Undo *undo);
void fazerMovimento(Board *board, Move *move, Undo *undo);
void desfazerMovimento(Board *board, Move *move, Undo *undo);
int moverScore(Board *board, Move *move);

void moverParaString(Move *move, char *str);
void moverDeString(Move *move, const char *str);
int aplicarMovimento(Board *board, Move *move);
void anotarMovimento(Board *board, Move *move, char *result);
void imprimirMovimento(Board *board, Move *move);
int analisarMovimento(Board *board, const char *notation, Move *move);
int analisar_pgn(Board *board, const char *pgn);

#endif