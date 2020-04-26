/* Niema Attarian - MD5 Hashing Algorithm */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Code has been adapted from https://en.wikipedia.org/wiki/MD5#Pseudocode  */
/* These will contain the hash  */
uint32_t hash0, hash1, hash2, hash3;

void md5(uint8_t *initial_msg, size_t initial_len){ /* This md5 function takes in parameters of any initial messgae of any length */
    /* Preparing the message */
    uint8_t *msg = NULL;
    
    /* Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating  */
    /* s specifies the per-round shift amounts  */
    uint32_t s[] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    /* Use binary integer part of the sines of integers (in radians) as constants */
    const uint32_t K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    /* Initialize variables:  */
    hash0 = 0x67452301;   // A
    hash1 = 0xefcdab89;   // B
    hash2 = 0x98badcfe;   // C
    hash3 = 0x10325476;   // D

    
    /*  Padding  */
    /*  The initial step of allocating "1" bit to the message
        followed by a number of "0" bits until the length is congruent to 488, modulo 512 in bits
    */

    /* Adapted from https://gist.github.com/creationix/4710780 */
    /* Appending the "1" bit */
    int new_len = ((((initial_len + 8) / 64) + 1) * 64) - 8;

    /* Appending the "0" bits (allocating 64 extra bytes) */
    msg = calloc(new_len + 64, 1);

    memcpy(msg, initial_msg, initial_len);
    /* Writing the "1" bit to the message */
    msg[initial_len] = 128;

    /* Appending the length */
    /* Adding the initial bit message input at the end of the buffer in the form of 64-bit representation */
    uint32_t bits_len = 8*initial_len;
    memcpy(msg + new_len, &bits_len, 4); /* Creates a memory block copy */

    /*
        x >> y - Shifting the x value to the right y bits
        x << y - Shifting the x value to the left y bits
    */
    /* This function rotates x values left n-number of bits */
    #define LEFT_ROTATE_FUNCTION(x, n) (((x) << (n)) | ((x) >> (32-(n))))

    /* Process Message in 16-word blocks */

    /* Initialising the MD Buffer */
    uint32_t A = hash0;
    uint32_t B = hash1;
    uint32_t C = hash2;
    uint32_t D = hash3;

    /* Main loop */
    int32_t i;
    for(i = 0; i<64; i++) {
        int32_t F, g;

        /* Defining the auxiliary functions that take input of three 32-bit words */
        /*
            & - bit-wise AND
            | - bit-wise OR
            ~ - bit-wise NOT
            ^ - bit-wise TO THE POWER OF
        */

        if(0 <= i <= 15){
            F = (B & C) | ((~B) & D);   /* (B and C) or ((not B) and D) */
            g = i;
        }
        else if(16 <= i <= 31){
            F = (D & B) | ((~D) & C);   /* (D and B) or ((not D) and C) */
            g = (5*i + 1) % 16;
        }
        else if(32 <= i <= 47){
            F = B ^ C ^ D;   /* B xor C xor D */
            g = (3*i + 5) % 16;
        }
        else if(48 <= i <= 63){
            F = C ^ (B | (~D));   /* C xor (B or (not D)) */
            g = (7*i) % 16;
        }
    }// end main loop
} // MD5


int main(int argc, char *argv[]) {

    FILE *infile;
    char filename[1000], c;

    /* Return error if single file name isn't given  */
    if (argc != 2){
        printf("Error: expected single filename as arguemnt.\n");
        return 1;
    }

    /* Reads in the file as a binary file  */
    /* Opening the File */
    infile = fopen(argv[1], "rb");
    
    /* Error if file cannot be opened  */
    if(!infile) {
        printf("Error: couldn't open file %s. \n", argv[1]);
        return 1;
    }

    /* Read contents of the file */
    c = fgetc(infile); 
    printf ("======   The contents of the file    ======\n"); 
    while (c != EOF) 
    { 
        /* Prints the contents of file to console */
        printf ("%c", c); 
        c = fgetc(infile); 
    } 
    printf ("\n"); 

    printf ("\n====== The hashed output of the file ======\n"); 

    /* close the file  */
    fclose(infile);
    return 0;
}
