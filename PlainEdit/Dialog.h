#pragma once

#include "PCH.h"

class MainWindow;
class Editor;
class Highlighter;

class Dialog : public QDialog {
    Q_OBJECT

public:
    Dialog(MainWindow *window, const QString &title, bool modal = false);

protected:
    MainWindow *window;
    Editor *editor;
    
    void keyPressEvent(QKeyEvent *event);
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
    QPushButton *findPrevButton, *findNextButton;

    QCheckBox *newOption(const QString &text, bool *var, int row);
};

class ReplaceDialog : public FindDialog {
    Q_OBJECT

public:
    ReplaceDialog(MainWindow *window);

private:
    QLineEdit *replaceField;
    QPushButton *replaceButton, *replaceAllButton;
};

class GoToDialog : public Dialog {
    Q_OBJECT

public:
    GoToDialog(MainWindow *window);

private:
    QLineEdit *lineField, *columnField;
    QPushButton *goButton;

    bool isFieldValid(QLineEdit *field);
    void go();
};

class AboutDialog : public Dialog {
    Q_OBJECT

public:
    AboutDialog(MainWindow *window);
};
