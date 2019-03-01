#include "assembler/outputs.h"

void writeWordsVectorToObjectFile(FILE * file, const WordsVector * vector){
    uint idx;
    Word word;
    char line[2];

    /* for each word, encode to base 64 and write to file */
    for (idx = 0; idx < vector->size; ++idx){
        word = wordsVectorGet(vector, idx);
        wordToBase64(line, word);
        fprintf(file, "%c%c\n", line[0], line[1]);
    }
}

void generateEntriesFile(string baseName, const AssemblyState * state) {
    uint idx;
    char path[MAX_FILE_PATH_LENGTH];
    Symbol * symbol;
    FILE * file = NULL;

    joinWithDelimiter(path, baseName, "ent", '.');

    /* iterate through symbols, if it's an entry, write it to file */
    for (idx = 0; idx < state->symbols->size; ++idx){
        symbol = symbolsTableGet(state->symbols, idx);
        if (has_flag(SYMBOL_FLAG_ENTRY, symbol->flags)){
            if (file == NULL){
                file = fopen(path, "w");
            }

            fprintf(file, "%s\t%d\n", symbol->key, symbol->value);
        }
    }

    if (file != NULL){
        fclose(file);
    }
}

void generateExternalsFile(string baseName, const AssemblyState * state) {
    uint idx;
    char path[MAX_FILE_PATH_LENGTH];
    Symbol * symbol;
    FILE * file = NULL;

    if (state->externalSymbols->size == 0){
        return;
    }

    joinWithDelimiter(path, baseName, "ext", '.');
    file = fopen(path, "w");

    /* iterate through external symbols, write them to file */
    for (idx = 0; idx < state->externalSymbols->size; ++idx){
        symbol = symbolsTableGet(state->externalSymbols, idx);
        fprintf(file, "%s\t%d\n", symbol->key, symbol->value);
    }

    if (file != NULL){
        fclose(file);
    }
}

void generateObjectFile(string baseName, const AssemblyState * state) {
    char path[MAX_FILE_PATH_LENGTH];
    FILE * file;
    
    joinWithDelimiter(path, baseName, "ob", '.');
    file = fopen(path, "w");

    /* write the header */
    fprintf(file, "%d %d\n", state->code->size, state->data->size);
    
    /* write the code section */
    writeWordsVectorToObjectFile(file, state->code);

    /* write the data section */
    writeWordsVectorToObjectFile(file, state->data);

    fclose(file);
}