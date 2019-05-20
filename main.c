// main.c
// Implementing: https://stackoverflow.com/a/4169105
// gcc -g main.c color.c `pkg-config --cflags --libs MagickWand`

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wand/MagickWand.h>
#include "color.h"

int main(int argc, char *argv[])
{
    printf("%lu\n", __SIZE_MAX__);

    char *img_arg = NULL;
    int c;

    while ((c = getopt(argc, argv, "i:" )) != -1)
    {
        switch(c)
        {
            case 'i':
                img_arg = optarg;
                break;
            case '?':
                if (optopt == 'i')
                    fprintf(stderr, "Option -%c requires an arg.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character.\n");
                return 1;
        }
    }

    if (img_arg == NULL)
    {
        fprintf(stderr, "You need to specify an img with -i argument.\n");
        return 1;
    }

    if (access(img_arg, F_OK) != -1)
    {
        printf("Opened: %s\n", img_arg);
    }
    else
    {
        printf("Can not open: %s\n", img_arg);
        return 0;
    }

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

    MagickReadImage(mw, img_arg);

    width = MagickGetImageWidth(mw);
    height = MagickGetImageHeight(mw);

    printf("\nImage information:\n\n");
    printf("Image Width: %lu\nImage Height: %lu\n", width, height);
    printf("Pixel number: %lu\n\n", width*height);

    // Array containing each pixels
    unsigned long ARR_SIZE = (width * height);

    color color_arr[ARR_SIZE];
    color simple_colors[ARR_SIZE];

    pi = NewPixelIterator(mw);

    unsigned long idx = 0;

    // read the image pixel by pixel.
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
        PixelSyncIterator(pi);
    }

    convert_all_colors(color_arr, ARR_SIZE, simple_colors);

    color popular_color = get_popular_color(simple_colors, ARR_SIZE);
    printf("-------\n\n");
    printf("Popular color: \n");
    printf("H: %d, S: %d, B: %d\n\n", popular_color.h * 16, popular_color.s * 10,
    popular_color.b * 10);

    pi = DestroyPixelIterator(pi);
    mw = DestroyMagickWand(mw);
    MagickWandTerminus();

    printf("Program executed successfully.\n");
    return 0;
}

