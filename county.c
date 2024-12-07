#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define _GNU_SOURCE

//structure to hold information
struct DemoInfo{
    char county[50];
    char state[50];
    float edu_bach;
    float edu_high;
    float eth_native;
    float eth_asian;
    float eth_black;
    float eth_hisp;
    float eth_island;
    float eth_white;
    int income_belowpov;
    int pop2014;
};

//function to display information for each entry
void display(struct DemoInfo *county, int blocks){
     int i;

    for (i = 0; i < blocks; i++){
        printf("Record %zu:\n", blocks);
        printf("  County: %s\n", (county + i)->county);
        printf("  State: %s\n",(county + i)->state);
        printf("  Education High: %.1f%%\n", (county + i)->edu_high);
        printf("  Education Bachelors: %.1f%%\n", (county + i)->edu_bach);
        printf("  Income: $%d\n", (county + i)->income_belowpov);
        printf("  Population: %d\n\n",(county + i)->pop2014);
    }
}



//function to filter by state
void filter_state(struct DemoInfo *stateinfo, char state, int blocks){
    //make new structure for filter
    struct DemoInfo *filtered;
    filtered  = (struct DemoInfo *)malloc(3200 *sizeof(struct DemoInfo));
    int entries = 0;
    //check if filter was succesful
    if (filtered == NULL){
        printf("Memory not allocated.\n");
        exit(0);
    }
    int i;
    //begin comparing states in structure to specified state
    for (i = 0; i < blocks; i++)
    {
        if ( strcmp( (stateinfo + i)->state != -1, state )){
            //if true, add state to filtered structure
            strcpy((filtered + i)->state, (stateinfo + i)->state);
            entries++;
        }
    }
    //begin printing contents of new structure
    printf("Filter: state == %s %d", state, entries);
    free(filtered);
}
//filter by ge/le
void filter_by_number(struct DemoInfo *stateinfo, char *field, char *compare, char *number, int blocks){
    //make new structure for filter
    struct DemoInfo *filtered;
    filtered  = (struct DemoInfo *)malloc(3200 *sizeof(struct DemoInfo));
    int entries = 0;
    //convert number back into int
    int num = atoi(number);
    //check if filter was succesful
    if (filtered == NULL){
        printf("Memory not allocated.\n");
        exit(0);
    }
    int i;
    int entries;

    //switch value to check what field we are comparing
    int sw;
    if (strcmp(field, "Education.High")){
        sw = 1;
    }
    else if(strcmp(field, "Education.Bachelor")){
        sw = 2;
    }
    else if(strcmp(field, "pop.2014")){
        sw = 3;
    }
    else if(strcmp(field, "Income.pov")){
        sw = 4;
    }

    

    //check which comparison we are doing
    if (strcmp("le", compare) != -1){
        switch(sw){

            //compare less than high school education
            case 1:
            for (i = 0; i < blocks; i++)
            {
                if ((stateinfo + i)->edu_high <num){
                    //if true, add to filtered structure
                    (filtered + i)->edu_high = (stateinfo + i)->edu_high;
                }
            } 
        }
    }
}



