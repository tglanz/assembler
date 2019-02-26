#ifndef __WORDS_VECTOR_H_
#define __WORDS_VECTOR_H_

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "core.h"
#include "log.h"

#define WORD_SIZE (12)
#define WORDS_VECTOR_INITIAL_CAPACITY (4)
#define WORDS_VECTOR_GROW_FACTOR (2)

typedef struct {
    uint raw: WORD_SIZE;
} Word;

typedef struct {
    string name;
    uint size;
    uint capacity;
    Word * data;
} WordsVector;

void wordToBase64(char * destination, Word word);

WordsVector * wordsVectorNew(string name);
void wordsVectorFree(WordsVector * vector);
void wordsVectorAppend(WordsVector * vector, Word word);
Word wordsVectorGet(const WordsVector * vector, uint index);
void wordsVectorSet(WordsVector * vector, uint index, Word word);

#endif /* __WORDS_VECTOR_H_ */