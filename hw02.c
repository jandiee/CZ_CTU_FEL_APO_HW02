#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 2000

int main(int argc, char** argv) {
    unsigned char *img_array;
    char *file_name;
    FILE *fp;
    int width;
    int height;
    int color_range;
    char *file_head = (char *) malloc(10 * sizeof(char));

    if(argc > 1) {
        file_name = argv[1];
    }
    printf("File name -- %s\n", argv[1]);

    if((fp = fopen(file_name, "rb")) == NULL) {
        fprintf(stderr, "Error opening file!\n");
        return 1;
    }

    if(fgets(file_head, 10, fp) == NULL) {
        fprintf(stderr, "Error reading first line of file!\n");
        return 1;
    }

    if(strcmp(file_head, "P6\n") != 0) {
        fprintf(stderr, "This file is not P6 file!\n");
        return 1;
    }
    
    printf("file_head -- %s\n", file_head);

    // don't need anything with the first line of file
    free(file_head);

    if(fscanf(fp, "%d %d %d", &width, &height, &color_range) != 3) {
        fprintf(stderr, "Error reading width and height!\n");
        return 1;
    }

    fgetc(fp);

    printf("Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);

    img_array = (unsigned char *) malloc(3 * width * height * sizeof(unsigned char));

    if(fread(img_array, 1, 3 * width * height, fp) != 3 * width * height) {
        fprintf(stderr, "Error reading the image and loading it into the array!\n");
        return 1;
    }

    // printing whole array into file to check if it's complete
    // FILE *txt = fopen("a.txt", "w");
    // for(int i = 0; i < (3 * width * height); i++) {
    //     fprintf(txt,"%c", img_array[i]);
    // }
    // fclose(txt);
    
    fclose(fp);

    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {

        }
    }

    return 0;
}