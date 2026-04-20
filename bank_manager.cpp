#include "bank_manager.h"
#include "ui_bank_manager.h"
#include "deposit_dialog.h"
#include "transfer_dialog.h"
#include <QMessageBox>
#include <QDateTime>
#include <algorithm>

Bank_Manager::Bank_Manager(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Bank_Manager)
{
    ui->setupUi(this);
    ui->pw_checkBox->setIcon(QIcon(":/resources/hidden.png"));

    // 더미데이터는 AccountManager 생성자에서 자동으로 로드됨
    // bank_table 초기 표시
    QList<Account>& accounts = accountManager.getAccounts();
    ui->bank_table->setRowCount(accounts.size());
    for(int i = 0; i < accounts.size(); i++) {
        ui->bank_table->setItem(i, 0, new QTableWidgetItem(accounts[i].getBank()));
        ui->bank_table->setItem(i, 1, new QTableWidgetItem(formatMoney(accounts[i].getBalance())));
    }

    ui->total_label->setText("총 자산: " + formatMoney(accountManager.getTotalBalance()));
    ui->history_table->setSelectionBehavior(QAbstractItemView::SelectRows);
}

Bank_Manager::~Bank_Manager()
{
    delete ui;
}

QString Bank_Manager::formatMoney(int amount)
{
    return QLocale(QLocale::Korean).toString(amount) + "원";
}

void Bank_Manager::refreshUI()
{
    updateAccountTable();
    updateHistoryTable();
    Account& cur = accountManager.getCurrentAccount();
    ui->account_label->setText(cur.getBank() + " : " + formatMoney(cur.getBalance()));
}

void Bank_Manager::updateAccountTable()
{
    QList<Account>& accounts = accountManager.getAccounts();
    for(int i = 0; i < accounts.size(); i++) {
        ui->bank_table->item(i, 1)->setText(formatMoney(accounts[i].getBalance()));
    }
    ui->total_label->setText("총 자산 : " + formatMoney(accountManager.getTotalBalance()));
}

void Bank_Manager::updateHistoryTable()
{
    ui->history_table->setSortingEnabled(false);
    ui->history_table->setRowCount(0);

    for(auto& t : accountManager.getCurrentAccount().getHistory()) {
        int row = ui->history_table->rowCount();
        ui->history_table->insertRow(row);

        QStringList values = {t.getDatetime(), t.getType(), t.getTarget()};
        for(int col = 0; col < 3; col++) {
            QTableWidgetItem* item = new QTableWidgetItem(values[col]);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->history_table->setItem(row, col, item);
        }

        NumericTableWidgetItem* amountItem = new NumericTableWidgetItem(formatMoney(t.getAmount()));
        amountItem->setFlags(amountItem->flags() & ~Qt::ItemIsEditable);
        ui->history_table->setItem(row, 3, amountItem);

        ui->history_table->setItem(row, 4, new QTableWidgetItem(t.getNote()));
    }
    ui->history_table->setSortingEnabled(true);
}

void Bank_Manager::on_login_btn_clicked()
{
    QString id = ui->id_lineEdit->text();
    QString pw = ui->pw_lineEdit->text();

    if(id == "veda" && pw == "1234")
        ui->stackedWidget->setCurrentIndex(1);
    else
        QMessageBox::warning(this, "오류", "ID 또는 비밀번호가 틀렸습니다.");
}

void Bank_Manager::on_back_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Bank_Manager::on_bank_table_cellDoubleClicked(int row, int column)
{
    accountManager.setCurrentIndex(row);
    ui->account_label->setText(accountManager.getCurrentAccount().getBank() + " : " +
                               formatMoney(accountManager.getCurrentAccount().getBalance()));
    updateHistoryTable();
    ui->stackedWidget->setCurrentIndex(2);
}

void Bank_Manager::on_pw_checkBox_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked) {
        ui->pw_lineEdit->setEchoMode(QLineEdit::Normal);
        ui->pw_checkBox->setIcon(QIcon(":/resources/eye.png"));
    } else {
        ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
        ui->pw_checkBox->setIcon(QIcon(":/resources/hidden.png"));
    }
}

void Bank_Manager::on_deposit_btn_clicked()
{
    deposit_dialog dlg(this);
    dlg.setCurrentAccount(accountManager.getCurrentAccount().getBank(),
                          formatMoney(accountManager.getCurrentAccount().getBalance()));

    if(dlg.exec() == QDialog::Accepted) {
        int amount = dlg.getAmount();
        if(!accountManager.deposit(amount)) {
            QMessageBox::warning(this, "한도 초과", "1회 한도는 1억원입니다.");
            return;
        }
        refreshUI();
    }
}

void Bank_Manager::on_withdraw_btn_clicked()
{
    deposit_dialog dlg(this, "출금");
    dlg.setCurrentAccount(accountManager.getCurrentAccount().getBank(),
                          formatMoney(accountManager.getCurrentAccount().getBalance()));

    if(dlg.exec() == QDialog::Accepted) {
        int amount = dlg.getAmount();
        if(amount > accountManager.getCurrentAccount().getBalance()) {
            QMessageBox::warning(this, "오류", "잔고가 부족합니다.");
            return;
        }
        if(!accountManager.withdraw(amount)) {
            QMessageBox::warning(this, "한도 초과", "1회 한도는 1억원입니다.");
            return;
        }
        refreshUI();
    }
}

void Bank_Manager::on_transfer_btn_clicked()
{
    transfer_dialog dlg(this);

    QStringList myAccounts;
    QList<Account>& accounts = accountManager.getAccounts();
    int curIdx = accountManager.getCurrentIndex();

    for(int i = 0; i < accounts.size(); i++) {
        if(i != curIdx)
            myAccounts << accounts[i].getBank() + " : " + formatMoney(accounts[i].getBalance());
    }

    dlg.setPhase1_Data(myAccounts);
    dlg.setPhase2_Data(accountManager.getCurrentAccount().getBank(),
                       accountManager.getCurrentAccount().getBalance(), "", 0);

    if(dlg.exec() == QDialog::Accepted) {
        int amount = dlg.getTransferAmount();
        QString targetBank = dlg.getTargetAccount().split(" :")[0];

        if(!accountManager.transfer(amount, targetBank, dlg.isMyAccountTransfer(),
                                     accountManager.getCurrentAccount().getBank())) {
            QMessageBox::warning(this, "오류", "송금에 실패했습니다.");
            return;
        }
        refreshUI();
    }
}

void Bank_Manager::on_history_delete_clicked()
{
    QList<int> rows;
    for(auto item : ui->history_table->selectedItems()) {
        if(!rows.contains(item->row()))
            rows.append(item->row());
    }
    if(rows.isEmpty()) {
        QMessageBox::warning(this, "오류", "삭제할 내역을 선택해주세요.");
        return;
    }
    std::sort(rows.begin(), rows.end(), std::greater<int>());
    for(int row : rows)
        accountManager.getCurrentAccount().getHistory().removeAt(row);
    updateHistoryTable();
}