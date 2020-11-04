#include "bmp.h"
#include <vector>
using namespace std;

void square_sampling(BYTE* s, BMP* p, int radius, int center_x, int center_y){
    int i=0;
    for(int y = center_y - radius; y <= center_y + radius; y++){
        for(int x = center_x - radius; x <= center_x + radius; x++){
            int _x = x, _y = y;
            if(_x < 0 || _x >= p->width) _x = 2*center_x - _x;  //对于超出图像边界的点，取其关于采样中心的对称点
            if(_y < 0 || _y >= p->height) _y = 2*center_y - _y;
            s[i] = p->a[_y][_x];
            i++;
        }
    }
}
