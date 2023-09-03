#pragma once

#include "PCH.h"

class MainWindow;
class Editor;
class Highlighter;

class Dialog : public QDialog {
    Q_OBJECT

public:
    Dialog(MainWindow *window, const QString &title, bool modal = false);

    void keyPressEvent(QKeyEvent *event);

protected:
    MainWindow *window;
    Editor *editor;
};

class FindDialog : public Dialog {
    Q_OBJECT

public:
    FindDialog(MainWindow *window);
    ~FindDialog();

protected:
    QGridLayout *gridLayout;
    Highlighter *highlighter;
    QLineEdit *findField;
    QPushButton *findPrevButton;
    QPushButton *findNextButton;

    QCheckBox *newOption(const QString &text, bool *var, int row);
};

class ReplaceDialog : public FindDialog {
    Q_OBJECT

public:
    ReplaceDialog(MainWindow *window);

private:
    QLineEdit *replaceField;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
};

class GoToDialog : public Dialog {
    Q_OBJECT

public:
    GoToDialog(MainWindow *window);

private:
    QLineEdit *lineField;
    QLineEdit *columnField;
    QPushButton *goButton;

    bool isFieldValid(QLineEdit *field);
    void go();
};

class AboutDialog : public Dialog {
    Q_OBJECT

public:
    AboutDialog(MainWindow *window);
};
