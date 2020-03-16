// Niema Attarian
// Padding for SHA256

#include <stdio.h>
#include <inttypes.h>

int main(int argc, char * argv[]){

    if(argc != 2){
        printf("Error: expected single filename as argument.\n");
        return 1;
    }
    // Open file
    FILE * infile = fopen(argv[1], "rb");
    if (!infile){
        printf("Error: couldn't open file %s.\n", argv[1]);
        return 1;
    }

    uint8_t b;
    uint64_t nobits;

    for (nobits = 0;fread(&b, 1, 1, infile) == 1; nobits += 8){
        // Format specifier
        printf("%02" PRIx8 " ", b);
    }
    // Format specifier
    printf("%016" PRIx64 "\n", nobits);
    
    printf("\n");

    // Close file
    fclose(infile);

    return 0;
}