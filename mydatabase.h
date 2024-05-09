#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

#include "taskdialog.h"

class MyDatabase : public QObject {
    Q_OBJECT
public:
    explicit MyDatabase(QObject *parent = nullptr);
    ~MyDatabase();

private:
    QSqlDatabase myDatabase;

public slots:
    bool openMyDatabase(const QString& path);
    void closeDatabase();
    bool createTaskTable();
    bool appendTask(const TaskData& data, int id);
    bool updateTask(const TaskData& data, int id);
    bool removeTask(int id);
    bool updateTaskId(int id, int newId);
    QList<TaskData> getAllTasks();
};

#endif // MYDATABASE_H
