/*
    Command Line Interface for Ender Compute
*/


#include <stdio.h>

void print_help() {
    printf("Usage:\n");
    printf("  enderc    Start scheduler and webserver\n");
    printf("  (there are no flags)\n");
}

int main(int argc, const char** argv) {

    int argi = 1;
    while (argi < argc) {
        const char* arg = argv[argi];
        argi++;

        if (!strcmp(arg, "-h")) {
            print_help();
            return 0;
        } else {
            fprintf(stderr, "No arguments allowed, '%s'\n", arg);
            return 1;
        }
    }

    

    return 0;
}

