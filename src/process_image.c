#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    return im.data[c*im.w*im.h + y*im.w + x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    im.data[c*im.w*im.h + y*im.w + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for (int cha = 0; cha < im.c; cha++) {
        for (int row = 0; row < im.h; row++) {
            for (int col = 0; col < im.w; col++) {
                copy.data[cha*im.w*im.h + row*im.w + col] = im.data[cha*im.w*im.h + row*im.w + col];
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for (int row = 0; row < im.h; row++) {
        for (int col = 0; col < im.w; col++) {
            float w_sum = 0.299*im.data[row*im.w + col] + 0.587*im.data[im.w*im.h + row*im.w + col] + 0.114*im.data[2*im.w*im.h + row*im.w + col];
            gray.data[row*im.w + col] = w_sum;
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int row = 0; row < im.h; row++) {
        for (int col = 0; col < im.w; col++) {
            im.data[c*im.w*im.h + row*im.w + col] += v;
        }
    }
}

void scale_image(image im, int c, float v)
{
    for (int row = 0; row < im.h; row++) {
        for (int col = 0; col < im.w; col++) {
            im.data[c*im.w*im.h + row*im.w + col] *= v;
        }
    }
}

void clamp_image(image im)
{
    for (int cha = 0; cha < im.c; cha++) {
        for (int row = 0; row < im.h; row++) {
            for (int col = 0; col < im.w; col++) {
                float *pixel = &(im.data[cha*im.w*im.h + row*im.w + col]);
                if (*pixel > 1) {
                    *pixel = 1;
                } else if (*pixel < 0) {
                    *pixel = 0;
                }
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    for (int row = 0; row < im.h; row++) {
        for (int col = 0; col < im.w; col++) {
            float *r = &(im.data[row*im.w + col]);
            float *g = &(im.data[im.w*im.h + row*im.w + col]);
            float *b = &(im.data[2*im.w*im.h + row*im.w + col]);

            float value = three_way_max(*r, *g, *b);
            float chroma = value-three_way_min(*r, *g, *b);

            float saturation;
            if (value == 0) {
                saturation = 0;
            } else {
                saturation = (chroma / value);
            }

            float hue;
            if (chroma == 0) {
                hue = 0;
            } else if (value == *r) {
                hue = (*g-*b)/chroma;
            } else if (value == *g) {
                hue = (*b-*r)/chroma + 2;
            } else if (value == *b) {
                hue = (*r-*g)/chroma + 4;
            }
            if (hue < 0) {
                hue = hue/6 + 1;
            } else {
                hue = hue/6;
            }

            *r = hue;
            *g = saturation;
            *b = value;
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int row = 0; row < im.h; row++) {
        for (int col = 0; col < im.w; col++) {
            float *h = &(im.data[row*im.w + col]);
            float *s = &(im.data[im.w*im.h + row*im.w + col]);
            float *v = &(im.data[2*im.w*im.h + row*im.w + col]);

            float chroma = (*v) * (*s);
            float min = *v - chroma;
            float x = chroma * (1.0-fabsf(fmodf(*h*6,2.0)-1.0));

            // printf("%f\n", *h);
            *h *= 6;
            if (*h >= 0 && *h <= 6) {
                if (*h <= 1) {
                    *h = chroma;
                    *s = x;
                    *v = 0;
                } else if (*h <= 2) {
                    *h = x;
                    *s = chroma;
                    *v = 0;
                } else if (*h <= 3) {
                    *h = 0;
                    *s = chroma;
                    *v = x;
                } else if (*h <= 4) {
                    *h = 0;
                    *s = x;
                    *v = chroma;
                } else if (*h <= 5) {
                    *h = x;
                    *s = 0;
                    *v = chroma;
                } else {
                    *h = chroma;
                    *s = 0;
                    *v = x;
                }
            } else {
                *h = 0;
                *s = 0;
                *v = 0;
            }

            *h += min;
            *s += min;
            *v += min;
        }
    }
}

