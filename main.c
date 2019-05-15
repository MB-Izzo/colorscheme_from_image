// main.c
// Implementing: https://stackoverflow.com/a/4169105
// gcc main.c `pkg-config --cflags --libs MagickWand`

#include <stdio.h>
#include <string.h>
#include <wand/MagickWand.h>
#include "color.h"

typedef struct {
    int h;
    int s;
    int b;
} color;

color new_color(double *h, double *s, double *b);
color simplify_color(color col);
void convert_all_colors(color arr[], int arr_size, color output_arr[]);
// Could find a way to not have these duplicates...
color get_popular_color(color arr[], int arr_size);

int main()
{
    int za = 3;
    int zb = 4;
    add(&za, &zb);

    MagickWand *mw;
    PixelIterator *pi;
    PixelWand **pw;

    unsigned long x;
    unsigned long y;
    unsigned long width;
    unsigned long height;

    Quantum qr;
    Quantum qg;
    Quantum qb;

    double h;
    double s;
    double b;

    // Init MagickWand
    MagickWandGenesis();
    mw = NewMagickWand();

    MagickReadImage(mw, "logored.jpg");

    width = MagickGetImageWidth(mw);
    height = MagickGetImageHeight(mw);

    printf("\nImage information:\n\n");
    printf("Image Width: %lu\nImage Height: %lu\n", width, height);
    printf("Pixel number: %lu\n\n", width*height);

    // Array containing pixels
    const int ARR_SIZE = (width * height);

    color color_arr[ARR_SIZE];
    color simple_colors[ARR_SIZE];

    pi = NewPixelIterator(mw);

    int idx = 0;

    // read the iamge pixel by pixel.
    for(y = 0; y < height; y++) {
        pw = PixelGetNextIteratorRow(pi, &width);
        for(x = 0; x < (long) width; x++) {
            qr = PixelGetRedQuantum(pw[x]);
            qg = PixelGetGreenQuantum(pw[x]);
            qb = PixelGetBlueQuantum(pw[x]);
            ConvertRGBToHSB(qr, qg, qb, &h, &s, &b);
            color_arr[idx] = new_color(&h, &s, &b);
            idx++;
        }
    }

    convert_all_colors(color_arr, ARR_SIZE, simple_colors);

    color a = get_popular_color(simple_colors, ARR_SIZE);
    printf("-------\n\n");
    printf("Popular color: \n");
    printf("H: %d, S: %d, B: %d\n\n", a.h * 16, a.s * 10 , a.b * 10);

    mw = DestroyMagickWand(mw);
    MagickWandTerminus();

    printf("Programmed executed successfully.\n");
    return 0;
}

// Clean the floating numbers.
color new_color(double *h, double *s, double *b)
{
    color col = {*h*100, *s*100, *b*100};
    return col;
}

// To categorize colors.
color simplify_color(color col)
{
    color c = {col.h / 16, col.s / 10, col.b / 10};
    return c;
}

void convert_all_colors(color arr[], int arr_size, color output_arr[]) 
{
    int i;
    for (i = 0; i < arr_size; i++) {
        output_arr[i] = simplify_color(arr[i]);
    }
}

color get_popular_color(color arr[], int arr_size)
{
    int count = 1, temp_count;
    color popular_color = arr[0];
    color temp_color;
    int temp = 0;
    int i = 0;
    int j = 0;
    for (i = 0; i < (arr_size-1); i++)
    {
        temp_color = arr[i];
        temp_count = 0;
        for(j = 0; j < arr_size; j++) {
            if (memcmp(&temp_color, &arr[j], sizeof(color)) == 0) {
                temp_count++;
            }
        }
        if (temp_count > count)
        {
            popular_color = temp_color;
            count = temp_count;
        }
    }
    return popular_color;
}
