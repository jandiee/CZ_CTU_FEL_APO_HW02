#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 22
#define RED(x) x
#define GREEN(x) x + 1
#define BLUE(x) x + 2
#define PIXEL_WIDTH 3

int main(int argc, char **argv)
{
    unsigned char *img_array;
    char *file_name;
    FILE *fp;
    FILE *final_picture;
    int width;
    int height;
    int color_range;
    char *file_head = (char *)malloc(10 * sizeof(char));

    if (argc > 1)
    {
        file_name = argv[1];
    }
    printf("File name -- %s\n", argv[1]);
    if (argc < 2)
    {
        fprintf(stderr, "Error: Name filename as first argument!\n");
        free(file_head);
        return 1;
    }

    if ((fp = fopen(file_name, "rb")) == NULL)
    {
        fprintf(stderr, "Error opening file!\n");
        free(file_head);
        return 1;
    }

    if (fgets(file_head, 10, fp) == NULL)
    {
        fprintf(stderr, "Error reading first line of file!\n");
        free(file_head);
        fclose(fp);
        return 1;
    }

    if (strcmp(file_head, "P6\n") != 0)
    {
        fprintf(stderr, "This file is not P6 file!\n");
        free(file_head);
        fclose(fp);
        return 1;
    }

    printf("file_head -- %s\n", file_head);

    // don't need anything with the first line of file
    free(file_head);

    if (fscanf(fp, "%d %d %d", &width, &height, &color_range) != 3)
    {
        fprintf(stderr, "Error reading width and height!\n");
        fclose(fp);
        return 1;
    }

    fgetc(fp);

    printf("Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);

    img_array = (unsigned char *)malloc(PIXEL_WIDTH * width * height * sizeof(unsigned char));

    if (fread(img_array, 1, PIXEL_WIDTH * width * height, fp) != PIXEL_WIDTH * width * height)
    {
        fprintf(stderr, "Error reading the image and loading it into the array!\n");
        free(img_array);
        fclose(fp);
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

    // iterate through the picture with blocks with the width of BLOCK_SIZE * PIXEL_WIDTH (iterate through whole height of the picture with this block size), then move through the picture width to next block with the width of BLOCK_SIZE * PIXEL_WIDTH and iterate through whole height of the picture...
    for (int i = 0; i <= ((width * PIXEL_WIDTH) / (BLOCK_SIZE * PIXEL_WIDTH)); i++)
    {
        for (int h = 0; h < (height - 1); h++)
        {
            if (h == 0)
            {
                continue;
            }
            for (int j = (i * BLOCK_SIZE * PIXEL_WIDTH); j < (i * BLOCK_SIZE * PIXEL_WIDTH + BLOCK_SIZE * PIXEL_WIDTH) && j < (width * PIXEL_WIDTH); j++)
            {
                if (j == 0 || j == ((width * PIXEL_WIDTH) - 1))
                {
                    continue;
                }

                img_array[(h * width * PIXEL_WIDTH) + RED(j)] = (-1 * img_array[(h * width * PIXEL_WIDTH) + RED(j) - PIXEL_WIDTH]) + (5 * img_array[(h * width * PIXEL_WIDTH) + RED(j)]) + ((-1) * img_array[h * width * PIXEL_WIDTH + RED(j) + PIXEL_WIDTH]) + (-1 * img_array[((h - 1) * width * PIXEL_WIDTH) + RED(j)]) + (-1 * img_array[((h + 1) * width * PIXEL_WIDTH) + RED(j)]);

                if (img_array[(h * width * PIXEL_WIDTH) + RED(j)] < 0)
                {
                    img_array[(h * width * PIXEL_WIDTH) + RED(j)] = 0;
                }

                if (img_array[(h * width * PIXEL_WIDTH) + RED(j)] > 255)
                {
                    img_array[(h * width * PIXEL_WIDTH) + RED(j)] = 255;
                }

                img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] = (-1 * img_array[(h * width * PIXEL_WIDTH) + GREEN(j) - PIXEL_WIDTH]) + (5 * img_array[(h * width * PIXEL_WIDTH) + GREEN(j)]) + ((-1) * img_array[h * width * PIXEL_WIDTH + GREEN(j) + PIXEL_WIDTH]) + (-1 * img_array[((h - 1) * width * PIXEL_WIDTH) + GREEN(j)]) + (-1 * img_array[((h + 1) * width * PIXEL_WIDTH) + GREEN(j)]);

                if (img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] < 0)
                {
                    img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] = 0;
                }

                if (img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] > 255)
                {
                    img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] = 255;
                }

                img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] = (-1 * img_array[(h * width * PIXEL_WIDTH) + BLUE(j) - PIXEL_WIDTH]) + (5 * img_array[(h * width * PIXEL_WIDTH) + BLUE(j)]) + ((-1) * img_array[h * width * PIXEL_WIDTH + BLUE(j) + PIXEL_WIDTH]) + (-1 * img_array[((h - 1) * width * PIXEL_WIDTH) + BLUE(j)]) + (-1 * img_array[((h + 1) * width * PIXEL_WIDTH) + BLUE(j)]);

                if (img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] < 0)
                {
                    img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] = 0;
                }

                if (img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] > 255)
                {
                    img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] = 255;
                }
            }
        }
    }

    // printing array after convolution into file
    FILE *final_array = fopen("final_array.txt", "w");
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

    final_picture = fopen("final_picture.ppm", "w");
    if ((fprintf(final_picture, "P6\n%d\n%d\n%d\n", width, height, 255)) != 15)
    {
        fprintf(stderr, "Error writing type, width, height and color range of pixels!\n");
        free(img_array);
        fclose(final_picture);
        return 1;
    }
    if ((fwrite(img_array, 1, (PIXEL_WIDTH * width * height), final_picture)) != (PIXEL_WIDTH * width * height))
    {
        fprintf(stderr, "Error writing the image from the array!\n");
        free(img_array);
        fclose(final_picture);
        return 1;
    }
    fclose(final_picture);

    free(img_array);
    return 0;
}
