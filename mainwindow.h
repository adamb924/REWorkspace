#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class QLineEdit;
class QListWidget;
class QTextEdit;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateResult();
    void escapeString();

private:
    QLineEdit *string, *re;
    QTextEdit *result;
    QListWidget *list;
};

#endif // MAINWINDOW_H
