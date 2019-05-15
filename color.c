#include "color.h"
#include <string.h>

struct color;

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
