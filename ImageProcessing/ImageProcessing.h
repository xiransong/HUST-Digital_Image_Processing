#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include "bmp.h"
#include <vector>
using namespace std;

void square_sampling(BYTE* s, BMP* p, int radius, int center_x, int center_y);

BMP* image_scaling_nearest_neighbor(BMP* p, double sx, double sy);
BMP* image_scaling_bilinear(BMP* p, double sx, double sy);
BMP* median_filtering_3x3(BMP* p);
BMP* gaussian_smoothing(BMP* p, double sigma);

BMP* histogram_equalization(BMP* p);
BMP* sharpening_sobel(BMP* p, double k1, double k2);
BMP* impulsive_noise_salt(BMP* p, double rate);
BMP* impulsive_noise_uniform(BMP* p, double rate);

#endif // IMAGEPROCESSING_H
