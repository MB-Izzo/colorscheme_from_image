# Color Scheme Generator from image.
**WIP**.
A C program to generate a color scheme from an image using ImageMagick C API.

This is a small project I am doing to learn C.

## Build

```
gcc main.c color.c `pkg-config --cflags --libs MagickWand
```

## Current state

It prints out the most popular color from "logored" image in HSB format.

Specify an image with the '-i' argument.

```
./a.out -i logo.jpg
```