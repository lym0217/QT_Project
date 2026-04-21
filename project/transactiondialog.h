#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionDialog(const QString &actionName,
                               const QString &currentAccountText,
                               QWidget *parent = nullptr);
    int amount() const;
    int password() const;

private:
    QString actionName;
    QLabel *accountValueLabel;
    QLineEdit *amountEdit;
    QLineEdit *passwordEdit;
    QPushButton *confirmButton;
    QPushButton *cancelButton;
};

#endif // TRANSACTIONDIALOG_H
