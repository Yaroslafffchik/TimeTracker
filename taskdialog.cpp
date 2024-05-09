#include "taskdialog.h"
QString buttonStyle1 = "QPushButton {"
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
                      "    margin-bottom: 6px;"
                      "    margin-left: 6px;"
                      "}"
                      "QPushButton:hover {"
                      "    background-color: #8A2BE2;"
                      "}"
                      "QPushButton:pressed {"
                      "    background-color: #800080;"
                      "}";

QString dialogStyle = "QDialog {"
                      "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #34495e, stop:1 #2c3e50);"
                      "    border-radius: 10px;"
                      "}";

QString lineEditStyle = "QLineEdit {"
                        "    height: 30px;"
                        "    background-color: #34495e;"
                        "    border: 1px solid #7B68EE;"
                        "    border-radius: 5px;"
                        "    color: white;"
                        "    font-size: 16px;"
                        "    font-weight: bold;"
                        "    font-family: Tahoma;"
                        "    margin-left: 6px;"
                        "}";

QString textEditStyle = "QTextEdit {"
                         "    height: 300px;"
                         "    background-color: #34495e;"
                         "    border: 1px solid #7B68EE;"
                         "    border-radius: 5px;"
                         "    color: white;"
                         "    font-size: 16px;"
                         "    font-weight: bold;"
                         "    font-family: Tahoma;"
                         "    margin-left: 6px;"
                         "}";

QString comboBoxStyle = "QComboBox {"
                        "    height: 30px;"
                        "    background-color: #34495e;"
                        "    border: 1px solid #7B68EE;"
                        "    border-radius: 5px;"
                        "    color: white;"
                        "    font-size: 16px;"
                        "    font-weight: bold;"
                        "    font-family: Tahoma;"
                        "    margin-left: 6px;"
                        "}";

QString timeEditStyle = "QTimeEdit {"
                        "    height: 30px;"
                        "    background-color: #34495e;"
                        "    border: 1px solid #7B68EE;"
                        "    border-radius: 5px;"
                        "    color: white;"
                        "    font-size: 16px;"
                        "    font-weight: bold;"
                        "    font-family: Tahoma;"
                        "    margin-left: 6px;"
                        "}";

QString signStyle = "QLabel {"
                    "       font-size: 30px;"
                    "       font-weight: 30px;"
                    "       font-family: Tahoma;"
                    "       color: white;"
                    "       margin-left: 6px;"
                    "}";

TaskDialog::TaskDialog(QWidget *parent) : QDialog(parent), existingTimerLabel(nullptr) {
    setMinimumSize(800, 600);
    setMaximumSize(800, 600);
    setStyleSheet(dialogStyle);
    QPixmap pixIcon(":/resources/img/icon.png");
    QLabel* iconLabel = new QLabel(this);
    iconLabel->setPixmap(pixIcon);

    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    policy.setHorizontalStretch(0);
    policy.setVerticalStretch(1);
    iconLabel->setSizePolicy(policy);
    iconLabel->setFixedWidth(500);
    iconLabel->setScaledContents(true);

    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setPlaceholderText("up to 50 characters..");
    nameLineEdit->setMaxLength(50);
    nameLineEdit->setStyleSheet(lineEditStyle);

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setStyleSheet(textEditStyle);
    descriptionTextEdit->setPlaceholderText("your description..");
    descriptionTextEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    descriptionTextEdit->setAlignment(Qt::AlignTop);

    priorityComboBox = new QComboBox(this);
    priorityComboBox->setStyleSheet(comboBoxStyle);
    priorityComboBox->addItems({"High","Medium","Low"});

    timeEdit = new QTimeEdit(this);
    timeEdit->setStyleSheet(timeEditStyle);
    timeEdit->setCurrentSection(QTimeEdit::MinuteSection);

    QPushButton *saveButton = new QPushButton("Save", this);
    saveButton->setStyleSheet(buttonStyle1);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    cancelButton->setStyleSheet(buttonStyle1);

    QVBoxLayout *formLayout = new QVBoxLayout;
    formLayout->setContentsMargins(0,0,0,0);

    QLabel *nameLabel = new QLabel;
    nameLabel->setText("Name:");
    nameLabel->setStyleSheet(signStyle);
    formLayout->addWidget(nameLabel);
    formLayout->addWidget(nameLineEdit);

    QLabel *descriptionLabel = new QLabel;
    descriptionLabel->setText("Description:");
    descriptionLabel->setStyleSheet(signStyle);
    formLayout->addWidget(descriptionLabel);
    formLayout->addWidget(descriptionTextEdit);

    QLabel *priorityLabel = new QLabel;
    priorityLabel->setText("Priority:");
    priorityLabel->setStyleSheet(signStyle);
    formLayout->addWidget(priorityLabel);
    formLayout->addWidget(priorityComboBox);

    QLabel *deadlineLabel = new QLabel;
    deadlineLabel->setText("Deadline:");
    deadlineLabel->setStyleSheet(signStyle);
    formLayout->addWidget(deadlineLabel);
    formLayout->addWidget(timeEdit);

    formLayout->addWidget(saveButton);
    formLayout->addWidget(cancelButton);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(iconLabel);

    setLayout(mainLayout);

    connect(saveButton, &QPushButton::clicked, this, &TaskDialog::saveButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void TaskDialog::setTaskData(const TaskData &data) {
    nameLineEdit->setText(data.name);
    descriptionTextEdit->setText(data.description);
    priorityComboBox->setCurrentText(data.priority);
    timeEdit->setTime(data.time->currentTime);
}

void TaskDialog::setExistingTimerLabel(QPointer<TimerLabel> timerLabel) {
    existingTimerLabel = timerLabel;
}

void TaskDialog::saveButtonClicked() {
    TaskData data;
    if (existingTimerLabel) {
        existingTimerLabel->setTime(timeEdit->time());
        data.time = existingTimerLabel;
        data.name = nameLineEdit->text();
        data.description = descriptionTextEdit->toPlainText();
        data.priority = priorityComboBox->currentText();
        emit editedTaskAccepted(data, rowInTable);
    }
    else {
        TimerLabel *newTimerLabel = new TimerLabel(this);
        newTimerLabel->setTime(timeEdit->time());
        data.time = newTimerLabel;
        data.name = nameLineEdit->text();
        data.description = descriptionTextEdit->toPlainText();
        data.priority = priorityComboBox->currentText();
        timeEdit->setTime(QTime(0,0));
        emit taskAccepted(data);
    }

    accept();
}
