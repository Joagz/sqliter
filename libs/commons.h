#include <sqlite3.h>
#include "libtdict.h"

#ifndef DB_NAME
#define DB_NAME "test"
#endif // PROVIDED BY Makefile

// SELECT * FROM [TABLE]
// INSERT INTO [TABLE] (val1, val2, ... valn) values (val1, val2, ... valn)
// SELECT * FROM [TABLE] WHERE val1 = x
// DELETE FROM [TABLE] WHERE val1 = x

int connect();
int disconnect();
void select_all(char * table, int (*callback)(void *, int, char **, char **), int limit);
void select_where(char * table, int (*callback)(void *, int, char **, char **), struct t_dict_arr * arr);
int insert_into(char * table, char ** values, int len);
int delete_where(char * table, struct t_dict_arr * arr);


