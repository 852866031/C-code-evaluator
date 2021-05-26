// Programmer : Jiaxuan Chen
// Created : 5/15/2021 to rate documentation
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
int rate_doc_head(char *filename);
int *rate_doc_func(char *filename);

int rate_doc(char **filenames, int file_num){
    // ---------------------------------------------------
    // Created : 5/26/2021 main rating function
    // Purpose:
    // rate all the documentation of the input files
    // 
    // Parameters : all the filenames and the number of files 
    // Returns : constant value
    // Side-effects : no side effects


    /*after you finish evaluate the files
    rememeber to use function fill_doc_summary to record the summary of your evaluation
    and use flag_code to flag the improper code
    these two functions are in output.c, check them out! 
    and modify it if you want!*/
    printf("rate_doc called\n");
    int num=0;
    int hits=0;
    int *result;
    for(int i=0; i<file_num; i++){
        num+=4;
        hits+=rate_doc_head(filenames[i]);
        result=rate_doc_func(filenames[i]);
        num+=result[1];
        hits+=result[0];
    }
    fill_doc_summary(hits, num);
    free(result);
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

int rate_doc_head(char *filename){
    int grade=0;
    int index;
    int size;
    int counter;
    char cur;
    char pre;
    char *sub;
    char *list[4] = {"Programmer", "Created", "Purpose", "Modifications"};
    char **tokens;
    FILE *fp = fopen(filename, "r");
    do {cur=fgetc_plus(fp); } while(cur==' ' || cur=='\t' || cur =='\n');
    size=ftell(fp);
    if(size==0){
        flag_code(filename, 0, "No documentation for this file, 4 hits missed", "-FILE START-\n");
        return 0;
    }
    char comment[size];
    fseek(fp, 0, SEEK_SET);
    for(int i=0; i<size; i++) comment[i]=fgetc(fp);
    for(index=1; index<size; index++){
        cur=comment[index];
        pre=comment[index-1];
        if(cur=='/' && pre=='/') break;
        if(cur=='*' && pre=='/') break;
    }
    if(index==size){
        flag_code(filename, 0, "No documentation for this file", "-FILE START-\n");
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
            flag_code(filename, at_line(fp), "Programmer tokens not sufficient, 1 hit missed", "--FILE START--\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fp), "Programmer section missing, 1 hit missed", "--FILE START--\n");

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
            flag_code(filename, at_line(fp), "Created tokens not sufficient, 1 hit missed", "--FILE START--\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fp), "Created section missing, 1 hit missed", "--FILE START--\n");
    
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
            flag_code(filename, at_line(fp), "Purpose tokens not sufficient, 1 hit missed", "--FILE START--\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fp), "Purpose section missing, 1 hit missed", "--FILE START--\n");

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
            flag_code(filename, at_line(fp), "Modifications tokens not sufficient, 1 hit missed", "--FILE START--\n");
        }else grade++;
    }
    else flag_code(filename, at_line(fp), "Modifications section missing, 1 hit missed", "--FILE START--\n");
    free(tokens);
    return grade;
}

int *rate_doc_func(char *filename){
    FILE *fp;
    struct FUNC *cur_func;
    char *comment=NULL;
    char *sub;
    char **tokens;
    char cur;
    char pre;
    char func_info[100];
    char *list[6] = {"Programmer", "Created", "Purpose", "Parameters", "Returns", "Side-effects"};
    int size;
    int line_num;
    int index;
    int counter;
    int total=0;
    int grade=0;
    int *result=malloc(2);
    int record=0;
    fp = fopen(filename, "r");
    if(fp==NULL) error_dealer(-2);
    do{
        size=0;
        line_num=0;
        index=0;
        fseek(fp, record, SEEK_SET);
        cur_func=get_next_function(fp);
        record=ftell(fp);
        if(cur_func==NULL) break;
        total+=5;
        sprintf(func_info, "Function: %s %s\n", cur_func->name, cur_func->parameters);
        fseek(fp, cur_func->func_start, SEEK_SET);
        line_num=at_line(fp);
        do {cur=fgetc_plus(fp); } while(cur==' ' || cur=='\t' || cur =='\n');
        size=ftell(fp)-cur_func->func_start;
        if(size==0){
            flag_code(filename, line_num, "No documentation for this function, 5 hits missed", func_info);
            continue;
        }
        if(comment==NULL) comment=malloc(size*sizeof(char));
        else comment=realloc(comment, size);
        fseek(fp, cur_func->func_start, SEEK_SET);
        for(index=0; index<size; index++) comment[index]=fgetc(fp);
        for(index=1; index<size; index++){
            cur=comment[index];
            pre=comment[index-1];
            if(cur=='/' && pre=='/') break;
            if(cur=='*' && pre=='/') break;
        }
        if(index==size){
            flag_code(filename, line_num, "No documentation for this function, 5 hits missed", func_info);
            continue;
        }
        
        sub=strstr(comment, "--------------------");
        total++;
        if(sub==NULL) flag_code(filename, line_num, "No horizontal line or horizontal line not long enough, 1 hit missed", func_info);
        else grade++;
        sub=strstr(comment, "Programmer");
        if(sub!=NULL){
            total++;
            tokens=tokenize_doc(sub);
            counter=0;
            index=1;
            while(tokens[index]!=NULL && is_in_list(tokens[index], list, 4)!=1){
                index++;
                if(strcmp(tokens[index], "//")==0) continue;
                counter++;
            }
            if(counter<2){
                flag_code(filename, line_num, "Programmer tokens not sufficient, 1 hit missed", func_info);
            }else grade++;
        }

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
                flag_code(filename, line_num, "Created tokens not sufficient, 1 hit missed", func_info);
            }else grade++;
        }
        else flag_code(filename, line_num, "Created section missing, 1 hit missed", func_info);

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
                flag_code(filename, line_num, "Purpose tokens not sufficient, 1 hit missed", func_info);
            }else grade++;
        }
        else flag_code(filename, line_num, "Purpose section missing, 1 hit missed", func_info);

        sub = strstr(comment, "Parameters");
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
            if(counter<2){
                flag_code(filename, line_num, "Parameters tokens not sufficient, 1 hit missed", func_info);
            }else grade++;
        }
        else flag_code(filename, line_num, "Parameters section missing, 1 hit missed", func_info);

        sub = strstr(comment, "Returns");
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
            if(counter<2){
                flag_code(filename, line_num, "Returns tokens not sufficient, 1 hit missed", func_info);
            }else grade++;
        }
        else flag_code(filename, line_num, "Returns section missing, 1 hit missed", func_info);

        sub = strstr(comment, "Side-effects");
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
            if(counter<2){
                flag_code(filename, line_num, "Side-effects tokens not sufficient, 1 hit missed", func_info);
            }else grade++;
        }
        else flag_code(filename, line_num, "Side-effects section missing, 1 hit missed", func_info);
    }while(cur_func!=NULL);
    free(tokens);
    free(comment);
    result[0]=grade;
    result[1]=total;
    return result;
}