#include "transfer_dialog.h"
#include "ui_transfer_dialog.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QIntValidator>


transfer_dialog::transfer_dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::transfer_dialog)
{
    ui->setupUi(this);
    // phase1 시작
    ui->stackedWidget->setCurrentIndex(0);

    // 내 계좌 기본 선택
    ui->my_radio->setChecked(true);
    ui->other_comboBox->setEnabled(false);
    ui->account_number->setEnabled(false);

    // 금액 한도 설정
    ui->money_lineEdit->setValidator(new QIntValidator(1, 100000000, this));

    // 비밀번호 숨김
    ui->pw_lineEdit->setEchoMode(QLineEdit::Password);

    // 라디오버튼 연결
    connect(ui->my_radio,    SIGNAL(toggled(bool)), this, SLOT(onRadioToggled()));
    connect(ui->other_radio, SIGNAL(toggled(bool)), this, SLOT(onRadioToggled()));

    // phase1 OK → goToPhase2, 취소
    connect(ui->buttonBox_2, SIGNAL(accepted()), this, SLOT(goToPhase2()));
    connect(ui->buttonBox_2, SIGNAL(rejected()), this, SLOT(reject()));

    // phase2 OK → accept, 취소 → phase1
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

transfer_dialog::~transfer_dialog()
{
    delete ui;
}
// 송금 버튼 누른 직후 phase1출력
void transfer_dialog::setPhase1_Data(QStringList myAccounts)
{
    ui->mybank_comboBox->clear();
    for (QString account : myAccounts)
        ui->mybank_comboBox->addItem(account);
}
// Radiobutton 함수
void transfer_dialog::onRadioToggled()
{
    bool mySelected = ui->my_radio->isChecked();
    ui->mybank_comboBox->setEnabled(mySelected);
    ui->other_comboBox->setEnabled(!mySelected);
    ui->account_number->setEnabled(!mySelected);
}
//ok버튼 누르면 phase2 송금 내계좌 외부계좌선택
void transfer_dialog::goToPhase2()
{
    QString toBank = "";
    QString toAccountNum = "";

    if (ui->my_radio->isChecked())
    {
        // 내 계좌 선택
        QString selected = ui->mybank_comboBox->currentText();
        // "KD은행 : 100-200-3000" 형식에서 분리
        QStringList parts = selected.split(" : ");
        toBank       = "[My] " + parts[0].trimmed();
        toAccountNum = parts.size() > 1 ? parts[1].trimmed() : "";
        myAccountSelected = true;
    } else
    {
        // 외부 계좌선택
        if (ui->account_number->text().isEmpty()) {
            QMessageBox::warning(this, "오류", "계좌번호를 입력해주세요.");
            return;
        }
        toBank       = ui->other_comboBox->currentText();
        toAccountNum = ui->account_number->text();
        myAccountSelected = false;
    }

    // phase2 데이터 세팅은 mainpage하고 여기에서는선 bank/account만 넘김
    ui->transfer_bank->setText(toBank);
    ui->transfer_account_2->setText(toAccountNum);
    ui->stackedWidget->setCurrentIndex(1);
}
// phase2 data정리
void transfer_dialog::setPhase2_Data(QString fromBank, int fromBalance, QString toBank, QString toAccountNum, bool isMyAccount)
{
    ui->current_account->setText(fromBank + " : ");
    ui->current_money->setText(QLocale(QLocale::Korean).toString(fromBalance) + "원");
    ui->transfer_bank->setText(isMyAccount ? "[My] " + toBank : toBank);
    ui->transfer_account_2->setText(toAccountNum);
}
// phase2에서 입력한 송금 금액 반환
int transfer_dialog::getTransferAmount()
{
    QString text = ui->money_lineEdit->text();
    text.remove(",");
    return text.toInt();
}
//phase2에서 입력한 비밀번호 반환
int transfer_dialog::getPassword()
{
    return ui->pw_lineEdit->text().toInt();
}
// 송금 대상 은행명 반환
QString transfer_dialog::getTargetBank()
{
    return ui->transfer_bank->text();
}
// 송금 대상 계좌번호
QString transfer_dialog::getTargetAccountNum()
{
    return ui->transfer_account_2->text();
}
// 내계좌인지 외부계좌인지 판단
bool transfer_dialog::isMyAccountTransfer()
{
    return myAccountSelected;
}