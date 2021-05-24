// Programmer : Jiaxuan Chen
// Created : 5/15/2021
// Purpose:
// rate the documentation of the input file
//
// Modifications:
// Initial Date Short Description 
// <none>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"output.h"
#include"main.h"
#include"helpers.h"
char **tokenize_doc(char *string);
int is_in_list(char *string, char **list, int length);
int rate_head_doc(char *filename);





int rate_doc(char **filenames, int file_num){
    // ---------------------------------------------------
    // Created : 
    // Purpose:
    // 
    //
    // Parameters : 
    // Returns : 
    // Side-effects : 


    /*after you finish evaluate the files
    rememeber to use function fill_doc_summary to record the summary of your evaluation
    and use flag_code to flag the improper code
    these two functions are in output.c, check them out! 
    and modify it if you want!*/
    printf("rate_doc called\n");
    int num=0;
    int hits=0;
    for(int i=0; i<file_num; i++){
        num+=4;
        hits+=rate_head_doc(filenames[i]);
    }
    fill_doc_summary(hits, num);
    return 1;
}

char **tokenize_doc(char *string) {
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

int is_in_list(char *string, char **list, int length){
    char *sub;
    for(int i=0; i<length; i++){
        sub=strstr(string, list[i]);
        if(sub!=NULL) return 1;
    }
    return 0;
}

int rate_head_doc(char *filename){
    int record;
    int counter=0;
    int grade=0;
    int index;
    char cur;
    char pre;
    char *sub;
    char *tmp;
    char **tokens;
    char *line = malloc(100*sizeof(char));
    char *comment;
    char *list[4] = {"Programmer", "Created", "Purpose", "Modifications"};
    FILE *fi = fopen(filename, "r");
    if(fi == NULL) error_dealer(-2); 
    while((cur=fgetc(fi))!='/') {
        if(cur==EOF) return 0;
    }
    record=ftell(fi);

    if((cur=fgetc(fi))=='*'){
        while(1){
            fgets(line, 100, fi);
            counter++;
            sub = strstr(line, "*/");
            if(sub!=NULL) break;
        }
    }
    else {
        fgets(line, 100, fi);
        counter++;
        while(1){
            fgets(line, 100, fi);
            sub = strstr(line, "//");
            if(sub==NULL) break;
            counter++;
        }
    }
    comment = malloc(100*counter*sizeof(char));
    fseek(fi, record, SEEK_SET);
    for(int i=0; i<counter; i++){
        fgets(line, 100, fi);
        strcat(comment, line);
    }
    if(strlen(comment)==0) {
        flag_code(filename, 0, "No documentation for this file", "-The document section-\n");
        return 0;
    }

    sub = strstr(comment, "Programmer");
    if(sub!=NULL){
        tokens=tokenize_doc(sub);
        counter=0;
        index=1;
        while(tokens[index]!=NULL && is_in_list(tokens[index], list, 4)!=1){
            index++;
            if(strcmp(tokens[index], "//")==0) continue;
            counter++;
        }
        if(counter<2){
            flag_code(filename, at_line(fi), "Programmer tokens not sufficient", "-The documentation section-\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fi), "Programmer section missing", "-The documentation section-\n");

    sub = strstr(comment, "Created");
    if(sub!=NULL){
        tokens=tokenize_doc(sub);
        counter=0;
        index=1;
        while(tokens[index]!=NULL && is_in_list(tokens[index], list, 4)!=1){
            index++;
            if(strcmp(tokens[index], "//")==0) continue;
            counter++;
        }
        if(counter<2){
            flag_code(filename, at_line(fi), "Created tokens not sufficient", "-The documentation section-\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fi), "Created section missing", "-The documentation section-\n");
    
    sub = strstr(comment, "Purpose");
    if(sub!=NULL){
        tokens=tokenize_doc(sub);
        counter=0;
        index=1;
        while(tokens[index]!=NULL && is_in_list(tokens[index], list, 4)!=1){
            index++;
            if(tokens[index]==NULL) break;
            if(strcmp(tokens[index], "//")==0) continue;
            counter++;
        }
        if(counter<4){
            flag_code(filename, at_line(fi), "Purpose tokens not sufficient", "-The documentation section-\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fi), "Purpose section missing", "-The documentation section-\n");

    sub = strstr(comment, "Modifications");
    if(sub!=NULL){
        tokens=tokenize_doc(sub);
        counter=0;
        index=1;
        while(tokens[index]!=NULL && is_in_list(tokens[index], list, 4)!=1){
            index++;
            if(tokens[index]==NULL) break;
            if(strcmp(tokens[index], "//")==0) continue;
            counter++;
        }
        if(counter<3){
            flag_code(filename, at_line(fi), "Modifications tokens not sufficient", "-The documentation section-\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fi), "Modifications section missing", "-The documentation section-\n");
    free(line);
    free(tokens);
    free(comment);
    fclose(fi);
    return grade;
}
