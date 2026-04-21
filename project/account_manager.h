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

    bool loadFromJsonByUsername(const QString& filePath, const QString& username);
    bool addAccountToUser(const QString& filePath,
                          const QString& username,
                          const QString& bank,
                          const QString& accountNumber,
                          int balance,
                          int balancePw,
                          QString& message);

    bool deposit(int amount);           //입금
    bool withdraw(int amount);          //출금
    bool transfer(int amount,
                  const QString& targetBank,
                  const QString& targetAccountNumber,
                  bool isMyAccount,
                  const QString& fromBank); //송금
private:
    bool hasValidCurrentAccount() const;
    bool saveCurrentUserAccounts();

    QList<Account> accounts;
    int currentIndex;
    QString currentFilePath;
    QString currentUsername;
};

#endif // ACCOUNT_MANAGER_H
