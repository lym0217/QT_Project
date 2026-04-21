#include "tx_dialog.h"
#include "ui_tx_dialog.h"
#include <QPushButton>
#include <QIntValidator>
#include <QMessageBox>

tx_dialog::tx_dialog(QWidget *parent, QString type)
    : QDialog(parent)
    , ui(new Ui::tx_dialog)
    , type(type)
{
    ui->setupUi(this);

    disconnect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()),
            this, SLOT(onOkClicked()));
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()),
            this, SLOT(reject()));

    // 타입에 따라 라벨 변경
    ui->type_label->setText(type + " 금액 :");
    ui->notice_label->setText("금액 한도는 100,000,000원입니다.");

    // OK/Cancel 버튼 텍스트
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(type);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("취소");

    // 금액 입력 한도 설정 (1억)
    ui->money_lineEdit->setValidator(new QIntValidator(1, 100000000, this));

    // 비밀번호 숨김처리
    ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
}

tx_dialog::~tx_dialog()
{
    delete ui;
}

void tx_dialog::setCurrentAccount(QString bank, QString balance)
{
    ui->current_account->setText(bank + " : " + balance);
}

int tx_dialog::getAmount()
{
    QString text = ui->money_lineEdit->text();
    text.remove(",");
    return text.toInt();
}

int tx_dialog::getPassword()
{
    return ui->pw_lineEdit->text().toInt();
}

void tx_dialog::setBalance(int balance)
{
    this->balance = balance;
}


void tx_dialog::onOkClicked()
{

    int amount = getAmount();
    if (amount <= 0) {
        QMessageBox::warning(this, "오류", "올바른 금액을 입력해주세요.");
        return;
    }

    accept();
}

