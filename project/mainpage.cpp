#include "mainpage.h"
#include "ui_mainpage.h"
#include "tx_dialog.h"
#include "transfer_dialog.h"
#include "addaccountdialog.h"
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLocale>
#include <QMessageBox>

MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainPage)
{
    ui->setupUi(this);

    setWindowTitle("계좌 관리 대시보드");

    ui->label_2->setText("T055 BANK");
    ui->label->setText("총 자산 0원");
    ui->label_3->setText("T055 BANK");
    ui->current_account->setText("대표 계좌 : 0원");
    ui->home_user_name_label->setText("OOO님");

    showHomePage();

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(36);
    ui->tableWidget->verticalHeader()->setMinimumSectionSize(36);
    ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget_2->verticalHeader()->setDefaultSectionSize(36);
    ui->tableWidget_2->verticalHeader()->setMinimumSectionSize(36);

    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget_2->setAlternatingRowColors(true);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget_2->setShowGrid(false);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    setStyleSheet(R"(
        QWidget#MainPage {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #f5f9ff, stop:0.55 #eefbf7, stop:1 #fff8ed);
        }
        QLabel {
            color: #284b63;
        }
        QLabel#label_2, QLabel#label_3 {
            color: #2a6f97;
            font-size: 14px;
            font-weight: 700;
            letter-spacing: 1px;
            padding: 10px 14px;
            background: rgba(255, 255, 255, 0.72);
            border: 1px solid #d4e4f1;
            border-radius: 14px;
            max-width: 140px;
        }
        QLabel#label {
            color: #17324d;
            font-size: 28px;
            font-weight: 700;
            padding: 16px 8px 14px 8px;
        }
        QLabel#current_account {
            color: #42657d;
            font-size: 16px;
            font-weight: 600;
            padding: 10px 6px;
        }
        QLabel#home_user_name_label {
            color: #35546b;
            font-size: 14px;
            font-weight: 700;
            padding: 10px 14px;
            background: rgba(255, 255, 255, 0.72);
            border: 1px solid #d4e4f1;
            border-radius: 14px;
            min-width: 88px;
        }
        QTableWidget {
            background: rgba(255, 255, 255, 0.88);
            alternate-background-color: rgba(235, 246, 255, 0.72);
            border: 1px solid #d6e5f0;
            border-radius: 16px;
            color: #213c4f;
            gridline-color: transparent;
            selection-background-color: #bfe9df;
            selection-color: #163447;
        }
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #5b86e5, stop:1 #36d1dc);
            color: white;
            border: none;
            padding: 10px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton {
            border: none;
            border-radius: 14px;
            padding: 11px 18px;
            font-size: 13px;
            font-weight: 700;
            color: white;
        }
        QPushButton#pushButton_4, QPushButton#back_btn {
            color: #29465b;
            background: rgba(255, 255, 255, 0.78);
            border: 1px solid #d0dfeb;
        }
        QPushButton#pushButton_4:hover, QPushButton#back_btn:hover {
            background: rgba(255, 255, 255, 0.95);
        }
        QPushButton#logout_btn {
            color: #fffaf7;
            background: #f26b5e;
        }
        QPushButton#logout_btn:hover {
            background: #df5b4f;
        }
        QPushButton#add_account_btn {
            background: #2a9d8f;
            min-width: 110px;
        }
        QPushButton#add_account_btn:hover {
            background: #23897d;
        }
        QPushButton#deposit_btn {
            background: #1fbf8f;
        }
        QPushButton#deposit_btn:hover {
            background: #15ab7f;
        }
        QPushButton#withdraw_btn {
            background: #ff8a5b;
        }
        QPushButton#withdraw_btn:hover {
            background: #f07a49;
        }
        QPushButton#transfer_btn {
            background: #4f7cff;
        }
        QPushButton#transfer_btn:hover {
            background: #416de6;
        }
    )");
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::setUserName(const QString &userName)
{
    const QString displayName = userName.isEmpty() ? "OOO님" : userName + "님";
    ui->home_user_name_label->setText(displayName);
}

void MainPage::setCurrentUser(const QString &userName, const QString &username)
{
    currentUsername = username;
    setUserName(userName);
    refreshAccountTable();
}

