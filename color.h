typedef struct {
    int h;
    int s;
    int b;
} color;

color new_color(double *h, double *s, double *b);
color simplify_color(color col);
void convert_all_colors(color arr[], int arr_size, color output_arr[]);
color get_popular_color(color arr[], int arr_size);