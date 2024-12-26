#include "autocomplete.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#define sentinelValue INT_MIN
#define and &&
#define or ||

typedef struct Term{
    char term[200];
    double weight;
} Term;

//Sorts terms in lexicographic order
int compareTerms(const void* a, const void* b){
    return strcmp(((struct term*)a)->term, ((struct term*)b)->term);
}

//Sorts terms by weight
int compareTermsByWeight(const void* a, const void* b){
    double res = (-((struct term*)a)->weight) + ((struct term*)b)->weight;
    if(res < 0){
        return -1;
    }
    else if(res > 0){
        return 1;
    }
    else{
        return 0;
    }
}

void read_in_terms(struct term** terms, int* pnterms, char* filename){
    char line[200];
    FILE* fp =fopen(filename, "r");
    fgets(line, sizeof(line), fp); //Reads the number of terms
    *pnterms = atoi(line);
    *terms = (struct term*)malloc(sizeof(struct term) * (*pnterms)); //Allocates memory for terms
    for(int i = 0;i < *pnterms; i++){ //Reads terms and weights from the file
        fgets(line, sizeof(line), fp);
        int j = 0;
        strcpy((*terms + i)-> term, line); //Copies term and weight from the line
        while((isspace((*terms + i)-> term[j])) != 0){
            j++;
        }int weight_start = j;
        while((isspace((*terms + i)->term[j]))== 0){
            j++;
        }
        int weight_end = j-1;
        int term_start = j + 1;
        int term_end = strlen((*terms + i)-> term) - 2;
        char weight[200]; //Copies and converts weight to double
        strncpy(weight, (*terms + i)->term + weight_start, weight_end);
        (*terms + i)->weight = (double)atol(weight);
        char term[200];
        strncpy(term, (*terms + i)->term + term_start, term_end);
        strcpy((*terms + i)->term, term);
        int len = strlen((*terms + i)->term);
        if ((*terms + i)-> term[len - 1] == '\n'){
            (*terms + i)-> term[len - 1] = '\0';
        }
    }
    qsort(*terms, *pnterms, sizeof(struct term), compareTerms); //Sorts terms in non-descending lexicographic order
    fclose(fp);
}

int lowest_match(struct term* terms, int nterms, char* substr){
    int lower = 0;
    int upper = nterms;
    int len = strlen(substr);
    while(lower < upper){
        int mid = (lower + upper) / 2;
        if(strncmp(substr, (terms + mid)-> term, len) != 0 and strcmp((terms + mid)-> term, substr) < 0){
            lower = mid + 1;
        }
        else if(strncmp(substr, (terms + mid)-> term, len) != 0 and strcmp((terms + mid)-> term, substr) > 0){
            upper = mid;
        }
        else if(mid > 0 and strncmp(substr, (terms + mid)-> term, len) == 0){
            upper = mid;
        }
        else{
            return mid;
        }
    }
    if(lower == upper){
        return lower;
    }
    return sentinelValue; //If no match found, sentinel value returned
}

int highest_match(struct term* terms, int nterms, char* substr){
    int lower = 0;
    int upper = nterms;
    int cursor = sentinelValue;
    int len = strlen(substr);
    while(lower <= upper){
        int mid = (lower + upper) / 2;
        if(strncmp(substr, (terms + mid)->term, len) != 0 and strcmp(substr, (terms + mid)-> term)> 0){
            lower = mid + 1;
        }
        else if(strncmp(substr, (terms + mid)-> term, len) != 0 and strcmp(substr, (terms + mid)-> term) < 0){
            upper = mid - 1;
        }
        else if(strncmp(substr, (terms + mid)-> term, len) == 0){ //Updates cursor when match found
            if(mid > cursor or cursor < 0){
                cursor = mid;
            }
            lower = mid + 1;
        }
    }
    return cursor;
}

void autocomplete(struct term** answer, int* n_answer, struct term* terms, int nterms, char* substr){
    int higher_match = highest_match(terms, nterms, substr);
    int lower_match = lowest_match(terms, nterms, substr);
    if(higher_match < 0 or lower_match < 0){
        *n_answer = 0;
        return;
    }
    *n_answer = (higher_match - lower_match) + 1; //Number of matching terms
    *answer = (struct term*)malloc(sizeof(struct term) * (*n_answer)); //Allocates memory for the answer
    int count = 0;
    for(int i = lower_match; i < lower_match + *n_answer; i++){ //Copies matching term to answer
        strcpy((*answer + count)-> term, (terms + i)-> term);
        (*answer + count)-> weight = (terms + i)-> weight;
        count++;
    }
    qsort(*answer, *n_answer, sizeof(struct term), compareTermsByWeight); //Sorts the answer by weight in non-descending order
}