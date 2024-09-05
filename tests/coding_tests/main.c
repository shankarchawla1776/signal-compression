#include <stdio.h>
#include <stdlib.h>
#include "coding_test.h"

// int main(int argc, char* argv[]) {
//     if (argc != 4) {
//         fprintf("User command syntax <fdt_filepath> <set_filepath> <output_filepath> \n", argv[0]);
//         return 1;
//     }
//
//     const char* fdt_filepath = argv[1];
//     const char* set_filepath = argv[2];
//     const char* output_filepath = argv[3];
//
//     Chunk chunk read_fdt_and_set(fdt_filepath, set_filepath);
//
//     apply_indicator(&chunk);
//
//     write_data(output_filepath, &chunk);
//
//     free_chunk(&chunk);
//
//     printf("comlete");
//
//     return 0;
// }
//

// Test
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_string>\n", argv[0]);
        return 1;
    }

    const char* number_string = argv[1];
    test_indicator(number_string);

    return 0;
}
