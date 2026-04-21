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


    void on_deposit_btn_clicked();

    void on_withdraw_btn_clicked();

    void on_trasfer_btn_clicked();

private:
    Ui::MainPage *ui;
    // AccountManager manager변수 선언
    AccountManager manager;

    // 입금 출금 송금 후 테이블, 잔고 갱신함수
    void refreshUI();
    void updateHistoryTable();
};

#endif // MAINPAGE_H
