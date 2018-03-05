#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <ctime>
#include <QTime>
#include <QFile>
#include <qtextstream.h>
#include <QCoreApplication>
#include <windows.h>
using namespace std;

int plane[16][2]={500,0,501,0,502,0,503,0,504,0,505,0,506,0,507,0,508,0,509,0,510,0,511,0,512,0,513,0,514,0,515,0};
int position[92][8];
int player;
int randnum;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::playerplus() //玩家变化
{
    player=(player+1)%4;
    startGame();
    return 1;
}

void sleep(unsigned int msec)
{
QTime dieTime = QTime::currentTime().addMSecs(msec);
while( QTime::currentTime() < dieTime )
QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

unsigned long MainWindow::snum()
{
    asm("rdtsc");
}

int MainWindow::Random()//生成随机数
{
    srand(clock());
    return rand()%6+1;
}

int victory()//判断胜利
{
    int i,j,k=0;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            if(plane[4*i+j][1]==1)k++;
            else break;
        }
        if(k==4)return i;
        else k=0;
    }
    return 5;
}

int MainWindow::allInHome()
{
    if(plane[4][0]+plane[5][0]+plane[6][0]+plane[7][0]==2022)return 1;
    else return 0;
}

int MainWindow::startGame() //游戏运行主线程
{
    int move=16;
    if(victory()!=5)
    {
        if(victory()==0)
        ui->label_13->setText("游戏结束 蓝色玩家胜利");
        if(victory()==1)
        ui->label_13->setText("游戏结束 黄色玩家胜利");
        if(victory()==2)
        ui->label_13->setText("游戏结束 绿色玩家胜利");
        if(victory()==3)
        ui->label_13->setText("游戏结束 红色玩家胜利");
        player=5;
        return 0;
    }
    else if(player==0)
    {
        sleep(1000);
        randnum=Random();
        ui->label_13->setText(QString::number(randnum));
        if(allInHome()==1 && randnum!=6)
        {
            playerplus();
        }
    }

    /*else if(player==0)
    {
        sleep(100);
        randnum=Random();
        ui->label_13->setText(QString::number(randnum));
        move=AI(1,randnum);
        cout<<plane[move][1]<<endl;
        if(move!=16)
            moveElement(move,randnum);
        playerplus();
    }*/

    else if(player==1)
    {
        sleep(100);
        randnum=Random();
        ui->label_13->setText(QString::number(randnum));
        move=AI(2,randnum);
        cout<<plane[move][1]<<endl;
        if(move!=16)
            moveElement(move,randnum);
        playerplus();
    }
    else if(player==2)
    {
        sleep(100);
        randnum=Random();
        ui->label_13->setText(QString::number(randnum));
        move=AI(3,randnum);
        cout<<plane[move][1]<<endl;
        if(move!=16)
            moveElement(move,randnum);
        playerplus();
    }
    else
    {
        sleep(100);
        randnum=Random();
        ui->label_13->setText(QString::number(randnum));
        move=AI(0,randnum);
        cout<<plane[move][1]<<endl;
        if(move!=16)
            moveElement(move,randnum);
        playerplus();
    }

    return 1;
}

