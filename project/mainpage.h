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

    void setUserName(const QString &userName);
    void setCurrentUser(const QString &userName, const QString &username);
    void showHomePage();
    void showDetailPage();

signals:
    void logoutRequested();

private slots:
    void on_logout_btn_clicked();
    void on_back_btn_clicked();
    void on_add_account_btn_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);


    void on_deposit_btn_clicked();

    void on_withdraw_btn_clicked();

    void on_trasfer_btn_clicked();

private:
    void refreshAccountTable();
    QString formatMoney(int amount) const;
    void updateDetailHeader();

    Ui::MainPage *ui;
    AccountManager manager;
    QString currentUsername;

    // 입금 출금 송금 후 테이블, 잔고 갱신함수
    void refreshUI();
    void updateHistoryTable();
};

#endif // MAINPAGE_H
