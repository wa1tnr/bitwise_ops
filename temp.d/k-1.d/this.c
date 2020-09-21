#include <stdio.h>

#define LIMIT 11

int j =  0;
int k = -1;

void report(void) {
    printf(" j: %d     ", j);
    printf(" k: %d\n"   , k);
}

void main(void) {
    printf("\n");

    for (k = 0; k < LIMIT; k++) {
        j = 1 << k;
        if (j > 255) j = 255;
        report();
    }
    printf("\n");
}
