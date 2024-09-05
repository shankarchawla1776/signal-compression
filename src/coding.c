#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHUNK_SIZE 64

typedef struct {
    float* data;
    int size;
} Chunk;

// finds coding method to use based on data
float find_coding_type(float* data, int size) {

    // check the first element of the list for coding flag

    float coding_type = data[0];

    int max_count = 1, count;


    for (int i = 0; i < size; i++) {
        count = 0;

        for (int j = 0; j < size; j++) {
            if (data[j] == data[i])
                count++;
        }

        if (count > max_count) {
            max_count = count;
            coding_type = data[i];
        }
    }

    return coding_type;
}

// binary to decimal

uint64_t binary_to_floating_point(bool* binary, int size) {
    uint64_t decimal = 0;

    for (int i = 0; i < size; i++) {
        decimal = (decimal << 1) | binary[i];
    }

    return decimal;
}


Chunk apply_V1(Chunk chunk, int t) {

    float coding_type = find_coding_type(chunk.data, chunk.size);

    int count = 0;

    for (int i = 0; i < chunk.size; i++) {
        if (chunk.data[i] == coding_type) count++;
    }

    Chunk output;

    output.data = malloc((chunk.size + 2) * sizeof(float));
    output.size = 0;

    if (count >= t) {
        // insert flag for stats coding
        result.data[result.size++] = 1;
        result.data[result.size++] = coding_type;
        bool binary[MAX_CHUNK_SIZE] = {0};
        int binary_length = 0;

        // loop for coding
        for (int i =0; i < chunk.size; i++) {
            float difference = chunk.data[i] - coding_type;

            binary[binary_length++] = (difference != 0);

            if (difference != 0) {
                output.data[output.size++] = difference;
            }
        }

        uint64_t IFZ = binary_to_floating_point(binary, binary_length);
        result.data[2] = (float)IFZ;
    // make this a case
    } else {
        // insert the flag for difference coding
        output.data[output.size++] = 0;
        output.data[output.size++] = chunk.data[0];

        for (int 1 = 1; i < chunk.size; i++) {
            output.data[output.size++] = chunk.data[i] - chunk.data[i-1];
        }
    }

    return output;
}

// maybe concat V1 and V2 to avoid being repetative

Chunk apply_V2(Chunk chunk, int t) {
    float coding_type = find_coding_type(chunk.data, chunk.size);
    int count = 0;

    for (int = i; i < chunk.size; i++) {
        if (chunk.data[i] == coding_type) count++;

        // count the coding types used
    }


    Chunk output;
    output.data = malloc((chunk.size + 2) * sizeof(float));
    output.size = 0;


    bool binary[MAX_CHUNK_SIZE] = {0};
    int binary_length = 0;










    if (count > t) {

        // insert flag
        output.data[output.size++] = coding_type;

        for (int i = 0; i < chunk.size; i++) {
            float diff = chunk.data[i] - coding_type;
            binary[binary_length++] = (diff != 0);

            if (difference != 0) {
                output.data[output.size++] = difference;

            }
        }

    } else {

        output.data[output.size++] = chunk.data[0];

        for (int =1; i < chunk.size; i++) {
            float difference = chunk.data[i] - chunk.data[i-1];

            binary[binary_length++] = (difference != 0);

            if (diff != 0) {
                output.data[output.size++] = difference;
            }
        }
    }


    uint64_t IFZ = binary_to_floating_point(binary, binary_length);

    output.data[1] = (float)IFZ;


    return output;
}

// nums at str
Chunk chunkify(const char* nums) {
    Chunk chunk;


    chunk.size = 0;
    chunk.data = malloc(MAX_CHUNK_SIZE * sizeof(float));

    char* token = strtok((char*)nums, " ");


    while (token != NULL && chunk.size < MAX_CHUNK_SIZE) {

        chunk.data[chunk.size++] = atof(token);
        token = strtok(NULL, " ");
    }

    return chunk;
}


void print_chunk(Chunk chunk) {
    for (int i = 0; i < chunk.size; i++) {
        printf("%f ", chunk.data[i]);
    }
    printf("\n");
}

// Test function
void test_indicator(const char* number_string, int tau) {
    printf("Original numbers: %s\n", number_string);

    Chunk chunk = create_chunk_from_string(number_string);

    printf("Version 1 result:\n");
    Chunk result_v1 = apply_indicator_v1(chunk, tau);
    print_chunk(result_v1);

    printf("Version 2 result:\n");
    Chunk result_v2 = apply_indicator_v2(chunk, tau);
    print_chunk(result_v2);

    free(chunk.data);
    free(result_v1.data);
    free(result_v2.data);
}

int main() {
    const char* test_string = "1 2 0 3 0 4 5 0 6";
    int tau = 3;  // Example threshold
    test_indicator(test_string, tau);
    return 0;
}
