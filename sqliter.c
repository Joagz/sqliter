// Created by cpm (C Project Manager) | Author: Joaquin Gomez

#include "./libs/commons.h"
#include <stdio.h>
#include <stdlib.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char **argv) 
{

    struct t_dict_arr *arr = (struct t_dict_arr *) malloc(sizeof(struct t_dict_arr));
    arr->values = (struct t_dict *) malloc(sizeof(struct t_dict) * 2);

    struct t_dict val1 = {"val1", INT, "1"};

    dict_append(arr, val1);

    printf("LEN %d\n", arr->length);

    if(connect() != 0) exit(-1);
  
    // char * vals[] = {"2", "'chau'"};

    // insert_into("test", vals, 2);

    // delete_where("test", arr);

    select_all("test", callback, 100);

    select_where("test", callback, arr);
    return 0;
}
