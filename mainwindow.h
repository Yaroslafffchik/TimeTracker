#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QTimer>
#include <QMap>
#include <QSqlDatabase>

#include "taskdialog.h"
#include "timerlabel.h"
#include "notification.h"
#include "mydatabase.h"

//Основной класс приложения (главное окно приложения):
class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    //Конструктор главного окна:
    explicit MainWindow(QWidget *parent = nullptr);
    //Деструктор:
    ~MainWindow();

private:
    //База данных:
    MyDatabase *myDatabase;
    //Таблица с записями:
    QTableWidget *taskTableWidget;
    //Диалоговое окно:
    TaskDialog *taskDialog;
    //Мапа для хранения кастомных виджетов TimerLabel, где ключ - номер строки таблицы:
    QMap<int, TimerLabel*> taskTimers;
    //Уведомление, всплывающее при завершении работы любого таймера таблицы:
    Notification* notification;
    //Кнопки:
    QPushButton *addTaskButton;
    QPushButton *editTaskButton;
    QPushButton *deleteTaskButton;

private slots:
    //Слот для вызова пустого диалогового окна:
    void openTaskDialog();
    //Слот для вызова диалогового окна с передачей данных в его ячейки:
    void openTaskDialogWithData(const QString &name, const QString &description, const QString &priority, QPointer<TimerLabel> time);
    //Слот для вызова диалогового окна с данными из выбранной строки таблицы:
    void openTaskDialogWithSelectedData();
    //Слот для добавления строки в таблицу:
    void addTask(const TaskData &data);
    //Слот для изменения данных в переданной строке таблицы:
    void editTask(const TaskData &data, int selectedRow);
    //Слот для удаления задачи из таблицы:
    void deleteTask();
    //Слот для работы кнопки Start/Stop, связанной с таймеров:
    void startStopTimer(QPushButton* startStopButton, TimerLabel* timerLabel);
    //Слот, который вызывается по истечению времени таймера:
    void handleTimeout();
    //Слот для отправки уведомлений по истечению таймера:
    void sendNotification();
    //Слот для загрузки задач из базы данных:
    void loadTasksFromDatabase();  
};

#endif // MAINWINDOW_H
