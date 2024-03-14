#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUERY_LEN 1024
#define FILENAME "database_dump.pl"

void error_exit(const char *msg, MYSQL *conn) {
  fprintf(stderr, "%s: %s\n", msg, mysql_error(conn));
  mysql_close(conn);
  exit(1);
}

int main() {
  MYSQL *conn = mysql_init(NULL);
  if (conn == NULL) {
    fprintf(stderr, "Error initializing MySQL: %s\n", mysql_error(conn));
    exit(1);
  }

  const char *host = "localhost";
  const char *user = "guest";
  const char *password = "34klq*";
  const char *database = "tabd_test";

  if (!mysql_real_connect(conn, host, user, password, database, 0, NULL, 0)) {
    error_exit("Error connecting to MySQL", conn);
  }

  FILE *file = fopen(FILENAME, "w");
  if (file == NULL) {
    error_exit("Error opening file for writing", conn);
  }

  MYSQL_RES *tables = mysql_list_tables(conn, NULL);
  if (tables == NULL) {
    error_exit("Error listing tables", conn);
  }

  MYSQL_ROW table_row;
  while ((table_row = mysql_fetch_row(tables))) {
    const char *table_name = table_row[0];
    printf("Dumping %s\n", table_name);

    char query[MAX_QUERY_LEN];
    sprintf(query, "SELECT * FROM %s", table_name);

    if (mysql_query(conn, query)) {
      error_exit("Error executing query", conn);
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
      error_exit("Error storing result", conn);
    }

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result))) {
      int num_fields = mysql_num_fields(result);

      fprintf(file, "%s(", table_name);

      for (int i = 0; i < num_fields; i++) {
        if (i == num_fields - 1) {
          fprintf(file, "\'%s\'", row[i] ? row[i] : "NULL");
        } else {
          fprintf(file, "\'%s\', ", row[i] ? row[i] : "NULL");
        }
      }

      fprintf(file, ").\n");

      // fprintf(file, "%s(%s,%s).\n", table_name, row[1], row[2]);
    }

    mysql_free_result(result);
  }

  printf("Database dumped successfully to %s\n", FILENAME);
  fclose(file);
  mysql_close(conn);

  return 0;
}
