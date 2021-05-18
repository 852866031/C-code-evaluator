// Programmer : Jiaxuan Chen
// Created : Jiaxuan Chen at 5/15/2021
// Purpose:
// The main program
//
// Modifications:
// Initial Date Short Description 
// <none>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"helpers.h"
//function is like
//type name(parameters){}

char **types=NULL;
int in_comment=0;
int in_string=0;

long avoid_comment(char *des, int size, FILE *file){
    char buffer[size-1];
    char *tmp;
    char cur=0;
    char next=0;
    int flag=0;
    int index=0;
    if(feof(file)) {
        in_comment=0;
        return EOF;
    }
    for(int i=0; i<size; i++){
        des[i]='\0';
    }
    for(int i=0; i<size-1; i++) buffer[i]='\0';
    if(in_comment){
        fgets(buffer, size-1, file);
        tmp=strstr(buffer, "*/");
        while(tmp==NULL) {
            fgets(buffer, size-1, file);
            tmp=strstr(buffer, "*/");
        }
        if(strlen(tmp)==2){
            in_comment=0;
            return avoid_comment(des, size, file);
        }
        for(int i=2; i<strlen(tmp); i++){
            des[i-2]=tmp[i];
        }
        in_comment=0;
        return ftell(file);
    }
    fgets(buffer, size-1, file);
    if(strstr(buffer, "//")==NULL && strstr(buffer, "/*")==NULL){
        for(int i=0; i<strlen(buffer); i++){
            des[i]=buffer[i];
        }
        des[strlen(des)]='\0';
        return ftell(file);
    }
    for(int i=0; i<strlen(buffer)-1; i++){
        cur=buffer[i];
        next=buffer[i+1];
        if(cur=='/' && next=='/') {
            des[index]='\n';
            index++;
            break;
        }
        if(cur=='/' && next=='*') {
            in_comment=1;
            continue;
        }
        if(cur=='*' && next=='/') {
            in_comment=0;
            continue;
        }
        if(in_comment==0 && cur!='/') {
            des[index]=cur;
            index++;
        }
        if(i==strlen(buffer)-2 && in_comment==0){
            des[index]=next;
            index++;
        }
    }
    if(in_comment==1){
        des[index]='\n';
        index++;
    }
    des[index]='\0';
    
    return ftell(file);
}

long read_line_code(char *des, int size, FILE *file){
    char buffer[size];
    char *tmp;
    int index=0;
    for(int i=0; i<size; i++){
        des[i]='\0';
    }
    for(int i=0; i<size; i++) buffer[i]='\0';
    avoid_comment(buffer, size, file);
    if(in_string){
        tmp=strchr(buffer, '"');
        while(tmp==NULL){
            avoid_comment(buffer, size, file);
            tmp=strchr(buffer, '"');
        }
        if(strlen(tmp)==1){
            in_string=0;
            return read_line_code(des, size, file);
        }
        for(int i=0; i<strlen(tmp); i++){
            des[i]=tmp[i];
        }
        des[strlen(tmp)]='\n';
        in_string=0;
        return ftell(file);
    }
    tmp=strchr(buffer, '"');
    if(tmp==NULL) {
        for(int i=0; i<strlen(buffer); i++){
            des[i]=buffer[i];
        }
        return ftell(file);
    }
    for(int i=0; i<strlen(buffer); i++){
        if(buffer[i]=='"' && in_string==0){
            des[index]='"';
            index++;
            in_string=1;
            continue;
        }
        if(buffer[i]=='"' && in_string==1){
            des[index]='"';
            index++;
            in_string=0;
            continue;
        }
        if(in_string==0){
            des[index]=buffer[i];
            index++;
        }
    }
    if(in_string==0 && des[strlen(des)-1]!='\n') des[index]='\n';
    return ftell(file);
}


char **get_defined_types(char *filename, int *num_element){}

int collect_all_types(char **filenames, int num){
    char *default_types={"int", "char", "void", "float", "double", "short", "long"};
    int number=7;
    int index=0;
    int len=0;
    int *num_element=&len;
    char **buffer;
    for(int i=0; i<num; i++){
        get_defined_types(filenames[i], num_element);
        number+=*num_element;
    }
    types[number];
    for(int i=0; i<num; i++){
        buffer = get_defined_types(filenames[i], num_element);
        for(int j=0; j<*num_element; j++){
            types[index]=strdup(buffer[j]);
            index++;
        }
    }
    for(int i=0; i<7; i++){
        types[index]=strdup(default_types[i]);
        index++;
    }
    free(buffer);
    return 1;
}

char ctegf(FILE *f){
    //note that ctegf is the reverse way to write fgetc
    //this function get the char before current position of f
    if(ftell(f)==SEEK_SET) return 0;
    fseek(f, -1, SEEK_CUR);
    char c=fgetc(f);
    fseek(f, -1, SEEK_CUR);
    return c;
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

char **tokenize(char *string) {
    char **tokens;
    char buffer[1000];
    int c=0, b=0, t=0;
    while(string[c]!=0){
        while(string[c]==' ' || string[c]=='\n' || string[c]==':') c++;
        if(string[c]==0) break;
        while(string[c]!=' ' && string[c]!=0 && string[c]!='\n') c++;
        t++;
    }
    tokens=malloc(sizeof(char *)*(t));
    c=0, t=0;
    while(string[c]!=0){
        while(string[c]==' ' || string[c]=='\n' || string[c]==':') c++;
        if(string[c]==0) break;
        b=0;
        while(string[c]!=' ' && string[c]!=0 && string[c]!='\n') {
            buffer[b]=string[c];
            b++;
            c++;
        }
        buffer[b]=0;
        tokens[t]=strdup(buffer);
        t++;
    }
    tokens[t]=NULL;
    return tokens;
}

