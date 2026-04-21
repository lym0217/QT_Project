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