int MainWindow::AI(int color,int random) //AI
{
    int plane_Order[4]={0};//飞机所在位置的编号
        int mark[4];//飞机是否飞完一轮的标志
        int importance_Degree[4]={0};//进行判断的依据
        int after_position[4];//如果进行操作后的下一步位置
        int plane_start[4]={40,1,14,27};//飞机的起点
        int plane_Corner[4]={37,50,11,24};//飞机进入安全通道前的位置
        int plane_FasterS[4]={5,18,31,44};//飞机快速航道起点
        int plane_FasterE[4]={17,30,43,4};//飞机快速航道终点
        int important_number[8]={-127,255,127,63,31,15,7,3};
        int allair_Order[16];
        int plane_FasterM[4]={302,402,102,202};
        int j;
        int acc=0;
        int dis[4];
        int distanceMin;
        for(int i=0;i<=3;i++){
            plane_Order[i]=plane[4*color+i][0];//将飞机现在的位置赋给数组pO[]
            if(plane_Order[i]>499){
                plane_Order[i]=0;
            }
        }
        if(random!=6&&plane_Order[0]==0&&plane_Order[1]==0&&plane_Order[2]==0&&plane_Order[3]==0){
            return 16;
        }//四个飞机都没出来且不是6，返回16
        for(j=0;j<=15;j++){
            allair_Order[j]=plane[j][0];
            if(j==4*color||j==4*color+1||j==4*color+2||j==4*color+3){
                allair_Order[j]=0;
            }//读出所有飞机的位置并将同色飞机位置置零
        }
        for(int i=0;i<=3;i++){
            mark[i]=plane[4*color+i][1];//将飞机是否飞过一轮的标志赋给数组mark[]
            if(mark[i]==1){
                importance_Degree[i]=-255;
            }else{
                plane_Order[i]=plane[4*color+i][0];//将飞机现在的位置赋给数组pO[]
                if(plane_Order[i]>499){
                    plane_Order[i]=0;
                }
                mark[i]=plane[4*color+i][1];//将飞机是否飞过一轮的标志赋给数组mark[]
                if(plane_Order[i]==0&&random!=6){
                    importance_Degree[i]=-255;//如果在家且骰子数不为6，赋予最小值，跳出
                }
                if((plane_Order[i]>99)&&(plane_Order[i]<499)&&
                   (plane_Order[i]+random!=(color+1)*100+5)){//如果在安全通道内且不会到达终点，最低重要度
                    importance_Degree[i]=importance_Degree[i]+important_number[0];
                }
                after_position[i]=plane_Order[i]+random;//现在的位置加上要走的步数之后的的初步位置
                if(random!=6&&plane_Order[i]>499){
                    after_position[i]=0;//飞机在起点且骰子不是6 不动
                }
                if(plane_Order[i]<=plane_Corner[color]&&after_position[i]>plane_Corner[color]){
                    after_position[i]=(color+1)*100+random+plane_Order[i]-plane_Corner[color]-1;//如果超过了拐点，进入安全通道
                    if(after_position[i]!=(color+1)*100+2){//如果在绝对安全区域
                        importance_Degree[i]=importance_Degree[i]+important_number[1];//第一重要度
                    }else{importance_Degree[i]=importance_Degree[i]+important_number[0];}//否则最低重要度
                }
                if(random==6&&plane_Order[i]==0&&mark[i]==0){
                    after_position[i]=plane_start[color];//如果骰子为6且飞机未起飞则出飞机
                    importance_Degree[i]=importance_Degree[i]+important_number[2];//第二重要度
                }
                if(after_position[i]==plane_FasterS[color]){
                    after_position[i]=plane_FasterE[color]+4;//如果飞机行进后落在快速航道起点，那么飞机行至终点时再进4格
                    importance_Degree[i]=importance_Degree[i]+important_number[5];
                    for(j=0;j<=15;j++){
                        if(allair_Order[j]==plane_FasterM[color]){
                            importance_Degree[i]=importance_Degree[i]+important_number[3];//会被撞掉的地方有飞机，再加一个重要度  3
                        }
                    }
                }
                if((after_position[i]-1)%4==color){//如果飞机到了同颜色格子
                    after_position[i]=after_position[i]+4;//进四个格子
                    if(after_position[i]==plane_FasterS[color]){//如果落点是快速航道起点
                        after_position[i]=plane_FasterE[color];//落点变成快速航道终点
                        importance_Degree[i]=importance_Degree[i]+important_number[5];
                        for(j=0;j<=15;j++){
                            if(allair_Order[j]==plane_FasterM[color]){
                                importance_Degree[i]=importance_Degree[i]+important_number[3];//会被撞掉的地方有飞机，再加一个重要度  3
                            }
                        }
                    }else{importance_Degree[i]=importance_Degree[i]+important_number[6];}//落点不是快速航道起点重要度为6
                }
                if(after_position[i]==(color+1)*100+5){
                    importance_Degree[i]=importance_Degree[i]+important_number[1]*2;//如果飞机可以立刻回家,主要度最高
                }
                for(j=0;j<=15;j++){
                    if(allair_Order[j]==after_position[i]){
                        acc=acc+1;//计算可以撞掉落点有多少架飞机
                    }
                }
                if(acc==1){//撞掉一架
                    importance_Degree[i]=importance_Degree[i]+important_number[3];//重要度  3
                }


                if(acc==2){//撞掉两架
                    importance_Degree[i]=importance_Degree[i]+important_number[4];//重要度  4
                }

                if(acc==3){//遇到3架
                    importance_Degree[i]=importance_Degree[i]-127;//重要度减127
                }

                for(int p=0;p<=3;p++){
                    if(mark[p]==0&&plane_Order[p]!=0){
                        dis[p]=(plane_Corner[color]+52-after_position[i])%52;
                    }else{dis[p]=100;}
                }//求距离

                if(dis[0]<=dis[1]&&dis[0]<=dis[2]&&dis[0]<=dis[3]){
                    distanceMin=0;
                }
                else if(dis[1]<=dis[0]&&dis[1]<=dis[2]&&dis[1]<=dis[3]){
                    distanceMin=1;
                }
                else if(dis[2]<=dis[1]&&dis[2]<=dis[0]&&dis[2]<=dis[3]){
                    distanceMin=2;
                }
                else{
                    distanceMin=3;
                }
                if(i==distanceMin){
                    importance_Degree[i]=importance_Degree[i]+important_number[7];
                }
            }
        }
        if(importance_Degree[0]>=importance_Degree[1]&&
           importance_Degree[0]>=importance_Degree[2]&&
           importance_Degree[0]>=importance_Degree[3]){
            return 4*color+0;
           }
        else if(importance_Degree[1]>=importance_Degree[0]&&
           importance_Degree[1]>=importance_Degree[2]&&
           importance_Degree[1]>=importance_Degree[3]){
            return 4*color+1;
           }
        else if(importance_Degree[2]>=importance_Degree[1]&&
           importance_Degree[2]>=importance_Degree[0]&&
           importance_Degree[2]>=importance_Degree[3]){
            return 4*color+2;
           }
        else if(importance_Degree[3]>=importance_Degree[1]&&
           importance_Degree[3]>=importance_Degree[2]&&
           importance_Degree[3]>=importance_Degree[0]){
            return 4*color+3;
           }
        return 16;
}

