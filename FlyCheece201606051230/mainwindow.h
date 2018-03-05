#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int playerplus();
    int startGame();
    int allInHome();
    int AI(int color,int random);
    int gohome(int element);
    int crash(int c);
    int crash2(int p,int w);
    int getmap(int a[92][8]);
    int moveElement(int element,int amount);
    unsigned long snum();
    int Random();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
