#include "account_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace {
Account accountFromJson(const QJsonObject& accountObject)
{
    Account account(accountObject["bank"].toString(),
                    accountObject["account_number"].toString(),
                    accountObject["balance"].toInt(),
                    accountObject["balance_pw"].toInt());

    const QJsonArray history = accountObject["history"].toArray();
    for (const auto& txValue : history) {
        const QJsonObject tx = txValue.toObject();
        account.addTransaction(tx["type"].toString(),
                               tx["amount"].toInt(),
                               tx["target"].toString(),
                               tx["note"].toString(),
                               tx["datetime"].toString());
    }

    return account;
}

QJsonObject transactionToJson(const Transaction& tx)
{
    QJsonObject txObject;
    txObject["type"] = tx.getType();
    txObject["amount"] = tx.getAmount();
    txObject["target"] = tx.getTarget();
    txObject["note"] = tx.getNote();
    txObject["datetime"] = tx.getDatetime();
    return txObject;
}

QJsonObject accountToJson(const Account& account)
{
    QJsonObject accountObject;
    accountObject["account_number"] = account.getAccountNumber();
    accountObject["bank"] = account.getBank();
    accountObject["balance"] = account.getBalance();
    accountObject["balance_pw"] = account.getBalancePw();

    QJsonArray historyArray;
    for (const auto& tx : account.getHistory()) {
        historyArray.append(transactionToJson(tx));
    }
    accountObject["history"] = historyArray;

    return accountObject;
}
}

AccountManager::AccountManager()
    : currentIndex(0)
{
}


// 입금 로직
bool AccountManager::deposit(int amount)
{
    if (!hasValidCurrentAccount() || amount <= 0 || amount > 100000000) {
        return false;
    }

    const QList<Account> backup = accounts;
    accounts[currentIndex].deposit(amount);
    accounts[currentIndex].addTransaction("입금", amount, "본인");

    if (!saveCurrentUserAccounts()) {
        accounts = backup;
        return false;
    }

    return true;
}

// 출금 로직
bool AccountManager::withdraw(int amount)
{
    if (!hasValidCurrentAccount() || amount <= 0 || amount > 100000000) {
        return false;
    }
    if (amount > accounts[currentIndex].getBalance()) {
        return false;
    }

    const QList<Account> backup = accounts;
    accounts[currentIndex].withdraw(amount);
    accounts[currentIndex].addTransaction("출금", amount, "본인");

    if (!saveCurrentUserAccounts()) {
        accounts = backup;
        return false;
    }

    return true;
}

// 송금 로직
bool AccountManager::transfer(int amount,
                              const QString& targetBank,
                              const QString& targetAccountNumber,
                              bool isMyAccount,
                              const QString& fromBank)
{
    if (!hasValidCurrentAccount() || amount <= 0 || amount > 100000000) {
        return false;
    }
    if (amount > accounts[currentIndex].getBalance()) {
        return false;
    }

    const QList<Account> backup = accounts;
    accounts[currentIndex].withdraw(amount);

    if (isMyAccount) {
        int targetIndex = -1;
        for (int i = 0; i < accounts.size(); ++i) {
            if (i == currentIndex) {
                continue;
            }
            if (accounts[i].getAccountNumber() == targetAccountNumber) {
                targetIndex = i;
                break;
            }
        }

        if (targetIndex < 0) {
            accounts = backup;
            return false;
        }

        accounts[targetIndex].deposit(amount);
        accounts[targetIndex].addTransaction("입금", amount, "[My] " + fromBank);
        accounts[currentIndex].addTransaction("송금", amount, "[My] " + accounts[targetIndex].getBank());
    } else {
        accounts[currentIndex].addTransaction("송금", amount, targetBank);
    }

    if (!saveCurrentUserAccounts()) {
        accounts = backup;
        return false;
    }

    return true;
}

