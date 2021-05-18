// Programmer : Jiaxuan Chen
// Created : Jiaxuan Chen at 5/11/2021
// Purpose:
// Create two temporay files to record the summary and flagged code
// merge them into a outoput file when the program ends
//
// Modifications:
// Initial Date Short Description 
// <none>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *summary=NULL;
FILE *flagged_code=NULL;

int create_output_files(){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // create to files to record the summary and flagged code
    //
    // Parameters : NULL
    // Returns : 1 for normal
    // Side-effects : two files will be created in current directory

    if(summary==NULL) {
        summary=fopen("c_evaluatot_summary", "w+");
    }
    if(flagged_code==NULL){
        flagged_code=fopen("c_evaluator_flagged", "w+");
    }
    return 1;
}

int fill_modular_summary(int hits, int nums){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // record the summary for modular programming test
    //
    // Parameters : the hits number and the total number
    // Returns : 1 for normal
    // Side-effects : NULL

    float result = 100*((float) hits)/((float) nums);
    char *line = malloc(100*sizeof(char));
    fseek(summary, 0, SEEK_END);
    sprintf(line, "Modular programming rating:     [%d]/[%d] hits = [%.2f%%]\n", hits, nums, result);
    fputs(line, summary);
    free(line);
    return 1;
}

int fill_indentation_summary(int hits, int nums){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // record the summary for code indentation programming test
    //
    // Parameters : the hits number and the total number
    // Returns : 1 for normal
    // Side-effects : NULL

    float result = 100*((float) hits)/((float) nums);
    char *line = malloc(100*sizeof(char));
    fseek(summary, 0, SEEK_END);
    sprintf(line, "Code indentation rating:        [%d]/[%d] hits = [%.2f%%]\n", hits, nums, result);
    fputs(line, summary);
    free(line);
    return 1;
}

int fill_comment_summary(int hits, int nums){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // record the summary for commenting programming test
    //
    // Parameters : the hits number and the total number
    // Returns : 1 for normal
    // Side-effects : NULL

    float result = 100*((float) hits)/((float) nums);
    char *line = malloc(100*sizeof(char));
    fseek(summary, 0, SEEK_END);
    sprintf(line, "Commenting rating:              [%d]/[%d] hits = [%.2f%%]\n", hits, nums, result);
    fputs(line, summary);
    free(line);
    return 1;
}

int fill_doc_summary(int hits, int nums){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // record the summary for documentation programming test
    //
    // Parameters : the hits number and the total number
    // Returns : 1 for normal
    // Side-effects : NULL

    float result = 100*((float) hits)/((float) nums);
    char *line = malloc(100*sizeof(char));
    fseek(summary, 0, SEEK_END);
    sprintf(line, "Documentation rating:           [%d]/[%d] hits = [%.2f%%]\n", hits, nums, result);
    fputs(line, summary);
    free(line);
    return 1;
}

int fill_var_summary(int hits, int nums){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // record the summary for variable naming programming test
    //
    // Parameters : the hits number and the total number
    // Returns : 1 for normal
    // Side-effects : NULL

    float result = 100*((float) hits)/((float) nums);
    char *line = malloc(100*sizeof(char));
    fseek(summary, 0, SEEK_END);
    sprintf(line, "Poor variable names rating:     [%d]/[%d] hits = [%.2f%%]\n", hits, nums, result);
    fputs(line, summary);
    free(line);
    return 1;
}

int fill_testcases_summary(int hits, int nums){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // record the summary for inner-built test programming test
    //
    // Parameters : the hits number and the total number
    // Returns : 1 for normal
    // Side-effects : NULL

    float result = 100*((float) hits)/((float) nums);
    char *line = malloc(100*sizeof(char));
    fseek(summary, 0, SEEK_END);
    sprintf(line, "Built-in test cases rating:     [%d]/[%d] hits = [%.2f%%]\n", hits, nums, result);
    fputs(line, summary);
    free(line);
    return 1;
}

int flag_code(char *filename, int line_number, char* error, char *code){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // record any flagged code in a file
    //
    // Parameters : the flagged filename the flagged line of code, the error description, the flagged line of code
    // Returns : 1 for normal
    // Side-effects : NULL

    char *line = malloc(100*sizeof(char));
    fseek(flagged_code, 0, SEEK_END);
    fputs("\n", flagged_code);
    sprintf(line, "[%s] [line: %d] [%s]\n", filename, line_number, error);
    fputs(line, flagged_code);
    fputs(code, flagged_code);
    fputs("\n", flagged_code);
    free(line);
    return 1;
}

int give_output(){
    // ---------------------------------------------------
    // Created : 5/11/2021
    // Purpose:
    // delete the two source file for summary and flagged code and generate the output in correct format
    // then display the output
    //
    // Parameters : NULL
    // Returns : 1 for normal
    // Side-effects : two source file will be deleted

    char *line = malloc(100*sizeof(char));
    char ch;
    rewind(summary);
    rewind(flagged_code);
    FILE *output = fopen("c_evaluator_output.txt", "w+");
    fputs("Ceval version 1.0\n", output);
    fputs("---= Code Evaluator Summary =---\n", output);
    while(fgets(line, 100, summary)!=NULL) {
        fputs(line, output);
    }
    fputs("---= Flagged Code =---\n", output);
    while(fgets(line, 100, flagged_code)!=NULL){
        fputs(line, output);
    }
    fputs("--= ceval END =--\n", output);
    fclose(summary);
    fclose(flagged_code);
    remove("c_evaluatot_summary");
    remove("c_evaluator_flagged");
    rewind(output);
    while((ch=fgetc(output))!=EOF) printf("%c", ch);
    fclose(output);
    free(line);
    return 1;
}