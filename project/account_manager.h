#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H
#include "account.h"
#include <QList>
#include <QString>

class AccountManager {
public:
    AccountManager();

    QList<Account>& getAccounts();
    const QList<Account>& getAccounts() const;
    Account& getCurrentAccount();
    void setCurrentIndex(int index);
    int  getCurrentIndex() const;
    int  getTotalBalance() const;

    bool loadFromJson(const QString& filePath, int userId);  // 추가
    bool loadFromJsonByUsername(const QString& filePath, const QString& username);
    bool addAccountToUser(const QString& filePath,
                          const QString& username,
                          const QString& bank,
                          const QString& accountNumber,
                          int balance,
                          int balancePw,
                          QString& message);
    bool depositToAccount(const QString& filePath,
                          const QString& username,
                          const QString& ownerName,
                          const QString& accountNumber,
                          int amount,
                          int balancePw,
                          QString& message);
    bool withdrawFromAccount(const QString& filePath,
                             const QString& username,
                             const QString& ownerName,
                             const QString& accountNumber,
                             int amount,
                             int balancePw,
                             QString& message);
    bool transferBetweenAccounts(const QString& filePath,
                                 const QString& username,
                                 const QString& ownerName,
                                 const QString& fromAccountNumber,
                                 const QString& targetBank,
                                 const QString& targetAccountNumber,
                                 int amount,
                                 int balancePw,
                                 bool ownAccountTransfer,
                                 QString& message);
    bool updateTransactionNote(const QString& filePath,
                               const QString& username,
                               const QString& accountNumber,
                               int historyIndex,
                               const QString& note);

private:
    QList<Account> accounts;
    int currentIndex;
};

#endif // ACCOUNT_MANAGER_H
