#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "account.h"
#include <QList>
#include <QString>

class AccountManager {
public:
    AccountManager();

    QList<Account>& getAccounts();
    Account& getCurrentAccount();
    void setCurrentIndex(int index);
    int getCurrentIndex() const;

    bool deposit(int amount);
    bool withdraw(int amount);
    bool transfer(int amount, QString targetBank, bool isMyAccount, QString fromBank);

    int getTotalBalance() const;

    // ↓ 삭제! Account 클래스 안으로 들어감
    // void addTransaction(QString type, int amount, QString target);

private:
    QList<Account> accounts;
    int currentIndex;
    void loadDummyData();
};

#endif // ACCOUNT_MANAGER_H