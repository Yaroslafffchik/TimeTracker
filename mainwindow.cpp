#include "mainwindow.h"
#include <QDebug>
QString tableStyle = "QTableWidget {"
                     "    background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1E90FF, stop:1 #00FFFF);"
                     "    border: 1px solid #c0c0c0;"
                     "    color: #002c5c;"
                     "    padding: 0px;"
                     "    margin: 0px;"
                     "    font-size: 14px;"
                     "    font-weight: bold;"
                     "    font-family: Arial;"
                     "}"
                     "QHeaderView::section {"
                     "    font-size: 14px;"
                     "    font-weight: bold;"
                     "    font-family: Arial;"
                     "    color: #fafafa;"
                     "    background-color: #23004a;"
                     "}"
                     "QTableWidget::item {"
                     "    padding: 5px;"
                     "}";

QString buttonStyle = "QPushButton {"
                      "    width: 30px;"
                      "    height: 30px;"
                      "    border: 2px solid transparent;"
                      "    border-radius: 10px;"
                      "    color: white;"
                      "    background-color: #7B68EE;"
                      "    font-size: 20px;"
                      "    font-weight: bold;"
                      "    font-family: Tahoma;"
                      "    margin-top: 6px;"
                      "}"
                      "QPushButton:hover {"
                      "    background-color: #8A2BE2;"
                      "}"
                      "QPushButton:pressed {"
                      "    background-color: #800080;"
                      "}";

QString startStopButtonStyle = "QPushButton {"
                               "    width: 100px;"
                               "    height: 50px;"
                               "    border: 2px solid transparent;"
                               "    border-radius: 10px;"
                               "    color: white;"
                               "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4AB3F4, stop:1 #64A6FF);"
                               "    font-size: 14px;"
                               "    font-weight: bold;"
                               "    text-align: center;"
                               "}"
                               "QPushButton:hover {"
                               "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3CA4DE, stop:1 #5597FF);"
                               "}"
                               "QPushButton:pressed {"
                               "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2A8DC8, stop:1 #3E82FF);"
                               "}";

QString mainWindowStyle = "QMainWindow {"
                          "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #170185, stop:1 #007fb5);"
                          "    border-radius: 10px;"
                          "}";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setMinimumSize(1100,700);
    setMaximumSize(1100,700);
    setStyleSheet(mainWindowStyle);
    QPixmap pixLeft(":/resources/img/backL.png");
    QLabel* backgroundLeft = new QLabel(this);
    backgroundLeft->setPixmap(pixLeft);
    QPixmap pixRight(":/resources/img/backR.png");
    QLabel* backgroundRight = new QLabel(this);
    backgroundRight->setPixmap(pixRight);

    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(1);
    backgroundLeft->setSizePolicy(policy);
    backgroundLeft->setFixedWidth(250);
    backgroundRight->setSizePolicy(policy);
    backgroundRight->setFixedWidth(250);
    backgroundLeft->setScaledContents(true);
    backgroundRight->setScaledContents(true);

    //Подключение базы данных:
    myDatabase = new MyDatabase(this);
    myDatabase->openMyDatabase("./TimeTrackerDatabase.db");

    //Создание таблицы:
    taskTableWidget = new QTableWidget(0, 5, this);
    QStringList headers = {"Name", "Description", "Priority", "Time", "Start/Stop"};
    taskTableWidget->setColumnWidth(0,100);
    taskTableWidget->setColumnWidth(1,200);
    taskTableWidget->setColumnWidth(2,100);
    taskTableWidget->setColumnWidth(3,85);
    taskTableWidget->setColumnWidth(4,100);
    taskTableWidget->setHorizontalHeaderLabels(headers);
    taskTableWidget->setStyleSheet(tableStyle);
    loadTasksFromDatabase();

    //Создание кнопок:
    addTaskButton = new QPushButton("Add", this);
    addTaskButton->setStyleSheet(buttonStyle);
    editTaskButton = new QPushButton("Edit", this);
    editTaskButton->setStyleSheet(buttonStyle);
    deleteTaskButton = new QPushButton("Delete", this);
    deleteTaskButton->setStyleSheet(buttonStyle);


    //Компоновка виджетов:
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addTaskButton);
    buttonLayout->addWidget(editTaskButton);
    buttonLayout->addWidget(deleteTaskButton);

    QVBoxLayout *tableLayout = new QVBoxLayout;
    tableLayout->addLayout(buttonLayout);
    tableLayout->addWidget(taskTableWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(backgroundLeft);
    mainLayout->addLayout(tableLayout);
    mainLayout->addWidget(backgroundRight);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    //Подключение кнопок к нужным слотам:
    connect(addTaskButton, &QPushButton::clicked, this, &MainWindow::openTaskDialog);
    connect(editTaskButton, &QPushButton::clicked, this, &MainWindow::openTaskDialogWithSelectedData);
    connect(deleteTaskButton, &QPushButton::clicked, this, &MainWindow::deleteTask);

    //Создание диалогового окна и подключение к слоту добавления:
    taskDialog = new TaskDialog(this);
    connect(taskDialog, &TaskDialog::taskAccepted, this, &MainWindow::addTask);

    notification = new Notification(this);
}

