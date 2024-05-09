#include "mydatabase.h"

MyDatabase::MyDatabase(QObject *parent) : QObject(parent) {}

MyDatabase::~MyDatabase() {
    closeDatabase();
}

bool MyDatabase::openMyDatabase(const QString &path) {
    myDatabase = QSqlDatabase::addDatabase("QSQLITE");
    myDatabase.setDatabaseName(path);
    if (myDatabase.open()) {
        createTaskTable();
        return true;
    } else {
        qDebug() << "Error: Unable to open database:" << myDatabase.lastError().text();
        return false;
    }
}


void MyDatabase::closeDatabase() {
    if (myDatabase.isOpen()) {
        myDatabase.close();
    }
}

bool MyDatabase::createTaskTable() {
    QSqlQuery request;
    request.prepare("CREATE TABLE IF NOT EXISTS allTasks ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "name TEXT NOT NULL, "
                    "description TEXT, "
                    "priority TEXT, "
                    "time TEXT)");

    if (!request.exec()) {
        qDebug() << "Error creating table:" << request.lastError().text();
        return false;
    }
    return true;
}


bool MyDatabase::appendTask(const TaskData &data, int id) {
    QSqlQuery request;

    request.prepare("INSERT INTO allTasks (id, name, description, priority, time) "
                    "VALUES (:id, :name, :description, :priority, :time)");

    request.bindValue(":id", id);
    request.bindValue(":name", data.name);
    request.bindValue(":description", data.description);
    request.bindValue(":priority", data.priority);
    request.bindValue(":time", data.time->currentTime.toString("hh:mm:ss"));

    if (!request.exec()) {
        qDebug() << "Error appending task:" << request.lastError().text();
        return false;
    }
    return true;
}


bool MyDatabase::updateTask(const TaskData &data, int id) {
    QSqlQuery request;

    request.prepare("UPDATE allTasks SET name = :name, description = :description, "
                    "priority = :priority, time = :time WHERE id = :id");

    request.bindValue(":name", data.name);
    request.bindValue(":description", data.description);
    request.bindValue(":priority", data.priority);
    request.bindValue(":time", data.time->currentTime.toString("hh:mm:ss"));
    request.bindValue(":id", id);

    if (!request.exec()) {
        qDebug() << "Error updating task:" << request.lastError().text();
        return false;
    }
    return true;
}


bool MyDatabase::removeTask(int id) {
    QSqlQuery request;

    request.prepare("DELETE FROM allTasks WHERE id = :id");
    request.bindValue(":id", id);

    if (!request.exec()) {
        qDebug() << "Error removing task:" << request.lastError().text();
        return false;
    }
    return true;
}

bool MyDatabase::updateTaskId(int id, int newId) {
    QSqlQuery request;

    request.prepare("UPDATE allTasks SET id = :newId WHERE id = :id");
    request.bindValue(":newId", newId);
    request.bindValue(":id", id);

    if (!request.exec()) {
        qDebug() << "Error updating task ID:" << request.lastError().text();
        return false;
    }
    return true;
}


QList<TaskData> MyDatabase::getAllTasks() {
    QList<TaskData> allTasks;

    QSqlQuery request("SELECT * FROM allTasks");
    while (request.next()) {
        TaskData data;
        data.name = request.value("name").toString();
        data.description = request.value("description").toString();
        data.priority = request.value("priority").toString();
        QString timeString = request.value("time").toString();
        data.time = QPointer<TimerLabel>(new TimerLabel(nullptr));
        data.time->setTime(QTime::fromString(timeString, "hh:mm:ss"));
        allTasks.append(data);
    }

    return allTasks;
}
