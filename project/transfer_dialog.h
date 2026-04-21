#ifndef TRANSFER_DIALOG_H
#define TRANSFER_DIALOG_H
#include <QDialog>

namespace Ui {
class transfer_dialog;
}

class transfer_dialog : public QDialog
{
    Q_OBJECT
public:
    explicit transfer_dialog(QWidget *parent = nullptr);
    ~transfer_dialog();

    // 계좌 콤보박스에 계좌 목록 채우기
    void setPhase1_Data(QStringList myAccounts);
    // phase2에 현재계좌, 송금계좌 정보 표시
    void setPhase2_Data(QString fromBank, int fromBalance, QString toBank, QString toAccountNum, bool isMyAccount);

    int getTransferAmount();            // 입력 송금 금액 반환
    int getPassword();                  // 입력 패스워드
    QString getTargetBank();            // 송금 대상 은행
    QString getTargetAccountNum();      // 송금대상 계좌번호
    bool isMyAccountTransfer();         // 내계좌인지 외부계좌인지 판단

private slots:
    void goToPhase2();
    void onRadioToggled();

private:
    Ui::transfer_dialog *ui;
    bool myAccountSelected = false;
};

#endif // TRANSFER_DIALOG_H