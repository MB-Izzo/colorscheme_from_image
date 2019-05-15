// main.c
// https://stackoverflow.com/a/4169105
#include <stdio.h>
#include <string.h>
#include <wand/MagickWand.h>

typedef struct {
    int h;
    int s;
    int b;
} color;

color NewColor(double *h, double *s, double *b);
color ColorToSimplified(color col);
void ConvertAllColor(color arr[], int arr_size, color output_arr[]);
// Could find a way to not have these duplicates...
color GetPopularColor(color arr[], int arr_size);

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

    MagickReadImage(mw, "logored.jpg");

    width = MagickGetImageWidth(mw);
    height = MagickGetImageHeight(mw);
    printf("Width: %lu\nHeight: %lu\n", width, height);

    // Array containing pixels
    const int ARR_SIZE = (width * height);

    color color_arr[ARR_SIZE];
    color simple_colors[ARR_SIZE];

    pi = NewPixelIterator(mw);

    // read the iamge pixel by pixel.
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

    ConvertAllColor(color_arr, ARR_SIZE, simple_colors);

    color a = GetPopularColor(simple_colors, ARR_SIZE);
    printf("h: %d, s: %d, b: %d\n", a.h * 16, a.s * 10 , a.b * 10);

    mw = DestroyMagickWand(mw);
    MagickWandTerminus();

    printf("Pixel number: %d\n", idx);

    return 0;
}

// Clean the floating numbers.
color NewColor(double *h, double *s, double *b)
{
    color col = {*h*100, *s*100, *b*100};
    return col;
}

// To categorize colors.
color ColorToSimplified(color col)
{
    color c = {col.h / 16, col.s / 10, col.b / 10};
    return c;
}

void ConvertAllColor(color arr[], int arr_size, color output_arr[]) 
{
    int i;
    for (i = 0; i < arr_size; i++)
    {
        output_arr[i] = ColorToSimplified(arr[i]);
    }
}

color GetPopularColor(color arr[], int arr_size)
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
        for(j = 0; j < arr_size; j++)
        {
            if (memcmp(&temp_color, &arr[j], sizeof(color)) == 0)
            {
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
