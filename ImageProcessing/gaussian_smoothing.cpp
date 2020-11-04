#include "bmp.h"
#include <cmath>
#include <QDebug>
#include <ImageProcessing.h>

void get_gaussian_kernel(double sigma, double* kernel, int radius){
    const double pi = 3.1415926;
    double x, y;
    int i = 0;
    for (int x = -radius; x <= radius; x++){
        int _x = x * x;
        for (int y = -radius; y <= radius; y++){
            int _y = y * y;
            double g = exp(-(_x + _y) / (2 * sigma * sigma));
            g /= 2 * pi * sigma * sigma;
            kernel[i] = g;
            i++;
        }
    }
}

BMP* gaussian_smoothing(BMP* p, double sigma){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(p->width, p->height);

    int width = (int)ceil(sigma*6);
    if((width + 1)%2) width += 1;  //窗口宽度取奇数
    int radius = width / 2;

    double* kernel = new double[width*width];
    get_gaussian_kernel(sigma, kernel, radius);

    BYTE* s = new BYTE[width*width];
    for(int y=0; y<p->height; y++){
        for(int x=0; x<p->width; x++){
            square_sampling(s, p, radius, x, y);
            double sum = 0;
            for(int i=0; i<width*width; i++){
                sum += kernel[i] * s[i];
            }
            pnew->a[y][x] = (BYTE)sum;
        }
    }

    delete[] kernel;
    delete[] s;
    return pnew;
}
