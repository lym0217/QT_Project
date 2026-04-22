#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include "account.h"
#include <QList>
#include <QString>

/**
 * @brief AccountManager 클래스는 현재 로그인한 사용자의 계좌 목록과 JSON 저장 로직을 관리한다.
 *
 * 멤버 변수
 * - m_accounts: 메모리에 로드된 계좌 목록
 * - m_currentIndex: 현재 선택된 계좌 인덱스
 *
 * 주요 함수
 * - loadFromJsonByUsername(): 특정 사용자의 계좌 데이터를 JSON에서 읽는다.
 * - addAccountToUser(): 새 계좌를 JSON에 저장한다.
 * - depositToAccount(), withdrawFromAccount(), transferBetweenAccounts(): 거래 처리 후 잔액과 거래내역을 저장한다.
 * - updateTransactionNote(): 거래 메모를 JSON에 반영한다.
 * - getAccounts(), getTotalBalance(): 화면 표시용 데이터를 제공한다.
 */
class AccountManager
{
public:
    AccountManager();

    QList<Account>& getAccounts();
    const QList<Account>& getAccounts() const;
    void setCurrentIndex(int index);
    int getCurrentIndex() const;
    int getTotalBalance() const;

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
    bool deleteUserByContact(const QString& filePath,
                             const QString& username,
                             const QString& phone,
                             const QString& email,
                             QString& message);

private:
    QList<Account> m_accounts;
    int m_currentIndex;
};

#endif // ACCOUNT_MANAGER_H
