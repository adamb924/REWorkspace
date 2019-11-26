#include "mainwindow.h"

#include <QtGui>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

//    string = new QLineEdit("\\uni0644.medi.preAlef");
//    re = new QLineEdit("\\\\uni([abcdefABCDEF1234567890]{4})(?:\\.(init|fina|medi))*(?:\\.([^\\.]*))?");
    string = new QLineEdit;
    re = new QLineEdit;
    result = new QTextEdit;
    list = new QListWidget;

    QFont f = string->font();
    f.setPointSize(20);
    string->setFont(f);
    re->setFont(f);
    result->setFont(f);
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
    layout->addWidget(new QLabel(tr("Result")));
    layout->addWidget(result);
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

    int from = match.capturedStart();
    int len = match.capturedLength();

    if(from != -1)
	output.replace( from , len , "<font color=\"red\">"+input.mid(from,len)+"</font>");
    result->setHtml(output);

    QStringList captures = match.capturedTexts();
    captures.removeAt(0);
    list->clear();
    list->addItems(captures);

    if(!expression.isValid())
    {
	list->setEnabled(false);
	result->setEnabled(false);
    }
    else
    {
	list->setEnabled(true);
	result->setEnabled(true);
    }

    this->setWindowTitle(tr("Regular Expression Workspace"));
}

void MainWindow::escapeString()
{
    QString escaped = re->text().replace("\\","\\\\");
    QInputDialog::getText( this, this->windowTitle(), tr("The C-escaped regular expression:"), QLineEdit::Normal, escaped );
}
