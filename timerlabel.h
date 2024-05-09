#ifndef TIMERLABEL_H
#define TIMERLABEL_H

#include <QLabel>
#include <QTimer>
#include <QTime>

class TimerLabel : public QLabel{
    Q_OBJECT
public:
    explicit TimerLabel(QWidget *parent = nullptr);
    QTime currentTime;

private:
    QTimer *timer;

public slots:
    void setTime(const QTime &time);
    void start();
    bool isRunning();
    void stop();

private slots:
    void updateTime();

signals:
    void timeout();
};

#endif // TIMERLABEL_H
