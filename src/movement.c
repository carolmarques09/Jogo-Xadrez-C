#include "movement.h"

#include <stdio.h>
#include <string.h>

static void casaParaString(unsigned char sq, char *out) {
    out[0] = (char)('a' + (sq % 8));
    out[1] = (char)('1' + (sq / 8));
    out[2] = '\0';
}

static unsigned char charPromocao(char c) {
    switch (c) {
        case 'n':
        case 'N':
            return KNIGHT;
        case 'b':
        case 'B':
            return BISHOP;
        case 'r':
        case 'R':
            return ROOK;
        case 'q':
        case 'Q':
        default:
            return QUEEN;
    }
}

static char promocaoParaChar(unsigned char promotion) {
    switch (promotion) {
        case KNIGHT:
            return 'n';
        case BISHOP:
            return 'b';
        case ROOK:
            return 'r';
        case QUEEN:
        default:
            return 'q';
    }
}

void moverParaString(Move *move, char *str) {
    char origem[3];
    char destino[3];
    casaParaString(move->src, origem);
    casaParaString(move->dst, destino);
    sprintf(str, "%s%s", origem, destino);
    if (move->promotion) {
        size_t len = strlen(str);
        str[len] = promocaoParaChar(move->promotion);
        str[len + 1] = '\0';
    }
}

int aplicarMovimento(Board *board, Move *move) {
    if (board == NULL || move == NULL) {
        return 0;
    }

    int src = move->src;
    int dst = move->dst;
    if (src < 0 || src > 63 || dst < 0 || dst > 63) {
        return 0;
    }

    int piece = board->squares[src];
    if (piece == EMPTY) {
        return 0;
    }
    if (COLOR(piece) != board->color) {
        return 0;
    }

    int alvo = board->squares[dst];
    if (alvo != EMPTY && COLOR(alvo) == COLOR(piece)) {
        return 0;
    }

    settingTabuleiro(board, src, EMPTY);

    int nova_peca = piece;
    int rank_dst = dst / 8;
    if (PIECE(piece) == PAWN) {
        if (!COLOR(piece) && rank_dst == 7) {
            nova_peca = WHITE_QUEEN;
        } else if (COLOR(piece) && rank_dst == 0) {
            nova_peca = BLACK_QUEEN;
        }
    }
    settingTabuleiro(board, dst, nova_peca);

    board->color ^= LADO_PRETO;
    return 1;
}

void moverDeString(Move *move, const char *str) {
    memset(move, 0, sizeof(Move));
    if (str == NULL || strlen(str) < 4) {
        return;
    }

    move->src = (unsigned char)((str[1] - '1') * 8 + (str[0] - 'a'));
    move->dst = (unsigned char)((str[3] - '1') * 8 + (str[2] - 'a'));
    if (strlen(str) >= 5) {
        move->promotion = charPromocao(str[4]);
    }
}
