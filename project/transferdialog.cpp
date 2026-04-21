#include "transferdialog.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

TransferDialog::TransferDialog(const QStringList &myAccounts, QWidget *parent)
    : QDialog(parent)
    , myAccountRadio(new QRadioButton("<내 계좌로 보내기>", this))
    , otherAccountRadio(new QRadioButton("<다른 계좌로 보내기>", this))
    , myAccountComboBox(new QComboBox(this))
    , otherBankComboBox(new QComboBox(this))
    , otherAccountEdit(new QLineEdit(this))
{
    setWindowTitle("송금");
    resize(430, 320);

    auto *brandLabel = new QLabel("[T055 BANK]", this);
    brandLabel->setObjectName("brandLabel");

    auto *titleLabel = new QLabel("어디로 돈을 보낼까요?", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    myAccountComboBox->addItems(myAccounts);
    myAccountComboBox->setEnabled(!myAccounts.isEmpty());

    const QStringList bankList = {
        "NH농혁은행",
        "KD국닌은행",
        "카카5뱅크",
        "신둘은행",
        "T055뱅크",
        "LBK기업은행",
        "NG헌마을금고",
        "좌체국은행",
        "두리은행",
        "혼자은행"
    };
    otherBankComboBox->addItems(bankList);
    otherAccountEdit->setPlaceholderText("계좌번호를 입력하세요");

    auto *otherAccountLayout = new QHBoxLayout();
    otherAccountLayout->addWidget(otherBankComboBox);
    otherAccountLayout->addWidget(otherAccountEdit);

    auto *okButton = new QPushButton("확인", this);
    auto *cancelButton = new QPushButton("취소", this);
    okButton->setObjectName("confirmButton");
    cancelButton->setObjectName("cancelButton");

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(myAccountRadio, &QRadioButton::toggled, this, [this]() { updateMode(); });
    connect(otherAccountRadio, &QRadioButton::toggled, this, [this]() { updateMode(); });

    myAccountRadio->setChecked(true);
    updateMode();

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(brandLabel, 0, Qt::AlignLeft);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(myAccountRadio);
    mainLayout->addWidget(myAccountComboBox);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(otherAccountRadio);
    mainLayout->addLayout(otherAccountLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #f8fbff, stop:0.55 #eef8ff, stop:1 #f4fffb);
        }
        QLabel {
            color: #284b63;
        }
        QLabel#brandLabel {
            color: #2a6f97;
            font-size: 14px;
            font-weight: 700;
        }
        QLabel#titleLabel {
            color: #17324d;
            font-size: 20px;
            font-weight: 700;
        }
        QRadioButton {
            color: #35546b;
            font-size: 14px;
            font-weight: 700;
            padding: 6px 0;
        }
        QComboBox, QLineEdit {
            background: white;
            border: 1px solid #c9dced;
            border-radius: 10px;
            padding: 9px 12px;
            min-height: 20px;
        }
        QComboBox:focus, QLineEdit:focus {
            border: 2px solid #5495d8;
        }
        QPushButton {
            background: white;
            color: black;
            border: 1px solid black;
            border-radius: 12px;
            padding: 10px 16px;
            font-size: 13px;
            font-weight: 700;
            min-width: 120px;
        }
        QPushButton#confirmButton {
            background: white;
            color: black;
            border: 1px solid black;
        }
        QPushButton#cancelButton {
            background: white;
            color: black;
            border: 1px solid black;
        }
    )");
}

void TransferDialog::updateMode()
{
    const bool useMyAccount = myAccountRadio->isChecked();
    myAccountComboBox->setEnabled(useMyAccount);
    otherBankComboBox->setEnabled(!useMyAccount);
    otherAccountEdit->setEnabled(!useMyAccount);
}

bool TransferDialog::isMyAccountTransfer() const
{
    return myAccountRadio->isChecked();
}

QString TransferDialog::selectedMyAccountText() const
{
    return myAccountComboBox->currentText();
}

QString TransferDialog::targetBank() const
{
    return otherBankComboBox->currentText();
}

QString TransferDialog::targetAccountNumber() const
{
    return otherAccountEdit->text().trimmed();
}
