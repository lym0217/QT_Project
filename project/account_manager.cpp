#include "account_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

namespace {
QString currentTimestamp()
{
    return QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
}

QJsonObject createHistoryObject(const QString& type,
                                int amount,
                                const QString& target,
                                const QString& note = QString())
{
    QJsonObject history;
    history["datetime"] = currentTimestamp();
    history["type"] = type;
    history["target"] = target;
    history["amount"] = amount;
    history["note"] = note;
    return history;
}
}

AccountManager::AccountManager()
{
    m_currentIndex = 0;
}

bool AccountManager::loadFromJsonByUsername(const QString& filePath, const QString& username)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray users = doc.object()["users"].toArray();

    m_accounts.clear();

    for (const auto& userVal : users) {
        QJsonObject user = userVal.toObject();

        if (user["username"].toString() != username) continue;

        QJsonArray accs = user["accounts"].toArray();
        for (const auto& accVal : accs) {
            QJsonObject acc = accVal.toObject();
            const QString bank = acc["bank"].toString();
            const QString accNum = acc["account_number"].toString();
            const int balance = acc["balance"].toInt();
            Account account(bank, accNum, balance);
            const QJsonArray historyArray = acc["history"].toArray();
            for (const auto& historyVal : historyArray) {
                const QJsonObject historyObj = historyVal.toObject();
                account.addTransaction(historyObj["type"].toString(),
                                       historyObj["amount"].toInt(),
                                       historyObj["target"].toString(),
                                       historyObj["note"].toString(),
                                       historyObj["datetime"].toString());
            }
            m_accounts << account;
        }
        return true;
    }

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

bool AccountManager::depositToAccount(const QString& filePath,
                                      const QString& username,
                                      const QString& ownerName,
                                      const QString& accountNumber,
                                      int amount,
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

        QJsonArray accountsArray = user["accounts"].toArray();
        for (int j = 0; j < accountsArray.size(); ++j) {
            QJsonObject account = accountsArray[j].toObject();
            if (account["account_number"].toString() != accountNumber) continue;

            if (account["balance_pw"].toInt() != balancePw) {
                message = "계좌 비밀번호가 올바르지 않습니다.";
                return false;
            }
            // 1억원 초과 메세지
            if (amount > 100000000) {
                message = "1회 입금 한도(1억 원)를 초과할 수 없습니다.";
                return false;
            }

            account["balance"] = account["balance"].toInt() + amount;
            QJsonArray history = account["history"].toArray();
            history.append(createHistoryObject("입금",
                                               amount,
                                               ownerName + ", " + account["bank"].toString() + ", " + accountNumber));
            account["history"] = history;
            accountsArray[j] = account;
            user["accounts"] = accountsArray;
            users[i] = user;
            root["users"] = users;

            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                message = "입금 정보를 저장할 수 없습니다.";
                return false;
            }

            file.write(QJsonDocument(root).toJson());
            file.close();
            message = "입금이 완료되었습니다.";
            return true;
        }
    }

    message = "계좌를 찾을 수 없습니다.";
    return false;
}

bool AccountManager::withdrawFromAccount(const QString& filePath,
                                         const QString& username,
                                         const QString& ownerName,
                                         const QString& accountNumber,
                                         int amount,
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

        QJsonArray accountsArray = user["accounts"].toArray();
        for (int j = 0; j < accountsArray.size(); ++j) {
            QJsonObject account = accountsArray[j].toObject();
            if (account["account_number"].toString() != accountNumber) continue;

            if (account["balance_pw"].toInt() != balancePw) {
                message = "계좌 비밀번호가 올바르지 않습니다.";
                return false;
            }
            // 1억원 초과 메세지
            if (amount > 100000000) {
                message = "1회 입금 한도(1억 원)를 초과할 수 없습니다.";
                return false;
            }

            if (account["balance"].toInt() < amount) {
                message = "잔액이 부족합니다.";
                return false;
            }

            account["balance"] = account["balance"].toInt() - amount;
            QJsonArray history = account["history"].toArray();
            history.append(createHistoryObject("출금",
                                               amount,
                                               ownerName + ", " + account["bank"].toString() + ", " + accountNumber));
            account["history"] = history;
            accountsArray[j] = account;
            user["accounts"] = accountsArray;
            users[i] = user;
            root["users"] = users;

            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                message = "출금 정보를 저장할 수 없습니다.";
                return false;
            }

            file.write(QJsonDocument(root).toJson());
            file.close();
            message = "출금이 완료되었습니다.";
            return true;
        }
    }

    message = "계좌를 찾을 수 없습니다.";
    return false;
}

