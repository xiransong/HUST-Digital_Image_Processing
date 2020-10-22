#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <rawbmp.h>

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

private:
    QString file_root;
    RawBMP rawbmp;

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
