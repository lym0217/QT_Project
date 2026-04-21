#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>
#include <QStringList>

class QButtonGroup;
class QComboBox;
class QLineEdit;
class QRadioButton;

class TransferDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferDialog(const QStringList &myAccounts, QWidget *parent = nullptr);
    bool isMyAccountTransfer() const;
    QString selectedMyAccountText() const;
    QString targetBank() const;
    QString targetAccountNumber() const;

private:
    void updateMode();

    QRadioButton *myAccountRadio;
    QRadioButton *otherAccountRadio;
    QComboBox *myAccountComboBox;
    QComboBox *otherBankComboBox;
    QLineEdit *otherAccountEdit;
};

#endif // TRANSFERDIALOG_H
