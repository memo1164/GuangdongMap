#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QClipboard>
#include <fstream>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->PathText->setWordWrap(true);
    //初始化
    src = 0, ter = 0;
    srcCity = "null", terCity = "null";

    //从文件中读取顶点与路径数据
    readFileFromData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFileFromData(){
    //程序根目录
    std::string path = QCoreApplication::applicationDirPath().toStdString();

    //读取城市(顶点)
    fstream readFileCity; int cnt = 0;
    readFileCity.open(path+"/data/city.txt");
    while(readFileCity>>city[cnt]) cnt++; //qDebug()<<QString::fromStdString(city[cnt-1]);
    readFileCity.close();

    //建图
    mp = new AdmGraph<std::string>(city,21);

    //读取路径
    fstream readFilePath;
    readFilePath.open(path+"/data/path.txt");
    int start,end,d;
    while(readFilePath>>start>>end>>d){
        //qDebug()<<start<<end<<d;
        mp->add(start,end,d,1);
    }
    readFilePath.close();
}

void MainWindow::setCityName(QString city){
    //设置出发城市或目标城市名称与编号
    if(on){
        ui->srcText->setText(city);
        src = mp->findNum(city.toStdString());
    }
    else{
        ui->terText->setText(city);
        ter = mp->findNum(city.toStdString());
    }
    //更新最短路
    setPathText();
}

void MainWindow::setPathText(){
    int pathCnt;    //里程数
    std::string s = mp->dijkstra(src,ter,pathCnt);

    //判断是否通路
    if(pathCnt==INF) pathCnt = 0;

    //将得到的数据转换
    QString Path = QString::fromStdString(s);
    QString PathCnt = QString::number(pathCnt);
    PathCnt += " km";

    //qDebug()<<Path;

    //更新窗口文本
    ui->PathText->setText(Path);
    ui->pathCntText->setText(PathCnt);
}

/*-------------------------------槽函数------------------------------------*/
//菜单按钮
void MainWindow::on_addPath_triggered()
{   //QInputDialog获取添加路径的值
    const QString pathNum = QInputDialog::getText(this,tr("添加路径"),tr("正在为当前所选城市间添加路径:"));
    if(pathNum.isEmpty()) return;
    mp->add(src,ter,pathNum.toInt(),1);
    setPathText();  //刷新路径
}


void MainWindow::on_delPath_triggered()
{
    QMessageBox* delMsgBox = new QMessageBox();
    delMsgBox->setWindowTitle("删除路径");
    delMsgBox->setText("将删除所选城市间路径，是否确定？");
    delMsgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //确认是否删除
    if(delMsgBox->exec() == QMessageBox::Yes) mp->del(src,ter,1);
    setPathText();  //刷新路径
    delete delMsgBox;
}

void MainWindow::on_exit_triggered()
{
    QMessageBox* exitMsgBox = new QMessageBox;
    exitMsgBox->setWindowTitle("退出程序");
    exitMsgBox->setText("确定要退出程序吗");
    exitMsgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //确认是否退出
    if(exitMsgBox->exec() == QMessageBox::Yes){
    QApplication* app;
    app->quit();
    }
}

void MainWindow::on_savePath_triggered()
{
    int num = 0;   //保存最短距离

    //QClipboard与剪切文本内容
    QClipboard* clip = QApplication::clipboard();
    QString text = QString::fromStdString(mp->dijkstra(src,ter,num));
    text += (" , " + QString::number(num) + " km");

    //提示窗口
    QMessageBox* saveMsgBox = new QMessageBox;
    saveMsgBox->setWindowTitle("保存路径");

    if(src && ter){             //成功保存
        clip->setText(text);
        saveMsgBox->setText("已将当前所选城市间最短路保存到剪切板");
    }
    else saveMsgBox->setText("未选择城市！");
    saveMsgBox->setStandardButtons(QMessageBox::Yes);
    saveMsgBox->exec();

    delete saveMsgBox;
}

