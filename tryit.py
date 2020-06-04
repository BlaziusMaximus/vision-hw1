from uwimg import *

# nearest neighbor
# im = load_image("data/dogsmall.jpg")
# a = nn_resize(im, im.w*4, im.h*4)
# save_image(a, "dog_test_4x")

# bilinear
# im = load_image("data/dog.jpg")
# a = bilinear_resize(im, im.w//7, im.h//7)
# save_image(a, "dog_test_bi7")

# convolution
# im = load_image("data/dog.jpg")
# f = make_box_filter(7)
# blur = convolve_image(im, f, 1)
# save_image(blur, "dog-box7")

# box filter resize
# im = load_image("data/dog.jpg")
# f = make_box_filter(7)
# blur = convolve_image(im, f, 1)
# thumb = nn_resize(blur, blur.w//7, blur.h//7)
# save_image(thumb, "dogthumb")

# emboss
# im = load_image("data/dog.jpg")
# f = make_sharpen_filter()
# em = convolve_image(im, f, 1)
# save_image(em, "dog_sh")

# gaussian
# im = load_image("data/dog.jpg")
# f = make_gaussian_filter(2)
# blur = convolve_image(im, f, 1)
# save_image(blur, "dog-gauss2")

# frequency
# im = load_image("data/dog.jpg")
# f = make_gaussian_filter(2)
# lfreq = convolve_image(im, f, 1)
# hfreq = sub_image(im, lfreq)
# reconstruct = add_image(lfreq, hfreq)
# save_image(lfreq, "low-frequency")
# save_image(hfreq, "high-frequency")
# save_image(reconstruct, "reconstruct")

# sobel
# im = load_image("data/dog.jpg")
# res = sobel_image(im)
# mag = res[1]
# feature_normalize(mag)
# save_image(mag, "direction")

# colorized sobel
im = load_image("data/dog.jpg")
co = colorize_sobel(im)
save_image(co, "color_sobel")