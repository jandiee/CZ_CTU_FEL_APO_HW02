#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 500
#define RED(x) x
#define GREEN(x) x + 1
#define BLUE(x) x + 2
#define PIXEL_WIDTH 3

int main(int argc, char **argv)
{
    unsigned char *img_array;
    char *file_name;
    FILE *fp;
    int width;
    int height;
    int color_range;
    char *file_head = (char *)malloc(10 * sizeof(char));
    int i = 0, k = 0, j = 0;

    if (argc > 1)
    {
        file_name = argv[1];
    }
    printf("File name -- %s\n", argv[1]);
    if (argc < 2)
    {
        fprintf(stderr, "Error: Name filename as first argument!\n");
        return 1;
    }

    if ((fp = fopen(file_name, "rb")) == NULL)
    {
        fprintf(stderr, "Error opening file!\n");
        return 1;
    }

    if (fgets(file_head, 10, fp) == NULL)
    {
        fprintf(stderr, "Error reading first line of file!\n");
        return 1;
    }

    if (strcmp(file_head, "P6\n") != 0)
    {
        fprintf(stderr, "This file is not P6 file!\n");
        return 1;
    }

    printf("file_head -- %s\n", file_head);

    // don't need anything with the first line of file
    free(file_head);

    if (fscanf(fp, "%d %d %d", &width, &height, &color_range) != 3)
    {
        fprintf(stderr, "Error reading width and height!\n");
        return 1;
    }

    fgetc(fp);

    printf("Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);

    img_array = (unsigned char *)malloc(PIXEL_WIDTH * width * height * sizeof(unsigned char));

    if (fread(img_array, 1, PIXEL_WIDTH * width * height, fp) != PIXEL_WIDTH * width * height)
    {
        fprintf(stderr, "Error reading the image and loading it into the array!\n");
        return 1;
    }

    // printing whole array into file to check if it's complete
    FILE *source_array = fopen("source_array.txt", "w");
    fprintf(source_array, "Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);
    for (int i = 0; i < (PIXEL_WIDTH * width * height); i++)
    {
        fprintf(source_array, "%d ", img_array[i]);
        if (i % (PIXEL_WIDTH * width) == 0 && i != 0)
        {
            fprintf(source_array, "\n");
        }
    }
    fclose(source_array);

    fclose(fp);

    // ignore the first and last row (i != 0 & i != height) and ignore first and last column (j != 0 & j != (PIXEL_WIDTH * width + PIXEL_WIDTH))
    // TODO: This for cycles doesn't fucking work !!! (k = 0 forever and ever)
    for (i = 1; i < (height - 1); i++)
    {
        printf("Line -- %d\n", i);
        for (j = 0; j < (width * PIXEL_WIDTH - PIXEL_WIDTH); (j + (BLOCK_SIZE * PIXEL_WIDTH)))
        {
            printf("J -- %d\n", j);
            for (k = j; k < (j + (BLOCK_SIZE * PIXEL_WIDTH)) && k < (width * PIXEL_WIDTH - PIXEL_WIDTH); (k + PIXEL_WIDTH))
            {
                printf("K -- %d\n", k);
                if (k == 0)
                {
                    continue;
                }

                img_array[i * width * PIXEL_WIDTH + RED(k)] = (-1 * img_array[i * width * PIXEL_WIDTH + RED(k) - PIXEL_WIDTH]) + (5 * img_array[i * width * PIXEL_WIDTH + RED(k)]) + ((-1) * img_array[i * width * PIXEL_WIDTH + RED(k) + PIXEL_WIDTH]) + (-1 * img_array[(i * width * PIXEL_WIDTH - 1) + RED(k)]) + (-1 * img_array[(i * width * PIXEL_WIDTH + 1) + RED(k)]);

                img_array[i * width * PIXEL_WIDTH + GREEN(k)] = (-1 * img_array[i * width * PIXEL_WIDTH + GREEN(k) - PIXEL_WIDTH]) + (5 * img_array[i * width * PIXEL_WIDTH + GREEN(k)]) + ((-1) * img_array[i * width * PIXEL_WIDTH + GREEN(k) + PIXEL_WIDTH]) + (-1 * img_array[(i * width * PIXEL_WIDTH - 1) + GREEN(k)]) + (-1 * img_array[(i * width * PIXEL_WIDTH + 1) + GREEN(k)]);

                img_array[i * width * PIXEL_WIDTH + BLUE(k)] = (-1 * img_array[i * width * PIXEL_WIDTH + BLUE(k) - PIXEL_WIDTH]) + (5 * img_array[i * width * PIXEL_WIDTH + BLUE(k)]) + ((-1) * img_array[i * width * PIXEL_WIDTH + BLUE(k) + PIXEL_WIDTH]) + (-1 * img_array[(i * width * PIXEL_WIDTH - 1) + BLUE(k)]) + (-1 * img_array[(i * width * PIXEL_WIDTH + 1) + BLUE(k)]);
            }
        }
    }

    // printing array after convolution into file
    FILE *final_array = fopen("final_arrays.txt", "w");
    fprintf(final_array, "Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);
    for (int i = 0; i < (PIXEL_WIDTH * width * height); i++)
    {
        fprintf(final_array, "%d ", img_array[i]);
        if (i % (PIXEL_WIDTH * width) == 0 && i != 0)
        {
            fprintf(final_array, "\n");
        }
    }
    fclose(final_array);

    return 0;
}
