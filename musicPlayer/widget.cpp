#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMediaPlayer>
#include <QAudioOutput>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //播放音乐  先加载模块multimedia
    //先new一个outpu对象
    audioOutPut = new QAudioOutput;
    //在创建一个媒体对象
    mediaPlayer = new QMediaPlayer(this);
//    mediaPlayer->setMedia(QUrl::
//                          fromLocalFile("D:/QTcode/5.14.2/musicPlayer/assets/music/Despacito(Versión_Pop)-Luis_Fonsi-25768118.mp3"));


    //获取当前媒体时长，通过信号关联获取
    connect(mediaPlayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration){
        ui->totalTimeLable->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg
                                    (duration/1000%60,2,10,QChar('0')));
        //设置进度条范围
        ui->playCourseSlider->setRange(0,duration);
    });

    //获取当前播放时长
    connect(mediaPlayer,&QMediaPlayer::positionChanged,this,[=](qint64 pos){
        ui->currTimeLable->setText(QString("%1:%2").arg(pos/1000/60,2,10,QChar('0')).arg
                                    (pos/1000%60,2,10,QChar('0')));
        //设置进度条改变
        ui->playCourseSlider->setValue(pos);

    });

    //拖动滑块音乐播放到相应的进度
    connect(ui->playCourseSlider,&QSlider::sliderMoved,mediaPlayer,&QMediaPlayer::setPosition);



}

Widget::~Widget()
{
    delete ui;
}

//获取音乐路径
void Widget::on_pushButton_clicked()
{
//    qInfo() << "hello Student";
    //getExistingDirectory()方法，三个参数，第一个哪个类处理（一般是this） 标题 默认目录 目录反斜杠一定要加一个反斜杠转义
    QString path =  QFileDialog::getExistingDirectory(this,"请选择音乐所在目录","D:\\QTcode\\5.14.2\\musicPlayer\\assets\\music");
    qDebug() << path;
    //根据路径获取里面所有的MP3，wav文件
    QDir dir(path);     //path类型转换
    QStringList musicList = dir.entryList(QStringList()<<"*.mp3"<<"*,wav");     //entryList()方法是获取所有满足规则的文件
    qInfo() << musicList;
    ui->listWidget->addItems(musicList);    //将获取的文件放到listWidget中

    //默认选中第一个音乐
    ui->listWidget->setCurrentRow(0);

    //存储音乐的完整路径
    for(auto file : musicList)
        playList.append(QUrl::fromLocalFile(path+"/"+file));

    qInfo() << playList;
}


//暂停/播放
void Widget::on_pushButton_4_clicked()
{
    if(playList.isEmpty()){
        return;
    }
    QMediaPlayer::State state = mediaPlayer->state(); // 获取当前播放状态

    switch (state) {
        case QMediaPlayer::PlayingState:    //正在播放就暂停
    {
            mediaPlayer->pause();
            break;
    }
        case QMediaPlayer::PausedState:     //停止播放就继续播放
    {
            mediaPlayer->play();

            break;
    }
        case QMediaPlayer::StoppedState:    //停止播放就开始播放选中的歌曲
    {
            //如果没有播放，播放当前选中的音乐
            //1.获取选中的行号
            curPlayIndex = ui->listWidget->currentRow();
            //2.播放对应下标的音乐
            mediaPlayer->setMedia(playList[curPlayIndex]);
            mediaPlayer->play();
            break;
    }
        default:
            qDebug() << "Unknown playback state";
            break;
    }
}

//下一首
void Widget::on_nextButton_clicked()
{
    //让listWidget选中下一行
    //    curPlayIndex++;
    //    //下标越界判断
    //    if(curPlayIndex >= playList.size()){
    //        curPlayIndex = 0;
    //    }

        //防止下标越界优化算法
        curPlayIndex = (curPlayIndex + 1) % playList.size();
        //将listWidget选中到下一个
        ui->listWidget->setCurrentRow(curPlayIndex);
        mediaPlayer->setMedia(playList[curPlayIndex]);
        mediaPlayer->play();
}

//上一首
void Widget::on_preButton_clicked()
{
    //防止下标越界优化算法
    curPlayIndex = (curPlayIndex - 1 + playList.size()) % playList.size();
    //将listWidget选中到下一个
    ui->listWidget->setCurrentRow(curPlayIndex);
    mediaPlayer->setMedia(playList[curPlayIndex]);
    mediaPlayer->play();
}


void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curPlayIndex = index.row();
    mediaPlayer->setMedia(playList[curPlayIndex]);
    mediaPlayer->play();
}
