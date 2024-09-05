#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_CHUNK_SIZE 64

typedef struct {
    float* data;
    int size;
} Chunk;

float find_mode(Chunk chunk) {
    float mode = chunk.data[0];
    int max_count = 1, current_count;

    for (int i = 0; i < chunk.size; i++) {
        current_count = 0;
        for (int j = 0; j < chunk.size; j++) {
            if (chunk.data[j] == chunk.data[i])
                current_count++;
        }
        if (current_count > max_count) {
            max_count = current_count;
            mode = chunk.data[i];
        }
    }

    return mode;
}

uint64_t binary_to_decimal(bool* binary, int size) {
    uint64_t decimal = 0;
    for (int i = 0; i < size; i++) {
        decimal = (decimal << 1) | binary[i];
    }
    return decimal;
}

Chunk apply_V1(Chunk chunk, int tau) {
    float mode = find_mode(chunk);
    int mode_count = 0;
    for (int i = 0; i < chunk.size; i++) {
        if (chunk.data[i] == mode) mode_count++;
    }

    Chunk result;
    result.data = malloc((chunk.size + 3) * sizeof(float)); // +3 for flag, mode/first value, and IFZ
    result.size = 0;

    if (mode_count >= tau) {
        result.data[result.size++] = 1; // Flag for statistical coding
        result.data[result.size++] = mode;

        bool binary[MAX_CHUNK_SIZE] = {0};
        int binary_size = 0;

        for (int i = 0; i < chunk.size; i++) {
            float diff = chunk.data[i] - mode;
            binary[binary_size++] = (diff != 0);
            if (diff != 0) {
                result.data[result.size++] = diff;
            }
        }

        uint64_t ifz = binary_to_decimal(binary, binary_size);
        result.data[2] = (float)ifz;
    } else {
        // Difference coding
        result.data[result.size++] = 0; // Flag for difference coding
        result.data[result.size++] = chunk.data[0];

        for (int i = 1; i < chunk.size; i++) {
            result.data[result.size++] = chunk.data[i] - chunk.data[i-1];
        }
    }

    return result;
}

Chunk apply_V2(Chunk chunk, int tau) {
    float mode = find_mode(chunk);
    int mode_count = 0;
    for (int i = 0; i < chunk.size; i++) {
        if (chunk.data[i] == mode) mode_count++;
    }

    Chunk result;
    result.data = malloc((chunk.size + 2) * sizeof(float)); // +2 for mode/first value and IFZ
    result.size = 0;

    bool binary[MAX_CHUNK_SIZE] = {0};
    int binary_size = 0;

    if (mode_count >= tau) {
        // Statistical coding
        result.data[result.size++] = mode;

        for (int i = 0; i < chunk.size; i++) {
            float diff = chunk.data[i] - mode;
            binary[binary_size++] = (diff != 0);
            if (diff != 0) {
                result.data[result.size++] = diff;
            }
        }
    } else {
        result.data[result.size++] = chunk.data[0];

        for (int i = 1; i < chunk.size; i++) {
            float diff = chunk.data[i] - chunk.data[i-1];
            binary[binary_size++] = (diff != 0);
            if (diff != 0) {
                result.data[result.size++] = diff;
            }
        }
    }

    uint64_t ifz = binary_to_decimal(binary, binary_size);
    result.data[1] = (float)ifz;

    return result;
}

Chunk create_chunk_from_string(const char* number_string) {
    Chunk chunk;
    chunk.size = 0;
    chunk.data = malloc(MAX_CHUNK_SIZE * sizeof(float));

    char* input_copy = strdup(number_string);
    char* token = strtok(input_copy, " ");
    while (token != NULL && chunk.size < MAX_CHUNK_SIZE) {
        chunk.data[chunk.size++] = atof(token);
        token = strtok(NULL, " ");
    }
    free(input_copy);

    return chunk;
}

void print_chunk(Chunk chunk) {
    for (int i = 0; i < chunk.size; i++) {
        printf("%f ", chunk.data[i]);
    }
    printf("\n");
}

void test_coding(const char* number_string, int tau) {
    printf("Original numbers: %s\n", number_string);

    Chunk chunk = create_chunk_from_string(number_string);

    printf("Original chunk: ");
    print_chunk(chunk);

    printf("Version 1 result:\n");
    Chunk result_v1 = apply_V1(chunk, tau);
    print_chunk(result_v1);

    printf("Version 2 result:\n");
    Chunk result_v2 = apply_V2(chunk, tau);
    print_chunk(result_v2);

    free(chunk.data);
    free(result_v1.data);
    free(result_v2.data);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_string> <tau>\n", argv[0]);
        return 1;
    }

    const char* number_string = argv[1];
    int tau = atoi(argv[2]);

    test_coding(number_string, tau);

    return 0;
}
