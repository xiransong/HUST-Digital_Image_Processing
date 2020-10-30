#include "bmp.h"
#include <QDebug>

BMP::BMP()
{
    this->bmpHead.bfType = 0;
    this->rgbQuad = NULL;
    this->a = NULL;
    this->b = NULL;
    this->data_gray = NULL;
    this->data_bgr = NULL;
}

BMP::~BMP()
{
    clean();
}

void BMP::clean()
{
    this->bmpHead.bfType = 0;

    if(this->rgbQuad != NULL) delete this->rgbQuad;

    if(this->bitsPerPixel == 8){
        delete this->data_gray;
        delete this->a;
    }else /*if(this->bitsPerPixel == 24)*/{
        delete this->data_bgr;
        delete this->b;
    }
}

int BMP::open(QString filename)
{
    FILE* fp = fopen(filename.toLatin1().data(), "rb");
    if(fp == NULL){
        return -1;  //文件打开失败
    }

    //读取bmp文件的文件头和信息头
    fread(&this->bmpHead, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&this->bmpInforHead, sizeof(BITMAPINFOHEADER), 1, fp);
    if(this->bmpHead.bfType != 0x4D42){
        fclose(fp);
        return -2;  //不是BMP文件类型
    }

    this->bitsPerPixel = bmpInforHead.biBitCount;
    if(bitsPerPixel != 8 && bitsPerPixel != 24){
        fclose(fp);
        return -3;  //仅支持8或24 bitsPerPixel
    }

    //读取调色板
    if(this->bitsPerPixel == 8){
        this->rgbQuad = new RGBQUAD[256];
        fread(this->rgbQuad, sizeof(RGBQUAD), 256, fp);
    }

    //读取图像数据
    this->width = bmpInforHead.biWidth;
    this->height = bmpInforHead.biHeight;
    this->bytesPerLine = (((width * bitsPerPixel) + 31) >> 5) << 2;
    this->bytesSkip = 4 - ((width * bitsPerPixel)>>3) & 3;

    if(this->bitsPerPixel == 8){
        this->data_gray = new BYTE[width * height];
        this->a = new BYTE*[height];
        for(int i=0; i<height; i++){
            a[i] = data_gray + i * width;
            fread(a[i], sizeof(BYTE), width, fp);
            fseek(fp, bytesSkip, SEEK_CUR);
        }
    }else /*if(this->bitsPerPixel == 24)*/{
        this->data_bgr = new RGBQUAD[width * height];
        this->b = new RGBQUAD*[height];
        for(int i=0; i<height; i++){
            b[i] = data_bgr + i * width;
            fread(b[i], sizeof(RGBQUAD), width, fp);
            fseek(fp, bytesSkip, SEEK_CUR);
        }
    }

    fclose(fp);
    return 0;
}

int BMP::save_as(QString filename)
{
    FILE* fp = fopen(filename.toLatin1().data(), "wb");
    if(fp == NULL){
        return -1;  //文件打开失败
    }

    fwrite(&this->bmpHead, sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&this->bmpInforHead, sizeof(BITMAPINFOHEADER), 1, fp);

    if(this->bitsPerPixel == 8){
        fwrite(this->rgbQuad, sizeof(RGBQUAD), 256, fp);
    }

    if(this->bitsPerPixel == 8){
        for(int i=0; i<height; i++){
            fwrite(a[i], sizeof(BYTE), width, fp);
            for(int j=0; j<bytesSkip; j++) fwrite("\0", sizeof(BYTE), 1, fp);
        }
    }else /*if(this->bitsPerPixel == 24)*/{
        for(int i=0; i<height; i++){
            fwrite(b[i], sizeof(RGBQUAD), width, fp);
            for(int j=0; j<bytesSkip; j++) fwrite("\0", sizeof(BYTE), 1, fp);
        }
    }

    fclose(fp);
    return 0;
}

int BMP::create_gray_bmp(int width, int height)
{
    this->width = width;
    this->height = height;
    this->bitsPerPixel = 8;
    this->bytesPerLine = (((width * bitsPerPixel) + 31) >> 5) << 2;
    this->bytesSkip = 4 - ((width * bitsPerPixel)>>3) & 3;

    this->bmpHead.bfType = 0x4D42;
    this->bmpHead.bfSize = 1078 + bytesPerLine * height;
    this->bmpHead.bfReserved1 = 0;
    this->bmpHead.bfReserved2 = 0;
    this->bmpHead.bfOffBits = 1078;  //54 + 256*4 = 1078

    this->bmpInforHead.biSize = 40;
    this->bmpInforHead.biWidth = width;
    this->bmpInforHead.biHeight = height;
    this->bmpInforHead.biPlanes = 1;
    this->bmpInforHead.biBitCount = 8;
    this->bmpInforHead.biCompression = 0;
    this->bmpInforHead.biSizeImage = bytesPerLine * height;
    this->bmpInforHead.biXPelsPerMeter = 0;
    this->bmpInforHead.biYPelsPerMeter = 0;
    this->bmpInforHead.biClrUsed = 256;
    this->bmpInforHead.biClrImportant = 256;

    this->rgbQuad = new RGBQUAD[256];
    for(int i=0; i<256; i++){
        rgbQuad[i].rgbRed = rgbQuad[i].rgbGreen = rgbQuad[i].rgbBlue = i;
        rgbQuad[i].rgbReserved = 0;
    }

    this->data_gray = new BYTE[width * height];
    this->a = new BYTE*[height];
    for(int i=0; i<height; i++){
        a[i] = data_gray + i * width;
        for(int j=0; j<width; j++){
            a[i][j] = 150;  //initiate gray scale
        }
    }

    return 0;
}
