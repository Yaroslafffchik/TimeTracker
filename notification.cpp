#include "notification.h"

Notification::Notification(QWidget *parent) : QWidget(parent){
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("notificationOpacity");
    connect(&animation, &QAbstractAnimation::finished, this, &Notification::hide);

    label.setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    label.setStyleSheet("QLabel {"
                        "      color: white; "
                        "      font-size: 30px;"
                        "      font-weight: bold;"
                        "      margin-top: 6px;"
                        "      margin-bottom: 6px;"
                        "      margin-left: 10px;"
                        "      margin-right: 10px;"
                        "}");

    layout.addWidget(&label,0,0);
    setLayout(&layout);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &Notification::startHideAnimation);
}

void Notification::paintEvent(QPaintEvent* event){
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rectangle;
    rectangle.setX(rect().x() + 5);
    rectangle.setY(rect().y() + 5);
    rectangle.setHeight(rect().height() - 10);
    rectangle.setWidth(rect().width() - 10);

    QLinearGradient gradient(rectangle.topLeft(), rectangle.bottomLeft());
    gradient.setColorAt(0, QColor(72,159,217,180));
    gradient.setColorAt(1, QColor(245,166,255,180));

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rectangle, 10, 10);
}

void Notification::setNotificationText(const QString &text){
    label.setText(text);
    label.setWordWrap(true);
    adjustSize();
}

void Notification::show(){
    setWindowOpacity(0.0);

    animation.setDuration(150);
    animation.setStartValue(0.0);
    animation.setEndValue(1.0);

    setGeometry(QGuiApplication::primaryScreen()->availableGeometry().width() - 36 - width() + QGuiApplication::primaryScreen()->availableGeometry().x(),
                QGuiApplication::primaryScreen()->availableGeometry().height() - 36 - height() + QGuiApplication::primaryScreen()->availableGeometry().y(),
                width(),
                height());
    QWidget::show();

    animation.start();
    timer->start(4000);
}

void Notification::startHideAnimation(){
    timer->stop();
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void Notification::hide(){
    if (getNotificationOpacity() == 0.0){
        QWidget::hide();
    }
}

void Notification::setNotificationOpacity(float opacity){
    notificationOpacity = opacity;
    setWindowOpacity(opacity);
}

float Notification::getNotificationOpacity() const {
    return notificationOpacity;
}
