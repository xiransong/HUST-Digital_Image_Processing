#include "bmp.h"
#include "ImageProcessing.h"
#include <cmath>

BMP* sharpening_sobel(BMP* p, double k1, double k2){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(p->width, p->height);

    int kernel_x[9] = {-1,0,1,-2,0,2,-1,0,1};
    int kernel_y[9] = {-1,-2,-1,0,0,0,1,2,1};
    BYTE s[9];
    for(int y=0; y<p->height; y++){
        for(int x=0; x<p->width; x++){
            square_sampling(s, p, 1, x, y);
            int grad_x=0, grad_y=0;
            for(int i=0; i<9; i++){
                grad_x += kernel_x[i] * s[i];
                grad_y += kernel_y[i] * s[i];
            }
            BYTE g = sqrt(grad_x*grad_x + grad_y*grad_y);
            pnew->a[y][x] = k1 * p->a[y][x] + k2 * g;
        }
    }

    return pnew;
}
