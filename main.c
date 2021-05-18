// Programmer : Jiaxuan Chen
// Created : Jiaxuan Chen at 5/11/2021
// Purpose:
// The main program and the error handler
//
// Modifications:
// Initial Date Short Description 
// <none>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"parser.h"
#include"output.h"
#include"rate_comment.h"
#include"rate_doc.h"
#include"rate_indentation.h"
#include"rate_modular.h"
#include"rate_testcases.h"
#include"rate_var.h"

int error_dealer(int code){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // handles all kind of error code
    //
    // Parameters : error code
    // Returns : NULL
    // Side-effects : exist the program

    printf("Error code is %d\n", code);
    if(code==-1){
        printf("Input is not valid\n");
    }
    else if(code==-2){
        printf("File not exists");
    }
    exit(code);
}

int main(int argc, char *argv[]){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose: main
    // Parameters : command line arguments
    // Returns : 0
    // Side-effects : this program starts running, bugs might happen, so good luck everyone!

    int index=1;
    int *modes=malloc(MODENUM*sizeof(int));
    int tmp=0;
    int *file_num=&tmp;
    mode_parser(modes, argv, argc);
    //find out if the mode is on for "-help", "-ssf", "-nodoc", "-notest", "-novars"
    if(modes[0]==1) return help_command();
    for(int i=0; i<MODENUM; i++) index+=modes[i];
    //index is where the filename arguments begins
    char **filenames = filenames_parser(file_num, argv, argc, index);
    if(filenames==NULL){
        error_dealer(-1);
    }
    printf("\nParsed filenames are:\n");
    for(int i=0; i<*file_num; i++){
        printf("%s ", filenames[i]);
    }
    printf("\n\n");
    create_output_files();
    if(modes[1]==0) rate_modular(filenames, *file_num);
    rate_indentation(filenames, *file_num);
    rate_comment(filenames, *file_num);
    if(modes[2]==0) rate_doc(filenames, *file_num);
    if(modes[4]==0) rate_var(filenames, *file_num);
    if(modes[3]==0) rate_testcases(filenames, *file_num);
    printf("\n");
    give_output();
    free(filenames);
    free(modes);
    return 0;
}