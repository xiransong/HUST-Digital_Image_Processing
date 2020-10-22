#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include "rawbmp.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::renew_current_image()
{
    QString tmp_file = this->file_root + "\\tmp.bmp";
    rawbmp.save_img(tmp_file);
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

    int re = rawbmp.load_img(this->file_root + "\\" + filename);

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
    if(0 != rawbmp.save_img(this->file_root + "\\" + filename)){
        QMessageBox::warning(NULL, "Error", "文件保存失败！");
    }
}

void MainWindow::on_bt_img_info_clicked()
{
    QString str;
    BITMAPFILEHEADER& fh = rawbmp.bmpHead;       //位图文件头
    BITMAPINFOHEADER& ih = rawbmp.bmpInforHead;  //位图信息头
    str += "bfType (file type) = " + QString().setNum(fh.bfType, 16);
    str += "\nbfSize (file length) = " + QString().setNum(fh.bfSize);
    str += "\nbfOffBits (offset of bitmap data in bytes) = " + QString().setNum(fh.bfOffBits);
    str += "\nbiSize (header structure length should be 40 or 0x28) = " + QString().setNum(fh.bfSize);
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
    str += "\nBytes per row in bitmap = " + QString().setNum(ih.biWidth * ih.biBitCount / 8 + rawbmp.skip);
    str += "\nTotal bytes of bitmap = " + QString().setNum(ih.biWidth * ih.biHeight * ih.biBitCount / 8);
    str += "\nActual pixels per raw in bitmap = " + QString().setNum(ih.biWidth);
    str += "\nTotal rows of bitmap = " + QString().setNum(ih.biHeight);
    str += "\nTotal colors = " + QString().setNum(1 << ih.biBitCount);
    str += "\nUsed colors = " + QString().setNum(ih.biClrUsed);
    QMessageBox::information(NULL, "Info", str);
}

void MainWindow::on_bt_get_pixel_clicked()
{
    if(rawbmp.data == NULL){
        QMessageBox::warning(NULL, "Error", "未打开图片！");
        return;
    }

    int x = this->ui->et_x->text().toInt();
    int y = this->ui->et_y->text().toInt();
    if(!(0<=x && x<=rawbmp.width && 0<=y && y<=rawbmp.height)){
        QMessageBox::warning(NULL, "Error", "Invalid coordinate!");
        return;
    }

    if(rawbmp.bitsPerPixel == 8){
        Pixel* p = rawbmp.get_gray_pixel(x, y);
        int gray = p->gray;

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", gray, gray, gray);
        this->ui->label_show_color->setStyleSheet(qss);

        this->ui->et_gray->setText(QString().setNum(gray));
    }else /*if(rawbmp.bitsPerPixel == 24)*/{
        Pixel* p = rawbmp.get_rgb_pixel(x, y);
        int b = p->b;
        int g = p->g;
        int r = p->r;

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", r, g, b);
        this->ui->label_show_color->setStyleSheet(qss);

        this->ui->et_b->setText(QString().setNum(b));
        this->ui->et_g->setText(QString().setNum(g));
        this->ui->et_r->setText(QString().setNum(r));
    }
}

void MainWindow::on_bt_set_pixel_clicked()
{
    if(rawbmp.data == NULL){
        QMessageBox::warning(NULL, "Error", "未打开图片！");
        return;
    }

    int x = this->ui->et_x->text().toInt();
    int y = this->ui->et_y->text().toInt();
    if(!(0<=x && x<=rawbmp.width && 0<=y && y<=rawbmp.height)){
        QMessageBox::warning(NULL, "Error", "Invalid coordinate!");
        return;
    }

    if(rawbmp.bitsPerPixel == 8){
        int gray = this->ui->et_gray->text().toInt();
        if(!(0<=gray && gray<=255)){
            QMessageBox::warning(NULL, "Error", "Invalid gray scale!");
            return;
        }

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", gray, gray, gray);
        this->ui->label_show_color->setStyleSheet(qss);

        Pixel* p = rawbmp.get_gray_pixel(x, y);
        p->gray = gray;
    }else /*if(rawbmp.bitsPerPixel == 24)*/{
        int b = this->ui->et_b->text().toInt();
        int g = this->ui->et_g->text().toInt();
        int r = this->ui->et_r->text().toInt();
        if(!(0<=b && b<=255 && 0<=g && g<=255 && 0<=r && r<=255)){
            QMessageBox::warning(NULL, "Error", "Invalid BGR scale!");
            return;
        }

        QString qss("QLabel{background:#");
        qss += QString().sprintf("%.2X%.2X%.2X;}", r, g, b);
        this->ui->label_show_color->setStyleSheet(qss);

        Pixel* p = rawbmp.get_rgb_pixel(x, y);
        p->b = b;
        p->g = g;
        p->r = r;
    }

    renew_current_image();
}
