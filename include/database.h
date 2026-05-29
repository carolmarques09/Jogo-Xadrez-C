#ifndef DATABASE_H
#define DATABASE_H

int conectarBanco(void);
void fecharBanco(void);
int bancoConectado(void);

int buscarOuCriarJogador(const char *nome);
int iniciarPartida(int jogador1, int jogador2);
int salvarMovimento(int partida_id, const char *jogada);
int finalizarPartida(int partida_id, int vencedor_id);

#endif
