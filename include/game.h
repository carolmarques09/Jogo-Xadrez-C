#ifndef GAME_H
#define GAME_H

#include "movement.h"

typedef struct {
    int partida_id;
    int jogador_brancas_id;
    int jogador_pretas_id;
} EstadoPartida;

extern EstadoPartida partida_atual;

void iniciarJogo(void);
void registrarJogada(const char *jogada);
void registrarJogadaMove(Move *move);
void finalizarJogo(int vencedor_id);
void trocarTurno(void);
void verificarXeque(void);

#endif
