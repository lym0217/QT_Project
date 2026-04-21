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
    int  getCurrentIndex() const;
    int  getTotalBalance() const;

    bool loadFromJson(const QString& filePath, int userId);  // Json 불러오기
    bool saveToJson(const QString& filePath, int userId);    // Json에 저장

    bool deposit(int amount);           //입금
    bool withdraw(int amount);          //출금
    bool transfer(int amount, QString targetBank, bool isMyAccount, QString fromBank);
                                        //송금
private:
    QList<Account> accounts;
    int currentIndex;
    QString currentFilePath;  // saveToJson 편하게 쓰려고
    int currentUserId;
};

#endif // ACCOUNT_MANAGER_H