#include "bmp.h"
#include <stdlib.h>
#include <time.h>

const int N = 2048;
bool flag[N];

BMP* impulsive_noise_salt(BMP* p, double rate){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(p->width, p->height);

    for(int i=0; i<N*rate; i++) flag[i] = true;
    for(int i=N*rate; i<N; i++) flag[i] = false;

    for(int y=0; y<p->height; y++){
        for(int x=0; x<p->width; x++){
            if(flag[rand() % N]){  // noise rate
                pnew->a[y][x] = rand()%2 ? 0 : 255;  // salt noise
            }else{
                pnew->a[y][x] = p->a[y][x];
            }
        }
    }

    return pnew;
}

BMP* impulsive_noise_uniform(BMP* p, double rate){
    BMP* pnew = new BMP;
    pnew->create_gray_bmp(p->width, p->height);

    for(int i=0; i<N*rate; i++) flag[i] = true;
    for(int i=N*rate; i<N; i++) flag[i] = false;

    for(int y=0; y<p->height; y++){
        for(int x=0; x<p->width; x++){
            if(flag[rand() % N]){  // noise rate
                pnew->a[y][x] = rand()%256;  // uniform noise
            }else{
                pnew->a[y][x] = p->a[y][x];
            }
        }
    }

    return pnew;
}
