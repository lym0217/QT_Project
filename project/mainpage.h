#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "account_manager.h"

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_pushButton_4_clicked();

private:
    Ui::MainPage *ui;
    // AccountManager manager변수 선언
    AccountManager manager;
};

#endif // MAINPAGE_H
