#include "commons.h"
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

sqlite3 * db;
// ============================ INTERNAL FUNCTIONS ============================

static void check_connected(sqlite3 * db)
{
    if(db == NULL)
    {
        fprintf(stderr, "[SQLiter] Base de datos desconectada, porfavor conecta e intenta de nuevo\n");
        exit(-1);
    }   
}


static void printsql(char * sql){
    printf("[SQLiter] SQL: %s\n", sql);
}

static char * create_where_clause(struct t_dict_arr * arr)
{
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

    return wh_clauses;

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

int select_all(char * table, int (*callback)(void *, int, char **, char **), int limit)
{
    char *zErrMsg = 0;
    check_connected(db);
        
    char * def_str = "SELECT * FROM ";
    char * sql = (char*) malloc(strlen(table) + strlen(def_str));

    strcpy(sql, def_str);
    strcat(sql, table);

    if(exec(sql, zErrMsg, callback) == EXIT_FAILURE) return EXIT_FAILURE;
    printsql(sql);

    free(sql);

    return EXIT_SUCCESS;
}

int select_where(char *table, int (*callback)(void *, int, char **, char **), struct t_dict_arr * arr)
{
    char *zErrMsg = 0;
    check_connected(db);

    char * wh_clauses = create_where_clause(arr);
    
    char * def_str = "SELECT * FROM ";
    char * sql = (char*) malloc(strlen(table) + strlen(def_str) + strlen(wh_clauses) + 1);

    strcpy(sql, def_str);
    strcat(sql, table);
    strcat(sql, " ");
    strcat(sql, wh_clauses);

    if(exec(sql, zErrMsg, callback) == EXIT_FAILURE) return EXIT_FAILURE;
    printsql(sql);

    free(sql);
    free(wh_clauses);

    return EXIT_SUCCESS;
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

    if(exec(sql, zErrMsg, NULL) == EXIT_FAILURE) return EXIT_FAILURE;

    printsql(sql);

    free(sql);
    free(value_str);
    return EXIT_SUCCESS;

}

int delete_where(char *table, struct t_dict_arr *arr) 
{
    
    check_connected(db);
    char *zErrMsg = 0;

    char * wh_clauses = create_where_clause(arr);
    
    char * def_str = "DELETE FROM ";
    char * sql = (char*) malloc(strlen(table) + strlen(wh_clauses) + strlen(def_str) + 1);

    strcpy(sql, def_str);
    strcat(sql, table);
    strcat(sql, " ");
    strcat(sql, wh_clauses);

    printsql(sql);
    if(exec(sql, zErrMsg, NULL) == EXIT_FAILURE) return EXIT_FAILURE;

    free(sql);
    free(wh_clauses);

    return EXIT_SUCCESS;
}

int exec(char * sql, char * zErrMsg, int (*callback)(void *, int, char **, char **)){

    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "[SQLiter] error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}

