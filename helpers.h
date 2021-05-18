#ifndef _HELPERS_H
#define _HELPERS_H
struct VAR{
    char *type;
    char *name;
};
struct FUNC{
    char *type;
    char *name;
    struct VAR **parameters;
    int func_start;
    int offset;
};
struct USER_DEFINED{
    char type_name;
    struct VAR **parameters;
};
char **tokenize(char *string);
int at_line(FILE *f);
#endif