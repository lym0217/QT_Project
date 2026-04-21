#include "account_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AccountManager::AccountManager()
{
    currentIndex = 0;
}

// Json파일 받기
bool AccountManager::loadFromJson(const QString& filePath, int userId)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray users = doc.object()["users"].toArray();

    for (const auto& userVal : users) {
        QJsonObject user = userVal.toObject();

        // userId 일치하는 유저 찾기
        if (user["user_id"].toInt() != userId) continue;

        QJsonArray accs = user["accounts"].toArray();
        accounts.clear();

        for (const auto& accVal : accs) {
            QJsonObject acc = accVal.toObject();
            QString bank       = acc["bank"].toString();
            QString accNum     = acc["account_number"].toString();
            int     balance    = acc["balance"].toInt();

            accounts << Account(bank, accNum, balance);
        }
        return true;
    }
    return false;  // userId 못 찾은 경우
}

bool AccountManager::loadFromJsonByUsername(const QString& filePath, const QString& username)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray users = doc.object()["users"].toArray();

    accounts.clear();

    for (const auto& userVal : users) {
        QJsonObject user = userVal.toObject();

        if (user["username"].toString() != username) continue;

        QJsonArray accs = user["accounts"].toArray();
        for (const auto& accVal : accs) {
            QJsonObject acc = accVal.toObject();
            const QString bank = acc["bank"].toString();
            const QString accNum = acc["account_number"].toString();
            const int balance = acc["balance"].toInt();

            accounts << Account(bank, accNum, balance);
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
