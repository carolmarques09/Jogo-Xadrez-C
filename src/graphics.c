#include "graphics.h"

#include "board.h"
#include "database.h"
#include "game.h"
#include "movement.h"

#include <stdio.h>

/* board.h antes de raylib.h (macros WHITE/BLACK do Raylib) */
#include "raylib.h"

#define LARGURA_TABULEIRO 640
#define TAM_CASA (LARGURA_TABULEIRO / 8)
#define ALTURA_BARRA 56
#define ALTURA_JANELA (LARGURA_TABULEIRO + ALTURA_BARRA)

static Color corClara = {240, 217, 181, 255};
static Color corEscura = {181, 136, 99, 255};
static Color corSelecao = {255, 255, 0, 120};
static Color corUltima = {155, 199, 86, 160};

static int ultima_origem = -1;
static int ultima_destino = -1;

static int rankDoMouse(int y) {
    if (y < 0 || y >= LARGURA_TABULEIRO) {
        return -1;
    }
    return 7 - (y / TAM_CASA);
}

static int fileDoMouse(int x) {
    if (x < 0 || x >= LARGURA_TABULEIRO) {
        return -1;
    }
    return x / TAM_CASA;
}

static int indiceCasa(int rank, int file) {
    return RF(rank, file);
}

static const char *rotuloPeca(int piece) {
    switch (PIECE(piece)) {
        case PAWN:
            return COLOR(piece) ? "p" : "P";
        case KNIGHT:
            return COLOR(piece) ? "n" : "N";
        case BISHOP:
            return COLOR(piece) ? "b" : "B";
        case ROOK:
            return COLOR(piece) ? "r" : "R";
        case QUEEN:
            return COLOR(piece) ? "q" : "Q";
        case KING:
            return COLOR(piece) ? "k" : "K";
        default:
            return "";
    }
}

static void desenharCasas(Board *board, int origem) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int x = file * TAM_CASA;
            int y = (7 - rank) * TAM_CASA;
            Color cor = ((rank + file) % 2 == 0) ? corClara : corEscura;
            DrawRectangle(x, y, TAM_CASA, TAM_CASA, cor);

            int sq = indiceCasa(rank, file);
            if (sq == origem) {
                DrawRectangle(x, y, TAM_CASA, TAM_CASA, corSelecao);
            }
            if (sq == ultima_origem || sq == ultima_destino) {
                DrawRectangle(x, y, TAM_CASA, TAM_CASA, corUltima);
            }
        }
    }
    (void)board;
}

static void desenharPecas(Board *board) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int sq = indiceCasa(rank, file);
            int piece = board->squares[sq];
            if (piece == EMPTY) {
                continue;
            }

            int x = file * TAM_CASA;
            int y = (7 - rank) * TAM_CASA;
            const char *rotulo = rotuloPeca(piece);
            Color cor_texto = COLOR(piece) ? (Color){40, 40, 40, 255}
                                           : (Color){250, 250, 250, 255};
            int tamanho = 44;
            int largura = MeasureText(rotulo, tamanho);
            DrawText(rotulo, x + (TAM_CASA - largura) / 2, y + 10, tamanho,
                     cor_texto);
        }
    }
}

static void montarStatus(char *buf, size_t tamanho, Board *board) {
    const char *turno = (board->color == LADO_BRANCO) ? "Brancas" : "Pretas";

    if (bancoConectado() && partida_atual.partida_id > 0) {
        snprintf(buf, tamanho,
                 "Partida #%d | Vez: %s | MySQL: conectado | Clique: origem -> "
                 "destino | R: reiniciar",
                 partida_atual.partida_id, turno);
    } else if (bancoConectado()) {
        snprintf(buf, tamanho,
                 "MySQL: conectado (sem partida) | Vez: %s | R: reiniciar",
                 turno);
    } else {
        snprintf(buf, tamanho,
                 "MySQL: offline (modo local) | Vez: %s | R: reiniciar", turno);
    }
}

static void reiniciarPartida(Board *board) {
    resetarTabuleiro(board);
    ultima_origem = -1;
    ultima_destino = -1;

    if (bancoConectado()) {
        partida_atual.partida_id =
            iniciarPartida(partida_atual.jogador_brancas_id,
                           partida_atual.jogador_pretas_id);
        if (partida_atual.partida_id > 0) {
            printf("Nova partida #%d iniciada.\n", partida_atual.partida_id);
        }
    }
}

void iniciarInterface(Board *board) {
    InitWindow(LARGURA_TABULEIRO, ALTURA_JANELA, "Jogo de Xadrez");
    SetTargetFPS(60);

    int origem = -1;
    char status[256];

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            reiniciarPartida(board);
            origem = -1;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();
            if (mouse.y < LARGURA_TABULEIRO) {
                int file = fileDoMouse((int)mouse.x);
                int rank = rankDoMouse((int)mouse.y);
                if (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
                    int sq = indiceCasa(rank, file);

                    if (origem < 0) {
                        if (board->squares[sq] != EMPTY &&
                            COLOR(board->squares[sq]) == board->color) {
                            origem = sq;
                        }
                    } else if (sq == origem) {
                        origem = -1;
                    } else {
                        Move move = {
                            .src = (unsigned char)origem,
                            .dst = (unsigned char)sq,
                            .promotion = 0,
                        };
                        if (aplicarMovimento(board, &move)) {
                            registrarJogadaMove(&move);
                            ultima_origem = origem;
                            ultima_destino = sq;
                            origem = -1;
                        } else {
                            if (board->squares[sq] != EMPTY &&
                                COLOR(board->squares[sq]) == board->color) {
                                origem = sq;
                            } else {
                                origem = -1;
                            }
                        }
                    }
                }
            }
        }

        montarStatus(status, sizeof(status), board);

        BeginDrawing();
        ClearBackground((Color){50, 50, 50, 255});

        desenharCasas(board, origem);
        desenharPecas(board);

        DrawRectangle(0, LARGURA_TABULEIRO, LARGURA_TABULEIRO, ALTURA_BARRA,
                      (Color){35, 35, 35, 255});
        DrawText(status, 10, LARGURA_TABULEIRO + 12, 18, LIGHTGRAY);
        DrawText("ESC: sair", 10, LARGURA_TABULEIRO + 34, 16, GRAY);

        EndDrawing();
    }

    CloseWindow();
}
