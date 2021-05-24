#ifndef _HELPERS_H
#define _HELPERS_H
struct FUNC{
    char *name;
    char *parameters;
    int func_start;
    int offset;
};
struct USER_DEFINED{
    char type_name;
    struct VAR **parameters;
};
int at_line(FILE *f);
#endif