int MainWindow::gohome(int element) //回家
{
    if(element==0) {ui->label->move(520,520); plane[0][0]=500;}
    if(element==1) {ui->label_2->move(580,520); plane[1][0]=501;}
    if(element==2) {ui->label_3->move(520,590); plane[2][0]=502;}
    if(element==3) {ui->label_4->move(580,590); plane[3][0]=503;}
    if(element==4) {ui->pushButton->move(50,520); plane[4][0]=504;}
    if(element==5) {ui->pushButton_2->move(110,520); plane[5][0]=505;}
    if(element==6) {ui->pushButton_3->move(50,580); plane[6][0]=506;}
    if(element==7) {ui->pushButton_4->move(110,580); plane[7][0]=507;}
    if(element==8) {ui->label_5->move(50,40); plane[8][0]=508;}
    if(element==9) {ui->label_6->move(110,40); plane[9][0]=509;}
    if(element==10) {ui->label_7->move(50,100); plane[10][0]=510;}
    if(element==11) {ui->label_8->move(110,100); plane[11][0]=511;}
    if(element==12) {ui->label_9->move(520,40); plane[12][0]=512;}
    if(element==13) {ui->label_10->move(580,40); plane[13][0]=513;}
    if(element==14) {ui->label_11->move(520,110); plane[14][0]=514;}
    if(element==15) {ui->label_12->move(580,110); plane[15][0]=515;}
    return 1;
}

int MainWindow::crash(int c) //滑道上出现的撞击
{
    int t=(c+2)%4,i=0;
    for(i=0;i<4;i++)
    {
        if(plane[4*t+i][0]==(t+1)*100+2) gohome(4*t+i);
    }
    return t;
}

int MainWindow::crash2(int p,int w) //判断落点会不会撞机
{
    int i,j=0,h[3];
    for(i=0;i<15;i++){
        if(plane[i][0]==w&&i/4!=p/4){
            h[j]=i;
            j++;
        }
    }
    if(j>=2)gohome(p);
    if(j>0&&j<3)
        for(i=0;i<j;i++)
            gohome(h[i]);
    return 1;
}

