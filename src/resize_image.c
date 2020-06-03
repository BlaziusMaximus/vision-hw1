#include <math.h>
#include <stdio.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return im.data[(int)roundf(c)*im.h*im.w + (int)roundf(y)*im.w + (int)roundf(x)];
}

image nn_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);

    for (float cha = 0; cha < im.c; cha++) {
        for (float row = 0; row < h; row++) {
            for (float col = 0; col < w; col++) {
                resized.data[(int)cha*h*w + (int)row*w + (int)col] = nn_interpolate(im, col*im.w/w, row*im.h/h, cha);
            }
        }
    }

    return resized;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    int floor_x = (int)floorf(x); int floor_y = (int)floorf(y);
    int ceil_x = (int)ceilf(x); int ceil_y = (int)ceilf(y);
    if (floor_x < 0) floor_x = 0; if (floor_y < 0) floor_y = 0;
    if (ceil_x >= im.w) ceil_x = im.w-1; if (ceil_y >= im.h) ceil_y = im.h-1;

    float v1 = get_pixel(im, floor_x, floor_y, c);
    float v2 = get_pixel(im, ceil_x, floor_y, c);
    float v3 = get_pixel(im, floor_x, ceil_y, c);
    float v4 = get_pixel(im, ceil_x, ceil_y, c);

    float a1 = (ceil_x-x) * (ceil_y-y);
    float a2 = (x-floor_x) * (ceil_y-y);
    float a3 = (ceil_x-x) * (y-floor_y);
    float a4 = (x-floor_x) * (y-floor_y);

    if (a1 + a2 + a3 + a4 == 0) { a1 = .25; a2 = .25; a3 = .25; a4 = .25; }

    return v1*a1 + v2*a2 + v3*a3 + v4*a4;
}

image bilinear_resize(image im, int w, int h)
{
    image resized = make_image(w, h, im.c);

    for (float cha = 0; cha < im.c; cha++) {
        for (float row = 0; row < h; row++) {
            for (float col = 0; col < w; col++) {
                resized.data[(int)cha*h*w + (int)row*w + (int)col] = bilinear_interpolate(im, col*im.w/w-.5+.5*im.w/w, row*im.h/h-.5+.5*im.h/h, cha);
            }
        }
    }

    return resized;
}

