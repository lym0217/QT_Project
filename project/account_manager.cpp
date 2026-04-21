#include "account_manager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

AccountManager::AccountManager()
{
    currentIndex = 0;
    // 헤더에서 만든것 초기화
    currentFilePath = "";
    currentUserId = 0;
}

// Json파일 받기
bool AccountManager::loadFromJson(const QString& filePath, int userId)
{
    currentFilePath = filePath;
    currentUserId = userId;

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
            QString bank = acc["bank"].toString();
            QString accNum = acc["account_number"].toString();
            int balance = acc["balance"].toInt();
            int balancePw = acc["balance_pw"].toInt();
            Account a(bank, accNum, balance, balancePw);

            // 히스토리 복원
            QJsonArray history = acc["history"].toArray();
            for (const auto& txVal : history) {
                QJsonObject tx = txVal.toObject();
                a.addTransaction(tx["type"].toString(),
                                 tx["amount"].toInt(),
                                 tx["target"].toString(),
                                 tx["note"].toString());
            }
            accounts << a;
        }
        return true;
    }
    return false;  // userId 못 찾은 경우
}


// 입금 로직
bool AccountManager::deposit(int amount)
{
    if(amount > 100000000) return false;
    accounts[currentIndex].deposit(amount);                             // ← Account 메서드 사용
    accounts[currentIndex].addTransaction("입금", amount, "본인");
    return true;
}

// 출금 로직
bool AccountManager::withdraw(int amount)
{
    if(amount > 100000000) return false;
    if(amount > accounts[currentIndex].getBalance()) return false;      // ← getter 사용
    accounts[currentIndex].withdraw(amount);                            // ← Account 메서드 사용
    accounts[currentIndex].addTransaction("출금", amount, "본인");
    return true;
}

// 송금 로직
bool AccountManager::transfer(int amount, QString targetBank, bool isMyAccount, QString fromBank)
{
    qDebug() << "transfer 호출됨";
    qDebug() << "amount:" << amount;
    qDebug() << "isMyAccount:" << isMyAccount;

    if(amount <= 0 || amount > 100000000) { qDebug() << "한도초과로 return"; return false; }
    if(amount > accounts[currentIndex].getBalance()) { qDebug() << "잔액부족으로 return"; return false; }

    if(amount <= 0 || amount > 100000000) return false;
    if(amount > accounts[currentIndex].getBalance()) return false;      // ← getter 사용

    accounts[currentIndex].withdraw(amount);                            // ← Account 메서드 사용

    if(isMyAccount) {
        for(auto& a : accounts) {
            if(a.getBank() == targetBank) {                             // ← getter 사용
                a.deposit(amount);                                      // ← Account 메서드 사용
                a.addTransaction("입금", amount, "[My] " + fromBank);
                break;
            }
        }
        accounts[currentIndex].addTransaction("송금", amount, "[My] " + targetBank);
    }
    else {
        accounts[currentIndex].addTransaction("송금", amount, targetBank);
    }
    return true;
}

// Json에 저장
bool AccountManager::saveToJson(const QString& filePath, int userId)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject root = doc.object();
    QJsonArray users = root["users"].toArray();

    for (int i = 0; i < users.size(); i++) {
        QJsonObject user = users[i].toObject();
        if (user["user_id"].toInt() != userId) continue;

        QJsonArray accs;
        for (const auto& a : accounts) {
            QJsonObject acc;
            acc["bank"]           = a.getBank();
            acc["account_number"] = a.getAccountNumber();
            acc["balance"]        = a.getBalance();
            acc["balance_pw"]     = a.getBalancePw();

            // 거래내역 저장
            QJsonArray history;
            for (const auto& t : const_cast<Account&>(a).getHistory()) {
                QJsonObject tx;
                tx["type"]     = t.getType();
                tx["amount"]   = t.getAmount();
                tx["target"]   = t.getTarget();
                tx["datetime"] = t.getDatetime();
                tx["note"]     = t.getNote();
                history.append(tx);
            }
            acc["history"] = history;
            accs.append(acc);
        }
        user["accounts"] = accs;
        users[i] = user;
        break;
    }

    root["users"] = users;
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(root).toJson());
    return true;
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