#include "mainpage.h"
#include "ui_mainpage.h"
#include "addaccountdialog.h"
#include "deleteaccountdialog.h"
#include "transactiondialog.h"
#include "transferdialog.h"

#include <QAbstractItemView>
#include <QHeaderView>
#include <QLocale>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

namespace {
void showStyledMessageBox(QWidget *parent,
                          QMessageBox::Icon icon,
                          const QString &title,
                          const QString &text)
{
    QMessageBox messageBox(parent);
    messageBox.setWindowTitle(title);
    messageBox.setText(text);
    messageBox.setIcon(icon);
    messageBox.addButton("확인", QMessageBox::AcceptRole);
    messageBox.setStyleSheet(R"(
        QMessageBox {
            background: white;
        }
        QPushButton {
            background: white;
            color: black;
            border: 1px solid black;
            border-radius: 6px;
            padding: 6px 16px;
            min-width: 72px;
        }
        QPushButton:hover {
            background: #f3f3f3;
        }
    )");
    messageBox.exec();
}
}

MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::MainPage)
{
    m_ui->setupUi(this);
    setWindowTitle("계좌 관리 대시보드");

    m_ui->label_2->setText("T055 BANK");
    m_ui->label->setText("총 자산 0원");
    m_ui->label_3->setText("T055 BANK");
    m_ui->label_4->setText("대표 계좌 : 0원");
    m_ui->home_user_name_label->setText("OOO님");

    showHomePage();

    m_ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    m_ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui->tableWidget->verticalHeader()->setDefaultSectionSize(36);
    m_ui->tableWidget->verticalHeader()->setMinimumSectionSize(36);
    m_ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui->tableWidget_2->verticalHeader()->setDefaultSectionSize(36);
    m_ui->tableWidget_2->verticalHeader()->setMinimumSectionSize(36);

    m_ui->tableWidget->setAlternatingRowColors(true);
    m_ui->tableWidget_2->setAlternatingRowColors(true);
    m_ui->tableWidget->setShowGrid(false);
    m_ui->tableWidget_2->setShowGrid(false);
    m_ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ui->tableWidget_2->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    m_ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ui->tableWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_ui->logout_btn, &QPushButton::clicked, this, &MainPage::onLogoutButtonClicked);
    connect(m_ui->back_btn, &QPushButton::clicked, this, &MainPage::onBackButtonClicked);
    connect(m_ui->add_account_btn, &QPushButton::clicked, this, &MainPage::onAddAccountButtonClicked);
    connect(m_ui->delete_account_btn, &QPushButton::clicked, this, &MainPage::onDeleteAccountButtonClicked);
    connect(m_ui->pushButton_2, &QPushButton::clicked, this, &MainPage::onDepositButtonClicked);
    connect(m_ui->pushButton_3, &QPushButton::clicked, this, &MainPage::onWithdrawButtonClicked);
    connect(m_ui->pushButton, &QPushButton::clicked, this, &MainPage::onTransferButtonClicked);
    connect(m_ui->lineEdit, &QLineEdit::textChanged, this, &MainPage::onMemoFilterTextChanged);
    connect(m_ui->tableWidget, &QTableWidget::cellDoubleClicked, this, &MainPage::onAccountTableCellDoubleClicked);
    connect(m_ui->tableWidget_2, &QTableWidget::itemChanged, this, &MainPage::onTransactionTableItemChanged);

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
        QLabel#label_4 {
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
        QPushButton#delete_account_btn {
            background: white;
            color: #a23d3d;
            border: 1px solid #d59d9d;
            border-radius: 10px;
            padding: 6px 10px;
            min-width: 72px;
        }
        QPushButton#delete_account_btn:hover {
            background: #fff4f4;
            border-color: #c97f7f;
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
        QPushButton#pushButton_2 {
            background: #1fbf8f;
        }
        QPushButton#pushButton_2:hover {
            background: #15ab7f;
        }
        QPushButton#pushButton_3 {
            background: #ff8a5b;
        }
        QPushButton#pushButton_3:hover {
            background: #f07a49;
        }
        QPushButton#pushButton {
            background: #4f7cff;
        }
        QPushButton#pushButton:hover {
            background: #416de6;
        }
    )");
}

MainPage::~MainPage()
{
    delete m_ui;
}

void MainPage::setUserName(const QString &userName)
{
    const QString displayName = userName.isEmpty() ? "OOO님" : userName + "님";
    m_ui->home_user_name_label->setText(displayName);
}

