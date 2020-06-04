#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    for (int cha = 0; cha < im.c; cha++) {
        for (int row = 0; row < im.h; row++) {
            for (int col = 0; col < im.w; col++) {
                set_pixel(im, col, row, cha, get_pixel(im, col, row, cha)/(float)(im.h*im.w));
            }
        }
    }
}

image make_box_filter(int w)
{
    image filter = make_image(w,w,1);
    for (int row = 0; row < w; row++) {
        for (int col = 0; col < w; col++) {
            set_pixel(filter, col, row, 0, 1.0);
        }
    }
    l1_normalize(filter);
    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(im.c==filter.c || filter.c==1);

    image convolved = make_image(im.w, im.h, preserve==1?im.c:1);

    for (int cha = 0; cha < im.c; cha++) {
        int f_cha = (filter.c==1)?0:cha;
        for (int row = 0; row < im.h; row++) {
            for (int col = 0; col < im.w; col++) {
                float kernel = 0;
                float fp, ip;
                for (int y = 0; y < filter.h; y++) {
                    for (int x = 0; x < filter.w; x++) {
                        int real_x = col-filter.w/2+x; int real_y = row-filter.h/2+y;
                        if (real_x < 0) real_x = 0; if (real_y < 0) real_y = 0;
                        if (real_x >= im.w) real_x = im.w-1; if (real_y >= im.h) real_y = im.h-1;
                        
                        fp = get_pixel(filter, x, y, f_cha);
                        ip = get_pixel(im, real_x, real_y, cha);
                        kernel += fp*ip;
                    }
                }
                // if (col == 0 || row == 0 || col == im.w-1 || row == im.h-1) kernel = 0;
                kernel = kernel<0?0:(kernel>1?1:kernel);
                // if (col == 0 && row < 100) printf("%f-%f\n", kernel, get_pixel(im, col, row, cha));
                set_pixel(convolved, col, row, (preserve==1?cha:0), kernel);
            }
        }
    }
    return convolved;
}

image make_highpass_filter()
{
    image filter = make_image(3,3,1);
    float data[9] = { 0, -1, 0, -1, 4, -1, 0, -1, 0 };
    memcpy(filter.data, data, sizeof(data));
    return filter;
}

image make_sharpen_filter()
{
    image filter = make_image(3,3,1);
    float data[9] = { 0, -1, 0, -1, 5, -1, 0, -1, 0 };
    memcpy(filter.data, data, sizeof(data));
    return filter;
}

image make_emboss_filter()
{
    image filter = make_image(3,3,1);
    float data[9] = { -2, -1, 0, -1, 1, 1, 0, 1, 2 };
    memcpy(filter.data, data, sizeof(data));
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    int size = 6*sigma + ((int)(6*sigma)%2==0 ? 1 : 0);
    image filter = make_box_filter(size);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            set_pixel(filter, x, y, 0, powf(M_E, -1.0*(powf(x-3*sigma,2)+powf(y-3*sigma,2))/(2*powf(sigma,2)))/(2*M_PI*powf(sigma,2)));
        }
    }
    // l1_normalize(filter);

    return filter;
}

image add_image(image a, image b)
{
    assert(a.w==b.w && a.h==b.h && a.c==b.c);
    
    image sum = make_image(a.w,a.h,a.c);
    
    for (int cha = 0; cha < a.c; cha++) {
        for (int row = 0; row < a.h; row++) {
            for (int col = 0; col < a.w; col++) {
                set_pixel(sum, col, row, cha, get_pixel(a, col, row, cha)+get_pixel(b, col, row, cha));
            }
        }
    }
    // clamp_image(sum);
    
    return sum;
}

image sub_image(image a, image b)
{
    assert(a.w==b.w && a.h==b.h && a.c==b.c);
    
    image sum = make_image(a.w,a.h,a.c);
    
    for (int cha = 0; cha < a.c; cha++) {
        for (int row = 0; row < a.h; row++) {
            for (int col = 0; col < a.w; col++) {
                set_pixel(sum, col, row, cha, get_pixel(a, col, row, cha)-get_pixel(b, col, row, cha));
            }
        }
    }
    // clamp_image(sum);
    
    return sum;
}

image make_gx_filter()
{
    image filter = make_image(3,3,1);
    float data[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    memcpy(filter.data, data, sizeof(data));
    return filter;
}

image make_gy_filter()
{
    image filter = make_image(3,3,1);
    float data[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
    memcpy(filter.data, data, sizeof(data));
    return filter;
}

void feature_normalize(image im)
{
    for (int cha = 0; cha < im.c; cha++) {
        float min = 1.0; float max = 0.0;
        for (int row = 0; row < im.h; row++) {
            for (int col = 0; col < im.w; col++) {
                float px = get_pixel(im, col, row, cha);
                if (px < min) min = px;
                if (px > max) max = px;
            }
        }
        for (int row = 0; row < im.h; row++) {
            for (int col = 0; col < im.w; col++) {
                if (max-min > 0) {
                    set_pixel(im, col, row, cha, (get_pixel(im, col, row, cha)-min)/(max-min));
                } else {
                    set_pixel(im, col, row, cha, 0);
                }
            }
        }
    }
}

image *sobel_image(image im)
{
    im = rgb_to_grayscale(im);
    image g_x = convolve_image(im, make_gx_filter(), 1);
    image g_y = convolve_image(im, make_gy_filter(), 1);
    image* sobel_image = calloc(2, sizeof(image));
    sobel_image[0] = make_image(im.w,im.h,1);
    sobel_image[1] = make_image(im.w,im.h,1);

    for (int row = 0; row < im.h; row++) {
        for (int col = 0; col < im.w; col++) {
            float gx_px = get_pixel(g_x, col, row, 0);
            float gy_px = get_pixel(g_y, col, row, 0);
            set_pixel(sobel_image[0], col, row, 0, sqrtf(gx_px*gx_px+gy_px*gy_px));
            set_pixel(sobel_image[1], col, row, 0, atan2f(gy_px,gx_px));
        }
    }

    return sobel_image;
}

image colorize_sobel(image im)
{
    image* sobel = sobel_image(im);
    image mag = sobel[0];
    feature_normalize(mag);
    image dir = sobel[1];
    feature_normalize(dir);

    rgb_to_hsv(im);
    for (int row = 0; row < im.h; row++) {
        for (int col = 0; col < im.w; col++) {
            float mag_px = get_pixel(mag, col, row, 0);
            float dir_px = get_pixel(dir, col, row, 0);
            set_pixel(im, col, row, 0, dir_px);
            set_pixel(im, col, row, 1, mag_px);
            set_pixel(im, col, row, 2, mag_px);
        }
    }
    hsv_to_rgb(im);

    return im;
}
