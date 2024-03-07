
// ... value types
#define INT 0x01
#define STR 0x02

typedef unsigned int type_d;
struct t_dict {
    char * key;
    type_d _t; // value types go here ...
    char * value;
};

struct t_dict_arr {
    struct t_dict* values;
    int length;
};

void dict_pop(struct t_dict_arr * arr);
void dict_append(struct t_dict_arr * arr, struct t_dict t);
void dict_foreach(int (*callback)(void), struct t_dict_arr arr);

