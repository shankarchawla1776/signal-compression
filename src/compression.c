#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Chunk read_fdt_and_set(const char* fdt_filepath, const char* set_filepath) {

    char* set_data = parse_set_metadata(set_filepath);

    if (!set_data) {
        fprintf(stderr, "No corresponding .set metadata found\n");

        exit(1);
    }

    // placeholders. will be found by implement.sh
    int channels = 64; // int
    float rate = 250.0; // float


    Chunk chunk = parse_fdt_data(fdt_filepath, channels, rate);

    free(set_data);

    return chunk;
}


void write_compressed_data(const char* output_filepath, Chunk* chunk) {
    FILE* fp = fopen(output_filepath, "w");

    if (!fp) {
        fprintf(stderr, "Error, no output file\n");
        return;
    }

    for (int i = 0; i < chunk->size; i++) {
        fprintf(fp, "%f ", chunk->data[i]);
    }

    fclose(fp);
}

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


char * parse_set_metadata(const char* filepath) {
    FILE* set_file = fopen(filepath, "rb");

    if (!set_file) {
        perror("Could not open metadata");
        return NULL;
    }

    fseek(set_file, 0, SEEK_END);
    long metadata_length = ftell(set_file);

    fseek(set_file, 0, SEEK_SET);

    char* buffer = malloc(metadata_length);

    if (!buffer) {
        fclose(set_file);
        return NULL;
    }

    fclose(set_file);
    return buffer;




}


Chunk parse_fdt_data(const char* fdt_file, int channels, float rate) {
    FILE* fdt_filepath = fopen(fdt_file, "rb");

    if (!fdt_filepath) {
        fprintf(stderr, "No data file found (.fdt)");
        exit(1);

    }


    fseek(fdt_filepath, 0, SEEK_END);
    long data_length = ftell(fdt_filepath);
    // rm if set metadata is not needed.
    // TODO: if set metadata includes rate and channels, use that rather than the python script
    fseek(fdt_filepath, 0, SEEK_SET);

    int samples = data_length / (4 * channels);

    Chunk chunk = {malloc(samples * channels * sizeof(float)), samples * channels};

    if (!chunk.data) {
        fclose(fdt_filepath);
        exit(1);
    }

    unsigned char buffer[4 * MAX_CHANNELS];

    int signal_idx = 0;

    for (int i = 0; i < samples; i++) {
        size_t bytes = fread(buffer, 1, 4 * channels, fdt_filepath);

        if (bytes != 4 * channels) {
            break;
        }

        for (int signal = 0; signal < channels; signal++) {
            chunk.data[signal++] = bytes_to_floating_point(&buffer[signal * 4]);
        }
    }

    fclose(fdt_filepath);
    return chunk;


}


float bytes_to_floating_point(const unsigned char* bytes) {
    uint32_t value = (uint32_t)bytes[0] |
                     ((uint32_t)bytes[1] << 8) |
                     ((uint32_t)bytes[2] << 16) |
                     ((uint32_t)bytes[3] << 24);
    return *(float*)&value;
}
