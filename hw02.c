#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 3
#define PIXEL_WIDTH 3
#define RED(x) x
#define GREEN(x) x + 1
#define BLUE(x) x + 2
#define MAX_COLOR_RANGE 255

int main(int argc, char **argv)
{
    unsigned char *img_array;
    unsigned char *img_array_02;
    char *file_name;
    FILE *fp;
    FILE *final_picture;
    FILE *histogram_output;
    int width;
    int height;
    int color_range;
    char *file_head = (char *)malloc(10 * sizeof(char));
    unsigned long cursor_position;

    // Histogram
    int hist_01 = 0;
    int hist_02 = 0;
    int hist_03 = 0;
    int hist_04 = 0;
    int hist_05 = 0;
    int hist_temp = 0;
    int hist_error = 0;

    if (argc > 1)
    {
        file_name = argv[1];
    }
    // printf("File name -- %s\n", argv[1]);
    if (argc < 2)
    {
        fprintf(stderr, "Error: Name filename as first argument!\n");
        free(file_head);
        return 1;
    }

    if ((fp = fopen(file_name, "rb")) == NULL)
    {
        fprintf(stderr, "Error opening source picture file!\n");
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

    // printf("file_head -- %s\n", file_head);

    // don't need anything with the first line of file
    free(file_head);

    fscanf(fp, "%d %d %d", &width, &height, &color_range);

    fgetc(fp);

    // printf("Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);

    fflush(fp);
    cursor_position = ftell(fp);

    if ((img_array = (unsigned char *)malloc(PIXEL_WIDTH * width * height * sizeof(unsigned char))) == NULL)
    {
        fprintf(stderr, "Error allocating image array!\n");
        fclose(fp);
        return 1;
    }

    if ((img_array_02 = (unsigned char *)malloc(PIXEL_WIDTH * width * height * sizeof(unsigned char))) == NULL)
    {
        fprintf(stderr, "Error allocating image array!\n");
        fclose(fp);
        free(img_array);
        return 1;
    }

    if (fread(img_array, 1, PIXEL_WIDTH * width * height, fp) != PIXEL_WIDTH * width * height)
    {
        fprintf(stderr, "Error reading the image and loading it into the array!\n");
        free(img_array);
        free(img_array_02);
        fclose(fp);
        return 1;
    }

    fseek(fp, cursor_position, SEEK_SET);

    if (fread(img_array_02, 1, PIXEL_WIDTH * width * height, fp) != PIXEL_WIDTH * width * height)
    {
        fprintf(stderr, "Error reading the image and loading it into the array!\n");
        free(img_array);
        free(img_array_02);
        fclose(fp);
        return 1;
    }

    if (fclose(fp) == EOF)
    {
        fprintf(stderr, "Error closing the picture file!\n");
        return 1;
    }

    // printing whole array into file to check if it's complete
    FILE *source_array = fopen("source_array.txt", "w");
    fprintf(source_array, "Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);
    for (int i = 1; i <= (PIXEL_WIDTH * width * height); i++)
    {
        // fprintf(source_array, "i -- %d ", i);
        fprintf(source_array, "%d ", img_array[i - 1]);
        if (i % (PIXEL_WIDTH * width) == 0 && i != 0)
        {
            fprintf(source_array, "\n");
        }
    }
    fclose(source_array);

    for (int i = 0; i <= width / BLOCK_SIZE; i++)
    {
        // printf("i -- %d\n", i);
        for (int h = 0; h < height; h++)
        {
            // printf("h -- %d\n", h);
            if(h == 0 || h == height - 1) {
                hist_temp = (int)(0.2126 * img_array[(h * width * PIXEL_WIDTH)]) + (int)(0.7152 * img_array[(h * width * PIXEL_WIDTH)]) + (int)(0.0722 * img_array[(h * width * PIXEL_WIDTH)]);

                if (hist_temp >= 0 && hist_temp < 51)
                {
                    hist_01++;
                }
                else if (hist_temp >= 51 && hist_temp < 102)
                {
                    hist_02++;
                }
                else if (hist_temp >= 102 && hist_temp < 153)
                {
                    hist_03++;
                }
                else if (hist_temp >= 153 && hist_temp < 204)
                {
                    hist_04++;
                }
                else if (hist_temp >= 204 && hist_temp <= 255)
                {
                    hist_05++;
                }
                else
                {
                    hist_error++;
                }
                
                continue;
            }

            for (int j = i * BLOCK_SIZE * PIXEL_WIDTH; j < i * BLOCK_SIZE * PIXEL_WIDTH + BLOCK_SIZE * PIXEL_WIDTH && j < width * PIXEL_WIDTH; j += PIXEL_WIDTH)
            {
                // printf("j -- %d\n", j);
                if (j == 0 || j == (width * PIXEL_WIDTH) - PIXEL_WIDTH)
                {
                    hist_temp = (int)(0.2126 * img_array[(h * width * PIXEL_WIDTH) + RED(j)]) + (int)(0.7152 * img_array[(h * width * PIXEL_WIDTH) + GREEN(j)]) + (int)(0.0722 * img_array[(h * width * PIXEL_WIDTH) + BLUE(j)]);

                    if (hist_temp >= 0 && hist_temp < 51)
                    {
                        hist_01++;
                    }
                    else if (hist_temp >= 51 && hist_temp < 102)
                    {
                        hist_02++;
                    }
                    else if (hist_temp >= 102 && hist_temp < 153)
                    {
                        hist_03++;
                    }
                    else if (hist_temp >= 153 && hist_temp < 204)
                    {
                        hist_04++;
                    }
                    else if (hist_temp >= 204 && hist_temp <= 255)
                    {
                        hist_05++;
                    }
                    else
                    {
                        hist_error++;
                    }
                    
                    continue;
                }

                img_array[(h * width * PIXEL_WIDTH) + RED(j)] = (-1 * img_array_02[(h * width * PIXEL_WIDTH) + RED(j) - PIXEL_WIDTH]) + (5 * img_array_02[(h * width * PIXEL_WIDTH) + RED(j)]) + ((-1) * img_array_02[h * width * PIXEL_WIDTH + RED(j) + PIXEL_WIDTH]) + (-1 * img_array_02[((h - 1) * width * PIXEL_WIDTH) + RED(j)]) + (-1 * img_array_02[((h + 1) * width * PIXEL_WIDTH) + RED(j)]);

                if (img_array[(h * width * PIXEL_WIDTH) + RED(j)] < 0)
                {
                    img_array[(h * width * PIXEL_WIDTH) + RED(j)] = 0;
                }

                if (img_array[(h * width * PIXEL_WIDTH) + RED(j)] > MAX_COLOR_RANGE)
                {
                    img_array[(h * width * PIXEL_WIDTH) + RED(j)] = MAX_COLOR_RANGE;
                }

                img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] = (-1 * img_array_02[(h * width * PIXEL_WIDTH) + GREEN(j) - PIXEL_WIDTH]) + (5 * img_array_02[(h * width * PIXEL_WIDTH) + GREEN(j)]) + ((-1) * img_array_02[h * width * PIXEL_WIDTH + GREEN(j) + PIXEL_WIDTH]) + (-1 * img_array_02[((h - 1) * width * PIXEL_WIDTH) + GREEN(j)]) + (-1 * img_array_02[((h + 1) * width * PIXEL_WIDTH) + GREEN(j)]);

                if (img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] < 0)
                {
                    img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] = 0;
                }

                if (img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] > MAX_COLOR_RANGE)
                {
                    img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] = MAX_COLOR_RANGE;
                }

                img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] = (-1 * img_array_02[(h * width * PIXEL_WIDTH) + BLUE(j) - PIXEL_WIDTH]) + (5 * img_array_02[(h * width * PIXEL_WIDTH) + BLUE(j)]) + ((-1) * img_array_02[h * width * PIXEL_WIDTH + BLUE(j) + PIXEL_WIDTH]) + (-1 * img_array_02[((h - 1) * width * PIXEL_WIDTH) + BLUE(j)]) + (-1 * img_array_02[((h + 1) * width * PIXEL_WIDTH) + BLUE(j)]);

                if (img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] < 0)
                {
                    img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] = 0;
                }

                if (img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] > MAX_COLOR_RANGE)
                {
                    img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] = MAX_COLOR_RANGE;
                }

                // Y = round(0.2126 * R + 0.7152 * G + 0.0722 * B)

                hist_temp = (int)(0.2126 * img_array[(h * width * PIXEL_WIDTH) + RED(j)]) + (int)(0.7152 * img_array[(h * width * PIXEL_WIDTH) + GREEN(j)]) + (int)(0.0722 * img_array[(h * width * PIXEL_WIDTH) + BLUE(j)]);

                if (hist_temp >= 0 && hist_temp < 51)
                {
                    hist_01++;
                }
                else if (hist_temp >= 51 && hist_temp < 102)
                {
                    hist_02++;
                }
                else if (hist_temp >= 102 && hist_temp < 153)
                {
                    hist_03++;
                }
                else if (hist_temp >= 153 && hist_temp < 204)
                {
                    hist_04++;
                }
                else if (hist_temp >= 204 && hist_temp <= 255)
                {
                    hist_05++;
                }
                else
                {
                    hist_error++;
                }
            }
        }
    }

    // printing array after convolution into file
    FILE *final_array = fopen("final_array.txt", "w");
    fprintf(final_array, "Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);
    for (int i = 1; i <= (PIXEL_WIDTH * width * height); i++)
    {
        // fprintf(final_array, "i -- %d ", i);
        fprintf(final_array, "%d ", img_array[i - 1]);
        if (i % (PIXEL_WIDTH * width) == 0 && i != 0)
        {
            fprintf(final_array, "\n");
        }
    }
    fclose(final_array);

    if ((final_picture = fopen("final_picture.ppm", "w")) == NULL)
    {
        fprintf(stderr, "Error opening final picture file!\n");
        free(img_array);
        free(img_array_02);
        return 1;
    }

    fprintf(final_picture, "P6\n%d\n%d\n%d\n", width, height, MAX_COLOR_RANGE);

    if ((fwrite(img_array, 1, (PIXEL_WIDTH * width * height), final_picture)) != (PIXEL_WIDTH * width * height))
    {
        fprintf(stderr, "Error writing the image from the array!\n");
        free(img_array);
        free(img_array_02);
        fclose(final_picture);
        return 1;
    }
    if (fclose(final_picture) == EOF)
    {
        fprintf(stderr, "Error closing the final picture file!\n");
        free(img_array);
        free(img_array_02);
        return 1;
    }

    if ((histogram_output = fopen("output.txt", "w")) == NULL)
    {
        fprintf(stderr, "Error opening histogram output file!\n");
        free(img_array);
        free(img_array_02);
        return 1;
    }

    if (hist_error != 0)
    {
        fprintf(stderr, "One of the histogram numbers is not in the stated interval!\n");
        free(img_array);
        free(img_array_02);
        fclose(histogram_output);
        return 1;
    }

    fprintf(histogram_output, "%d %d %d %d %d", hist_01, hist_02, hist_03, hist_04, hist_05);

    if (fclose(histogram_output) == EOF)
    {
        fprintf(stderr, "Error closing the histogram output file!\n");
        free(img_array);
        free(img_array_02);
        return 1;
    }

    free(img_array);
    free(img_array_02);
    return 0;
}
