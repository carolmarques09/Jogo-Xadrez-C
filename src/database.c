/* parte de Pedro */

#include <mysql.h>
#include <stdio.h>

MYSQL *conn;

void conectarBanco() {

    conn = mysql_init(NULL);

    mysql_real_connect(
        conn,
        "localhost",
        "root",
        "DataBase*801200",
        "xadrez",
        3306,
        NULL,
        0
    );

    printf("Banco conectado!\n");
}