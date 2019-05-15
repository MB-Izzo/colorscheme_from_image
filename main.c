// main.c
// https://stackoverflow.com/a/4169105
#include <stdio.h>
#include <wand/MagickWand.h>

typedef struct {
    int h;
    int s;
    int b;
} color;

color NewColor(double *h, double *s, double *b);
color ColorToSimplified(color col);
void ConvertAllColor(color arr[], int arr_size, color new_arr[]);
void PrintAll(color arr[], int arr_size);
int GetPopularH(color arr[], int arr_size);
int GetPopularS(color arr[], int arr_size);
int GetPopularB(color arr[], int arr_size);


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
    int popular_h = GetPopularH(simple_colors, ARR_SIZE);
    int popular_s = GetPopularS(simple_colors, ARR_SIZE);
    int popular_b = GetPopularB(simple_colors, ARR_SIZE);

    printf("The most popular color is: \n");
    printf("H: %d\nS: %d\nB: %d\n", popular_h * 16, popular_s * 10, popular_b * 10);


    mw = DestroyMagickWand(mw);
    MagickWandTerminus();

    printf("Pixel number: %d\n", idx);

    return 0;
}

color NewColor(double *h, double *s, double *b)
{
    color col = {*h*100, *s*100, *b*100};
    return col;
}

color ColorToSimplified(color col)
{
    color c = {col.h / 16, col.s / 10, col.b / 10};
    return c;
}

void ConvertAllColor(color arr[], int arr_size, color arr_to_populate[]) 
{
    int i;
    for (i = 0; i < arr_size; i++)
    {
        arr_to_populate[i] = ColorToSimplified(arr[i]);
    }
}

void PrintAll(color arr[], int arr_size)
{
    int i;
    for (i = 0; i < arr_size; i++)
    {
        printf("H: %d, S: %d, B: %d\n", arr[i].h, arr[i].s, arr[i].b);
    }
}

int GetPopularH(color arr[], int arr_size)
{
    int count = 1, temp_count;
    int popular = arr[0].h;
    int temp = 0;
    int i = 0;
    int j = 0;
    for (i = 0; i < (arr_size-1); i++)
    {
        temp = arr[i].h;
        temp_count = 0;
        for(j = 0; j < arr_size; j++)
        {
            if (temp == arr[j].h)
            {
                temp_count++;
            }
        }
        if (temp_count > count)
        {
            popular = temp;
            count = temp_count;
        }
    }
    return popular;
}

int GetPopularS(color arr[], int arr_size)
{
    int count = 1, temp_count;
    int popular = arr[0].s;
    int temp = 0;
    int i = 0;
    int j = 0;
    for (i = 0; i < (arr_size-1); i++)
    {
        temp = arr[i].s;
        temp_count = 0;
        for(j = 0; j < arr_size; j++)
        {
            if (temp == arr[j].s)
            {
                temp_count++;
            }
        }
        if (temp_count > count)
        {
            popular = temp;
            count = temp_count;
        }
    }
    return popular;
}

int GetPopularB(color arr[], int arr_size)
{
    int count = 1, temp_count;
    int popular = arr[0].b;
    int temp = 0;
    int i = 0;
    int j = 0;
    for (i = 0; i < (arr_size-1); i++)
    {
        temp = arr[i].b;
        temp_count = 0;
        for(j = 0; j < arr_size; j++)
        {
            if (temp == arr[j].b)
            {
                temp_count++;
            }
        }
        if (temp_count > count)
        {
            popular = temp;
            count = temp_count;
        }
    }
    return popular;
}