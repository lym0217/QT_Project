#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QString>
#include <QList>
#include <QDateTime>
#include <QTableWidgetItem>

//거래내역 데이터 정리
class Transaction {
public:
    Transaction(QString type, int amount, QString target, QString note = "")
        : type(type), amount(amount), target(target), note(note)
    {
        datetime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    }
    Transaction(QString type, int amount, QString target, QString note, QString datetime)
        : type(type), amount(amount), datetime(datetime), target(target), note(note) {}
// 입금, 출금, 송금 보낼 때마다 정리되는 type들
    QString getType()        const { return type; }     // 거래 종류: 입금-출금-송금
    QString getDatetime()    const { return datetime; } // 거래가 이뤄지는 시간
    QString getTarget()      const { return target; }   // 상대방 이름 + 계좌
    QString getNote()        const { return note; }     // 메모 칸
    void setNote(QString n)  { note = n; }              // 메모만 수정할 수 있게 따로 구현
    int getAmount()          const { return amount; }   // 거래 금액

private:
    QString type;
    int     amount;
    QString datetime;
    QString target;
    QString note;
};

// 계좌 금액, 기록
class Account {
public:
    Account(QString bank, QString accountNumber, int balance = 0)           // bank: 은행이름, balance: 초기 잔액
        : bank(bank), accountNumber(accountNumber), balance(balance) {}

    QString getBank()          const { return bank; }
    QString getAccountNumber() const { return accountNumber; }
    int     getBalance()       const { return balance; }

    void deposit(int amount)
    {
        balance += amount;          // 입금시 잔액 증가 구현
    }
    void withdraw(int amount)
    {
        balance -= amount;          // 출금시 잔액 감소 구현
    }

    void addTransaction(QString type, int amount, QString target, QString note = "")
    {
        history.append(Transaction(type, amount, target, note));
    }
    void addTransaction(QString type, int amount, QString target, QString note, QString datetime)
    {
        history.append(Transaction(type, amount, target, note, datetime));
    }

    QList<Transaction>& getHistory()
    {
        return history;
    }
    const QList<Transaction>& getHistory() const
    {
        return history;
    }

private:
    QString            bank;                // 은행이름
    QString            accountNumber;       // 계좌 번호
    int                balance;             // 현재 잔액
    QList<Transaction> history;             // 거래 내역 리스트
};

// 금액정렬 시 ,로 인한 정렬 오류를 해결
class NumericTableWidgetItem : public QTableWidgetItem {
public:
    NumericTableWidgetItem(QString text) : QTableWidgetItem(text) {}
    bool operator<(const QTableWidgetItem& other) const override
    {
        QString a = text(), b = other.text();
        a.remove(",").remove("원");
        b.remove(",").remove("원");
        return a.toLongLong() < b.toLongLong();
    }
};

#endif // ACCOUNT_H
