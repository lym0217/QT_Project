#ifndef ADDACCOUNTDIALOG_H
#define ADDACCOUNTDIALOG_H

#include <QDialog>

class QComboBox;
class QLineEdit;

/**
 * @brief AddAccountDialog 클래스는 새 계좌 등록 정보를 입력받는 다이얼로그다.
 *
 * 멤버 변수
 * - m_bankComboBox: 은행 목록 선택 콤보박스
 * - m_accountNumberEdit: 계좌번호 입력창
 * - m_balanceEdit: 초기 잔액 표시창
 * - m_balancePasswordEdit: 계좌 비밀번호 입력창
 *
 * 주요 함수
 * - bank(), accountNumber(), balance(), balancePassword(): 입력값 반환
 * - validateAndAccept(): 필수 입력 확인 후 다이얼로그를 닫는다.
 */
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
    QComboBox *m_bankComboBox;
    QLineEdit *m_accountNumberEdit;
    QLineEdit *m_balanceEdit;
    QLineEdit *m_balancePasswordEdit;
};

#endif // ADDACCOUNTDIALOG_H
