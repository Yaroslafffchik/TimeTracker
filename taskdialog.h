#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QTimeEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPointer>

#include "timerlabel.h"

struct TaskData {
    QString name;
    QString description;
    QString priority;
    QPointer<TimerLabel> time;
};

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget *parent = nullptr);
    int rowInTable;

public slots:
    void setTaskData(const TaskData &data);
    void setExistingTimerLabel(QPointer<TimerLabel> timerLabel);

signals:
    void taskAccepted(const TaskData &data);
    void editedTaskAccepted(const TaskData& data, int row);

private slots:
    void saveButtonClicked();

private:
    QLineEdit *nameLineEdit;
    QTextEdit *descriptionTextEdit;
    QComboBox *priorityComboBox;
    QTimeEdit *timeEdit;
    QPointer<TimerLabel> existingTimerLabel;
};

#endif // TASKDIALOG.H
