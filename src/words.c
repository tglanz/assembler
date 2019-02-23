#include "words.h"

/* internal utility, re-allocate memory if required */
void wordsVectorGrowAccordingly(WordsVector * vector){
    int idx;
    Word * data;

    if (vector != NULL){
        if (vector->size == vector->capacity){
            if (vector->capacity == 0){
                vector->capacity = WORDS_VECTOR_INITIAL_CAPACITY;
            } else {
                vector->capacity *= WORDS_VECTOR_GROW_FACTOR;
            }
            data = (Word*)calloc(vector->capacity, sizeof(Word));
            if (vector->data != NULL){
                for (idx = 0; idx < vector->size; ++idx){
                    data[idx] = vector->data[idx];
                }
                free(vector->data);
            }

            vector->data = data;
        }
    }
}

WordsVector * wordsVectorNew() {
    WordsVector * vector = (WordsVector*)malloc(sizeof(WordsVector));;
    vector->capacity = 0;
    vector->size = 0;
    vector->data = NULL;
    wordsVectorGrowAccordingly(vector);
    return vector;
}
void wordsVectorFree(WordsVector * vector) {
    if (vector != NULL){
        if (vector->data != NULL){
            free(vector->data);
        }

        free(vector);
    }
}
void wordsVectorAppend(WordsVector * vector, Word word) {
    vector->data[vector->size] = word;
    ++vector->size;
    wordsVectorGrowAccordingly(vector);
    logDebug("added word: 0x%03x", word.raw);
}