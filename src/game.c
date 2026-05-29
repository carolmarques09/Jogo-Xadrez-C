#include "game.h"

#include "bb.h"
#include "board.h"
#include "database.h"
#include "graphics.h"
#include "movement.h"

#include <stdio.h>

EstadoPartida partida_atual = {0, 0, 0};

void registrarJogada(const char *jogada) {
    if (!bancoConectado() || partida_atual.partida_id <= 0) {
        return;
    }
    if (salvarMovimento(partida_atual.partida_id, jogada) == 0) {
        printf("Jogada gravada: %s\n", jogada);
    }
}

void registrarJogadaMove(Move *move) {
    char notacao[8];
    moverParaString(move, notacao);
    registrarJogada(notacao);
}

void finalizarJogo(int vencedor_id) {
    if (!bancoConectado() || partida_atual.partida_id <= 0) {
        return;
    }
    if (finalizarPartida(partida_atual.partida_id, vencedor_id) == 0) {
        printf("Partida %d finalizada. Vencedor id: %d\n", partida_atual.partida_id,
               vencedor_id);
    }
    partida_atual.partida_id = 0;
}

void trocarTurno(void) {
    /* reservado para regras de turno com o tabuleiro */
}

void verificarXeque(void) {
    /* reservado para detecção de xeque */
}

static int iniciarFluxoMysql(void) {
    printf("[1/4] A conectar ao MySQL...\n");
    if (conectarBanco() != 0) {
        printf("      Falhou. Jogo em modo local (sem gravar movimentos).\n");
        return 0;
    }

    printf("[2/4] A registar jogadores...\n");
    partida_atual.jogador_brancas_id = buscarOuCriarJogador("Jogador Brancas");
    partida_atual.jogador_pretas_id = buscarOuCriarJogador("Jogador Pretas");

    if (partida_atual.jogador_brancas_id < 0 ||
        partida_atual.jogador_pretas_id < 0) {
        fprintf(stderr, "      Nao foi possivel registar jogadores.\n");
        return 0;
    }

    printf("      Brancas id=%d | Pretas id=%d\n",
           partida_atual.jogador_brancas_id, partida_atual.jogador_pretas_id);

    printf("[3/4] A abrir partida na base de dados...\n");
    partida_atual.partida_id =
        iniciarPartida(partida_atual.jogador_brancas_id,
                       partida_atual.jogador_pretas_id);

    if (partida_atual.partida_id < 0) {
        fprintf(stderr, "      Nao foi possivel iniciar a partida.\n");
        return 0;
    }

    printf("      Partida #%d criada.\n", partida_atual.partida_id);
    return 1;
}

void iniciarJogo(void) {
    Board board;

    printf("=== Jogo de Xadrez ===\n\n");

    printf("[0/4] A preparar o tabuleiro...\n");
    bb_inicializar_hash();
    resetarTabuleiro(&board);
    printarTabuleiro(&board);
    printf("\n");

    iniciarFluxoMysql();

    printf("[4/4] A abrir interface grafica (Raylib)...\n");
    printf("      Clique: casa de origem -> casa de destino\n");
    printf("      Tecla R: nova partida | ESC: sair\n\n");

    iniciarInterface(&board);

    if (bancoConectado() && partida_atual.partida_id > 0) {
        printf("\nPartida #%d permanece aberta (sem vencedor definido).\n",
               partida_atual.partida_id);
        printf("Use finalizarJogo() no codigo ou feche manualmente no MySQL.\n");
    }

    printf("\nFim do jogo.\n");
}
