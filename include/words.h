/**
 * Words related definitions according to our virtual hardware spec
 * 
 * In this context - 
 *      - capacity will mean allocated space count (1 unit for a single value)
 *      - size will mean the used up space count (1 unit for a single value)
 */
#ifndef __WORDS_H_
#define __WORDS_H_

/* standard */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* services */
#include "core.h"
#include "log.h"

/* size of a word */
#define WORD_SIZE (12)

/* the capacity of newly created words vectors */
#define WORDS_VECTOR_INITIAL_CAPACITY (4)

/* the growing factor when re-allocating space for words vectors */
#define WORDS_VECTOR_GROW_FACTOR (2)

/**
 * A Word
 */
typedef struct {
    uint raw: WORD_SIZE;
} Word;

/**
 * A dynamically growing vector of words
 */
typedef struct {
    /**
     * a name for the container, can be used for debugging, reporting etc..
     * no real functionality as of now
     */
    string name;

    /* the size of the container */
    uint size;

    /* the capacity of the container */
    uint capacity;

    /* the words held within this container */
    Word * data;
} WordsVector;

/**
 * Creates a new words vectors
 * 
 * Arguments
 *      name: string - the name to assign to the container
 * 
 * Returns
 *      WordsVector* - the newly created words vector
 */
WordsVector * wordsVectorNew(string name);

/**
 * Free the given container and all of it's resources
 * 
 * Arguments
 *      vector: WordsVector* - the container to free
 */
void wordsVectorFree(WordsVector * vector);

/**
 * Appends a word to the end of the vector
 * 
 * Arguments
 *      vector: WordsVector* - the container to append a word to
 *      word: Word - the word to append
 */
void wordsVectorAppend(WordsVector * vector, Word word);

/**
 * Get a word by it's index in the vector
 * 
 * Arguments
 *      vector: WordsVector* - the container to get the words from
 *      index: uint - the index of the word to get
 * 
 * Returns
 *      Word - the word gotten from the vector
 *          No checks for out of bounds
 */
Word wordsVectorGet(const WordsVector * vector, uint index);

/**
 * Set a word by it's index in the vector
 * 
 * Arguments
 *      vector: WordsVector* - the container to set the word in
 *      index: uint - the index of the word to set
 *      word: Word - the word to set
 */
void wordsVectorSet(WordsVector * vector, uint index, Word word);

#endif /* __WORDS_H_ */