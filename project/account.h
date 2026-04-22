#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QList>
#include <QDateTime>
#include <QTableWidgetItem>

/**
 * @brief Transaction 클래스는 한 건의 거래내역 정보를 저장한다.
 *
 * 멤버 변수
 * - m_type: 거래 구분(입금, 출금, 송금)
 * - m_amount: 거래 금액
 * - m_datetime: 거래가 발생한 날짜와 시간
 * - m_target: 상대 이름 또는 계좌 정보
 * - m_note: 사용자가 수정할 수 있는 메모
 *
 * 주요 함수
 * - getType(), getDatetime(), getTarget(), getNote(), getAmount(): 거래 정보 조회
 * - setNote(): 메모 수정
 */
class Transaction
{
public:
    Transaction(QString type, int amount, QString target, QString note = "")
        : m_type(type), m_amount(amount), m_target(target), m_note(note)
    {
        m_datetime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    }

    Transaction(QString type, int amount, QString target, QString note, QString datetime)
        : m_type(type), m_amount(amount), m_datetime(datetime), m_target(target), m_note(note) {}

    QString getType() const { return m_type; }
    QString getDatetime() const { return m_datetime; }
    QString getTarget() const { return m_target; }
    QString getNote() const { return m_note; }
    void setNote(const QString &note) { m_note = note; }
    int getAmount() const { return m_amount; }

private:
    QString m_type;
    int m_amount;
    QString m_datetime;
    QString m_target;
    QString m_note;
};

/**
 * @brief Account 클래스는 한 개의 계좌 정보와 거래내역 목록을 저장한다.
 *
 * 멤버 변수
 * - m_bank: 은행명
 * - m_accountNumber: 계좌번호
 * - m_balance: 현재 잔액
 * - m_history: 해당 계좌의 거래내역 목록
 *
 * 주요 함수
 * - addTransaction(): 거래내역 추가
 * - getHistory(): 거래내역 목록 조회
 */
class Account
{
public:
    Account(QString bank, QString accountNumber, int balance = 0)
        : m_bank(bank), m_accountNumber(accountNumber), m_balance(balance) {}

    QString getBank() const { return m_bank; }
    QString getAccountNumber() const { return m_accountNumber; }
    int getBalance() const { return m_balance; }

    void addTransaction(QString type, int amount, QString target, QString note = "")
    {
        m_history.append(Transaction(type, amount, target, note));
    }

    void addTransaction(QString type, int amount, QString target, QString note, QString datetime)
    {
        m_history.append(Transaction(type, amount, target, note, datetime));
    }

    QList<Transaction>& getHistory()
    {
        return m_history;
    }

    const QList<Transaction>& getHistory() const
    {
        return m_history;
    }

private:
    QString m_bank;
    QString m_accountNumber;
    int m_balance;
    QList<Transaction> m_history;
};

/**
 * @brief NumericTableWidgetItem 클래스는 금액 문자열을 숫자 기준으로 정렬하기 위한 아이템이다.
 */
class NumericTableWidgetItem : public QTableWidgetItem
{
public:
    explicit NumericTableWidgetItem(QString text) : QTableWidgetItem(text) {}

    bool operator<(const QTableWidgetItem& other) const override
    {
        QString leftText = text();
        QString rightText = other.text();
        leftText.remove(",").remove("원");
        rightText.remove(",").remove("원");
        return leftText.toLongLong() < rightText.toLongLong();
    }
};

#endif // ACCOUNT_H