void MainPage::setCurrentUser(const QString &userName, const QString &username)
{
    m_currentUsername = username;
    m_currentOwnerName = userName;
    setUserName(userName);
    refreshAccountTable();
}

void MainPage::showHomePage()
{
    m_ui->stackedWidget->setCurrentIndex(0);
}

void MainPage::showDetailPage()
{
    updateDetailHeader();
    refreshTransactionTable();
    m_ui->stackedWidget->setCurrentIndex(1);
}

void MainPage::onLogoutButtonClicked()
{
    m_currentUsername.clear();
    m_currentOwnerName.clear();
    m_accountManager.getAccounts().clear();
    m_ui->home_user_name_label->setText("OOO님");
    refreshAccountTable();
    showHomePage();
    emit logoutRequested();
}

void MainPage::onBackButtonClicked()
{
    showHomePage();
}

void MainPage::onAddAccountButtonClicked()
{
    if (m_currentUsername.isEmpty()) {
        showStyledMessageBox(this, QMessageBox::Warning, "오류", "로그인한 사용자 정보가 없습니다.");
        return;
    }

    AddAccountDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString message;
    const bool success = m_accountManager.addAccountToUser("users.json",
                                                  m_currentUsername,
                                                  dialog.bank(),
                                                  dialog.accountNumber(),
                                                  dialog.balance(),
                                                  dialog.balancePassword(),
                                                  message);

    if (!success) {
        showStyledMessageBox(this, QMessageBox::Warning, "계좌 추가 실패", message);
        return;
    }

    refreshAccountTable();
    showStyledMessageBox(this, QMessageBox::Information, "완료", message);
}

void MainPage::onAccountTableCellDoubleClicked(int row, int column)
{
    if (column != 0) {
        return;
    }

    const QList<Account>& accounts = m_accountManager.getAccounts();
    if (row < 0 || row >= accounts.size()) {
        return;
    }

    m_accountManager.setCurrentIndex(row);
    showDetailPage();
}

void MainPage::refreshAccountTable()
{
    m_ui->tableWidget->setRowCount(0);

    if (m_currentUsername.isEmpty()) {
        m_ui->label->setText("총 자산 0원");
        m_ui->label_4->setText("대표 계좌 : 0원");
        m_ui->tableWidget_2->setRowCount(0);
        return;
    }

    if (!m_accountManager.loadFromJsonByUsername("users.json", m_currentUsername)) {
        m_ui->label->setText("총 자산 0원");
        m_ui->label_4->setText("대표 계좌 : 0원");
        m_ui->tableWidget_2->setRowCount(0);
        return;
    }

    const QList<Account>& accounts = m_accountManager.getAccounts();
    m_ui->tableWidget->setRowCount(accounts.size());

    for (int row = 0; row < accounts.size(); ++row) {
        const Account& account = accounts[row];
        m_ui->tableWidget->setItem(row, 0, new QTableWidgetItem(account.getBank()));
        m_ui->tableWidget->setItem(row, 1, new QTableWidgetItem(account.getAccountNumber()));
        m_ui->tableWidget->setItem(row, 2, new NumericTableWidgetItem(formatMoney(account.getBalance())));
    }

    m_ui->label->setText("총 자산 " + formatMoney(m_accountManager.getTotalBalance()));

    if (!accounts.isEmpty()) {
        const Account& firstAccount = accounts.first();
        m_ui->label_4->setText(firstAccount.getBank() + " : " + formatMoney(firstAccount.getBalance()));
    } else {
        m_ui->label_4->setText("대표 계좌 : 0원");
    }
}

QString MainPage::formatMoney(int amount) const
{
    return QLocale(QLocale::Korean, QLocale::SouthKorea).toString(amount) + "원";
}

void MainPage::updateDetailHeader()
{
    const QList<Account>& accounts = m_accountManager.getAccounts();
    const int currentIndex = m_accountManager.getCurrentIndex();

    if (accounts.isEmpty() || currentIndex < 0 || currentIndex >= accounts.size()) {
        m_ui->label_4->setText("대표 계좌 : 0원");
        return;
    }

    const Account& currentAccount = accounts[currentIndex];
    m_ui->label_4->setText(currentAccount.getBank() + " " +
                         currentAccount.getAccountNumber() + " : " +
                         formatMoney(currentAccount.getBalance()));
}

QString MainPage::currentAccountSummary() const
{
    const QList<Account>& accounts = m_accountManager.getAccounts();
    const int currentIndex = m_accountManager.getCurrentIndex();

    if (accounts.isEmpty() || currentIndex < 0 || currentIndex >= accounts.size()) {
        return "계좌 없음 : 0원";
    }

    const Account& currentAccount = accounts[currentIndex];
    return currentAccount.getBank() + " : " + formatMoney(currentAccount.getBalance());
}

