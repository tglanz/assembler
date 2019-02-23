#include <string.h>
#include <stdio.h>

char * getSplitComponent(char * string, const char * delimiter, int index){
    char * token;

    if (strstr(string, delimiter) == NULL){
        return NULL;
    }

    token = strtok(string, delimiter);
    while (index > 0 && token != NULL){
        token = strtok(NULL, delimiter);
        --index;
    }
    return token;
}

int main(int argc, char ** argv){

    int count;
    char line1[] = "some: line";
    char line2[] = "line";
    char buffer[255];

    count = sscanf(line1, "%s: ", buffer);
    count = sscanf(line2, "%s: ", buffer);


    return 1;
}