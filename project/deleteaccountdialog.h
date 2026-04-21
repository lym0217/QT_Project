#ifndef DELETEACCOUNTDIALOG_H
#define DELETEACCOUNTDIALOG_H

#include <QDialog>

class QLineEdit;

class DeleteAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteAccountDialog(QWidget *parent = nullptr);

    QString phone() const;
    QString email() const;

private slots:
    void validateAndAccept();

private:
    QLineEdit *phoneEdit;
    QLineEdit *emailEdit;
};

#endif // DELETEACCOUNTDIALOG_H
