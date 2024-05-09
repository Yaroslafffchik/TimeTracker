#include "timerlabel.h"

TimerLabel::TimerLabel(QWidget *parent) : QLabel(parent){
    setText("00:00:00");
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TimerLabel::updateTime);
}

void TimerLabel::setTime(const QTime &time){
    currentTime = time;
    setText(time.toString("hh:mm:ss"));
}

void TimerLabel::start(){
    timer->start(1000);
}

bool TimerLabel::isRunning(){
    return timer->isActive();
}

void TimerLabel::stop(){
    timer->stop();
}

void TimerLabel::updateTime(){
    currentTime = currentTime.addSecs(-1);
    setText(currentTime.toString("hh:mm:ss"));
    if (currentTime == QTime(0, 0)) {
        stop();
        emit timeout();
    }
}
