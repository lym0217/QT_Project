#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>
#include <QList>
#include <QDateTime>
#include <QTableWidgetItem>

class Transaction {
public:
    Transaction(QString type, int amount, QString target, QString note = "")
        : type(type), amount(amount), target(target), note(note)
    {
        datetime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
    }

    QString getType()     const { return type; }
    int     getAmount()   const { return amount; }
    QString getDatetime() const { return datetime; }
    QString getTarget()   const { return target; }
    QString getNote()     const { return note; }
    void    setNote(QString n)  { note = n; }

private:
    QString type;
    int     amount;
    QString datetime;
    QString target;
    QString note;
};

class Account {
public:
    Account(QString bank, int balance = 0)
        : bank(bank), balance(balance) {}

    QString getBank()    const { return bank; }
    int     getBalance() const { return balance; }

    void deposit(int amount)  { balance += amount; }
    void withdraw(int amount) { balance -= amount; }

    void addTransaction(QString type, int amount, QString target, QString note = "") {
        history.append(Transaction(type, amount, target, note));
    }

    QList<Transaction>& getHistory() { return history; }

private:
    QString            bank;
    int                balance;
    QList<Transaction> history;
};

class NumericTableWidgetItem : public QTableWidgetItem {
public:
    NumericTableWidgetItem(QString text) : QTableWidgetItem(text) {}
    bool operator<(const QTableWidgetItem& other) const override {
        QString a = text(), b = other.text();
        a.remove(",").remove("원");
        b.remove(",").remove("원");
        return a.toLongLong() < b.toLongLong();
    }
};

#endif // ACCOUNT_H