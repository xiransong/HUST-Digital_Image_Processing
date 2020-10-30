#ifndef BMP_H
#define BMP_H
#include <windows.h>
#include <QString>

class BMP
{
public:
    BMP();
    ~BMP();
    int open(QString filename);
    int save_as(QString filename);
    int create_gray_bmp(int width, int height);  //创建灰度图
    void clean();  //释放数据缓存，复位

    BITMAPFILEHEADER bmpHead;       //位图文件头
    BITMAPINFOHEADER bmpInforHead;  //位图信息头
    RGBQUAD* rgbQuad;               //调色板

    int width, height;
    int bitsPerPixel;
    int bytesPerLine;  //每行实际存储的字节数
    int bytesSkip;     //每行填充的字节数

    BYTE* data_gray;    //图像数据
    RGBQUAD* data_bgr;

    BYTE** a;     //图像数据矩阵
    RGBQUAD** b;
};

#endif // BMP_H