QStringList MainPage::currentAccountOptions() const
{
    QStringList options;
    const QList<Account>& accounts = m_accountManager.getAccounts();
    const QString nowAccount = currentAccountNumber(); // 현재 계좌번호


    for (const Account& account : accounts) {
        if (account.getAccountNumber() == nowAccount) continue; // 현재 계좌 스킵
        options << (account.getBank() + " : " + account.getAccountNumber());
    }

    return options;
}

QString MainPage::currentAccountNumber() const
{
    const QList<Account>& accounts = m_accountManager.getAccounts();
    const int currentIndex = m_accountManager.getCurrentIndex();

    if (accounts.isEmpty() || currentIndex < 0 || currentIndex >= accounts.size()) {
        return QString();
    }

    return accounts[currentIndex].getAccountNumber();
}

void MainPage::onDepositButtonClicked()
{
    handleTransactionDialog("입금");
}

void MainPage::onWithdrawButtonClicked()
{
    handleTransactionDialog("출금");
}

void MainPage::onTransferButtonClicked()
{
    TransferDialog transferDialog(currentAccountOptions(), this);
    if (transferDialog.exec() != QDialog::Accepted) {
        return;
    }

    QString targetBank;
    QString targetAccountNumber;

    if (transferDialog.isMyAccountTransfer()) {
        const QString selected = transferDialog.selectedMyAccountText();
        const QStringList parts = selected.split(" : ");
        if (parts.size() >= 2) {
            targetBank = parts[0];
            targetAccountNumber = parts[1];
        }
    } else {
        targetBank = transferDialog.targetBank();
        targetAccountNumber = transferDialog.targetAccountNumber();
    }

    handleTransactionDialog("송금",
                            targetBank,
                            targetAccountNumber,
                            transferDialog.isMyAccountTransfer());
}

