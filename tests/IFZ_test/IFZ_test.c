#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void apply_indicator(Chunk* chunk) {
    bool binary[MAX_CHUNK_SIZE] = {0};

    int binary_size = 0;

    int first_zero_idx = -1;

    for (int i = 0; i < chunk->size; i++) {
        binary[i] = chunk->data[i] != 0;
        binary_size++;

        if (chunk->data[i] == 0 && first_zero_idx == -1) {
            first_zero_idx = i;
        }
    }

    uint64_t decimal_conversion = convert_binary(binary, binary_size);


    // replace the first zero in the chunk with the decimal and ommit other zeros
    // g(C_i, D_i), g: Z^|C_i| * Z -> Z^|C_i|
    //

    if (first_zero_idx != -1) {
        // insert decimal
        chunk->data[first_zero_idx] = (float)decimal_conversion;

        int write_idx = first_zero_idx + 1;

        for (int i = first_zero_idx+1; i < chunk->size; i++) {
            if (chunk->data[i] != 0) {
                chunk->data[write_idx++] = chunk->data[i];
            }
        }
        chunk->size = write_idx;
    }



}

uint64_t convert_binary(bool* binary, int size) {
    uint64_t converted_decimal = 0;

    for (int i = 0; i < size; i++) {

        converted_decimal = (converted_decimal << 1) | binary[i];
    }
    return converted_decimal;

}


void free_chunk(Chunk* chunk) {
    if (chunk->data) {
        free(chunk->data);
    }
}




float bytes_to_floating_point(const unsigned char* bytes) {
    uint32_t value = (uint32_t)bytes[0] |
                     ((uint32_t)bytes[1] << 8) |
                     ((uint32_t)bytes[2] << 16) |
                     ((uint32_t)bytes[3] << 24);
    return *(float*)&value;
}

////////// THE FOLLOWING CODE IS FOR TESTING IFZ ON NUMBER STRINGS ///////////////

Chunk create_chunk_from_string(const char* number_string) {
    Chunk chunk;
    chunk.size = 0;
    chunk.data = malloc(MAX_CHUNK_SIZE * sizeof(float));

    char* token = strtok((char*)number_string, " ");
    while (token != NULL && chunk.size < MAX_CHUNK_SIZE) {
        chunk.data[chunk.size++] = atof(token);
        token = strtok(NULL, " ");
    }

    return chunk;
}

// Add this function to test the indicator function
void test_indicator(const char* number_string) {
    printf("Original numbers: %s\n", number_string);

    Chunk chunk = create_chunk_from_string(number_string);

    printf("Before applying indicator:\n");
    for (int i = 0; i < chunk.size; i++) {
        printf("%.2f ", chunk.data[i]);
    }
    printf("\n");

    apply_indicator(&chunk);

    printf("After applying indicator:\n");
    for (int i = 0; i < chunk.size; i++) {
        printf("%.2f ", chunk.data[i]);
    }
    printf("\n");

    free_chunk(&chunk);
}
