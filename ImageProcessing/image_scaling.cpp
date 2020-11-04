#include "bmp.h"
#include <cmath>
#include <QDebug>

BMP* image_scaling_nearest_neighbor(BMP* p, double sx, double sy){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(int(sx * p->width), int(sy * p->height));

    double sx_reci = 1/sx;
    double sy_reci = 1/sy;
    for(int y=0; y<pnew->height; y++){
        for(int x=0; x<pnew->width; x++){
            int _x = (int)(sx_reci * x);
            int _y = (int)(sy_reci * y);
            pnew->a[y][x] = p->a[_y][_x];
        }
    }

    return pnew;
}

BMP* image_scaling_bilinear(BMP* p, double sx, double sy){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(int(sx * p->width), int(sy * p->height));

    double sx_reci = 1/sx;
    double sy_reci = 1/sy;
    for(int y=0; y<pnew->height; y++){
        for(int x=0; x<pnew->width; x++){
            double _x = sx_reci * x;
            double _y = sy_reci * y;
            int x_floor = (int)floor(_x);
            int x_ceil = (int)ceil(_x);
            if(x_ceil > p->width) x_ceil = x_floor;
            int y_floor = (int)floor(_y);
            int y_ceil = (int)ceil(_y);
            if(y_ceil > p->height) y_ceil = y_floor;
            BYTE g1 = p->a[y_floor][x_floor];
            BYTE g2 = p->a[y_ceil][x_floor];
            BYTE g3 = p->a[y_floor][x_ceil];
            BYTE g4 = p->a[y_ceil][x_ceil];
            double dx = _x - x_floor;
            double dy = _y - y_floor;
            double ga = g1 + dy*(g2 - g1);
            double gb = g3 + dy*(g4 - g3);
            double g = ga + dx*(gb - ga);
            pnew->a[y][x] = g;
        }
    }

    return pnew;
}
