#include "mainwindow.h"

#include <QtGui>
#include <QtWidgets>

/// https://stackoverflow.com/a/14424003/1447002
static void setLineEditTextFormat(QLineEdit* lineEdit, const QList<QTextLayout::FormatRange>& formats)
{
    if(!lineEdit)
        return;

    QList<QInputMethodEvent::Attribute> attributes;
    foreach(const QTextLayout::FormatRange& fr, formats)
    {
        QInputMethodEvent::AttributeType type = QInputMethodEvent::TextFormat;
        int start = fr.start - lineEdit->cursorPosition();
        int length = fr.length;
        QVariant value = fr.format;
        attributes.append(QInputMethodEvent::Attribute(type, start, length, value));
    }
    QInputMethodEvent event(QString(), attributes);
    QCoreApplication::sendEvent(lineEdit, &event);
}

static void clearLineEditTextFormat(QLineEdit* lineEdit)
{
    setLineEditTextFormat(lineEdit, QList<QTextLayout::FormatRange>());
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

//    string = new QLineEdit("\\uni0644.medi.preAlef");
//    re = new QLineEdit("\\\\uni([abcdefABCDEF1234567890]{4})(?:\\.(init|fina|medi))*(?:\\.([^\\.]*))?");
    string = new QLineEdit;
    re = new QLineEdit;
    list = new QListWidget;

    QFont f = string->font();
    f.setPointSize(20);
    string->setFont(f);
    re->setFont(f);
    list->setFont(f);

    QHBoxLayout *reLayout = new QHBoxLayout;

    layout->addWidget(new QLabel(tr("Input")));
    layout->addWidget(string);
    layout->addWidget(new QLabel(tr("Regular Expression")));
    reLayout->addWidget(re,100);

    QPushButton *escapeButton = new QPushButton("\\");
    escapeButton->setToolTip(tr("Get this expression as an escaped, C-style string"));
    connect(escapeButton,SIGNAL(clicked()),this,SLOT(escapeString()));
    reLayout->addWidget(escapeButton,0);

    layout->addLayout(reLayout);
    layout->addWidget(new QLabel(tr("Matches")));
    layout->addWidget(list);

    connect(string,SIGNAL(textChanged(QString)),this,SLOT(updateResult()));
    connect(re,SIGNAL(textChanged(QString)),this,SLOT(updateResult()));

    QWidget *cw = new QWidget;
    cw->setLayout(layout);
    this->setCentralWidget(cw);

    updateResult();
}

MainWindow::~MainWindow()
{

}

void MainWindow::updateResult()
{
    QString input = string->text();
    QString output = input;
    QRegularExpression expression( re->text() );

    QRegularExpressionMatch match = expression.match(input);
    QRegularExpressionMatchIterator mIter = expression.globalMatch(input);

    QTextCharFormat f;
    f.setForeground(QBrush(Qt::red));

    /// https://stackoverflow.com/a/14424003/1447002
    QList<QTextLayout::FormatRange> formats;
    while( mIter.hasNext() )
    {
        QRegularExpressionMatch m = mIter.next();


        QTextLayout::FormatRange fr_tracker;
        fr_tracker.start = m.capturedStart();
        fr_tracker.length = m.capturedLength();
        fr_tracker.format = f;

        formats.append(fr_tracker);
    }

    setLineEditTextFormat(string, formats);

    QStringList captures = match.capturedTexts();
    captures.removeAt(0);
    list->clear();
    list->addItems(captures);

    if(!expression.isValid())
    {
	list->setEnabled(false);
    }
    else
    {
	list->setEnabled(true);
    }

    this->setWindowTitle(tr("Regular Expression Workspace"));
}

void MainWindow::escapeString()
{
    QString escaped = re->text().replace("\\","\\\\");
    QInputDialog::getText( this, this->windowTitle(), tr("The C-escaped regular expression:"), QLineEdit::Normal, escaped );
}
