#include <windows.h>
#include <QString>
#include <stdio.h>

void read_bmp(const QString& filename){
    BITMAPFILEHEADER pBmpHead;
    BITMAPINFOHEADER pBmpInforHead;
    FILE* fp;
    fp = fopen(filename.toLatin1().data(),"rb");

}
