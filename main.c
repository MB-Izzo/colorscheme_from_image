// main.c
// -L/usr/include/ImageMagick-6
#include <stdio.h>
#include <wand/MagickWand.h>


typedef struct {
    float h;
    float s;
    float b;
} color;

color NewColor(double *h, double *s, double *b);


int main()
{
    MagickWand *mw;
    PixelIterator *pi;
    PixelWand **pw;

    unsigned long x, y;
    unsigned long width, height;
    Quantum qr, qg, qb;
    double h, s, b;

    int idx = 0;

    // Init MagickWand
    MagickWandGenesis();
    mw = NewMagickWand();

    MagickReadImage(mw, "logo.jpg");

    width = MagickGetImageWidth(mw);
    height = MagickGetImageHeight(mw);
    printf("Width: %lu\nHeight: %lu\n", width, height);

    // Array containing pixels
    const int ARR_SIZE = (width * height);

    color color_arr[ARR_SIZE];

    pi = NewPixelIterator(mw);

    for(y = 0; y < height; y++) {
        pw = PixelGetNextIteratorRow(pi, &width);
        for(x = 0; x < (long) width; x++) {
            qr = PixelGetRedQuantum(pw[x]);
            qg = PixelGetGreenQuantum(pw[x]);
            qb = PixelGetBlueQuantum(pw[x]);
            ConvertRGBToHSB(qr, qg, qb, &h, &s, &b);
            color_arr[idx] = NewColor(&h, &s, &b);
            idx++;
        }
    }
    printf("Col: %f, %f, %f \n", color_arr[1].h, color_arr[1].s, color_arr[1].b);
    
    
    mw = DestroyMagickWand(mw);
    MagickWandTerminus();

    printf("Pixel number: %d\n", idx);

    return 0;
}

color NewColor(double *h, double *s, double *b)
{
    *h = (float)*h;
    *s = (float)*s;
    *b = (float)*b;

    color col = {*h, *s, *b};

    return col;
}