int checkSlideBoard(int a,int n) //查询是否有跳板
{
    int t=0;
    if(a==0&&n%4==1&&n!=37&&n<100)t=1;
    if(a==1&&n%4==2&&n!=50&&n<100)t=1;
    if(a==2&&n%4==3&&n!=11&&n<100)t=1;
    if(a==3&&n%4==0&&n!=24&&n<100)t=1;
    return t;
}

int MainWindow::getmap(int a[92][8]) //将地图读取到内存中
{
    int i,j,k=0,l=0;
    char* ca;
    char t[50];
    QString str="123";
    QFile file("position.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))return 0;
    QTextStream in(&file);
    for(i=0;i<92;i++){
        QString line = in.readLine();
        QByteArray ba = line.toLatin1();
        ca=ba.data();
        strcpy(t,ca);
        for(j=0;l<8;j++){
            if(t[j]=='0')k=k*10;
            if(t[j]=='1')k=k*10+1;
            if(t[j]=='2')k=k*10+2;
            if(t[j]=='3')k=k*10+3;
            if(t[j]=='4')k=k*10+4;
            if(t[j]=='5')k=k*10+5;
            if(t[j]=='6')k=k*10+6;
            if(t[j]=='7')k=k*10+7;
            if(t[j]=='8')k=k*10+8;
            if(t[j]=='9')k=k*10+9;
            if(t[j]=='#'){
                a[i][l]=k;
                k=0;
                l++;
            }
        }
        l=0;
    }
    return 1;
}

int MainWindow::moveElement(int element,int amount) //移动飞机的主函数
{
    int number;//所在序号
    int conner=0;
    QPoint p1;
    int plusamount;//前进步数
    int color;
    int springamount=1;//跳跃次数
    number=plane[element][0];//获取飞机所在位置序号

    if(number>499) //飞机还在家 (考虑6\任务完成)
    {
        if(element>=0&&element<=3)
        {
            if(element==0) ui->label->move(560,420);
            if(element==1) ui->label_2->move(560,420);
            if(element==2) ui->label_3->move(560,420);
            if(element==3) ui->label_4->move(560,420);
            number=40;
        }
        if(element>=4&&element<=7)
        {
            if(element==4) ui->pushButton->move(190,560);
            if(element==5) ui->pushButton_2->move(190,560);
            if(element==6) ui->pushButton_3->move(190,560);
            if(element==7) ui->pushButton_4->move(190,560);
            number=1;
        }
        if(element>=8&&element<=11)
        {
            if(element==8) ui->label_5->move(50,200);
            if(element==9) ui->label_6->move(50,200);
            if(element==10) ui->label_7->move(50,200);
            if(element==11) ui->label_8->move(50,200);
            number=14;
        }
        if(element>=12&&element<=15)
        {
            if(element==12) ui->label_9->move(420,60);
            if(element==13) ui->label_10->move(420,60);
            if(element==14) ui->label_11->move(420,60);
            if(element==15) ui->label_12->move(420,60);
            number=27;
        }
        plusamount=0;
        //移动到起始点
    }
    else{ //否则就向前移动
        plusamount=amount;
        if(number>=100&&number<500)//飞机已经进入拐点
        {
            label1:
            int direction=1;//初始化正向；
            if(conner==1)//如果是拐进来的
            {
                if(element>=0&&element<=3)
                    number=100;//拐点序号校正
                if(element>=4&&element<=7)
                    number=200;//拐点序号校正
                if(element>=8&&element<=11)
                    number=300;//拐点序号校正
                if(element>=12&&element<=15)
                    number=400;//拐点序号校正
                if(plusamount>1) plusamount--;
            }

            for(;plusamount>0;plusamount--)
            {
                //移动骰子的数目
                if(element>=0&&element<=3)
                {
                    if(direction==1)
                    {
                        if(conner==0)
                        {
                            number++;
                        }
                        if(conner==1) conner=0;
                        p1=*new QPoint(position[number-42][1],position[number-42][2]);

                        if(element==0) ui->label->move(p1);
                        if(element==1) ui->label_2->move(p1);
                        if(element==2) ui->label_3->move(p1);
                        if(element==3) ui->label_4->move(p1);


                    if(number==105)
                    {
                        direction=0;
                        if(plusamount==1)//成功的标志
                        {
                            plane[element][1]=1;


                            if(element==0)
                            {
                                p1=*new QPoint(position[76][1],position[76][2]);
                                ui->label->move(p1);
                                plane[element][0]=500;
                                plane[element][1]=1;
                            }
                            if(element==1)
                            {
                                p1=*new QPoint(position[77][1],position[77][2]);
                                ui->label_2->move(p1);
                                plane[element][0]=501;
                                plane[element][1]=1;
                            }
                            if(element==2)
                            {
                                p1=*new QPoint(position[78][1],position[78][2]);
                                ui->label_3->move(p1);
                                plane[element][0]=502;
                                plane[element][1]=1;
                            }
                            if(element==3)
                            {
                                p1=*new QPoint(position[79][1],position[79][2]);
                                ui->label_4->move(p1);
                                plane[element][0]=503;
                                plane[element][1]=1;
                            }
                            return 0;
                        }
                    }

                }


                    else if(direction==0)
                    {
                        number--;
                        p1=*new QPoint(position[number-42][1],position[number-42][2]);
                        if(element==0) ui->label->move(p1);
                        if(element==1) ui->label_2->move(p1);
                        if(element==2) ui->label_3->move(p1);
                        if(element==3) ui->label_4->move(p1);

                    }
                }


                if(element>=4&&element<=7)
                {
                    if(direction==1)
                    {
                        if(conner==0)
                        {
                            number++;
                        }
                        if(conner==1) conner=0;
                        p1=*new QPoint(position[number-148][1],position[number-148][2]);

                        if(element==4) ui->pushButton->move(p1);
                        if(element==5) ui->pushButton_2->move(p1);
                        if(element==6) ui->pushButton_3->move(p1);
                        if(element==7) ui->pushButton_4->move(p1);


                    if(number==205)
                    {
                        direction=0;
                        if(plusamount==1)//成功的标志
                        {
                            plane[element][1]=1;

                            if(element==4)
                            {
                                p1=*new QPoint(position[80][1],position[80][2]);
                                ui->pushButton->move(p1);
                                plane[element][0]=504;
                                plane[element][1]=1;
                            }
                            if(element==5)
                            {
                                p1=*new QPoint(position[81][1],position[81][2]);
                                ui->pushButton_2->move(p1);
                                plane[element][0]=505;
                                plane[element][1]=1;
                            }
                            if(element==6)
                            {
                                p1=*new QPoint(position[82][1],position[82][2]);
                                ui->pushButton_3->move(p1);
                                plane[element][0]=506;
                                plane[element][1]=1;
                            }
                            if(element==7)
                            {
                                p1=*new QPoint(position[83][1],position[83][2]);
                                ui->pushButton_4->move(p1);
                                plane[element][0]=507;
                                plane[element][1]=1;
                            }
                            return 0;
                        }
                    }

                }


                    else if(direction==0)
                    {
                        number--;
                        p1=*new QPoint(position[number-148][1],position[number-148][2]);
                        if(element==4) ui->pushButton->move(p1);
                        if(element==5) ui->pushButton_2->move(p1);
                        if(element==6) ui->pushButton_3->move(p1);
                        if(element==7) ui->pushButton_4->move(p1);

                    }
                }

                if(element>=8&&element<=11)
                {
                    if(direction==1)
                    {
                        if(conner==0)
                        {
                            number++;
                        }
                        if(conner==1) conner=0;
                        p1=*new QPoint(position[number-236][1],position[number-236][2]);

                        if(element==8) ui->label_5->move(p1);
                        if(element==9) ui->label_6->move(p1);
                        if(element==10) ui->label_7->move(p1);
                        if(element==11) ui->label_8->move(p1);


                    if(number==305)
                    {
                        direction=0;
                        if(plusamount==1)//成功的标志
                        {
                            plane[element][1]=1;

                            if(element==8)
                            {
                                p1=*new QPoint(position[84][1],position[84][2]);
                                ui->label_5->move(p1);
                                plane[element][0]=508;
                                plane[element][1]=1;
                            }
                            if(element==9)
                            {
                                p1=*new QPoint(position[85][1],position[85][2]);
                                ui->label_6->move(p1);
                                plane[element][0]=509;
                                plane[element][1]=1;
                            }
                            if(element==10)
                            {
                                p1=*new QPoint(position[86][1],position[86][2]);
                                ui->label_7->move(p1);
                                plane[element][0]=510;
                                plane[element][1]=1;
                            }
                            if(element==11)
                            {
                                p1=*new QPoint(position[87][1],position[87][2]);
                                ui->label_8->move(p1);
                                plane[element][0]=511;
                                plane[element][1]=1;
                            }
                            return 0;
                        }
                    }

                }


                    else if(direction==0)
                    {
                        number--;
                        p1=*new QPoint(position[number-236][1],position[number-236][2]);
                        if(element==8) ui->label_5->move(p1);
                        if(element==9) ui->label_6->move(p1);
                        if(element==10) ui->label_7->move(p1);
                        if(element==11) ui->label_8->move(p1);

                    }
                }

                if(element>=12&&element<=15)
                {
                    if(direction==1)
                    {
                        if(conner==0)
                        {
                            number++;
                        }
                        if(conner==1) conner=0;
                        p1=*new QPoint(position[number-330][1],position[number-330][2]);

                        if(element==12) ui->label_9->move(p1);
                        if(element==13) ui->label_10->move(p1);
                        if(element==14) ui->label_11->move(p1);
                        if(element==15) ui->label_12->move(p1);


                    if(number==405)
                    {
                        direction=0;
                        if(plusamount==1)//成功的标志
                        {
                            plane[element][1]=1;

                            if(element==12)
                            {
                                p1=*new QPoint(position[88][1],position[88][2]);
                                ui->label_9->move(p1);
                                plane[element][0]=512;
                                plane[element][1]=1;
                            }
                            if(element==13)
                            {
                                p1=*new QPoint(position[89][1],position[89][2]);
                                ui->label_10->move(p1);
                                plane[element][0]=513;
                                plane[element][1]=1;
                            }
                            if(element==14)
                            {
                                p1=*new QPoint(position[90][1],position[90][2]);
                                ui->label_11->move(p1);
                                plane[element][0]=514;
                                plane[element][1]=1;
                            }
                            if(element==15)
                            {
                                p1=*new QPoint(position[91][1],position[91][2]);
                                ui->label_12->move(p1);
                                plane[element][0]=515;
                                plane[element][1]=1;
                            }
                            return 0;
                        }
                    }

                }


                    else if(direction==0)
                    {
                        number--;
                        p1=*new QPoint(position[number-330][1],position[number-330][2]);
                        if(element==12) ui->label_9->move(p1);
                        if(element==13) ui->label_10->move(p1);
                        if(element==14) ui->label_11->move(p1);
                        if(element==15) ui->label_12->move(p1);

                    }
                }
            }
        }

        else if(number==11&&(element==8||element==9||element==10||element==11)) //将会进入拐点
        {
            conner=1;
            goto label1;

        }
        else if(number==37&&(element==0||element==1||element==2||element==3)) //将会进入拐点
        {
            conner=1;
            goto label1;
        }
        else if(number==50&&(element==4||element==5||element==6||element==7)) //将会进入拐点
        {
            conner=1;
            goto label1;
        }
        else if(number==24&&(element==12||element==13||element==14||element==15)) //将会进入拐点
        {
            conner=1;
            goto label1;
        }

        else//还没有进入拐点
        {
            for(;plusamount>0;plusamount--)
            {
                //移动骰子的数目
                number=(number+1)%52;
                if(number==0) number=52;
                p1=*new QPoint(position[number-1][1],position[number-1][2]);
                if(element==0) ui->label->move(p1);
                if(element==1) ui->label_2->move(p1);
                if(element==2) ui->label_3->move(p1);
                if(element==3) ui->label_4->move(p1);
                if(element==4) ui->pushButton->move(p1);
                if(element==5) ui->pushButton_2->move(p1);
                if(element==6) ui->pushButton_3->move(p1);
                if(element==7) ui->pushButton_4->move(p1);
                if(element==8) ui->label_5->move(p1);
                if(element==9) ui->label_6->move(p1);
                if(element==10) ui->label_7->move(p1);
                if(element==11) ui->label_8->move(p1);
                if(element==12) ui->label_9->move(p1);
                if(element==13) ui->label_10->move(p1);
                if(element==14) ui->label_11->move(p1);
                if(element==15) ui->label_12->move(p1);
                if(plusamount>1&&number==11&&(element==8||element==9||element==10||element==11)) //将会进入拐点
                {
                    conner=1;
                    goto label1;

                }
                if(plusamount>1&&number==37&&(element==0||element==1||element==2||element==3)) //将会进入拐点
                {
                    conner=1;
                    goto label1;
                }
                if(plusamount>1&&number==50&&(element==4||element==5||element==6||element==7)) //将会进入拐点
                {
                    conner=1;
                    goto label1;
                }
                if(plusamount>1&&number==24&&(element==12||element==13||element==14||element==15)) //将会进入拐点
                {
                    conner=1;
                    goto label1;
                }

                if(plusamount==1&&(
                            number==5&&(
                                element==0||element==1||element==2||element==3
                                )))//进入滑道；
                {
                    if(element==0)
                    {
                        ui->label->move(160,190);
                        number=17;
                    }
                    if(element==1)
                    {
                        ui->label_2->move(160,190);
                        number=17;
                    }
                    if(element==2)
                    {
                        ui->label_3->move(160,190);
                        number=17;
                    }
                    if(element==3)
                    {
                        ui->label_4->move(160,190);
                        number=17;
                    }
                    crash(0);
                }
                if(plusamount==1&&(
                            number==18&&(
                                element==4||element==5||element==6||element==7
                                )))//进入滑道；
                {
                    if(element==4)
                    {
                        ui->pushButton->move(420,170);
                        number=30;
                    }
                    if(element==5)
                    {
                        ui->pushButton_2->move(420,170);
                        number=30;
                    }
                    if(element==6)
                    {
                        ui->pushButton_3->move(420,170);
                        number=30;
                    }
                    if(element==7)
                    {
                        ui->pushButton_4->move(420,170);
                        number=30;
                    }
                    crash(1);
                }
                if(plusamount==1&&(
                            number==31&&(
                                element==8||element==9||element==10||element==11
                                )))//进入滑道；
                {
                    if(element==8)
                    {
                        ui->label_5->move(400,420);
                        number=43;
                    }
                    if(element==9)
                    {
                        ui->label_6->move(400,420);
                        number=43;
                    }
                    if(element==10)
                    {
                        ui->label_7->move(400,420);
                        number=43;
                    }
                    if(element==11)
                    {
                        ui->label_8->move(400,420);
                        number=43;
                    }
                    crash(2);
                }
                if(plusamount==1&&(
                            number==44&&(
                                element==12||element==13||element==14||element==15
                                )))//进入滑道；
                {
                    if(element==12)
                    {
                        ui->label_9->move(190,450);
                        number=4;
                    }
                    if(element==13)
                    {
                        ui->label_10->move(190,450);
                        number=4;
                    }
                    if(element==14)
                    {
                        ui->label_11->move(190,450);
                        number=4;
                    }
                    if(element==15)
                    {
                        ui->label_12->move(190,450);
                        number=4;
                    }
                    crash(3);
                }


            }
        }
    }
    if(element==0||element==1||element==2||element==3) color=0;
    if(element==4||element==5||element==6||element==7) color=1;
    if(element==8||element==9||element==10||element==11) color=2;
    if(element==12||element==13||element==14||element==15) color=3;

    if(checkSlideBoard(color,number)&& springamount==1) //进入跳板；
    {
        number=(number+4)%52;
        if(number==0) number=52;
        p1=*new QPoint(position[number-1][1],position[number-1][2]);
        if(element==0) ui->label->move(p1);
        if(element==1) ui->label_2->move(p1);
        if(element==2) ui->label_3->move(p1);
        if(element==3) ui->label_4->move(p1);
        if(element==4) ui->pushButton->move(p1);
        if(element==5) ui->pushButton_2->move(p1);
        if(element==6) ui->pushButton_3->move(p1);
        if(element==7) ui->pushButton_4->move(p1);
        if(element==8) ui->label_5->move(p1);
        if(element==9) ui->label_6->move(p1);
        if(element==10) ui->label_7->move(p1);
        if(element==11) ui->label_8->move(p1);
        if(element==12) ui->label_9->move(p1);
        if(element==13) ui->label_10->move(p1);
        if(element==14) ui->label_11->move(p1);
        if(element==15) ui->label_12->move(p1);
        springamount=0;
    }

    if((
                number==5&&(
                    element==0||element==1||element==2||element==3
                    )))//进入滑道；
    {
        if(element==0)
        {
            ui->label->move(160,190);
            number=17;
        }
        if(element==1)
        {
            ui->label_2->move(160,190);
            number=17;
        }
        if(element==2)
        {
            ui->label_3->move(160,190);
            number=17;
        }
        if(element==3)
        {
            ui->label_4->move(160,190);
            number=17;
        }
        crash(0);
    }
    if((
                number==18&&(
                    element==4||element==5||element==6||element==7
                    )))//进入滑道；
    {
        if(element==4)
        {
            ui->pushButton->move(420,170);
            number=30;
        }
        if(element==5)
        {
            ui->pushButton_2->move(420,170);
            number=30;
        }
        if(element==6)
        {
            ui->pushButton_3->move(420,170);
            number=30;
        }
        if(element==7)
        {
            ui->pushButton_4->move(420,170);
            number=30;
        }
        crash(1);
    }
    if((
                number==31&&(
                    element==8||element==9||element==10||element==11
                    )))//进入滑道；
    {
        if(element==8)
        {
            ui->label_5->move(400,420);
            number=43;
        }
        if(element==9)
        {
            ui->label_6->move(400,420);
            number=43;
        }
        if(element==10)
        {
            ui->label_7->move(400,420);
            number=43;
        }
        if(element==11)
        {
            ui->label_8->move(400,420);
            number=43;
        }
        crash(2);
    }
    if((
                number==44&&(
                    element==12||element==13||element==14||element==15
                    )))//进入滑道；
    {
        if(element==12)
        {
            ui->label_9->move(190,450);
            number=4;
        }
        if(element==13)
        {
            ui->label_10->move(190,450);
            number=4;
        }
        if(element==14)
        {
            ui->label_11->move(190,450);
            number=4;
        }
        if(element==15)
        {
            ui->label_12->move(190,450);
            number=4;
        }
        crash(3);
    }

    crash2(element,number);
    if(checkSlideBoard(color,number)&& springamount==1) //进入跳板；
    {
        number=(number+4)%52;
        if(number==0) number=52;
        p1=*new QPoint(position[number-1][1],position[number-1][2]);
        if(element==0) ui->label->move(p1);
        if(element==1) ui->label_2->move(p1);
        if(element==2) ui->label_3->move(p1);
        if(element==3) ui->label_4->move(p1);
        if(element==4) ui->pushButton->move(p1);
        if(element==5) ui->pushButton_2->move(p1);
        if(element==6) ui->pushButton_3->move(p1);
        if(element==7) ui->pushButton_4->move(p1);
        if(element==8) ui->label_5->move(p1);
        if(element==9) ui->label_6->move(p1);
        if(element==10) ui->label_7->move(p1);
        if(element==11) ui->label_8->move(p1);
        if(element==12) ui->label_9->move(p1);
        if(element==13) ui->label_10->move(p1);
        if(element==14) ui->label_11->move(p1);
        if(element==15) ui->label_12->move(p1);
        springamount=0;
    }

    plane[element][0]=number;

    return 1;
}




void MainWindow::on_pushButton_clicked() //按动第一架飞机
{
        if(player==0 && plane[4][0]==504 && randnum==6 && plane[4][1]!=1)
        {
            moveElement(4,randnum);
            playerplus();
            return;
        }
        if(player==0 && plane[4][0]!=504)
        {
            moveElement(4,randnum);
            playerplus();
            return;
        }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(player==0 && plane[5][0]==505 && randnum==6 && plane[5][1]!=1)
    {
        moveElement(5,randnum);
        playerplus();
        return;
    }
    if(player==0 && plane[5][0]!=505)
    {
        moveElement(5,randnum);
        playerplus();
        return;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(player==0 && plane[6][0]==506 && randnum==6 && plane[6][1]!=1)
    {
        moveElement(6,randnum);
        playerplus();
        return;
    }
    if(player==0 && plane[6][0]!=506)
    {
        moveElement(6,randnum);
        playerplus();
        return;
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    if(player==0 && plane[7][0]==507 && randnum==6 && plane[7][1]!=1)
    {
        moveElement(7,randnum);
        playerplus();
        return;
    }
    if(player==0 && plane[7][0]!=507)
    {
        moveElement(7,randnum);
        playerplus();
        return;
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    getmap(position);
    player=0;
    startGame();
}
