#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"helpers.h"
#include"main.h"

char **types;
int type_number=0;

char fgetc_plus(FILE *fp){

    // ---------------------------------------------------
    // Created : 5/21/2021
    // Purpose: 
    // skip the contents between ' ', " ", { }, /**/ and after //
    //
    // Parameters : file pointer
    // Returns : same as fgetc after skipping the contents
    // Side-effects : move the file pointer by one
    int brackets = 0;
    int cur, next;

    cur = fgetc(fp);
    next = fgetc(fp);
    if (feof(fp)) return cur;
    fseek(fp, -1, SEEK_CUR);
    if (cur == '\n') return ' ';

    // skip character
    if (cur == '\'') {
        while(1){
            cur = fgetc(fp);
            if (cur == '\'') return fgetc_plus(fp);
        }
    }

    // skip string
    if (cur == '\"') {
        while(1){
            cur = fgetc(fp);
            if (cur == '\"')  return fgetc_plus(fp); 
        }
    }

    // skip single-line comment
    if (cur == '/' && next == '/') {
        fseek(fp, 1, SEEK_CUR);
        while(1){
            cur = fgetc(fp);
                if (cur == '\n') return ' ';
                if (cur == EOF) return cur;
        }
    }

    // skip multi-line comment
    if (cur == '/' && next == '*') {
        fseek(fp, 1, SEEK_CUR);
        while(1){
            cur = fgetc(fp);
            next = fgetc(fp);
            if (feof(fp)) return cur;
            fseek(fp, -1, SEEK_CUR);
            if (cur == '*' && next == '/') {
                fseek(fp, 1, SEEK_CUR);
                return fgetc_plus(fp);
            }
        }
    }
    
    // skip bracket
    if (cur == '{'){
        int temp;
        int offset = 0;
        brackets += 1;
        while(1){
            temp = fgetc_plus(fp);
            if (temp == '{') brackets ++;
            if (temp == '}') brackets --;
            if (brackets == 0){
                fseek(fp, -1, SEEK_CUR);
                return cur;
            }
        }
    }
    return cur;
}

char **tokenize_with_size(char *str, int* size) {
    char **tokens;
    char *token;
    const char s[2] = " ";
    int i = 0;
    char *dup1 = strdup(str);
    char *dup2 = strdup(str);

    token = strtok(dup1, s);
    while( token != NULL ) {
        *size+=1;
        token = strtok(NULL, s);
    }

    tokens = malloc(*size * sizeof(char*));
    token = strtok(dup2, s);
    while( token != NULL ) {
        tokens[i++] = token;
        token = strtok(NULL, s);
    }

    return tokens;
}

char **get_defined_types(char *filename, int *num_element){
    FILE *fp;
    int cur;
    int buffer_len = 1024;
    char buffer[buffer_len];
    char** defined_types = malloc(sizeof(char*));
    char *ptr1, *ptr2, **words;
    int size = 0, count = 0;
    fp = fopen(filename, "r");
    while (1){
        cur = fgetc_plus(fp);
        if (cur == EOF) break;
        if (cur != ';'){
            //count += 1;
            if(cur=='{') {
                buffer[count++]=' ';
                buffer[count++]='{';
            } else if(cur=='}'){
                buffer[count++]='}';
                buffer[count++]=' ';
            } else{
                buffer[count++] = cur;
            }
        }
        else{
            //count += 1;
            buffer[count++] = 0;
            count = 0;
            ptr1 = strstr(buffer, "struct");
            ptr2 = strstr(buffer, "typedef");
            if (ptr1 != NULL || ptr2 != NULL) {
                //printf("buffer is : %s \n", buffer);
                words = tokenize_with_size(buffer, &size);

                // for (int i = 0; i < size; i++){
                //     printf("%s ", words[i]);
                // }
                // printf("\n-----------\n");

                /*
                case 1: struct a{...}; 
                        store "struct a";
                */
                if (ptr1 != NULL && ptr2 == NULL){
                    *num_element += 1;
                    defined_types = realloc(defined_types, *num_element * sizeof(char*));
                    char *result;
                    char *temp = strdup(words[1]);
                    result = malloc(strlen("struct ") + strlen(temp) + 1);
                    strcat(result, "struct ");
                    strcat(result, temp);
                    free(temp);
                    defined_types[*num_element -1] = strdup(result);
                }

                /*
                case 2: typedef a b
                        store "b"
                */
                else if (strstr(buffer, "{") == NULL){
                    *num_element += 1;
                    defined_types = realloc(defined_types, *num_element * sizeof(char*));
                    defined_types[*num_element -1] = strdup(words[size-1]);
                }

                /*
                case 3: typedef struct {} a;
                        store "a"
                */

                else if (words[2][0] == '{'){
                    *num_element += 1;
                    defined_types = realloc(defined_types, *num_element * sizeof(char*));
                    defined_types[*num_element -1] = strdup(words[size-1]);
                }
                
                /*
                case 4: typedef struct a {} b;
                        store "sturct a" and "b"
                */

                else{
                    char *result, *dup;
                    *num_element += 1;
                    defined_types = realloc(defined_types, *num_element * sizeof(char *));
                    dup = strdup(words[2]);
                    result = malloc(strlen("struct ") + strlen(dup) + 1);
                    strcat(result, "struct ");
                    strcat(result, dup);
                    free(dup);
                    defined_types[*num_element-1] = strdup(result);
                    *num_element += 1;
                    defined_types = realloc(defined_types, *num_element * sizeof(char*));
                    defined_types[*num_element -1] = strdup(words[size-1]);
                    
                }
                size = 0;
            }
        }  
    }
    fclose(fp);
    return defined_types;
};

