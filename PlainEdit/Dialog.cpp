#include "Dialog.h"
#include "MainWindow.h"
#include "Editor.h"
#include "Attr.h"

Dialog::Dialog(MainWindow *window, const QString &title, bool modal)
    : QDialog(window, Qt::MSWindowsFixedSizeDialogHint) {
    this->window = window;
    editor = window->getEditor();

    setWindowTitle(title);
    setModal(modal);
    setAttribute(Qt::WA_DeleteOnClose);
}

void Dialog::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

FindDialog::FindDialog(MainWindow *window) : Dialog(window, "Find") {
    gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(30, 30, 30, 30);

    highlighter = new Highlighter(editor);
    highlighter->updateTarget();

    findField = new QLineEdit(Attr::findTarget, this);
    findField->setFixedWidth(200);
    findField->setPlaceholderText("Find...");
    connect(findField, &QLineEdit::textChanged, this, [this] {
        Attr::findTarget = findField->text();
        highlighter->updateTarget();
        findPrevButton->setEnabled(Attr::findTarget != "");
        findNextButton->setEnabled(Attr::findTarget != "");
    });
    gridLayout->addWidget(findField, 0, 0);

    findPrevButton = new QPushButton("Find Prev", this);
    findPrevButton->setEnabled(Attr::findTarget != "");
    connect(findPrevButton, &QPushButton::clicked, this, [this] {
        if (editor->findPrevious().isNull()) {
            QMessageBox::critical(this, "PlainEdit",
                                  "Cannot find \"" + Attr::findTarget + "\"!");
        }
    });
    gridLayout->addWidget(findPrevButton, 0, 1);

    findNextButton = new QPushButton("Find Next", this);
    findNextButton->setEnabled(Attr::findTarget != "");
    connect(findNextButton, &QPushButton::clicked, this, [this] {
        if (editor->findNext().isNull()) {
            QMessageBox::critical(this, "PlainEdit",
                                  "Cannot find \"" + Attr::findTarget + "\"!");
        }
    });
    findNextButton->setDefault(true);
    gridLayout->addWidget(findNextButton, 0, 2);
    gridLayout->addItem(new QSpacerItem(0, 30), 2, 0);

    connect(findField, &QLineEdit::returnPressed,
            findNextButton, &QPushButton::click);

    newOption("Match Case", &Attr::matchCase, 3);
    newOption("Match Whole Word", &Attr::matchWholeWord, 4);
}

FindDialog::~FindDialog() {
    editor->blockSignals(true);
    highlighter->setDocument(nullptr);
    highlighter->deleteLater();
    editor->blockSignals(false);
}

QCheckBox *FindDialog::newOption(const QString &text, bool *var, int row) {
    auto box = new QCheckBox(text, this);
    box->setChecked(*var);
    connect(box, &QCheckBox::stateChanged, this, [this, box, var] {
        *var = box->isChecked();
        highlighter->updateTarget();
    });
    gridLayout->addWidget(box, row, 0);
    return box;
}

ReplaceDialog::ReplaceDialog(MainWindow *window) : FindDialog(window) {
    setWindowTitle("Replace");

    connect(findField, &QLineEdit::textChanged, this, [this] {
        replaceButton->setEnabled(Attr::findTarget != "");
        replaceAllButton->setEnabled(Attr::findTarget != "");
    });

    replaceField = new QLineEdit(Attr::replaceTarget, this);
    replaceField->setPlaceholderText("Replace...");
    connect(replaceField, &QLineEdit::returnPressed,
            findNextButton, &QPushButton::click);
    connect(replaceField, &QLineEdit::textChanged, this, [this] {
        Attr::replaceTarget = replaceField->text();
    });
    gridLayout->addWidget(replaceField, 1, 0);

    replaceButton = new QPushButton("Replace", this);
    connect(replaceButton, &QPushButton::clicked, this, [this] {
        editor->replace();
    });
    gridLayout->addWidget(replaceButton, 1, 1);

    replaceAllButton = new QPushButton("Replace All", this);
    connect(replaceAllButton, &QPushButton::clicked, this, [this] {
        editor->replaceAll();
    });
    gridLayout->addWidget(replaceAllButton, 1, 2);
}

GoToDialog::GoToDialog(MainWindow *window) : Dialog(window, "Go To", true) {
    auto hboxLayout = new QHBoxLayout(this);
    hboxLayout->setSpacing(5);
    hboxLayout->setContentsMargins(30, 30, 30, 30);

    lineField = new QLineEdit(this);
    lineField->setPlaceholderText("Line");
    connect(lineField, &QLineEdit::textChanged, this, [this] {
        bool valid = isFieldValid(lineField) && isFieldValid(columnField);
        goButton->setEnabled(valid);
    });
    hboxLayout->addWidget(lineField);

    columnField = new QLineEdit(this);
    columnField->setPlaceholderText("Column");
    connect(columnField, &QLineEdit::textChanged, this, [this] {
        bool valid = isFieldValid(lineField) && isFieldValid(columnField);
        goButton->setEnabled(valid);
    });
    hboxLayout->addWidget(columnField);

    goButton = new QPushButton("Go", this);
    goButton->setEnabled(false);
    connect(goButton, &QPushButton::clicked, this, &GoToDialog::go);
    hboxLayout->addWidget(goButton);

    connect(lineField, &QLineEdit::returnPressed,
            goButton, &QPushButton::click);
    connect(columnField, &QLineEdit::returnPressed,
            goButton, &QPushButton::click);
}

bool GoToDialog::isFieldValid(QLineEdit *field) {
    static QRegularExpression ex("^(?:0|[1-9]\\d*)(?:\\.\\d+)?$");
    return field->text() != "" && ex.match(field->text()).hasMatch();
}

void GoToDialog::go() {
    int line = lineField->text().toInt() - 1;
    int column = columnField->text().toInt() - 1;
    int numLines = editor->blockCount();
    int numColumns = editor->document()->findBlockByLineNumber(line).length();

    if (line > numLines || column > numColumns) {
        QMessageBox::critical(this, "PlainEdit",
                              "Cannot locate the given position!");
        return;
    }

    editor->goTo(line, column);
    close();
}

AboutDialog::AboutDialog(MainWindow *window)
    : Dialog(window, "About PlainEdit", true) {
    auto gridLayout = new QGridLayout(this);
    gridLayout->setHorizontalSpacing(10);
    gridLayout->setVerticalSpacing(10);
    gridLayout->setContentsMargins(30, 30, 30, 30);

    auto logoButton = new QPushButton(this);
    logoButton->setIcon(getIcon("Logo.png"));
    logoButton->setIconSize(QSize(200, 200));
    logoButton->setStyleSheet("border: 0");
    gridLayout->addWidget(logoButton, 0, 0, 2, 1);

    auto textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setHtml(File::readAll("About.html"));
    gridLayout->addWidget(textEdit, 0, 1, 1, 2);

    auto webButton = new QPushButton("Website", this);
    connect(webButton, &QPushButton::clicked, this, [] {
        static QUrl url("https://github.com/HenryZhao2020/PlainEdit");
        QDesktopServices::openUrl(url);
    });
    gridLayout->addWidget(webButton, 1, 1);

    auto qtButton = new QPushButton("About Qt", this);
    connect(qtButton, &QPushButton::clicked, this, &QApplication::aboutQt);
    gridLayout->addWidget(qtButton, 1, 2);
}
