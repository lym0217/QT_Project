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
    
    // UI표시 유용하게
    QString formatMoney(int amount);        // 금액에 000,000로 표시해주기
    void updateAccountTable();              // bank_table 계좌목록 갱신     
    void updateHistoryTable();              // 거래내역 갱신
    void refreshUI();                       // 위 두 함수 + account_label 한번에 갱신
                                            // 입금, 출금, 송금을 한번에 업데이트 
};



#endif // BANK_MANAGER_H