MainWindow::~MainWindow(){
    delete myDatabase;
    delete taskTableWidget;
    delete addTaskButton;
    delete editTaskButton;
    delete deleteTaskButton;
    delete taskDialog;
    delete notification;
}

void MainWindow::openTaskDialog() {
    openTaskDialogWithData("", "", "High", QPointer<TimerLabel>(new TimerLabel(this)));
}

void MainWindow::openTaskDialogWithData(const QString &name, const QString &description, const QString &priority, QPointer<TimerLabel> time) {
    taskDialog->setTaskData(TaskData{name, description, priority, time});
    taskDialog->exec();
}

void MainWindow::openTaskDialogWithSelectedData(){
    //Для множественного редактирования
    disconnect(taskDialog, &TaskDialog::editedTaskAccepted, this, &MainWindow::editTask);
    int selectedRow = taskTableWidget->currentRow();
    if(selectedRow >= 0) {
        TimerLabel *timerLabel = taskTimers.value(selectedRow);
        if (timerLabel){
            if (timerLabel->isRunning()){
                timerLabel->stop();
            }
            timerLabel->setTime(QTime(0,0));
        }
        QString name = taskTableWidget->item(selectedRow, 0)->text();
        QString description = taskTableWidget->item(selectedRow, 1)->text();
        QString priority = taskTableWidget->item(selectedRow, 2)->text();

        taskDialog->rowInTable = selectedRow;
        taskDialog->setExistingTimerLabel(QPointer<TimerLabel>(timerLabel));
        connect(taskDialog, &TaskDialog::editedTaskAccepted, this, &MainWindow::editTask);
        openTaskDialogWithData(name,description,priority,QPointer<TimerLabel>(timerLabel));
    }
}

void MainWindow::addTask(const TaskData &data) {
    int row = taskTableWidget->rowCount();
    myDatabase->appendTask(data, row+1);
    taskTableWidget->insertRow(row);

    taskTableWidget->setItem(row, 0, new QTableWidgetItem(data.name));
    taskTableWidget->setItem(row, 1, new QTableWidgetItem(data.description));
    taskTableWidget->setItem(row, 2, new QTableWidgetItem(data.priority));

    TimerLabel *timerLabel = taskTimers.value(row);
    if (!timerLabel) {
        timerLabel = new TimerLabel(this);
        taskTimers.insert(row, timerLabel);
    }
    timerLabel->setTime(data.time->currentTime);
    timerLabel->setStyleSheet( "QLabel {"
                              "    color: #002c5c;"
                              "    padding: 0px;"
                              "    margin: 0px;"
                              "    font-size: 14px;"
                              "    font-weight: bold;"
                              "    font-family: Arial;"
                              "}");
    connect(timerLabel, &TimerLabel::timeout, this, &MainWindow::handleTimeout);
    taskTableWidget->setCellWidget(row, 3, timerLabel);

    QPushButton* startStopButton = new QPushButton("Start", this);
    startStopButton->setStyleSheet(startStopButtonStyle);
    taskTableWidget->setCellWidget(row, 4, startStopButton);

    connect(startStopButton, &QPushButton::clicked, this, [=]() {
        startStopTimer(startStopButton, timerLabel);
    });
}

