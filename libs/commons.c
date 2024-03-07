#include "commons.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sqlite3 * db;

void printsql(char * sql){
    printf("[SQLiter] SQL: %s\n", sql);
}

// Execute any SQL statement
int exec(char * sql, char * zErrMsg, int (*callback)(void *, int, char **, char **)){

    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "[SQLiter] error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        exit(-1);
    }

    return rc;

}

void check_connected(sqlite3 * db)
{
    if(db == NULL)
    {
        fprintf(stderr, "[SQLiter] Base de datos desconectada, porfavor conecta e intenta de nuevo\n");
        exit(-1);
    }   
}

// ============================ CONNECT AND DISCONNECT FUNCTIONS ============================

int disconnect()
{
    if(sqlite3_close(db) != SQLITE_OK) return -1;
    return 0;
}

int connect()
{
    int rc;
    
    rc = sqlite3_open(DB_NAME, &db);
    
    if( rc != SQLITE_OK )
    {
        fprintf(stderr, "[SQLiter] No se pudo conectar a la base de datos: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(-1);
    }

    return 0;
}

// ============================ EXECUTE COMMON SQL STATEMENTS ============================

void select_all(char * table, int (*callback)(void *, int, char **, char **), int limit)
{
    int rc;
    char *zErrMsg = 0;
    check_connected(db);
        
    char * def_str = "SELECT * FROM ";
    char * sql = (char*) malloc(strlen(table) + strlen(def_str));

    strcpy(sql, def_str);
    strcat(sql, table);

    exec(sql, zErrMsg, callback);
    printsql(sql);

    free(sql);

}

void select_where(char *table, int (*callback)(void *, int, char **, char **), struct t_dict_arr * arr)
{
    int rc;
    char *zErrMsg = 0;
    check_connected(db);

    size_t prompt_s = 0;

    for(int i = 0; i < arr->length; i++)
    {   
        if(arr->values[i]._t == INT){
            prompt_s += strlen(arr->values[i].key) + strlen(arr->values[i].value) + 6;
        } else {
            // add 2 for  ' ' and '=', and 2 more for quotes
            prompt_s += strlen(arr->values[i].key) + strlen(arr->values[i].value) + 8;         
        }
    }

    
    char * wh_clauses = (char*) malloc(prompt_s + strlen("WHERE "));

    strcpy(wh_clauses, "WHERE ");

    for(int i = 0; i < arr->length; i++)
    {
        strcat(wh_clauses, arr->values[i].key);
        strcat(wh_clauses, "=");
        // Determine type of value and add ''s or not if required
        if(arr->values[i]._t == STR){
            strcat(wh_clauses, "'");
            strcat(wh_clauses, arr->values[i].value);
            strcat(wh_clauses, "'");
        } else if(arr->values[i]._t == INT) {
            strcat(wh_clauses, arr->values[i].value);
        }
        strcat(wh_clauses, " ");
        if( i != arr->length-1)
            strcat(wh_clauses, "AND ");
    }

    char * def_str = "SELECT * FROM ";
    char * sql = (char*) malloc(strlen(table) + strlen(def_str) + strlen(wh_clauses) + 1);

    strcpy(sql, def_str);
    strcat(sql, table);
    strcat(sql, " ");
    strcat(sql, wh_clauses);

    exec(sql, zErrMsg, callback);
    printsql(sql);

    free(sql);
    free(wh_clauses);

}

int insert_into(char *table, char **values, int len)
{
    
    check_connected(db);
    char *zErrMsg = 0;

    size_t prompt_s = 0;

    for(int i = 0; i < len; i++){
        prompt_s += strlen(values[i]) + 1;
    }
    
    char * value_str = (char*) malloc(prompt_s + strlen(" VALUES ") + 3);
    
    strcpy(value_str, " VALUES ");
    strcat(value_str, "(");
    
    for(int i = 0; i < len; i++){
        strcat(value_str, values[i]);
        if(i != len-1)
            strcat(value_str, ",");
    }
    strcat(value_str, ")");
        
    char * def_str = "INSERT INTO ";
    char * sql = malloc(strlen(table) + strlen(def_str) + strlen(value_str));
    
    strcpy(sql, def_str);
    strcat(sql, table);
    strcat(sql, value_str);

    exec(sql, zErrMsg, 0x00);
    printsql(sql);

    free(sql);
    free(value_str);

    return EXIT_SUCCESS;

}


