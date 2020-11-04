#include "bmp.h"
#include <QDebug>
#include <ImageProcessing.h>

BMP* median_filtering_3x3(BMP* p){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(p->width, p->height);
    BYTE* s = new BYTE[9];

    for(int y=0; y<p->height; y++){
        for(int x=0; x<p->width; x++){
            square_sampling(s, p, 1, x, y);
            sort(s, s+9);
            pnew->a[y][x] = s[4];
        }
    }

    delete[] s;
    return pnew;
}
