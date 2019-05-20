// main.c
// Implementing: https://stackoverflow.com/a/4169105
// gcc main.c color.c `pkg-config --cflags --libs MagickWand`

#include <stdio.h>
#include <string.h>
#include <wand/MagickWand.h>
#include "color.h"

int main()
{
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

    printf("Program executed successfully.\n");
    return 0;
}

