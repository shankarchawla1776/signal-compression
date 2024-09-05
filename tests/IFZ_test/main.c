#include <stdio.h>
#include <stdlib.h>
#include "IFZ_test.h"


// test
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_string>\n", argv[0]);
        return 1;
    }

    const char* number_string = argv[1];
    test_indicator(number_string);

    return 0;
}
