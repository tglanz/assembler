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
    int raw: WORD_SIZE;
} Word;

typedef struct {
    const char * name;
    unsigned int size;
    unsigned int capacity;
    Word * data;
} WordsVector;

WordsVector * wordsVectorNew(const char * name);
void wordsVectorFree(WordsVector * vector);
void wordsVectorAppend(WordsVector * vector, Word word);
Word wordsVectorGet(WordsVector * vector, int index);
void wordsVectorSet(WordsVector * vector, int index, Word word);

#endif /* __WORDS_VECTOR_H_ */