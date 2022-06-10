/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "cachesim.h"

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <direct, fully, or n-way> <trace file name>\n", argv[0]);
        return 1;
    }

#ifdef DBG
    printf("PART %1.1f\n", PART);
    printf("\n***********************************************************\n\n");
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("\n");
#endif

    srand((unsigned int)time(NULL));
    char* trace_file_name = argv[2];
    struct direct_mapped_cache d_cache;
    struct fully_associative_cache a_cache;
    struct n_way_associative_cache n_cache;
    char mem_request[20];
    uint64_t address;
    FILE *fp;


    /* Opening the memory trace file */
    fp = fopen(trace_file_name, "r");

    if (strncmp(argv[1], "direct", 6)==0) { /* Simulating direct-mapped cache */

        /* Initialization */
        for (int i=0; i<NUM_BLOCKS; i++) {
            d_cache.valid_field[i] = 0;
            d_cache.dirty_field[i] = 0;
            d_cache.tag_field[i] = 0;
        }
        d_cache.hits = 0;
        d_cache.misses = 0;

        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            direct_mapped_cache_access(&d_cache, address);
        }
        d_cache.hit_rate = (float)d_cache.hits / (float)(d_cache.hits + d_cache.misses);
        d_cache.miss_rate = (float)d_cache.misses / (float)(d_cache.hits + d_cache.misses);

        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    Direct-Mapped Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", d_cache.hits);
        printf("Cache Misses:  %d\n", d_cache.misses);
        printf("Cache Hit Rate: %0.2f\n", d_cache.hit_rate);
        printf("Cache Miss Rate: %0.2f\n", d_cache.miss_rate);
        printf("\n\n\n\n");
    }

    else if (strncmp(argv[1], "fully", 5)==0) { /* Simulating fully-associative-mapped cache */

        /* Initialization */
        for (int i=0; i<NUM_BLOCKS; i++) {
            a_cache.valid_field[i] = 0;
            a_cache.dirty_field[i] = 0;
            a_cache.tag_field[i] = 0;
            a_cache.block_address[i] = 0;
        }
        a_cache.hits = 0;
        a_cache.misses = 0;

        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            fully_associative_cache_access(&a_cache, address);
        }
        a_cache.hit_rate = (float)a_cache.hits / (float)(a_cache.hits + a_cache.misses);
        a_cache.miss_rate = (float)a_cache.misses / (float)(a_cache.hits + a_cache.misses);

        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    Fully-Associative-Mapped Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", a_cache.hits);
        printf("Cache Misses:  %d\n", a_cache.misses);
        printf("Cache Hit Rate: %0.2f\n", a_cache.hit_rate);
        printf("Cache Miss Rate: %0.2f\n", a_cache.miss_rate);
        printf("\n\n\n\n");
    }

    else if (strncmp(argv[1], "n-way", 5)==0) { /* Simulating n-way-associative-mapped cache */

        /* Initialization */
        for (int i = 0; i < NUM_SETS; i++) {
            for (int j = 0; j < WAY_SIZE; j++) {
                n_cache.valid_field[i][j] = 0;
                n_cache.dirty_field[i][j] = 0;
                n_cache.tag_field[i][j] = 0;
                n_cache.set_address[i][j] = 0;
            }
        }
        n_cache.hits = 0;
        n_cache.misses = 0;

        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            n_way_associative_cache_access(&n_cache, address);
        }
        n_cache.hit_rate = (float)n_cache.hits / (float)(n_cache.hits + n_cache.misses);
        n_cache.miss_rate = (float)n_cache.misses / (float)(n_cache.hits + n_cache.misses);

        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    N-Way-Associative-Mapped Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", n_cache.hits);
        printf("Cache Misses:  %d\n", n_cache.misses);
        printf("Cache Hit Rate: %0.2f\n", n_cache.hit_rate);
        printf("Cache Miss Rate: %0.2f\n", n_cache.miss_rate);
        printf("\n\n\n\n");
    }

    fclose(fp);

    return 0;
}

uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}

void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

    if (cache->valid_field[index] && cache->tag_field[index] == tag) { /* Cache hit */
        cache->hits += 1;
#ifdef DBG
        printf("Hit!\n");
#endif
    } else {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }
        cache->tag_field[index] = tag;
        cache->valid_field[index] = 1;
        cache->dirty_field[index] = 0;
    }
}

void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);
    int flag = -1; /* -1 if cache miss otherwise flag = index */
    int r = rand() % NUM_BLOCKS; /* random replacement policy */
    int empty = -1; /* stores the cache index of an empty block */

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

    for(int num = 0; num < NUM_BLOCKS; num++) {
        if (cache->tag_field[num] == tag && cache->block_address[num] == index && cache->valid_field[num]) {
            /* Cache hit */
            cache->hits += 1;
            flag = num;
#ifdef DBG
            printf("Hit!\n");
#endif
            break;
        }
        else if (!cache->valid_field[num]) {
            empty = num;
        }
    }
    if(flag == -1) {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[flag] && cache->dirty_field[flag]) {
            /* Write the cache block back to memory */
        }

        if(empty == -1) {
            /* if the cache is full replace a random block*/
            cache->tag_field[r] = tag;
            cache->valid_field[r] = 1;
            cache->dirty_field[r] = 0;
            cache->block_address[r] = index;
        }
        else {
            /* if the cache is not full write into an empty block */
            cache->tag_field[empty] = tag;
            cache->valid_field[empty] = 1;
            cache->dirty_field[empty] = 0;
            cache->block_address[empty] = index;
        }
    }
}

void n_way_associative_cache_access(struct n_way_associative_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t set = index % NUM_SETS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);
    uint64_t flag = -1; /* -1 if cache miss otherwise flag = way # */
    int r = rand() % WAY_SIZE; /* random replacement policy */
    int64_t empty = -1; /* stores the cache index of an empty block */

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Set: %llu, Tag: %llu ", address, block_addr, index, set, tag);
#endif
    for(int i = 0; i < WAY_SIZE; i++) {
        if (cache->tag_field[set][i] == tag && cache->set_address[set][i] == index && cache->valid_field[set][i]) {
            /* Search every way within the appropriate set for a hit */

            /* Cache hit */
            cache->hits += 1;
            flag = i;
#ifdef DBG
            printf("Hit!\n");
#endif
            break;
        }
        if (!cache->valid_field[set][i]) {
            empty = i;
        }
    }
    if(flag == -1) {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[set][flag] && cache->dirty_field[set][flag]) {
            /* Write the cache block back to memory */
        }

        if(empty == -1) {
            /* if the cache is full replace a random block*/
            cache->tag_field[set][r] = tag;
            cache->valid_field[set][r] = 1;
            cache->dirty_field[set][r] = 0;
            cache->set_address[set][r] = index;
        }
        else {
            /* if the cache is not full write into an empty block */
            cache->tag_field[set][empty] = tag;
            cache->valid_field[set][empty] = 1;
            cache->dirty_field[set][empty] = 0;
            cache->set_address[set][empty] = index;
        }
    }
}