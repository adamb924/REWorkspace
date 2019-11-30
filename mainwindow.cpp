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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    mInputEdit = new QLineEdit;
    mRegularExpressionEdit = new QLineEdit;
    mCaptureList = new QListWidget;

    QFont f = mInputEdit->font();
    f.setPointSize(20);
    mInputEdit->setFont(f);
    mRegularExpressionEdit->setFont(f);
    mCaptureList->setFont(f);

    QHBoxLayout *reLayout = new QHBoxLayout;

    layout->addWidget(new QLabel(tr("Input")));
    layout->addWidget(mInputEdit);
    layout->addWidget(new QLabel(tr("Regular Expression")));
    reLayout->addWidget(mRegularExpressionEdit,100);

    QPushButton *escapeButton = new QPushButton("\\");
    escapeButton->setToolTip(tr("Get this expression as an escaped, C-style string"));
    connect(escapeButton,SIGNAL(clicked()),this,SLOT(escapeString()));
    reLayout->addWidget(escapeButton,0);

    layout->addLayout(reLayout);
    layout->addWidget(new QLabel(tr("Matches")));
    layout->addWidget(mCaptureList);

    connect(mInputEdit,SIGNAL(textChanged(QString)),this,SLOT(updateResult()));
    connect(mRegularExpressionEdit,SIGNAL(textChanged(QString)),this,SLOT(updateResult()));

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
    QString input = mInputEdit->text();
    QString output = input;
    QRegularExpression expression( mRegularExpressionEdit->text() );

    if(!expression.isValid())
    {
        mCaptureList->setEnabled(false);
        mRegularExpressionEdit->setStyleSheet("QLineEdit { border: 1px solid red; }");
        return;
    }
    else
    {
        mCaptureList->setEnabled(true);
        mRegularExpressionEdit->setStyleSheet("");
    }

    QRegularExpressionMatch match = expression.match(input);
    QRegularExpressionMatchIterator mIter = expression.globalMatch(input);

    QTextCharFormat f;
    f.setForeground(QBrush(Qt::red));

    /// https://stackoverflow.com/a/14424003/1447002
    QList<QTextLayout::FormatRange> formats;
    while( mIter.hasNext() )
    {
        QRegularExpressionMatch m = mIter.next();

        QTextLayout::FormatRange format;
        format.start = m.capturedStart();
        format.length = m.capturedLength();
        format.format = f;

        formats.append(format);
    }

    setLineEditTextFormat(mInputEdit, formats);

    QStringList captures = match.capturedTexts();
    captures.removeAt(0);
    mCaptureList->clear();
    mCaptureList->addItems(captures);

    this->setWindowTitle(tr("Regular Expression Workspace"));
}

void MainWindow::escapeString()
{
    QString escaped = mRegularExpressionEdit->text().replace("\\","\\\\");
    QInputDialog::getText( this, this->windowTitle(), tr("The C-escaped regular expression:"), QLineEdit::Normal, escaped );
}
