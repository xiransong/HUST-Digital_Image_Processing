#ifndef RAWBMP_H
#define RAWBMP_H
#include <QString>
#include <windows.h>

struct Pixel{
    union{
        BYTE b;
        BYTE gray;
    };
    BYTE g;
    BYTE r;

    Pixel(){}
    Pixel(BYTE _b, BYTE _g, BYTE _r):b(_b),g(_g),r(_r){}
    Pixel(BYTE _gray):gray(_gray){}
};

class RawBMP
{
public:
    QString filename;  //图像文件名
    BITMAPFILEHEADER bmpHead;  //位图文件头
    BITMAPINFOHEADER bmpInforHead;  //位图信息头
    RGBQUAD rgbQuad[256];  //调色板

    unsigned int width;   //图像宽度
    unsigned int height;  //图像高度
    unsigned int bitsPerPixel;  //每像素的比特数（仅支持8或24）
    unsigned int bytesPerLine;
    unsigned int skip;
    unsigned char* data;  //图像数据


public:
    RawBMP();
    int load_img(QString filename);
    int save_img(QString new_filename);
    Pixel* get_gray_pixel(int x, int y);
    Pixel* get_rgb_pixel(int x, int y);
};

#endif // RAWBMP_H
