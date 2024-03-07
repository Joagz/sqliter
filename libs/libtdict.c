#include "libtdict.h"
#include <stdlib.h>

void dict_foreach(int (*callback)(), struct t_dict_arr arr)
{
    for(int i = 0; i < arr.length; i++)
    {
        (*callback)();
    }
}

void dict_append(struct t_dict_arr * arr, struct t_dict t)
{
    arr->values[arr->length] = t;
    arr->length = arr->length + 1;
}

void dict_pop(struct t_dict_arr * arr)
{
    free(&arr->values[arr->length]);
    arr->length = arr->length - 1;
}



