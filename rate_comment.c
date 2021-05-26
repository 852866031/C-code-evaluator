// Programmer : 
// Created : 
// Purpose:
//
//
// Modifications:
// Initial Date Short Description 
// <none>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"output.h"
#include"helpers.h"
#include"main.h"
int *one_comment_per_func(char *filename);

int rate_comment(char **filenames, int file_num){
    // ---------------------------------------------------
    // Created : 
    // Purpose:
    // 
    //
    // Parameters : 
    // Returns : 
    // Side-effects : 


    /*after you finish evaluate the files
    rememeber to use function fill_comment_summary to record the summary of your evaluation
    and use flag_code to flag the improper code
    these two functions are in output.c, check them out! 
    and modify it if you want!*/
    printf("rate_comment called\n");
    int num=0;
    int hits=0;
    int *result;
    for(int i=0; i<file_num; i++){
        result=one_comment_per_func(filenames[i]);
        num+=result[1];
        hits+=result[0];
    }
    fill_comment_summary(hits, num);
    free(result);
    return 1;
}

int *one_comment_per_func(char *filename){
    FILE *fp;
    struct FUNC *cur_func;
    int record=0;
    int total=0;
    int grade=0;
    int index;
    int line_num;
    char func_info[100];
    int *result=malloc(2);
    char pre;
    char cur;
    fp=fopen(filename, "r");
    if(fp==NULL) error_dealer(-2);
    do{
        fseek(fp, record, SEEK_SET);
        cur_func=get_next_function(fp);
        record=ftell(fp);
        if(cur_func==NULL) break;
        total++;
        index=0;
        line_num=at_line(fp);
        sprintf(func_info, "Function: %s %s\n", cur_func->name, cur_func->parameters);
        fseek(fp, cur_func->func_start, SEEK_SET);
        for(index=0; index<cur_func->offset; index++){
            pre=cur;
            cur=fgetc(fp);
            if(cur=='"') {
                do{cur=getc(fp);} while(cur!='"');
                cur=fgetc(fp);
                continue;
                //ignore string
            }
            if(pre=='/' && (cur=='/' || cur=='*')) {
                grade++;
                break;
            }
        }
        if(index==cur_func->offset){
            flag_code(filename, line_num, "This function has no comment, 1 hit missed", func_info);
        }
    }while(cur_func!=NULL);
    result[0]=grade;
    result[1]=total;
    return result;
}
