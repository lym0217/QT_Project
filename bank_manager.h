#ifndef BANK_MANAGER_H
#define BANK_MANAGER_H

#include "account.h"    // 계정관리
#include "account_manager.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class Bank_Manager;
}
QT_END_NAMESPACE





class Bank_Manager : public QMainWindow
{
    Q_OBJECT

public:
    explicit Bank_Manager(QWidget *parent = nullptr);
    ~Bank_Manager() override;

private slots:
    void on_login_btn_clicked();
    void on_back_btn_clicked();
    void on_bank_table_cellDoubleClicked(int row, int column);
    void on_pw_checkBox_stateChanged(int arg1);
    void on_deposit_btn_clicked();
    void on_withdraw_btn_clicked();
    void on_transfer_btn_clicked();
    void on_history_delete_clicked();

private:
    Ui::Bank_Manager *ui;
    AccountManager accountManager;

    QString formatMoney(int amount);        // 금액 ,로 표시해주기
    // 금액? 잔고 업데이트 함수
    void updateAccountTable();
    void addTransaction(QString type, int amount, QString target ="");
    void updateHistoryTable();

    //???????????
    void refreshUI();                // ← 추가 (두 업데이트 함수 묶기)
};



#endif // BANK_MANAGER_H
