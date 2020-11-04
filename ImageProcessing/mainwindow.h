#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bmp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    int renew_current_image();

    void on_bt_open_clicked();

    void on_bt_save_clicked();

    void on_bt_img_info_clicked();

    void on_bt_set_file_root_clicked();

    void on_bt_get_pixel_clicked();

    void on_bt_set_pixel_clicked();

    void on_bt_nn_scaling_clicked();

    void on_bt_median_filtering_clicked();

    void on_bt_gaussian_smoothing_clicked();

    void on_bt_bilinear_clicked();

    void on_bt_histogram_equalization_clicked();

    void on_bt_sharpen_clicked();

    void on_bt_impulsive_salt_clicked();

    void on_bt_impulsive_uniform_clicked();

private:
    QString file_root;
    BMP* bmp;

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
