#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);
  char query[256];

  if (con == NULL)
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "guest", "34klq*",
          "fib", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "DROP TABLE IF EXISTS fibonacci")) {
      finish_with_error(con);
  }

  if (mysql_query(con, "CREATE TABLE fibonacci(n INT PRIMARY KEY AUTO_INCREMENT, fib INT)")) {
      finish_with_error(con);
  }

  if (mysql_query(con, "INSERT INTO fibonacci VALUES(1,0)")) {
      finish_with_error(con);
  }

  if (mysql_query(con, "INSERT INTO fibonacci VALUES(2,1)")) {
      finish_with_error(con);
  }
  
  int f1 = 0, f2 = 1;
  for(int n = 3; n < 21; n++){
    int fn = f2 + f1;
    int temp = f2;
    sprintf(query, "INSERT INTO fibonacci VALUES(%d, %d)", n, fn);
    if (mysql_query(con, query)) {
        finish_with_error(con);
    }
    f2 = fn;
    f1 = temp;

  }

  mysql_close(con);
  exit(0);
}