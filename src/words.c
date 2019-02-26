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

WordsVector * wordsVectorNew(string name) {
    WordsVector * vector = (WordsVector*)malloc(sizeof(WordsVector));;
    vector->name = name;
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
    logDebug("words vector: %s, added: 0x%03x, size: %d", vector->name, word.raw, vector->size);
}

Word wordsVectorGet(const WordsVector * vector, uint index){
    return vector->data[index];
}

void wordsVectorSet(WordsVector * vector, uint index, Word word){
    vector->data[index] = word;
}