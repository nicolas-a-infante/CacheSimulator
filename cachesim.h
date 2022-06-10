/*
 * CS3375 Computer Architecture
 * Course Project
 * Cache Simulator Design and Development
 * FALL 2017
 * By Yong Chen
 */

#define BLOCK_SIZE  64        /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */
#define WAY_SIZE    2         /* Associativity; 1-way = direct-mapped */
#define CACHE_SIZE  65536     /* Cache capacity in bytes (must be power of 2)*/

#define NUM_BLOCKS  (CACHE_SIZE / BLOCK_SIZE)
#define NUM_SETS    (NUM_BLOCKS / WAY_SIZE)

#define DBG
#define PART 3.2

/*The data structure of direct-mapped cache*/
struct direct_mapped_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    double hit_rate;                   /* Hit rate */
    double miss_rate;                  /* Miss rate */
};

/*The data structure of a fully-associative cache*/
struct fully_associative_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int block_address[NUM_BLOCKS];
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    double hit_rate;                   /* Hit rate */
    double miss_rate;                  /* Miss rate */
};

/*The data structure of a n-way-associative cache*/
struct n_way_associative_cache {
    unsigned valid_field[NUM_SETS][WAY_SIZE];   /* Valid field */
    unsigned dirty_field[NUM_SETS][WAY_SIZE];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_SETS][WAY_SIZE];     /* Tag field */
    char data_field[NUM_SETS][WAY_SIZE][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int set_address[NUM_SETS][WAY_SIZE];
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
    double hit_rate;                   /* Hit rate */
    double miss_rate;                  /* Miss rate */
};


/*Read the memory traces and convert it to binary*/
uint64_t convert_address(char memory[]);

/*Simulate all the caches*/
void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address);
void fully_associative_cache_access(struct fully_associative_cache *cache, uint64_t address);
void n_way_associative_cache_access(struct n_way_associative_cache *cache, uint64_t address);