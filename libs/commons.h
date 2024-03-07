#include <sqlite3.h>
#include "libtdict.h"

#ifndef DB_NAME
#define DB_NAME "test"
#endif

// ACLARACIONES:
// Podemos ver que hay, en casi todas las funciones definidas posteriormente, un argumento
// llamado 'callback'. Callback es pedida por sqlite3 para (una vez ejecutadas las funciones
// SQL) trabajar con los datos obtenidos.
//
// IMPORTANTE: Es decir que desde 'callback' podemos tomar esos datos y mandarlos a una lista para utilizarlos
// luego.

// CONECTAR Y DESCONECTAR LA BASE DE DATOS
int connect();
int disconnect();

// SELECT * FROM [TABLE]
int select_all(char * table, int (*callback)(void *, int, char **, char **), int limit);

// SELECT * FROM [TABLE] WHERE val1 = x
int select_where(char * table, int (*callback)(void *, int, char **, char **), struct t_dict_arr * arr);

// INSERT INTO [TABLE] values (val1, val2, ... valn)
int insert_into(char * table, char ** values, int len);

// DELETE FROM [TABLE] WHERE val1 = x1, val2 = x2 ..., valn = xn
int delete_where(char * table, struct t_dict_arr * arr);

// EJECUTAR UN COMANDO SQL
int exec(char * sql, char * zErrMsg, int (*callback)(void *, int, char **, char **));


