#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>

class QComboBox;
class QLineEdit;

class AddAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddAccountDialog(QWidget *parent = nullptr);

    QString bank() const;
    QString accountNumber() const;
    int balance() const;
    int balancePassword() const;

private slots:
    void validateAndAccept();

private:
    QComboBox *bankComboBox;
    QLineEdit *accountNumberEdit;
    QLineEdit *balanceEdit;
    QLineEdit *balancePasswordEdit;
};

#endif // ADDACCOUNTDIALOG_H