void MainWindow::on_Instructions_triggered(){
    //获取根目录
    std::string path = QCoreApplication::applicationDirPath().toStdString();
    //读取文档
    fstream readIns;
    readIns.open(path+"/data/help.txt");
    istreambuf_iterator<char> beg(readIns),end;
    std::string helpText(beg,end);
    //将文本添加到QMessageBox
    QMessageBox* helpMsgBox = new QMessageBox;
    helpMsgBox->setWindowTitle("使用说明");
    helpMsgBox->setText(QString::fromStdString(helpText));
    helpMsgBox->setStandardButtons(QMessageBox::Ok);
    helpMsgBox->exec();
    delete helpMsgBox;
}

void MainWindow::on_attention_triggered()
{
    QMessageBox* attMsgBox = new QMessageBox;
    attMsgBox->setWindowTitle("注意事项");
    attMsgBox->setText("本程序数据采用城市市中心间的飞行距离，实际参考价值较低");
    attMsgBox->setStandardButtons(QMessageBox::Yes);
    attMsgBox->exec();
    delete attMsgBox;
}

void MainWindow::on_staff_triggered()
{
    //获取根目录
    std::string path = QCoreApplication::applicationDirPath().toStdString();
    //读取文档
    fstream readStaff;
    readStaff.open(path+"/data/staff.txt");
    istreambuf_iterator<char> beg(readStaff),end;
    std::string StaffText(beg,end);
    //将文本添加到QMessageBox
    QMessageBox* StaffMsgBox = new QMessageBox;
    StaffMsgBox->setWindowTitle("制作人员");
    StaffMsgBox->setText(QString::fromStdString(StaffText));
    StaffMsgBox->setStandardButtons(QMessageBox::Ok);
    StaffMsgBox->exec();
    delete StaffMsgBox;
}


//出发地按钮
void MainWindow::on_srcBotton_clicked()
{
    on = 1, off = 0;
}

//目的地按钮
void MainWindow::on_terButton_clicked()
{
    off = 1, on = 0;
}

void MainWindow::on_zhanjiang_clicked()
{
    setCityName("湛江市");
}

void MainWindow::on_maoming_clicked()
{
    setCityName("茂名市");
}


void MainWindow::on_yangjiang_clicked()
{
    setCityName("阳江市");
}


void MainWindow::on_yunfu_clicked()
{
    setCityName("云浮市");
}


void MainWindow::on_jiangmen_clicked()
{
    setCityName("江门市");
}


void MainWindow::on_zhaoqing_clicked()
{
    setCityName("肇庆市");
}


void MainWindow::on_foshan_clicked()
{
    setCityName("佛山市");
}


void MainWindow::on_zhongshan_clicked()
{
    setCityName("中山市");
}


void MainWindow::on_zhuhai_clicked()
{
    setCityName("珠海市");
}


void MainWindow::on_qingyuan_clicked()
{
    setCityName("清远市");
}


void MainWindow::on_guangzhou_clicked()
{
    setCityName("广州市");
}


void MainWindow::on_dongguan_clicked()
{
    setCityName("东莞市");
}


void MainWindow::on_shenzhen_clicked()
{
    setCityName("深圳市");
}


void MainWindow::on_shaoguan_clicked()
{
    setCityName("韶关市");
}


void MainWindow::on_heyuan_clicked()
{
    setCityName("河源市");
}


void MainWindow::on_shanwei_clicked()
{
    setCityName("汕尾市");
}


void MainWindow::on_meizhou_clicked()
{
    setCityName("梅州市");
}


void MainWindow::on_jieyang_clicked()
{
    setCityName("揭阳市");
}


void MainWindow::on_chaozhou_clicked()
{
    setCityName("潮州市");
}


void MainWindow::on_shantou_clicked()
{
    setCityName("汕头市");
}


void MainWindow::on_huizhou_clicked()
{
    setCityName("惠州市");
}

/*----------------------------------------------------------------------*/


