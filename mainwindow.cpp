#include "mainwindow.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    string = new QLineEdit("\\uni0644.medi.preAlef");
    re = new QLineEdit("\\\\uni([abcdefABCDEF1234567890]{4})(?:\\.(init|fina|medi))*(?:\\.([^\\.]*))?");
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

    reStyle = new QComboBox;
    reStyle->addItem("RegExp");
    reStyle->addItem("RegExp2");
    reStyle->addItem("Wildcard");
    reStyle->addItem("Wildcard");
    reStyle->addItem("WildcardUnix");
    reStyle->addItem("FixedString");
    reStyle->addItem("W3CXmlSchema11");
    connect(reStyle,SIGNAL(currentIndexChanged(QString)),this,SLOT(setRETypeToolTip()));
    connect(reStyle,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateResult()));
    reLayout->addWidget(reStyle);

    QPushButton *escapeButton = new QPushButton("\\");
    connect(escapeButton,SIGNAL(clicked()),this,SLOT(escapeString()));
    reLayout->addWidget(escapeButton,0);

    layout->addLayout(reLayout);
    layout->addWidget(new QLabel(tr("Result")));
    layout->addWidget(result);
    layout->addWidget(new QLabel(tr("Matchs")));
    layout->addWidget(list);

    connect(string,SIGNAL(textChanged(QString)),this,SLOT(updateResult()));
    connect(re,SIGNAL(textChanged(QString)),this,SLOT(updateResult()));

    QWidget *cw = new QWidget;
    cw->setLayout(layout);
    this->setCentralWidget(cw);

    updateResult();
    setRETypeToolTip();
}

MainWindow::~MainWindow()
{

}

void MainWindow::updateResult()
{
    QString input = string->text();
    QString output = input;
    QRegExp expression( re->text() );

    QString strCurrent = reStyle->currentText();
    if(strCurrent == "RegExp") {
	expression.setPatternSyntax(QRegExp::RegExp);
    }
    else if(strCurrent == "RegExp2") {
	expression.setPatternSyntax(QRegExp::RegExp2);
    }
    else if(strCurrent == "Wildcard") {
	expression.setPatternSyntax(QRegExp::Wildcard);
    }
    else if(strCurrent == "WildcardUnix") {
	expression.setPatternSyntax(QRegExp::WildcardUnix);
    }
    else if(strCurrent == "FixedString") {
	expression.setPatternSyntax(QRegExp::FixedString);
    }
    else if(strCurrent == "W3CXmlSchema11") {
	expression.setPatternSyntax(QRegExp::W3CXmlSchema11);
    }


    int from = expression.indexIn(input);
    int len = expression.matchedLength();

    if(from != -1)
	output.replace( from , len , "<font color=\"red\">"+input.mid(from,len)+"</font>");
    result->setHtml(output);

    QStringList captures = expression.capturedTexts();
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

void MainWindow::setRETypeToolTip()
{
    QString strCurrent = reStyle->currentText();
    if(strCurrent == "RegExp") {
	reStyle->setToolTip("A rich Perl-like pattern matching syntax. This is the default.");
    }
    else if(strCurrent == "RegExp2") {
	reStyle->setToolTip("Like RegExp, but with greedy quantifiers. This will be the default in Qt 5. (Introduced in Qt 4.2.)");
    }
    else if(strCurrent == "Wildcard") {
	reStyle->setToolTip("This provides a simple pattern matching syntax similar to that used by shells (command interpreters) for \"file globbing\". See Wildcard Matching.");
    }
    else if(strCurrent == "WildcardUnix") {
	reStyle->setToolTip("This is similar to Wildcard but with the behavior of a Unix shell. The wildcard characters can be escaped with the character \"\\\".");
    }
    else if(strCurrent == "FixedString") {
	reStyle->setToolTip("The pattern is a fixed string. This is equivalent to using the RegExp pattern on a string in which all metacharacters are escaped using escape().");
    }
    else if(strCurrent == "W3CXmlSchema11") {
	reStyle->setToolTip("The pattern is a regular expression as defined by the W3C XML Schema 1.1 specification.");
    }
}
