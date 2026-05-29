/* parte de Pedro */

#include "database.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NO_MYSQL

int conectarBanco(void) {
    fprintf(stderr,
            "MySQL nao disponivel (instale em C:/mysql ou recompile sem "
            "-DNO_MYSQL).\n");
    return -1;
}

void fecharBanco(void) {}

int bancoConectado(void) { return 0; }

int buscarOuCriarJogador(const char *nome) {
    (void)nome;
    return -1;
}

int iniciarPartida(int jogador1, int jogador2) {
    (void)jogador1;
    (void)jogador2;
    return -1;
}

int salvarMovimento(int partida_id, const char *jogada) {
    (void)partida_id;
    (void)jogada;
    return -1;
}

int finalizarPartida(int partida_id, int vencedor_id) {
    (void)partida_id;
    (void)vencedor_id;
    return -1;
}

#else

#include <mysql/mysql.h>

#define CONFIG_PATH "database/config.env"
#define SQL_BUF_SIZE 512

static MYSQL *conn;
static int conectado;

static void trim(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' ||
                        str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[--len] = '\0';
    }
    char *start = str;
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

static int carregarConfig(char *host, char *user, char *password, char *db_name,
                          unsigned int *port) {
    strcpy(host, "localhost");
    strcpy(user, "root");
    strcpy(password, "senha");
    strcpy(db_name, "xadrez");
    *port = 3306;

    FILE *file = fopen(CONFIG_PATH, "r");
    if (!file) {
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        trim(line);
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }

        char *eq = strchr(line, '=');
        if (!eq) {
            continue;
        }

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;
        trim(key);
        trim(value);

        if (strcmp(key, "DB_HOST") == 0) {
            strncpy(host, value, 127);
            host[127] = '\0';
        } else if (strcmp(key, "DB_USER") == 0) {
            strncpy(user, value, 127);
            user[127] = '\0';
        } else if (strcmp(key, "DB_PASSWORD") == 0) {
            strncpy(password, value, 255);
            password[255] = '\0';
        } else if (strcmp(key, "DB_NAME") == 0) {
            strncpy(db_name, value, 127);
            db_name[127] = '\0';
        } else if (strcmp(key, "DB_PORT") == 0) {
            *port = (unsigned int)atoi(value);
        }
    }

    fclose(file);
    return 0;
}

static int executarSql(const char *sql) {
    if (!conectado || conn == NULL) {
        return -1;
    }
    if (mysql_query(conn, sql) != 0) {
        fprintf(stderr, "Erro SQL: %s\n", mysql_error(conn));
        return -1;
    }
    return 0;
}

int conectarBanco(void) {
    if (conectado) {
        return 0;
    }

    char host[128];
    char user[128];
    char password[256];
    char db_name[128];
    unsigned int port;

    carregarConfig(host, user, password, db_name, &port);

    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "Erro: mysql_init falhou.\n");
        return -1;
    }

    if (mysql_real_connect(conn, host, user, password, db_name, port, NULL, 0) ==
        NULL) {
        fprintf(stderr, "Erro ao conectar: %s\n", mysql_error(conn));
        mysql_close(conn);
        conn = NULL;
        return -1;
    }

    conectado = 1;
    printf("Banco conectado (%s).\n", db_name);
    return 0;
}

void fecharBanco(void) {
    if (conn != NULL) {
        mysql_close(conn);
        conn = NULL;
    }
    conectado = 0;
}

int bancoConectado(void) {
    return conectado;
}

int buscarOuCriarJogador(const char *nome) {
    if (!conectado || nome == NULL || nome[0] == '\0') {
        return -1;
    }

    char nome_esc[120];
    mysql_real_escape_string(conn, nome_esc, nome, (unsigned long)strlen(nome));

    char sql[SQL_BUF_SIZE];
    snprintf(sql, sizeof(sql),
             "SELECT id FROM jogadores WHERE nome = '%s' LIMIT 1", nome_esc);
    if (executarSql(sql) != 0) {
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row != NULL && row[0] != NULL) {
        int id = atoi(row[0]);
        mysql_free_result(result);
        return id;
    }
    mysql_free_result(result);

    snprintf(sql, sizeof(sql), "INSERT INTO jogadores (nome) VALUES ('%s')",
             nome_esc);
    if (executarSql(sql) != 0) {
        return -1;
    }

    return (int)mysql_insert_id(conn);
}

int iniciarPartida(int jogador1, int jogador2) {
    if (!conectado || jogador1 <= 0 || jogador2 <= 0) {
        return -1;
    }

    char sql[SQL_BUF_SIZE];
    snprintf(sql, sizeof(sql),
             "INSERT INTO partidas (jogador1, jogador2, vencedor) "
             "VALUES (%d, %d, NULL)",
             jogador1, jogador2);
    if (executarSql(sql) != 0) {
        return -1;
    }

    return (int)mysql_insert_id(conn);
}

int salvarMovimento(int partida_id, const char *jogada) {
    if (!conectado || partida_id <= 0 || jogada == NULL || jogada[0] == '\0') {
        return -1;
    }

    char jogada_esc[40];
    mysql_real_escape_string(conn, jogada_esc, jogada,
                             (unsigned long)strlen(jogada));

    char sql[SQL_BUF_SIZE];
    snprintf(sql, sizeof(sql),
             "INSERT INTO movimentos (partida_id, jogada) VALUES (%d, '%s')",
             partida_id, jogada_esc);
    return executarSql(sql);
}

int finalizarPartida(int partida_id, int vencedor_id) {
    if (!conectado || partida_id <= 0 || vencedor_id <= 0) {
        return -1;
    }

    char sql[SQL_BUF_SIZE];
    snprintf(sql, sizeof(sql),
             "UPDATE partidas SET vencedor = %d WHERE id = %d", vencedor_id,
             partida_id);
    if (executarSql(sql) != 0) {
        return -1;
    }

    snprintf(sql, sizeof(sql),
             "UPDATE jogadores SET vitorias = vitorias + 1 WHERE id = %d",
             vencedor_id);
    return executarSql(sql);
}

#endif /* NO_MYSQL */
