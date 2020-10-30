#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include "ImageProcessing.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->bmp = new BMP;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->bmp;
}

int MainWindow::renew_current_image()
{
    QString tmp_file = this->file_root + "\\tmp.bmp";
    int re = bmp->save_as(tmp_file);

    QImage img(tmp_file);
    this->ui->label_img_test->resize(img.width(),img.height());
    this->ui->label_img_test->setPixmap(QPixmap::fromImage(img));

    return 0;
}

void MainWindow::on_bt_set_file_root_clicked()
{
    this->file_root = this->ui->et_file_root->text();
    qDebug() << "file_root:" << this->file_root;
}

void MainWindow::on_bt_open_clicked()
{
    QString filename = this->ui->et_open->text();

    int re = bmp->open(this->file_root + "\\" + filename);

    if(-1 == re){
        QMessageBox::warning(NULL, "Error", "文件打开失败！");
        return;
    }else if(-2 == re){
        QMessageBox::warning(NULL, "Error", "不是BMP文件！");
        return;
    }else if(-3 == re){
        QMessageBox::warning(NULL, "Error", "仅支持8或24 bits per pixel！");
        return;
    }

    renew_current_image();
}

void MainWindow::on_bt_save_clicked()
{
    QString filename = this->ui->et_save->text();
    if(0 != bmp->save_as(this->file_root + "\\" + filename)){
        QMessageBox::warning(NULL, "Error", "文件保存失败！");
    }
}

void MainWindow::on_bt_img_info_clicked()
{
    QString str;
    BITMAPFILEHEADER& fh = bmp->bmpHead;       //位图文件头
    BITMAPINFOHEADER& ih = bmp->bmpInforHead;  //位图信息头
    str += "bfType (file type) = " + QString().setNum(fh.bfType, 16);
    str += "\nbfSize (file length) = " + QString().setNum(fh.bfSize);
    str += "\nbfOffBits (offset of bitmap data in bytes) = " + QString().setNum(fh.bfOffBits);
    str += "\nbiSize (header structure length should be 40 or 0x28) = " + QString().setNum(ih.biSize);
    str += "\nbiWidth (image width) = " + QString().setNum(ih.biWidth);
    str += "\nbiHeight (image height) = " + QString().setNum(ih.biHeight);
    str += "\nbiPlanes (must equal to 1) = " + QString().setNum(ih.biPlanes);
    str += "\nbiBitCount (color/pixel bits) = " + QString().setNum(ih.biBitCount);
    str += "\nbiCompression (compressed?) = " + QString().setNum(ih.biCompression);
    str += "\nbiSizeImage (length of bitmap data in bytes must be the times of 4) = " + QString().setNum(ih.biSizeImage);
    str += "\nbiXPelsPerMeter (horizontal resolution of target device in pixels/metre) = " + QString().setNum(ih.biXPelsPerMeter);
    str += "\nbiYPelsPerMeter (vertical resolution of target device in pixels/metre) = " + QString().setNum(ih.biYPelsPerMeter);
    str += "\nbiClrUsed (number of colors used in bitmap) = " + QString().setNum(ih.biClrUsed);
    str += "\nbiClrImportant (number of important colors) = " + QString().setNum(ih.biClrImportant);
    str += "\n\nThe following is additional infomation:";
    str += "\nBytes per row in bitmap = " + QString().setNum(bmp->bytesPerLine);
    str += "\nTotal bytes of bitmap = " + QString().setNum(bmp->height * bmp->bytesPerLine);
    str += "\nActual pixels per raw in bitmap = " + QString().setNum(ih.biWidth);
    str += "\nTotal rows of bitmap = " + QString().setNum(ih.biHeight);
    str += "\nTotal colors = " + QString().setNum(1 << ih.biBitCount);
    str += "\nUsed colors = " + QString().setNum(ih.biClrUsed);
    QMessageBox::information(NULL, "Info", str);
}

void MainWindow::on_bt_get_pixel_clicked()
{
    if((bmp->bitsPerPixel == 8 && bmp->data_gray == NULL) ||
       (bmp->bitsPerPixel == 24 && bmp->data_bgr == NULL)){
        QMessageBox::warning(NULL, "Error", "未打开图片！");
        return;
    }

    int x = this->ui->et_x->text().toInt();
    int y = this->ui->et_y->text().toInt();
    if(!(0<=x && x<=bmp->width && 0<=y && y<=bmp->height)){
        QMessageBox::warning(NULL, "Error", "Invalid coordinate!");
        return;
    }

    if(bmp->bitsPerPixel == 8){
        BYTE gray = bmp->a[x][y];

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", gray, gray, gray);
        this->ui->label_show_color->setStyleSheet(qss);

        this->ui->et_gray->setText(QString().setNum(gray));

    }else /*if(rawbmp.bitsPerPixel == 24)*/{
        RGBQUAD rgb = bmp->b[x][y];

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);
        this->ui->label_show_color->setStyleSheet(qss);

        this->ui->et_b->setText(QString().setNum(rgb.rgbBlue));
        this->ui->et_g->setText(QString().setNum(rgb.rgbGreen));
        this->ui->et_r->setText(QString().setNum(rgb.rgbRed));
    }
}

void MainWindow::on_bt_set_pixel_clicked()
{
    if((bmp->bitsPerPixel == 8 && bmp->data_gray == NULL) ||
       (bmp->bitsPerPixel == 24 && bmp->data_bgr == NULL)){
        QMessageBox::warning(NULL, "Error", "未打开图片！");
        return;
    }

    int x = this->ui->et_x->text().toInt();
    int y = this->ui->et_y->text().toInt();
    if(!(0<=x && x<=bmp->width && 0<=y && y<=bmp->height)){
        QMessageBox::warning(NULL, "Error", "Invalid coordinate!");
        return;
    }

    if(bmp->bitsPerPixel == 8){
        BYTE gray = this->ui->et_gray->text().toInt();
        if(!(0<=gray && gray<=255)){
            QMessageBox::warning(NULL, "Error", "Invalid gray scale!");
            return;
        }

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", gray, gray, gray);
        this->ui->label_show_color->setStyleSheet(qss);

        bmp->a[y][x] = gray;

    }else /*if(rawbmp->bitsPerPixel == 24)*/{
        BYTE b = this->ui->et_b->text().toInt();
        BYTE g = this->ui->et_g->text().toInt();
        BYTE r = this->ui->et_r->text().toInt();
        if(!(0<=b && b<=255 && 0<=g && g<=255 && 0<=r && r<=255)){
            QMessageBox::warning(NULL, "Error", "Invalid BGR scale!");
            return;
        }

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", r, g, b);
        this->ui->label_show_color->setStyleSheet(qss);

        RGBQUAD& rgb = bmp->b[y][x];
        rgb.rgbRed = r;
        rgb.rgbGreen = g;
        rgb.rgbBlue = b;
    }

    renew_current_image();
}

void MainWindow::on_bt_nn_scaling_clicked()
{
    double sx = this->ui->et_sx->text().toDouble();
    double sy = this->ui->et_sy->text().toDouble();

    //TODO: 检查输入合法性

    BMP* pnew = image_scaling_nearest_neighbor(this->bmp, sx, sy);

    qDebug() << "bt" << "ok1";

    delete this->bmp;
    qDebug() << "bt" << "ok2";
    this->bmp = pnew;

    this->renew_current_image();
    qDebug() << "bt" << "ok3";
}