bool AccountManager::loadFromJsonByUsername(const QString& filePath, const QString& username)
{
    const QString selectedAccountNumber =
        hasValidCurrentAccount() ? accounts[currentIndex].getAccountNumber() : QString();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        accounts.clear();
        currentFilePath.clear();
        currentUsername.clear();
        currentIndex = 0;
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    const QJsonArray users = doc.object()["users"].toArray();
    file.close();

    for (const auto& userValue : users) {
        const QJsonObject user = userValue.toObject();

        if (user["username"].toString() != username) {
            continue;
        }

        accounts.clear();
        const QJsonArray accountArray = user["accounts"].toArray();
        for (const auto& accountValue : accountArray) {
            accounts.append(accountFromJson(accountValue.toObject()));
        }

        currentFilePath = filePath;
        currentUsername = username;
        currentIndex = 0;

        for (int i = 0; i < accounts.size(); ++i) {
            if (accounts[i].getAccountNumber() == selectedAccountNumber) {
                currentIndex = i;
                break;
            }
        }
        return true;
    }

    accounts.clear();
    currentFilePath.clear();
    currentUsername.clear();
    currentIndex = 0;
    return false;
}

bool AccountManager::addAccountToUser(const QString& filePath,
                                      const QString& username,
                                      const QString& bank,
                                      const QString& accountNumber,
                                      int balance,
                                      int balancePw,
                                      QString& message)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        message = "사용자 파일을 열 수 없습니다.";
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();
    QJsonArray users = root["users"].toArray();
  
    for (int i = 0; i < users.size(); ++i) {
        QJsonObject user = users[i].toObject();

        if (user["username"].toString() != username) continue;


        QJsonArray accountArray = user["accounts"].toArray();
        for (const auto& accVal : accountArray) {
            const QJsonObject acc = accVal.toObject();
            if (acc["account_number"].toString() == accountNumber) {
                message = "이미 등록된 계좌번호입니다.";
                return false;
            }
        }

        QJsonObject newAccount;
        newAccount["account_number"] = accountNumber;
        newAccount["bank"] = bank;
        newAccount["balance"] = balance;
        newAccount["balance_pw"] = balancePw;
        newAccount["history"] = QJsonArray();

        accountArray.append(newAccount);
        user["accounts"] = accountArray;
        users[i] = user;
        root["users"] = users;

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            message = "계좌 정보를 저장할 수 없습니다.";
            return false;
        }

        file.write(QJsonDocument(root).toJson());
        file.close();

        message = "계좌가 추가되었습니다.";
        return true;
    }

    message = "로그인한 사용자 정보를 찾을 수 없습니다.";
    return false;
}

// 원본 계좌 호출
QList<Account>& AccountManager::getAccounts()
{
    return accounts;
}
// 현재 계좌 호출 + 순서
Account& AccountManager::getCurrentAccount()
{
    return accounts[currentIndex];
}
// 클릭한 행
void AccountManager::setCurrentIndex(int index)
{
    currentIndex = index;
}
// 클릭한 행 호출
int AccountManager::getCurrentIndex() const
{
    return currentIndex;
}
// balance 잔액 전체 합
int AccountManager::getTotalBalance() const
{
    int total = 0;
    for (const auto& a : accounts) total += a.getBalance();
    return total;
}

bool AccountManager::hasValidCurrentAccount() const
{
    return currentIndex >= 0 && currentIndex < accounts.size();
}

bool AccountManager::saveCurrentUserAccounts()
{
    if (currentFilePath.isEmpty() || currentUsername.isEmpty()) {
        return false;
    }

    QFile file(currentFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();
    QJsonArray users = root["users"].toArray();

    for (int i = 0; i < users.size(); ++i) {
        QJsonObject user = users[i].toObject();
        if (user["username"].toString() != currentUsername) {
            continue;
        }

        QJsonArray accountArray;
        for (const auto& account : accounts) {
            accountArray.append(accountToJson(account));
        }

        user["accounts"] = accountArray;
        users[i] = user;
        root["users"] = users;

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            return false;
        }

        file.write(QJsonDocument(root).toJson());
        file.close();
        return true;
    }

    return false;
}
