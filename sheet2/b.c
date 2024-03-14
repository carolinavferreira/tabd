#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
    fprintf(stderr, "Error initializing MySQL: %s\n", mysql_error(conn));
    exit(1);
    }
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "localhost";
    char *user = "guest";
    char *password = "34klq*"; /* set me first */
    char *database = "tabd_test";

    FILE *file = fopen("b.pl", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }


    // Conecte ao banco de dados 
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    // Person
    if (mysql_query(conn, "SELECT * FROM person")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);
    
    while ((row = mysql_fetch_row(res)) != NULL)
        fprintf(file, "person(%s,'%s',%s,'%s','%s').\n", row[0], row[1], row[2], row[3], row[4] ? row[4] : "NULL");

    //Marriage
    if (mysql_query(conn, "SELECT * FROM marriage")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL)
        fprintf(file, "marriage(%s,'%s',%s,'%s','%s').\n", row[0], row[1], row[2], row[3], row[4] ? row[4] : "NULL");
    
    
    //Parent
    if (mysql_query(conn, "SELECT * FROM parent")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL)
        fprintf(file, "parent(%s,%s,%s).\n", row[0], row[1], row[2]);
    
    mysql_free_result(res);

    // Feche a conexão
    mysql_close(conn);
    return 0;
}

