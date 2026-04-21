#include "mainpage.h"
#include "ui_mainpage.h"
#include "tx_dialog.h"
#include "transfer_dialog.h"
#include<QMessageBox>

MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainPage)
{
    ui->setupUi(this);

    // mainpage에서 테이블 수정 불가하게
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // mainpage 셀이 전체 다 클릭되게
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // JSON 로드
    manager.loadFromJson("data.json", 1);  // userId = 1

    // 총자산 라벨
    int total = manager.getTotalBalance();
    ui->label->setText("총 자산 : " +
                       QString("%L1").arg(total) + "원");

    // 테이블 세팅
    QList<Account>& accounts = manager.getAccounts();
    ui->tableWidget->setRowCount(accounts.size());
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"은행명", "계좌번호", "잔액"});

    for (int i = 0; i < accounts.size(); i++) {
        ui->tableWidget->setItem(i, 0,
                                 new QTableWidgetItem(accounts[i].getBank()));
        ui->tableWidget->setItem(i, 1,
                                 new QTableWidgetItem(accounts[i].getAccountNumber()));
        ui->tableWidget->setItem(i, 2,
                                 new NumericTableWidgetItem(
                                     QString("%L1").arg(accounts[i].getBalance()) + "원"));
    }

}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::on_tableWidget_cellClicked(int row, int column)
{
    manager.setCurrentIndex(row);
}


void MainPage::on_tableWidget_cellDoubleClicked(int row, int column)
{
    manager.setCurrentIndex(row);
    // stackedWidget에서 mainpage2로 전환
    ui->stackedWidget->setCurrentIndex(1);
}


void MainPage::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// 총자산 갱신
void MainPage::refreshUI()
{
    ui->label->setText("총 자산 : " +
                       QString("%L1").arg(manager.getTotalBalance()) + "원");

    // 계좌 테이블 잔액 갱신
    QList<Account>& accounts = manager.getAccounts();
    for (int i = 0; i < accounts.size(); i++) {
        ui->tableWidget->item(i, 2)->setText(
            QString("%L1").arg(accounts[i].getBalance()) + "원");
    }
}

void MainPage::updateHistoryTable()
{
    ui->tableWidget_2->setSortingEnabled(false);
    ui->tableWidget_2->setRowCount(0);

    for (auto& t : manager.getCurrentAccount().getHistory()) {
        int row = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(row);

        QStringList values = {t.getDatetime(), t.getType(), t.getTarget()};
        for (int col = 0; col < 3; col++) {
            QTableWidgetItem* item = new QTableWidgetItem(values[col]);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_2->setItem(row, col, item);
        }
        NumericTableWidgetItem* amountItem = new NumericTableWidgetItem(
            QString("%L1").arg(t.getAmount()) + "원");
        amountItem->setFlags(amountItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget_2->setItem(row, 3, amountItem);
    }
    ui->tableWidget_2->setSortingEnabled(true);
}

void MainPage::on_deposit_btn_clicked()
{
    tx_dialog dlg(this, "입금");
    dlg.setCurrentAccount(manager.getCurrentAccount().getBank(),
                          QString("%L1").arg(manager.getCurrentAccount().getBalance()) + "원");

    if (dlg.exec() == QDialog::Accepted) {
        // 1순위: 금액 한도 체크
        if (dlg.getAmount() > 100000000) {
            QMessageBox::warning(this, "한도 초과", "금액 한도는 1억원입니다.");
            return;
        }
        // 2순위: 비밀번호 체크
        if (!manager.getCurrentAccount().checkPassword(dlg.getPassword())) {
            QMessageBox::warning(this, "오류", "비밀번호가 일치하지 않습니다.");
            return;
        }
        manager.deposit(dlg.getAmount());
        manager.saveToJson("data.json", 1);
        refreshUI();
        updateHistoryTable();
    }
}


void MainPage::on_withdraw_btn_clicked()
{
    tx_dialog dlg(this, "출금");
    dlg.setCurrentAccount(manager.getCurrentAccount().getBank(),
                          QString("%L1").arg(manager.getCurrentAccount().getBalance()) + "원");
    dlg.setBalance(manager.getCurrentAccount().getBalance());

    if (dlg.exec() == QDialog::Accepted) {
        // 1순위: 금액 한도 체크
        if (dlg.getAmount() > 100000000) {
            QMessageBox::warning(this, "한도 초과", "1회 한도는 100,000,000원입니다.");
            return;
        }
        // 2순위: 잔액 부족 체크
        if (dlg.getAmount() > manager.getCurrentAccount().getBalance()) {
            QMessageBox::warning(this, "잔액 부족", "잔액이 부족합니다.");
            return;
        }
        // 3순위: 비밀번호 체크
        if (!manager.getCurrentAccount().checkPassword(dlg.getPassword())) {
            QMessageBox::warning(this, "오류", "비밀번호가 일치하지 않습니다.");
            return;
        }
        manager.withdraw(dlg.getAmount());
        manager.saveToJson("data.json", 1);
        refreshUI();
        updateHistoryTable();
    }
}

void MainPage::on_trasfer_btn_clicked()
{
    transfer_dialog dlg(this);

    // 내 계좌 목록 (현재 계좌 제외)
    QStringList myAccounts;
    QList<Account>& accounts = manager.getAccounts();
    int curIdx = manager.getCurrentIndex();
    for (int i = 0; i < accounts.size(); i++) {
        if (i != curIdx)
            myAccounts << accounts[i].getBank() + " : " + accounts[i].getAccountNumber();
    }
    dlg.setPhase1_Data(myAccounts);

    if (dlg.exec() == QDialog::Accepted) {
        int amount = dlg.getTransferAmount();
        int pw = dlg.getPassword();
        QString targetBank = dlg.getTargetBank();
        // 1순위: 금액 한도 체크
        if (amount > 100000000) {
            QMessageBox::warning(this, "한도 초과", "1회 한도는 100,000,000원입니다.");
            return;
        }
        // 2순위: 잔액 부족 체크
        if (amount > manager.getCurrentAccount().getBalance()) {
            QMessageBox::warning(this, "잔액 부족", "잔액이 부족합니다.");
            return;
        }
        // 3순위: 비밀번호 체크
        if (!manager.getCurrentAccount().checkPassword(pw)) {
            QMessageBox::warning(this, "오류", "비밀번호가 일치하지 않습니다.");
            return;
        }
        manager.transfer(amount, targetBank, dlg.isMyAccountTransfer(),
                         manager.getCurrentAccount().getBank());
        manager.saveToJson("data.json", 1);
        refreshUI();
        updateHistoryTable();
    }
}

