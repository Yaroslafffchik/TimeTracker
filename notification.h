#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPainter>
#include <QApplication>
#include <QScreen>

class Notification : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(float notificationOpacity READ getNotificationOpacity WRITE setNotificationOpacity)

    void setNotificationOpacity(float opacity);
    float getNotificationOpacity() const;

public:
    explicit Notification(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

public slots:
    void setNotificationText(const QString& text);
    void show();

private slots:
    void startHideAnimation();
    void hide();

private:
    QLabel label;
    QGridLayout layout;
    QPropertyAnimation animation;
    float notificationOpacity;
    QTimer *timer;

};

#endif // NOTIFICATION_H
