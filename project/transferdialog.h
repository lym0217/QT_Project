#ifndef TRANSFERDIALOG_H
#define TRANSFERDIALOG_H

#include <QDialog>
#include <QStringList>

class QComboBox;
class QLineEdit;
class QRadioButton;

/**
 * @brief TransferDialog 클래스는 송금 대상 계좌를 선택하거나 입력받는다.
 *
 * 멤버 변수
 * - m_myAccountRadio, m_otherAccountRadio: 송금 대상 유형 선택 라디오 버튼
 * - m_myAccountComboBox: 내 계좌 목록 선택 콤보박스
 * - m_otherBankComboBox: 외부 은행 선택 콤보박스
 * - m_otherAccountEdit: 외부 계좌번호 입력창
 *
 * 주요 함수
 * - isMyAccountTransfer(): 내 계좌 송금 여부 반환
 * - selectedMyAccountText(), targetBank(), targetAccountNumber(): 선택된 송금 대상 정보 반환
 * - updateMode(): 라디오 버튼 상태에 따라 입력 가능 항목을 전환한다.
 */
class TransferDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TransferDialog(const QStringList &myAccounts, QWidget *parent = nullptr);
    bool isMyAccountTransfer() const;
    QString selectedMyAccountText() const;
    QString targetBank() const;
    QString targetAccountNumber() const;

private:
    void updateMode();

    QRadioButton *m_myAccountRadio;
    QRadioButton *m_otherAccountRadio;
    QComboBox *m_myAccountComboBox;
    QComboBox *m_otherBankComboBox;
    QLineEdit *m_otherAccountEdit;
};

#endif // TRANSFERDIALOG_H
