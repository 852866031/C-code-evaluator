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