#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define BLOCK_SIZE 550
#define PIXEL_WIDTH 3
#define RED(x) x
#define GREEN(x) x + 1
#define BLUE(x) x + 2
#define MAX_COLOR_RANGE 255

// int round_it(double x)
// {
//     if (x < 0.0)
//         return (int)(x - 0.5);
//     else
//         return (int)(x + 0.5);
// }

int main(int argc, char **argv)
{
    unsigned char *img_array;
    unsigned char *img_array_02;
    int tmp_red;
    int tmp_green;
    int tmp_blue;
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

    // don't need anything with the first line of file
    free(file_head);

    if((fscanf(fp, "%d %d %d", &width, &height, &color_range)) != 3) {
        fprintf(stderr, "Error reading width, height and color range!\n");
        fclose(fp);
        return 1;
    }

    fgetc(fp);

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
    // FILE *source_array = fopen("source_array.txt", "w");
    // fprintf(source_array, "Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);
    // for (int i = 1; i <= (PIXEL_WIDTH * width * height); i++)
    // {
    //     // fprintf(source_array, "i -- %d ", i);
    //     fprintf(source_array, "%d ", img_array[i - 1]);
    //     if (i % (PIXEL_WIDTH * width) == 0 && i != 0)
    //     {
    //         fprintf(source_array, "\n");
    //     }
    // }
    // fclose(source_array);

    for (int i = 0; i <= width / BLOCK_SIZE; i++)
    {
        for (int h = 0; h < height; h++)
        {
            for (int j = i * BLOCK_SIZE * PIXEL_WIDTH; j < i * BLOCK_SIZE * PIXEL_WIDTH + BLOCK_SIZE * PIXEL_WIDTH && j < width * PIXEL_WIDTH; j += PIXEL_WIDTH)
            {
                if (j == 0 || j == (width * PIXEL_WIDTH) - PIXEL_WIDTH || h == 0 || h == height - 1)
                {
                    hist_temp = round((0.2126 * img_array[(h * width * PIXEL_WIDTH) + RED(j)]) + (0.7152 * img_array[(h * width * PIXEL_WIDTH) + GREEN(j)]) + (0.0722 * img_array[(h * width * PIXEL_WIDTH) + BLUE(j)]));

                    if (hist_temp >= 0 && hist_temp <= 50)
                    {
                        hist_01++;
                    }
                    else if (hist_temp >= 51 && hist_temp <= 101)
                    {
                        hist_02++;
                    }
                    else if (hist_temp >= 102 && hist_temp <= 152)
                    {
                        hist_03++;
                    }
                    else if (hist_temp >= 153 && hist_temp <= 203)
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

                tmp_red = (-1 * img_array_02[(h * width * PIXEL_WIDTH) + RED(j) - PIXEL_WIDTH]) + (5 * img_array_02[(h * width * PIXEL_WIDTH) + RED(j)]) + (-1 * img_array_02[(h * width * PIXEL_WIDTH) + RED(j) + PIXEL_WIDTH]) + (-1 * img_array_02[((h - 1) * width * PIXEL_WIDTH) + RED(j)]) + (-1 * img_array_02[((h + 1) * width * PIXEL_WIDTH) + RED(j)]);

                tmp_green = (-1 * img_array_02[(h * width * PIXEL_WIDTH) + GREEN(j) - PIXEL_WIDTH]) + (5 * img_array_02[(h * width * PIXEL_WIDTH) + GREEN(j)]) + (-1 * img_array_02[(h * width * PIXEL_WIDTH) + GREEN(j) + PIXEL_WIDTH]) + (-1 * img_array_02[((h - 1) * width * PIXEL_WIDTH) + GREEN(j)]) + (-1 * img_array_02[((h + 1) * width * PIXEL_WIDTH) + GREEN(j)]);

                tmp_blue = (-1 * img_array_02[(h * width * PIXEL_WIDTH) + BLUE(j) - PIXEL_WIDTH]) + (5 * img_array_02[(h * width * PIXEL_WIDTH) + BLUE(j)]) + (-1 * img_array_02[(h * width * PIXEL_WIDTH) + BLUE(j) + PIXEL_WIDTH]) + (-1 * img_array_02[((h - 1) * width * PIXEL_WIDTH) + BLUE(j)]) + (-1 * img_array_02[((h + 1) * width * PIXEL_WIDTH) + BLUE(j)]);            

                if ((tmp_red >> 8) != 0)
                {
                    if (tmp_red < 0) 
                    {
                        tmp_red = 0;
                    }
                    else 
                    {
                        tmp_red = MAX_COLOR_RANGE;
                    }
                }

                if ((tmp_green >> 8) != 0)
                {
                    if (tmp_green < 0) 
                    {
                        tmp_green = 0;
                    }
                    else 
                    {
                        tmp_green = MAX_COLOR_RANGE;
                    }
                }

                if ((tmp_blue >> 8) != 0)
                {
                    if (tmp_blue < 0) 
                    {
                        tmp_blue = 0;
                    }
                    else 
                    {
                        tmp_blue = MAX_COLOR_RANGE;
                    }
                }

                img_array[(h * width * PIXEL_WIDTH) + RED(j)] = tmp_red;
                img_array[(h * width * PIXEL_WIDTH) + GREEN(j)] = tmp_green;
                img_array[(h * width * PIXEL_WIDTH) + BLUE(j)] = tmp_blue;

                // Y = round(0.2126 * R + 0.7152 * G + 0.0722 * B)

                hist_temp = round((0.2126 * img_array[(h * width * PIXEL_WIDTH) + RED(j)]) + (0.7152 * img_array[(h * width * PIXEL_WIDTH) + GREEN(j)]) + (0.0722 * img_array[(h * width * PIXEL_WIDTH) + BLUE(j)]));

                if (hist_temp >= 0 && hist_temp <= 50)
                {
                    hist_01++;
                }
                else if (hist_temp >= 51 && hist_temp <= 101)
                {
                    hist_02++;
                }
                else if (hist_temp >= 102 && hist_temp <= 152)
                {
                    hist_03++;
                }
                else if (hist_temp >= 153 && hist_temp <= 203)
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
    // FILE *final_array = fopen("final_array.txt", "w");
    // fprintf(final_array, "Width -- %d\nHeight -- %d\nColor Range -- %d\n", width, height, color_range);
    // for (int i = 1; i <= (PIXEL_WIDTH * width * height); i++)
    // {
    //     // fprintf(final_array, "i -- %d ", i);
    //     fprintf(final_array, "%d ", img_array[i - 1]);
    //     if (i % (PIXEL_WIDTH * width) == 0 && i != 0)
    //     {
    //         fprintf(final_array, "\n");
    //     }
    // }
    // fclose(final_array);

    if ((final_picture = fopen("output.ppm", "w")) == NULL)
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