void MainWindow::editTask(const TaskData &data, int selectedRow) {
    if (selectedRow >= 0) {
        taskTableWidget->item(selectedRow, 0)->setText(data.name);
        taskTableWidget->item(selectedRow, 1)->setText(data.description);
        taskTableWidget->item(selectedRow, 2)->setText(data.priority);
        TimerLabel *timerLabel = taskTimers.value(selectedRow);
        if (timerLabel) {
            timerLabel->setTime(data.time->currentTime);
        }
        QPushButton* startStopButton = new QPushButton("Start", this);
        startStopButton->setStyleSheet(startStopButtonStyle);
        taskTableWidget->setCellWidget(selectedRow, 4, startStopButton);
        connect(startStopButton, &QPushButton::clicked, this, [=]() {
            startStopTimer(startStopButton, timerLabel);
        });
        myDatabase->updateTask(data,selectedRow+1);
    }
}

void MainWindow::deleteTask() {
    int selectedRow = taskTableWidget->currentRow();
    if (selectedRow >= 0) {
        myDatabase->removeTask(selectedRow+1);
        taskTableWidget->removeRow(selectedRow);

        for (int row = selectedRow; row < taskTableWidget->rowCount(); row++){
            myDatabase->updateTaskId(row + 2, row + 1);
        }

        TimerLabel *timerLabel = taskTimers.take(selectedRow);
        if (timerLabel) {
            delete timerLabel;
        }
    }
}

void MainWindow::startStopTimer(QPushButton* startStopButton, TimerLabel* timerLabel){
    if (timerLabel->text() == "00:00:00") {
        timerLabel->setTime(QTime(0, 0));
    }
    if (timerLabel->isRunning()) {
        timerLabel->stop();
        startStopButton->setText("Start");
    }
    else {
        timerLabel->start();
        startStopButton->setText("Stop");
    }
}

void MainWindow::sendNotification(){
    notification->setNotificationText("The deadline for completing \
                                       the task has expired. Switch \
                                       to another one!");
    notification->show();
}

void MainWindow::handleTimeout(){
    TimerLabel *timerLabel = qobject_cast<TimerLabel*>(sender());
    if (timerLabel) {
        if (timerLabel->currentTime == QTime(0, 0)) {
            QPushButton* startStopButton = nullptr;
            for (int row = 0; row < taskTableWidget->rowCount(); ++row) {
                if (taskTableWidget->cellWidget(row, 3) == timerLabel) {
                    startStopButton = qobject_cast<QPushButton*>(taskTableWidget->cellWidget(row, 4));
                    break;
                }
            }
            if (startStopButton) {
                startStopButton->setText("Delete");
                connect(startStopButton, &QPushButton::clicked, this, &MainWindow::deleteTask);
                sendNotification();
            }
            timerLabel->stop();
        }
    }
}

void MainWindow::loadTasksFromDatabase(){
    QList<TaskData> allTasks = myDatabase->getAllTasks();

    foreach (const TaskData& data, allTasks){
        int row = taskTableWidget->rowCount();
        taskTableWidget->insertRow(row);

        taskTableWidget->setItem(row, 0, new QTableWidgetItem(data.name));
        taskTableWidget->setItem(row, 1, new QTableWidgetItem(data.description));
        taskTableWidget->setItem(row, 2, new QTableWidgetItem(data.priority));

        TimerLabel *timerLabel = taskTimers.value(row);
        if (!timerLabel) {
            timerLabel = new TimerLabel(this);
            taskTimers.insert(row, timerLabel);
        }
        timerLabel->setTime(data.time->currentTime);
        timerLabel->setStyleSheet( "QLabel {"
                                            "    color: #002c5c;"
                                            "    padding: 0px;"
                                            "    margin: 0px;"
                                            "    font-size: 14px;"
                                            "    font-weight: bold;"
                                            "    font-family: Arial;"
                                            "}");
        connect(timerLabel, &TimerLabel::timeout, this, &MainWindow::handleTimeout);
        taskTableWidget->setCellWidget(row, 3, timerLabel);

        QPushButton* startStopButton = new QPushButton("Start", this);
        startStopButton->setStyleSheet(startStopButtonStyle);
        taskTableWidget->setCellWidget(row, 4, startStopButton);

        connect(startStopButton, &QPushButton::clicked, this, [=]() {
            startStopTimer(startStopButton, timerLabel);
        });
    }
}