int main(int argc, char *argv[]){
    //check if at least three arguments are given
    if (argc != 3){
         fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    //open files and check if they exist
    //csv file
    printf("Opening files\n");
    FILE *csv = fopen(argv[1], "r");
     if (csv == NULL) {
        perror("Error opening file");
    }

    //operations file
    FILE *op = fopen(argv[2], "r");
     if (op == NULL) {
        perror("Error opening file");
    }

    //begin building structure
    struct DemoInfo *info;
    //dynamically allocated structure
    info = (struct DemoInfo *)malloc(3200 *sizeof(struct DemoInfo));
    //checks if allocation is succesful
    if (info== NULL){
        printf("Memory not allocated.\n");
        exit(0);
    }
    //values to read csv file
    char *line = NULL; //store each line
    unsigned int len = 0; //length of line
    int read; //used for reading each line
    int block = 0; //will be used for accessing blocks in allocated structure
    int size = 50; //size for strncpy

    //skip header line
    // getline(&line, (size_t *)&len, csv);
    //begin reading line by line
    while ((read = getline(&line, (size_t *)&len, csv)) != -1){
        // char *line_copy = strdup(line); // Duplicate line
        // //checks if line copy failed
        // if (line_copy == NULL) {
        // perror("Error duplicating line");
        // exit(1);
        // }
        //tokenize string since information is separated by commas
        char *token= strtok(line, ",");
        float value;

        //will be used for switch cases
        int index = 0;
        while (token != NULL){
            switch(index){
                //0 means we are in first column which cotains county
                case 0: 
                    //     (info + block)->county = (char *)malloc(strlen(token) + 1); // Allocate memory for the county name
                    //     if ((info + block)->county == NULL) {
                    //     perror("Memory allocation failed for county");
                    //     exit(1);
                    //  }
                        strncpy((info + block)->county, token, size);
                    //     printf("%s\n", token);
                    // (info + block)->county[size - 1] = '\0'; // Ensure null termination if size is smaller than the token length
                    // // (info + block)->county = strdup(token);
                        break;
                    

                //1 means we are in second column which contains state
                case 1:  
                        //  (info + block)->state = (char *)malloc(strlen(token) + 1); // Allocate memory for the state name
                        // if ((info + block)->state == NULL) {
                        //     perror("Memory allocation failed for state");
                        //     exit(1);
                        // }
                            strncpy((info + block)->state, token, size);
                        //      printf("%s\n", token);
                        //     (info + block)->state[size - 1] = '\0'; // Ensure null termination if size is smaller than the token length
                        // // (info + block)->state = strdup(token);
                            break;

                //5 means we are in sixth column which contains education for bachelors
                case 5: //remove leading and trailing commas
                        if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float and
                        //add value to structure
                        (info + block)->edu_bach = atof(token);

                break;

                //6 means high school education
                case 6:  if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                    
                        //add value to structure
                        (info + block)->edu_high = atof(token);

                break;
                //native american or alaska native
                case 11:  if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->eth_native = atof(token);

                break;

                //asian
                case 12:  if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->eth_asian = atof(token);

                break;

                //black
                case 13:  if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->eth_black = atof(token);

                break;

                //hispanic or latino
                case 14: if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->eth_hisp = atof(token);

                break;

                //pacific islander
                case 15:  if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->eth_island = atof(token);

                break;

                //white
                case 17: if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->eth_white =atof(token);

                break;

                //income below pov
                case 27: if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->income_belowpov = atoi(token);

                break;

                //2014 population
                case 37:  if (token[0] == '"') {
                             //Skip the first quote
                        token++;
                        }
                        if (token[strlen(token) - 1] == '"') {
                            // remove the last quote
                            token[strlen(token) - 1] = '\0';  // remove the last quote
                        }

                        // convert the token to a float
                        
                        //add value to structure
                        (info + block)->pop2014= atoi(token);

                break;
            }
            //move on to next field
            token = strtok(NULL, ",");
            index++;
        }
        //increase block by 1
        block++;
        printf("Total records processed: %d\n", block);
    }



    //begin reading ops file
    char *lineop = NULL;
    unsigned int lenop = 0;
    int readop;

    while ((read = getline(&lineop, (size_t *)&lenop, op)) != -1){
        //tokenize line
        char *token2= strtok(lineop, ":");
        //checks if command is filter-state
        if (strcmp(token2, "filter-state")){
            token2 = strtok(NULL,":");
            filter_state(info, token2, block);

        }
        //filter by ge/le
        else if(strcmp(token2, "filter")){

            char *field = strtok(NULL, ":");
            char *compare = strtok(NULL, ":");
            char *number_str = strtok(NULL, ":");
            if (field != NULL && compare != NULL && number_str != NULL) {
                    float number = atof(number_str);
                    filter_field(info, field, compare, number, block);
                }


        }

    }

   

    //test
    display(info, block);

    //free
    free(line);
    free(info);
    fclose(csv);
    fclose(op);
    
    return 0;
}