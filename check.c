#include <string.h>
#include <stdio.h>

int main(int argc, char ** argv){
    char b1[255];
    char b2[255];
    int count;

    count = sscanf("a: .b", "%s\: .%s", b1, b2);

    printf("done: %d", count);
    return 1;
}