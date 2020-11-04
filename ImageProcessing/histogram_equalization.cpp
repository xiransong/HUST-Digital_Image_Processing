#include "bmp.h"
const int L = 256;

BMP* histogram_equalization(BMP* p){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(p->width, p->height);

    double total_pixel_num = p->width * p->height;
    int n[L];
    for(int i=0; i<L; i++) n[i] = 0;  //灰度等级

    for(int y=0; y<p->height; y++){
        for(int x=0; x<p->width; x++){
            int g = p->a[y][x];
            n[g]++;  //统计各灰度等级像素个数
        }
    }

    double h_cumulate[L];
    h_cumulate[0] = n[0] / total_pixel_num;  //计算累计直方图
    for(int i=1; i<L; i++){
        h_cumulate[i] = h_cumulate[i-1] + n[i] / total_pixel_num;
    }

    BYTE gray_new[L];
    for(int i=0; i<L; i++){  //计算变换后的灰度值
        gray_new[i] = int((L-1)*h_cumulate[i] + 0.5);
    }

    for(int y=0; y<pnew->height; y++){
        for(int x=0; x<pnew->width; x++){
            int g = p->a[y][x];
            pnew->a[y][x] = gray_new[g];  //按照灰度对应关系生成图像
        }
    }

    return pnew;
}
