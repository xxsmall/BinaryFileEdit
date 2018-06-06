#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTime>

namespace Ui {
class MainWindow;
}

class recPosition
{
public:
  qint16  row;
  qint16 columnStart;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    QStandardItemModel *modelFile;//表的model
    QStandardItemModel *modelFileSecond;//表的model
    QFile fileObject;//文件对象
    int size;
    int sizeSecond;
    qint64 row; // last byte row
    qint64 columnEndAt; //last byte column
    qint64 rowSecond; // last byte row
    qint64 columnEndAtSecond; //last byte column
    qint64 displaySearchPos;//  use in seach, to display which search resault need to be display,min 0,max,findResault.count()
    qint64 displaySearchPosSave;//  same function as befor
    qint64 displayDiffPos;//  same function as befor
    QString  needFindStr;//需要查找的字符串
    QList<recPosition> findResault;//记录查找结果字符的首 row和column队列
    QList<recPosition> findResaultSave;//use to search in loop
    QList<recPosition>  findResaultDiff;//diffrent search resault to save
    QList<qint8> fileList;
    QString fileFirstSaveStr;//use to save file 1 in old path
    qint64 UIisCN;
    QList<QStandardItem*> cellItemList;//save all the table cell item,and clear it when openfile.
    QList<QStandardItem*> cellItemList2;//save all the table cell item,and clear it when openfile.

    void setTableHeader();
    void openFile();
    void openFileSecond();
    void printSearchResault();
    void saveFile();
    void printDiffResault();
private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