void to_upper(char* string)
{
    const char OFFSET = 'a' - 'A';
    while (*string)
    {
        *string = (*string >= 'a' && *string <= 'z') ? *string -= OFFSET : *string;
        string++;
    }
}

int fn_exists(char *filename, char *fn_to_check){
    // ---------------------------------------------------
    // Created : Yuyan Chen 5/24/2021
    // Purpose: 
    // check if the function contains a main function or not
    //
    // Parameters : a file pointer
    // Returns : 1 if there is a main function, 0 otherwise
    // Side-effects : None
    FILE *fp = fopen(filename, "r");
    int cur;
    int buffer_len = 1024, count = 0;
    char buffer[buffer_len];
    while (1) {
        cur = fgetc_plus(fp);
        if (cur == EOF) break;
        if (cur != '{'){
            //count += 1;
            buffer[count++] = cur;
        }
        else{
            //count += 1;
            buffer[count++] = 0;
            count = 0;
            if (strstr(buffer, fn_to_check) != NULL && strstr(buffer, "(") != NULL && strstr(buffer, ")") != NULL) return 1;
        }
    }
    fclose(fp);
    return 0;
}

int at_line(FILE *f){
    // which line you are currently at the file
    int ini = ftell(f);
    rewind(f);
    int cur = 0;
    int line = 1;
    char c;
    while(cur < ini){
        c=fgetc(f);
        if(c=='\n') line++;
        cur = ftell(f);
    }
    fseek(f, ini, SEEK_SET);
    return line;
}

int in_list(char *string, char **stringlist, int list_size){
    for(int i=0; i<list_size; i++){
        if(strcmp(string, stringlist[i])==0) return 1;
    }
    return 0;
}

int collect_all_types(char **filenames, int filenumber){
    char *default_types[7]={"int", "char", "void", "float", "double", "short", "long"};
    type_number=7;
    int index=0;
    char **buffer;
    int num_element;
    types=malloc(type_number*sizeof(char *));
    for(int i=0; i<7; i++){
        types[i]=strdup(default_types[i]);
        index++;
    }
    for(int i=0; i<filenumber; i++){
        buffer = get_defined_types(filenames[i], &num_element);
        for(int j=0; j<num_element; j++){
            if(in_list(buffer[j], types, type_number)==0){
                type_number++;
                types=realloc(types, type_number*sizeof(char *));
                types[index++]=strdup(buffer[j]);
            }
        }
    }
    free(buffer);
    return 1;
}


struct FUNC *get_next_function(FILE *fp){
    if(fp==NULL) return NULL;
    char cur=0;
    int index1=0;
    int index2=0;
    int index3=0;
    int tmp;
    int start;
    char buffer[1024];
    char para_buffer[1024];
    char name[100];
    int empty_para=0;
    while(1){
        index1=0;
        index2=0;
        index3=0;
        tmp=0;
        cur=0;
        while(cur!='{' && !feof(fp)) {
            start=ftell(fp);
            cur=fgetc(fp);
            buffer[index1++]=cur;
            if(cur==EOF) return NULL;
        }
        buffer[index1]='\0';
        for(tmp=strlen(buffer)-2; tmp>=0; tmp--){
            if(buffer[tmp]!=' ')break;
        }
        if(tmp<0 || buffer[tmp]!=')') continue;
        for(tmp=tmp; tmp>=0; tmp--){
            if(buffer[tmp]=='(') break;
        }
        if(tmp<0 || buffer[tmp]!='(') continue;
        if(buffer[tmp+1]==')') empty_para=1;
        else{
            for(int i=tmp; i<strlen(buffer); i++) {
                para_buffer[index2++]=buffer[i];
                if(buffer[i]==')') break;
            }
            para_buffer[index2]='\0';
        }
        tmp--;
        while(buffer[tmp]==' ') tmp--;
        while(buffer[tmp]!=' ' && tmp>=0) tmp--;
        tmp++;
        if(tmp<0) continue;
        while(buffer[tmp]=='*') tmp++;
        while(buffer[tmp]!='(' && buffer[tmp]!=' ') name[index3++]=buffer[tmp++];
        name[index3]='\0';
        break;
    }
    struct FUNC *func=malloc(sizeof(struct FUNC));
    func->func_start=start+1;
    fseek(fp, start, SEEK_SET);
    fgetc_plus(fp);
    func->offset=ftell(fp)-start;
    func->name=strdup(name);
    if(empty_para) func->parameters=NULL;
    else{
        for(tmp=0; tmp<strlen(para_buffer); tmp++){
            if(para_buffer[tmp]!=' ') break;
        }
        if(tmp==strlen(para_buffer)) func->parameters=NULL;
        else func->parameters=strdup(para_buffer);
    }
    return func;
}

