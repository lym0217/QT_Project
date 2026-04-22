#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief TransactionDialog 클래스는 입금, 출금, 송금 시 금액과 계좌 비밀번호를 입력받는다.
 *
 * 멤버 변수
 * - m_accountValueLabel: 현재 계좌 요약 표시 라벨
 * - m_amountEdit: 금액 입력창
 * - m_passwordEdit: 계좌 비밀번호 입력창
 * - m_confirmButton, m_cancelButton: 다이얼로그 제어 버튼
 *
 * 주요 함수
 * - amount(): 입력 금액 반환
 * - password(): 입력한 계좌 비밀번호 반환
 */
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
    QLabel *m_accountValueLabel;
    QLineEdit *m_amountEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_confirmButton;
    QPushButton *m_cancelButton;
};

#endif // TRANSACTIONDIALOG_H
