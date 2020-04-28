/* 
Niema Attarian - MD5 Hashing Algorithm 
*
*
Compile and run the code using the following:
    - make main
    - ./main *insert text*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
    x >> y - Shifting the x value to the right y bits
    x << y - Shifting the x value to the left y bits

    This function rotates x values left n-number of bits 
*/
#define LEFT_ROTATE_FUNCTION(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/*  
    Code has been adapted from https://en.wikipedia.org/wiki/MD5#Pseudocode  
    These will contain the hash  
*/
uint32_t hash0, hash1, hash2, hash3;

void md5(uint8_t *initial_message){ /* This md5 function takes in parameters of any initial messgae of any length */
    
    size_t initial_length = strlen(initial_message);    /* takes in the message as an argument and returns its length */
    
    /* Preparing the message */
    uint8_t *message = NULL;
    /*  
        Note: All variables are unsigned 32 bit and wrap modulo 2^32 when calculating 
        s specifies the per-round shift amounts  
    */
    uint32_t s[] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };

    /* Use binary integer part of the sines of integers (in radians) as constants */
    const uint32_t K[] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    /* Initialize variables:  */
    hash0 = 0x67452301;   // A
    hash1 = 0xefcdab89;   // B
    hash2 = 0x98badcfe;   // C
    hash3 = 0x10325476;   // D

    /*  
        Padding  
        The initial step of allocating "1" bit to the message
        followed by a number of "0" bits until the length is congruent to 488, modulo 512 in bits
    */

    /*  
        Adapted from https://gist.github.com/creationix/4710780
        Appending the "1" bit 
    */
    int new_length = ((((initial_length + 8) / 64) + 1) * 64) - 8;

    /* Appending the "0" bits (allocating 64 extra bytes) */
    message = calloc(new_length + 64, 1);   /* calloc() allocates the memory and also initializes the allocated memory block to zero. */

    memcpy(message, initial_message, initial_length);

    /* Writing the "1" bit to the message */
    message[initial_length] = 128;

    /* 
        Appending the length
        Adding the initial bit message input at the end of the buffer in the form of 64-bit representation 
    */
    uint32_t bits_length = 8 * initial_length;
    memcpy(message + new_length, &bits_length, 4); /* Creates a memory block copy */

    /* Process Message in 16-word blocks */
    for(int chunk = 0; chunk < new_length; chunk += 64)
    {
        /* Break chunk into sixteen 32-bit words M[j], 0 ≤ j ≤ 15 */
        uint32_t *M = (uint32_t *) (message + chunk);
        // char M[64];
        // M[64] = '\0';
        // strncpy(M, &new_length[chunk*64], 64);
        // printf("Chunk(no: %d) %s\n", chunk, M);

        /* Initialising the MD Buffer */
        uint32_t A = hash0;
        uint32_t B = hash1;
        uint32_t C = hash2;
        uint32_t D = hash3;

        /* Main loop */
        int32_t i;
        for(i = 0; i < 64; i++) {
            
            int32_t F, g;

            /* Defining the auxiliary functions that take input of three 32-bit words
                & - bit-wise AND
                | - bit-wise OR
                ~ - bit-wise NOT
                ^ - bit-wise NOT EQUALS TO
            */

            if(i < 16){
                F = (B & C) | ((~B) & D);   /* (B and C) or ((not B) and D) */
                g = i;
            }
            else if(i < 32){
                F = (D & B) | ((~D) & C);   /* (D and B) or ((not D) and C) */
                g = ((5 * i) + 1) % 16;
            }
            else if(i < 48){
                F = B ^ C ^ D;              /* B xor C xor D */
                g = ((3 * i) + 5) % 16;
            }
            else {
                F = C ^ (B | (~D));         /* C xor (B or (not D)) */
                g = (7 * i) % 16;
            }

            /* M[g] must be a 32-bits block */
            uint32_t temp = D;
            //F = F + A + K[i] + M[g];
            //A = D;
            D = C;
            C = B;
            B = B + LEFT_ROTATE_FUNCTION(F + A + K[i] + M[g], s[i]);
            A = temp;

        }// end main loop

        /* Add this chunk's hash to results so far */
        hash0 = hash0 + A;
        hash1 = hash1 + B;
        hash2 = hash2 + C;
        hash3 = hash3 + D;

    } // end for

} // MD5

int main(int argc, char *argv[]) {

    /* var char digest[16] := hash0, append hash1, append hash2, append hash3 (Output is in little-endian) */
    char *message = argv[1];
 
    /* Calling the MD5 function and parameters */
    md5(message);

    // Displaying input
    printf ("======  The Input text   ======\n"); 
    printf("%d\n", strlen(message));

    uint8_t *append;

    /* Display hashed output */

    printf ("\n====== The hashed output ======\n"); 
    append=(uint8_t *)&hash0;
    printf("%2.2x%2.2x%2.2x%2.2x", append[0], append[1], append[2], append[3], hash0);
    append=(uint8_t *)&hash1;
    printf("%2.2x%2.2x%2.2x%2.2x", append[0], append[1], append[2], append[3], hash1);
    append=(uint8_t *)&hash2;
    printf("%2.2x%2.2x%2.2x%2.2x", append[0], append[1], append[2], append[3], hash2);
    append=(uint8_t *)&hash3;
    printf("%2.2x%2.2x%2.2x%2.2x", append[0], append[1], append[2], append[3], hash3);
    
    printf("\n");

    // FILE *infile;
    // char c;
    
    // /* Return error if single file name isn't given  */
    // if (argc != 2){
    //     printf("Error: expected single filename as arguemnt.\n");
    //     return 1;
    // }

    // /* Reads in the file as a binary file  */
    // /* Opening the File */
    // infile = fopen(argv[1], "rb");
    
    // /* Error if file cannot be opened  */
    // if(!infile) {
    //     printf("Error: couldn't open file %s. \n", argv[1]);
    //     return 1;
    // }

    // /* Read contents of the file */
    // c = fgetc(infile); 
    // printf ("======   The contents of the file    ======\n"); 
    // while (c != EOF) 
    // { 
    //     /* Prints the contents of file to console */
    //     printf ("%c", c); 
    //     c = fgetc(infile); 
    // } 
    // printf ("\n"); 

    /* close the file  */
    // fclose(infile);

    return 0;
}
