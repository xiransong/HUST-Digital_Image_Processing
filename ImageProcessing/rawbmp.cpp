#include "rawbmp.h"
#include <cstdio>
#include <QDebug>

RawBMP::RawBMP()
{
    this->bmpHead.bfType = 0;
    this->data = NULL;
}

int RawBMP::load_img(QString filename)
{
    this->filename = filename;
    FILE* fp;
    fp = fopen(this->filename.toLatin1().data(), "rb");
    if(fp == NULL){
        return -1;  //文件打开失败
    }

    //读取bmp文件的文件头和信息头
    fread(&this->bmpHead, 1, sizeof(BITMAPFILEHEADER), fp);
    fread(&this->bmpInforHead, 1, sizeof(BITMAPINFOHEADER), fp);
    if(this->bmpHead.bfType != 0x4D42){
        fclose(fp);
        return -2;  //不是BMP文件类型
    }

    this->bitsPerPixel = bmpInforHead.biBitCount;
    if(bitsPerPixel == 8){
        //this->get_pixel = this->get_gray_pixel;
    }else if(bitsPerPixel == 24){

    }else{
        fclose(fp);
        return -3;  //仅支持8或24 bitsPerPixel
    }

    //读取调色板
    for(unsigned int nCounti=0; nCounti<this->bmpInforHead.biClrUsed; nCounti++)
    {
        fread(&(this->rgbQuad[nCounti].rgbBlue), 1, sizeof(BYTE), fp);
        fread(&(this->rgbQuad[nCounti].rgbGreen), 1, sizeof(BYTE), fp);
        fread(&(this->rgbQuad[nCounti].rgbRed), 1, sizeof(BYTE), fp);
        fread(&(this->rgbQuad[nCounti].rgbReserved), 1, sizeof(BYTE), fp);
    }

    //读取图像数据
    this->width = bmpInforHead.biWidth;
    this->height = bmpInforHead.biHeight;

    if(this->data != NULL) delete this->data;
    this->data = new unsigned char[width*height*bitsPerPixel/8];

    unsigned int lineByteCnt = (((width * bitsPerPixel) + 31) >> 5) << 2;
    this->bytesPerLine = lineByteCnt;
    this->skip = 4 - ((width * bitsPerPixel)>>3) & 3;  //读完一行数据后需要跳过的字节数

    unsigned int cnt = 0;
    for(unsigned int i=0; i<height; i++){
        cnt += fread(this->data + cnt, 1, lineByteCnt, fp);
        fseek(fp, skip, SEEK_CUR);
    }
    fclose(fp);
    return 0;
}

int RawBMP::save_img(QString new_filename)
{
    FILE* fp;
    fp = fopen(new_filename.toLatin1().data(), "wb");
    if(fp == NULL){
        return -1;
    }
    fwrite(&(this->bmpHead), sizeof(BITMAPFILEHEADER), 1, fp);
    fwrite(&(this->bmpInforHead), sizeof(BITMAPINFOHEADER), 1, fp);
    for(unsigned int nCounti=0; nCounti<this->bmpInforHead.biClrUsed; nCounti++)
    {
        fwrite(&(this->rgbQuad[nCounti].rgbBlue), 1, sizeof(BYTE), fp);
        fwrite(&(this->rgbQuad[nCounti].rgbGreen), 1, sizeof(BYTE), fp);
        fwrite(&(this->rgbQuad[nCounti].rgbRed), 1, sizeof(BYTE), fp);
        fwrite(&(this->rgbQuad[nCounti].rgbReserved), 1, sizeof(BYTE), fp);
    }
    unsigned int cnt = 0;
    for(unsigned int i=0; i<height; i++){
        cnt += fwrite(this->data + cnt, 1, bytesPerLine, fp);
        fwrite("\0", 1, skip, fp);
    }
    fclose(fp);
    return 0;
}

Pixel *RawBMP::get_gray_pixel(int x, int y)
{
    return (Pixel*)(this->data + y * this->width + x);
}

Pixel *RawBMP::get_rgb_pixel(int x, int y)
{
    return (Pixel*)(this->data + y * 3 * this->width + x * 3);
}