void MainPage::showHomePage()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainPage::showDetailPage()
{
    updateDetailHeader();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainPage::on_logout_btn_clicked()
{
    currentUsername.clear();
    manager.getAccounts().clear();
    ui->home_user_name_label->setText("OOO님");
    refreshAccountTable();
    showHomePage();
    emit logoutRequested();
}

void MainPage::on_back_btn_clicked()
{
    showHomePage();
}

void MainPage::on_add_account_btn_clicked()
{
    if (currentUsername.isEmpty()) {
        QMessageBox::warning(this, "오류", "로그인한 사용자 정보가 없습니다.");
        return;
    }

    AddAccountDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString message;
    const bool success = manager.addAccountToUser("users.json",
                                                  currentUsername,
                                                  dialog.bank(),
                                                  dialog.accountNumber(),
                                                  dialog.balance(),
                                                  dialog.balancePassword(),
                                                  message);

    if (!success) {
        QMessageBox::warning(this, "계좌 추가 실패", message);
        return;
    }

    refreshAccountTable();
    QMessageBox::information(this, "완료", message);
}

void MainPage::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if (column != 0) {
        return;
    }

    const QList<Account>& accounts = manager.getAccounts();
    if (row < 0 || row >= accounts.size()) {
        return;
    }

    manager.setCurrentIndex(row);
    showDetailPage();
}

void MainPage::refreshAccountTable()
{
    ui->tableWidget->setRowCount(0);

    if (currentUsername.isEmpty()) {
        ui->label->setText("총 자산 0원");
        ui->current_account->setText("대표 계좌 : 0원");
        return;
    }

    if (!manager.loadFromJsonByUsername("users.json", currentUsername)) {
        ui->label->setText("총 자산 0원");
        ui->current_account->setText("대표 계좌 : 0원");
        return;
    }

    const QList<Account>& accounts = manager.getAccounts();
    ui->tableWidget->setRowCount(accounts.size());

    for (int row = 0; row < accounts.size(); ++row) {
        const Account& account = accounts[row];
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(account.getBank()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(account.getAccountNumber()));
        ui->tableWidget->setItem(row, 2, new NumericTableWidgetItem(formatMoney(account.getBalance())));
    }

    ui->label->setText("총 자산 " + formatMoney(manager.getTotalBalance()));

    if (!accounts.isEmpty()) {
        const Account& firstAccount = accounts.first();
        ui->current_account->setText(firstAccount.getBank() + " : " + formatMoney(firstAccount.getBalance()));
    } else {
        ui->current_account->setText("대표 계좌 : 0원");
    }
}

QString MainPage::formatMoney(int amount) const
{
    return QLocale(QLocale::Korean, QLocale::SouthKorea).toString(amount) + "원";
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
        if (!manager.deposit(dlg.getAmount())) {
            QMessageBox::warning(this, "오류", "입금 내용을 저장하지 못했습니다.");
            return;
        }
        refreshUI();
        updateDetailHeader();
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
        if (!manager.withdraw(dlg.getAmount())) {
            QMessageBox::warning(this, "오류", "출금 내용을 저장하지 못했습니다.");
            return;
        }
        refreshUI();
        updateDetailHeader();
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
        if (!manager.transfer(amount,
                              targetBank,
                              dlg.getTargetAccountNum(),
                              dlg.isMyAccountTransfer(),
                              manager.getCurrentAccount().getBank())) {
            QMessageBox::warning(this, "오류", "송금 내용을 저장하지 못했습니다.");
            return;
        }
        refreshUI();
        updateDetailHeader();
        updateHistoryTable();
    }
}

void MainPage::updateDetailHeader()
{
    const QList<Account>& accounts = manager.getAccounts();
    const int currentIndex = manager.getCurrentIndex();

    if (accounts.isEmpty() || currentIndex < 0 || currentIndex >= accounts.size()) {
        ui->current_account->setText("대표 계좌 : 0원");
        return;
    }

    const Account& currentAccount = accounts[currentIndex];
    ui->current_account->setText(currentAccount.getBank() + " " +
                         currentAccount.getAccountNumber() + " : " +
                         formatMoney(currentAccount.getBalance()));
}
