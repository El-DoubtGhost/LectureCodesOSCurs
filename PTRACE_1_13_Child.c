#include <stdio.h>

int main (int argc, char **argv) {
    printf("Traced process was born");
    return argc/0;
}