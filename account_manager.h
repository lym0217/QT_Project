#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "account.h"
#include <QList>
#include <QString>


// 계좌 목록 정리
class AccountManager {
public:
    AccountManager();
    
    //계좌 목록 관련
    QList<Account>& getAccounts();              // 전체 계좌 목록
    Account& getCurrentAccount();               // 현재 계좌 목록
    void setCurrentIndex(int index);            // 현재 계좌 변경(더블 클릭하면 호출하는 구조)
    int getCurrentIndex() const;                // 현재 계좌 이름, 번호 반환
    
    //거래 관련
    bool deposit(int amount);                   //입금
    bool withdraw(int amount);                  //출금
    bool transfer(int amount, QString targetBank, bool isMyAccount, QString fromBank);
    // 송금
    
    int getTotalBalance() const;                // 전체 계좌 잔액 합산

private:
    QList<Account> accounts; 
    int currentIndex;
    void loadDummyData();
};

#endif // ACCOUNT_MANAGER_H