bool AccountManager::transferBetweenAccounts(const QString& filePath,
                                             const QString& username,
                                             const QString& ownerName,
                                             const QString& fromAccountNumber,
                                             const QString& targetBank,
                                             const QString& targetAccountNumber,
                                             int amount,
                                             int balancePw,
                                             bool ownAccountTransfer,
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

    int senderUserIndex = -1;
    int senderAccountIndex = -1;

    for (int i = 0; i < users.size(); ++i) {
        const QJsonObject user = users[i].toObject();
        if (user["username"].toString() != username) continue;

        const QJsonArray accountsArray = user["accounts"].toArray();
        for (int j = 0; j < accountsArray.size(); ++j) {
            const QJsonObject account = accountsArray[j].toObject();
            if (account["account_number"].toString() == fromAccountNumber) {
                senderUserIndex = i;
                senderAccountIndex = j;
                break;
            }
        }
        break;
    }

    if (senderUserIndex < 0 || senderAccountIndex < 0) {
        message = "송금 계좌를 찾을 수 없습니다.";
        return false;
    }

    QJsonObject senderUser = users[senderUserIndex].toObject();
    QJsonArray senderAccounts = senderUser["accounts"].toArray();
    QJsonObject senderAccount = senderAccounts[senderAccountIndex].toObject();

    if (senderAccount["balance_pw"].toInt() != balancePw) {
        message = "계좌 비밀번호가 올바르지 않습니다.";
        return false;
    }
    // 1억원 초과 메세지
    if (amount > 100000000) {
        message = "1회 입금 한도(1억 원)를 초과할 수 없습니다.";
        return false;
    }

    if (senderAccount["balance"].toInt() < amount) {
        message = "잔액이 부족합니다.";
        return false;
    }

    int targetUserIndex = -1;
    int targetAccountIndex = -1;

    for (int i = 0; i < users.size(); ++i) {
        QJsonObject user = users[i].toObject();
        QJsonArray accountsArray = user["accounts"].toArray();
        for (int j = 0; j < accountsArray.size(); ++j) {
            const QJsonObject account = accountsArray[j].toObject();
            if (account["bank"].toString() == targetBank &&
                account["account_number"].toString() == targetAccountNumber) {
                targetUserIndex = i;
                targetAccountIndex = j;
                break;
            }
        }
        if (targetUserIndex >= 0) break;
    }

    if (ownAccountTransfer && (targetUserIndex < 0 || targetAccountIndex < 0)) {
        message = "선택한 내 계좌를 찾을 수 없습니다.";
        return false;
    }

    senderAccount["balance"] = senderAccount["balance"].toInt() - amount;
    QJsonArray senderHistory = senderAccount["history"].toArray();
    senderHistory.append(createHistoryObject("송금",
                                             amount,
                                             targetBank + ", " + targetAccountNumber));
    senderAccount["history"] = senderHistory;
    senderAccounts[senderAccountIndex] = senderAccount;
    senderUser["accounts"] = senderAccounts;
    users[senderUserIndex] = senderUser;

    if (targetUserIndex >= 0 && targetAccountIndex >= 0) {
        QJsonObject targetUser = users[targetUserIndex].toObject();
        QJsonArray targetAccounts = targetUser["accounts"].toArray();
        QJsonObject targetAccount = targetAccounts[targetAccountIndex].toObject();
        targetAccount["balance"] = targetAccount["balance"].toInt() + amount;

        QJsonArray targetHistory = targetAccount["history"].toArray();
        targetHistory.append(createHistoryObject("입금",
                                                 amount,
                                                 ownerName + ", " +
                                                 senderAccount["bank"].toString() + ", " +
                                                 fromAccountNumber));
        targetAccount["history"] = targetHistory;
        targetAccounts[targetAccountIndex] = targetAccount;
        targetUser["accounts"] = targetAccounts;
        users[targetUserIndex] = targetUser;
    }

    root["users"] = users;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        message = "송금 정보를 저장할 수 없습니다.";
        return false;
    }

    file.write(QJsonDocument(root).toJson());
    file.close();
    message = "송금이 완료되었습니다.";
    return true;
}

bool AccountManager::updateTransactionNote(const QString& filePath,
                                           const QString& username,
                                           const QString& accountNumber,
                                           int historyIndex,
                                           const QString& note)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();
    QJsonArray users = root["users"].toArray();

    for (int i = 0; i < users.size(); ++i) {
        QJsonObject user = users[i].toObject();
        if (user["username"].toString() != username) continue;

        QJsonArray accountsArray = user["accounts"].toArray();
        for (int j = 0; j < accountsArray.size(); ++j) {
            QJsonObject account = accountsArray[j].toObject();
            if (account["account_number"].toString() != accountNumber) continue;

            QJsonArray history = account["history"].toArray();
            if (historyIndex < 0 || historyIndex >= history.size()) {
                return false;
            }

            QJsonObject historyObj = history[historyIndex].toObject();
            historyObj["note"] = note;
            history[historyIndex] = historyObj;
            account["history"] = history;
            accountsArray[j] = account;
            user["accounts"] = accountsArray;
            users[i] = user;
            root["users"] = users;

            if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                return false;
            }

            file.write(QJsonDocument(root).toJson());
            file.close();
            return true;
        }
    }

    return false;
}

bool AccountManager::deleteUserByContact(const QString& filePath,
                                         const QString& username,
                                         const QString& phone,
                                         const QString& email,
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
        const QJsonObject user = users[i].toObject();
        if (user["username"].toString() != username) continue;

        if (user["phone"].toString() != phone.trimmed() ||
            user["email"].toString() != email.trimmed()) {
            message = "전화번호와 이메일이 일치하지 않습니다.";
            return false;
        }

        users.removeAt(i);
        root["users"] = users;

        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            message = "계정 정보를 저장할 수 없습니다.";
            return false;
        }

        file.write(QJsonDocument(root).toJson());
        file.close();
        message = "계정이 삭제되었습니다.";
        return true;
    }

    message = "삭제할 계정을 찾을 수 없습니다.";
    return false;
}

// 원본 계좌 호출
QList<Account>& AccountManager::getAccounts()
{
    return m_accounts;
}

const QList<Account>& AccountManager::getAccounts() const
{
    return m_accounts;
}
void AccountManager::setCurrentIndex(int index)
{
    m_currentIndex = index;
}
// 클릭한 행 호출
int AccountManager::getCurrentIndex() const
{
    return m_currentIndex;
}
// balance 잔액 전체 합
int AccountManager::getTotalBalance() const
{
    int total = 0;
    for (const auto& account : m_accounts) {
        total += account.getBalance();
    }
    return total;
}
