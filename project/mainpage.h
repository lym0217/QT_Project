#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "account_manager.h"

class QTableWidgetItem;

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void setUserName(const QString &userName);
    void setCurrentUser(const QString &userName, const QString &username);
    void showHomePage();
    void showDetailPage();

signals:
    void logoutRequested();

private slots:
    void on_logout_btn_clicked();
    void on_back_btn_clicked();
    void on_add_account_btn_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_clicked();
    void on_tableWidget_2_itemChanged(QTableWidgetItem *item);
    void on_delete_account_btn_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    void refreshAccountTable();
    void refreshTransactionTable();
    QString formatMoney(int amount) const;
    void updateDetailHeader();
    QString currentAccountSummary() const;
    QStringList currentAccountOptions() const;
    QString currentAccountNumber() const;
    void handleTransactionDialog(const QString &actionName,
                                 const QString &targetBank = QString(),
                                 const QString &targetAccountNumber = QString(),
                                 bool ownAccountTransfer = false);

    Ui::MainPage *ui;
    AccountManager manager;
    QString currentUsername;
    QString currentOwnerName;
    bool updatingTransactionTable = false;
    bool accountDeleted = false;
};

#endif // MAINPAGE_H
