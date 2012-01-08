#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
class QLineEdit;
class QListWidget;
class QTextEdit;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateResult();
    void escapeString();
    void setRETypeToolTip();

private:
    QLineEdit *string, *re;
    QTextEdit *result;
    QListWidget *list;
    QComboBox *reStyle;
};

#endif // MAINWINDOW_H
