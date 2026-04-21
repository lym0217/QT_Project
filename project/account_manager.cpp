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
