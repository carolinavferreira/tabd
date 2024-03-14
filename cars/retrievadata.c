#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void print_divider(int *field_lengths, int num_fields) {
    printf("+");
    for (int i = 0; i < num_fields; i++) {
        for (int j = 0; j < field_lengths[i] + 2; j++) { // +2 para espaço extra nas laterais
            printf("-");
        }
        printf("+");
    }
    printf("\n");
}

int main(int argc, char **argv)
{
  MYSQL *con = mysql_init(NULL);

  if (con == NULL)
  {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
  }

  if (mysql_real_connect(con, "localhost", "guest", "34klq*",
          "testdb", 0, NULL, 0) == NULL)
  {
      finish_with_error(con);
  }

  if (mysql_query(con, "SELECT * FROM cars"))
{
      finish_with_error(con);
  }

  MYSQL_RES *result = mysql_store_result(con);

  if (result == NULL)
  {
      finish_with_error(con);
  }

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  MYSQL_FIELD *field;
  int *field_lengths; 
  field_lengths = malloc(num_fields * sizeof(int));
  // Inicializar array com 0
  for(int i = 0; i < num_fields; i++) {
    field_lengths[i] = 0;
  }
   // print_divider(field_lengths, num_fields);
int i = 0;
  while((field = mysql_fetch_field(result))) {
    field_lengths[i] = field->max_length;
    if(IS_NUM(field->type)){
        printf("|%*s", (field_lengths[i]+2), field->name);
    } else {
        printf("|%-*s", (field_lengths[i]+2), field->name);
    }
    i++;
  }
  printf("\n");
  print_divider(field_lengths, num_fields);
    
  while ((row = mysql_fetch_row(result)))
  {
    printf("|");
      for(int i = 0; i < num_fields; i++)
      {
          printf("%-*s|", (field_lengths[i]+2), row[i] ? row[i] : "NULL");
      }

      printf("\n");
  }
  print_divider(field_lengths, num_fields);
  mysql_free_result(result);
  mysql_close(con);

  exit(0);
}