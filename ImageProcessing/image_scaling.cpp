#include "bmp.h"
#include <cmath>
#include <QDebug>

BMP* image_scaling_nearest_neighbor(BMP* p, double sx, double sy){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(int(sx * p->width), int(sy * p->height));

    double sx_reci = 1/sx;
    double sy_reci = 1/sy;

    qDebug() << "ok1" << int(sx * p->width) << int(sy * p->height) << sx_reci << sy_reci;

    for(int y=0; y<pnew->height; y++){
        for(int x=0; x<pnew->width; x++){
            int _x = (int)(sx_reci * x);
            int _y = (int)(sy_reci * y);
            pnew->a[y][x] = p->a[_y][_x];
        }
    }

    qDebug() << "ok2";

    return pnew;
}
