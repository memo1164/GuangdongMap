#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "admgraph.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::string city[30];
    AdmGraph<std::string>* mp;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setCityName(QString city);     //更新窗口出发地与目的地文本并调用settingShortPathText方法
    void setPathText();            //更新窗口内最短路径与最短里程
    void readFileFromData();                //从文本文件中读取数据

    //出发及目标城市编号与名称
    int src;
    int ter;
    QString srcCity;
    QString terCity;

    //选择开关控制,on代表正在修改出发地，off代表正在修改目的地
    int on = 1;
    int off = 0;

    //槽函数
private slots:

    void on_zhanjiang_clicked();

    void on_srcBotton_clicked();

    void on_terButton_clicked();

    void on_maoming_clicked();

    void on_yangjiang_clicked();

    void on_yunfu_clicked();

    void on_jiangmen_clicked();

    void on_zhaoqing_clicked();

    void on_foshan_clicked();

    void on_zhongshan_clicked();

    void on_zhuhai_clicked();

    void on_qingyuan_clicked();

    void on_guangzhou_clicked();

    void on_dongguan_clicked();

    void on_shenzhen_clicked();

    void on_shaoguan_clicked();

    void on_heyuan_clicked();

    void on_shanwei_clicked();

    void on_meizhou_clicked();

    void on_jieyang_clicked();

    void on_chaozhou_clicked();

    void on_shantou_clicked();

    void on_huizhou_clicked();

    void on_addPath_triggered();

    void on_delPath_triggered();

    void on_Instructions_triggered();

    void on_exit_triggered();

    void on_savePath_triggered();

    void on_attention_triggered();

    void on_staff_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
