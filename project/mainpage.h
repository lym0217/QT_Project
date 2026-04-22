#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>
#include "account_manager.h"

class QTableWidgetItem;

namespace Ui {
class MainPage;
}

/**
 * @brief MainPage 클래스는 로그인 후 표시되는 메인 대시보드 화면을 담당한다.
 *
 * 멤버 변수
 * - m_ui: Qt Designer로 만든 메인 화면 UI
 * - m_accountManager: 계좌 및 거래내역 로드/저장 로직
 * - m_currentUsername: 로그인한 사용자의 아이디
 * - m_currentOwnerName: 로그인한 사용자의 실명
 * - m_isUpdatingTransactionTable: 거래내역 테이블을 코드로 갱신 중인지 여부
 *
 * 주요 함수
 * - setCurrentUser(): 현재 로그인 사용자 정보를 설정한다.
 * - showHomePage(), showDetailPage(): 메인 1페이지/2페이지 전환
 * - refreshAccountTable(), refreshTransactionTable(): 화면 데이터 갱신
 * - handleTransactionDialog(): 입금, 출금, 송금 공통 처리
 */
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
    void onLogoutButtonClicked();
    void onBackButtonClicked();
    void onAddAccountButtonClicked();
    void onAccountTableCellDoubleClicked(int row, int column);
    void onDepositButtonClicked();
    void onWithdrawButtonClicked();
    void onTransferButtonClicked();
    void onTransactionTableItemChanged(QTableWidgetItem *item);
    void onDeleteAccountButtonClicked();
    void onMemoFilterTextChanged(const QString &text);

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

    Ui::MainPage *m_ui;
    AccountManager m_accountManager;
    QString m_currentUsername;
    QString m_currentOwnerName;
    bool m_isUpdatingTransactionTable = false;
};

#endif // MAINPAGE_H
