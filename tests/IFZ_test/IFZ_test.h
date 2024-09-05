#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_CHUNK_SIZE 64
#define MAX_CHANNELS 64

typedef struct {
    float* data;
    int size;
} Chunk;

Chunk read_fdt_and_set(const char* fdt_filepath, const char* set_filepath);

void write_data(const char* output_filepath, Chunk* chunk);
void apply_indicator(Chunk* chunk);

uint64_t convert_binary(bool* binary, int size);
void free_chunk(Chunk* chunk);

char* parse_set_metadata(const char* filepath);
Chunk parse_fdt_data(const char* fdt_file, int channels, float rate);
float bytes_to_floating_point(const unsigned char* bytes);




// FUNCTION PROTOTYPES FOR TESTING //
Chunk create_chunk_from_string(const char* number_string);
void test_indicator(const char* number_string);
/////////////////////////////////////




#endif