void MainPage::handleTransactionDialog(const QString &actionName,
                                       const QString &targetBank,
                                       const QString &targetAccountNumber,
                                       bool ownAccountTransfer)
{
    if (m_currentUsername.isEmpty() || currentAccountNumber().isEmpty()) {
        showStyledMessageBox(this, QMessageBox::Warning, "오류", "현재 선택된 계좌가 없습니다.");
        return;
    }

    TransactionDialog dialog(actionName, currentAccountSummary(), this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    if (dialog.amount() <= 0) {
        showStyledMessageBox(this, QMessageBox::Warning, "오류", "0원보다 큰 금액을 입력하세요.");
        return;
    }

    if (dialog.password() <= 0) {
        showStyledMessageBox(this, QMessageBox::Warning, "오류", "계좌 비밀번호를 입력하세요.");
        return;
    }

    QString message;
    bool success = false;
    const QString accountNumber = currentAccountNumber();

    if (actionName == "입금") {
        success = m_accountManager.depositToAccount("users.json",
                                          m_currentUsername,
                                          m_currentOwnerName,
                                          accountNumber,
                                          dialog.amount(),
                                          dialog.password(),
                                          message);
    } else if (actionName == "출금") {
        success = m_accountManager.withdrawFromAccount("users.json",
                                             m_currentUsername,
                                             m_currentOwnerName,
                                             accountNumber,
                                             dialog.amount(),
                                             dialog.password(),
                                             message);
    } else if (actionName == "송금") {
        if (targetBank.isEmpty() || targetAccountNumber.isEmpty()) {
            showStyledMessageBox(this, QMessageBox::Warning, "오류", "송금할 계좌 정보를 입력하세요.");
            return;
        }

        if (ownAccountTransfer && targetAccountNumber == accountNumber) {
            showStyledMessageBox(this, QMessageBox::Warning, "오류", "현재 계좌와 같은 계좌로는 송금할 수 없습니다.");
            return;
        }

        success = m_accountManager.transferBetweenAccounts("users.json",
                                                  m_currentUsername,
                                                  m_currentOwnerName,
                                                  accountNumber,
                                                  targetBank,
                                                  targetAccountNumber,
                                                  dialog.amount(),
                                                  dialog.password(),
                                                  ownAccountTransfer,
                                                  message);
    }

    if (!success) {
        showStyledMessageBox(this, QMessageBox::Warning, actionName + " 실패", message);
        return;
    }

    refreshAccountTable();
    refreshTransactionTable();
    updateDetailHeader();

    showStyledMessageBox(this, QMessageBox::Information, "완료", message);
}

void MainPage::refreshTransactionTable()
{
    m_isUpdatingTransactionTable = true;
    m_ui->tableWidget_2->setRowCount(0);

    const QList<Account>& accounts = m_accountManager.getAccounts();
    const int currentIndex = m_accountManager.getCurrentIndex();
    if (accounts.isEmpty() || currentIndex < 0 || currentIndex >= accounts.size()) {
        m_isUpdatingTransactionTable = false;
        return;
    }

    const QList<Transaction>& history = accounts[currentIndex].getHistory();
    m_ui->tableWidget_2->setRowCount(history.size());

    for (int row = 0; row < history.size(); ++row) {
        const Transaction& transaction = history[row];
        auto *dateTimeItem = new QTableWidgetItem(transaction.getDatetime());
        auto *typeItem = new QTableWidgetItem(transaction.getType());
        auto *targetItem = new QTableWidgetItem(transaction.getTarget());
        auto *amountItem = new NumericTableWidgetItem(formatMoney(transaction.getAmount()));
        auto *noteItem = new QTableWidgetItem(transaction.getNote());
        noteItem->setData(Qt::UserRole, row);

        dateTimeItem->setFlags(dateTimeItem->flags() & ~Qt::ItemIsEditable);
        typeItem->setFlags(typeItem->flags() & ~Qt::ItemIsEditable);
        targetItem->setFlags(targetItem->flags() & ~Qt::ItemIsEditable);
        amountItem->setFlags(amountItem->flags() & ~Qt::ItemIsEditable);

        m_ui->tableWidget_2->setItem(row, 0, dateTimeItem);
        m_ui->tableWidget_2->setItem(row, 1, typeItem);
        m_ui->tableWidget_2->setItem(row, 2, targetItem);
        m_ui->tableWidget_2->setItem(row, 3, amountItem);
        m_ui->tableWidget_2->setItem(row, 4, noteItem);
    }

    m_isUpdatingTransactionTable = false;
}

void MainPage::onTransactionTableItemChanged(QTableWidgetItem *item)
{
    if (m_isUpdatingTransactionTable || item == nullptr || item->column() != 4) {
        return;
    }

    const QString accountNumber = currentAccountNumber();
    if (accountNumber.isEmpty()) {
        return;
    }

    const int historyIndex = item->data(Qt::UserRole).toInt();
    const int currentIndex = m_accountManager.getCurrentIndex();
    if (currentIndex >= 0 && currentIndex < m_accountManager.getAccounts().size()) {
        QList<Transaction> &history = m_accountManager.getAccounts()[currentIndex].getHistory();
        if (historyIndex >= 0 && historyIndex < history.size()) {
            history[historyIndex].setNote(item->text().trimmed());
        }
    }

    const bool saved = m_accountManager.updateTransactionNote("users.json",
                                                     m_currentUsername,
                                                     accountNumber,
                                                     historyIndex,
                                                     item->text().trimmed());

    if (!saved) {
        showStyledMessageBox(this, QMessageBox::Warning, "메모 저장 실패", "메모를 JSON 파일에 저장하지 못했습니다.");
    }
}

void MainPage::onMemoFilterTextChanged(const QString &text)
{
    for (int i = 0; i < m_ui->tableWidget_2->rowCount(); ++i) {
        QTableWidgetItem* memoItem = m_ui->tableWidget_2->item(i, 4);

        if (text.isEmpty()) {
            m_ui->tableWidget_2->setRowHidden(i, false);
            continue;
        }

        if (!memoItem || memoItem->text().isEmpty()) {
            m_ui->tableWidget_2->setRowHidden(i, true);
            continue;
        }

        const bool match = memoItem->text().contains(text, Qt::CaseInsensitive);
        m_ui->tableWidget_2->setRowHidden(i, !match);
    }
}

void MainPage::onDeleteAccountButtonClicked()
{
    if (m_currentUsername.isEmpty()) {
        showStyledMessageBox(this, QMessageBox::Warning, "오류", "현재 로그인한 계정이 없습니다.");
        return;
    }

    DeleteAccountDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString message;
    const bool deleted = m_accountManager.deleteUserByContact("users.json",
                                                     m_currentUsername,
                                                     dialog.phone(),
                                                     dialog.email(),
                                                     message);

    if (!deleted) {
        showStyledMessageBox(this, QMessageBox::Warning, "계정 삭제 실패", message);
        return;
    }

    showStyledMessageBox(this, QMessageBox::Information, "완료", message);
    onLogoutButtonClicked();
}
