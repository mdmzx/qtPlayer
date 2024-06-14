#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QUrl>
#include <QList>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_nextButton_clicked();

    void on_preButton_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::Widget *ui;
    int curPlayIndex = 0;
    bool isPlay;
    QAudioOutput *audioOutPut;
    QMediaPlayer *mediaPlayer;
    QList<QUrl> playList;

};
#endif // WIDGET_H
