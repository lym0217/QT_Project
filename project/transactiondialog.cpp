#include "transactiondialog.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

TransactionDialog::TransactionDialog(const QString &actionName,
                                     const QString &currentAccountText,
                                     QWidget *parent)
    : QDialog(parent)
    , actionName(actionName)
    , accountValueLabel(new QLabel(currentAccountText, this))
    , amountEdit(new QLineEdit(this))
    , passwordEdit(new QLineEdit(this))
    , confirmButton(new QPushButton("확인", this))
    , cancelButton(new QPushButton("취소", this))
{
    setWindowTitle(actionName);
    resize(380, 320);

    auto *brandLabel = new QLabel("[T055 BANK]", this);
    brandLabel->setObjectName("brandLabel");

    auto *currentAccountLabel = new QLabel("현재 계좌", this);
    currentAccountLabel->setAlignment(Qt::AlignCenter);
    currentAccountLabel->setObjectName("titleLabel");

    accountValueLabel->setAlignment(Qt::AlignCenter);
    accountValueLabel->setObjectName("accountValueLabel");

    amountEdit->setPlaceholderText("금액을 입력하세요");
    amountEdit->setValidator(new QIntValidator(0, 100000000, this));

    passwordEdit->setPlaceholderText("계좌 비밀번호를 입력하세요");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setValidator(new QIntValidator(0, 999999, this));

    auto *amountLayout = new QHBoxLayout();
    auto *amountLabel = new QLabel("금액", this);
    amountLabel->setFixedWidth(52);
    amountLayout->addWidget(amountLabel);
    amountLayout->addWidget(amountEdit);

    auto *passwordLayout = new QHBoxLayout();
    auto *passwordLabel = new QLabel("비밀번호", this);
    passwordLabel->setFixedWidth(52);
    passwordLayout->addWidget(passwordLabel);
    passwordLayout->addWidget(passwordEdit);

    auto *limitLabel = new QLabel("이체금액한도 1억", this);
    limitLabel->setAlignment(Qt::AlignCenter);
    limitLabel->setObjectName("limitLabel");

    auto *buttonLayout = new QHBoxLayout();
    confirmButton->setObjectName("confirmButton");
    cancelButton->setObjectName("cancelButton");
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addWidget(cancelButton);

    connect(confirmButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(brandLabel, 0, Qt::AlignLeft);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(currentAccountLabel);
    mainLayout->addWidget(accountValueLabel);
    mainLayout->addSpacing(8);
    mainLayout->addLayout(amountLayout);
    mainLayout->addLayout(passwordLayout);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(limitLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #f8fbff, stop:0.55 #eef8ff, stop:1 #f4fffb);
        }
        QLabel {
            color: #284b63;
            font-size: 13px;
        }
        QLabel#brandLabel {
            color: #2a6f97;
            font-size: 14px;
            font-weight: 700;
        }
        QLabel#titleLabel {
            color: #17324d;
            font-size: 18px;
            font-weight: 700;
        }
        QLabel#accountValueLabel {
            color: #35546b;
            font-size: 15px;
            font-weight: 600;
            background: rgba(255, 255, 255, 0.82);
            border: 1px solid #d4e4f1;
            border-radius: 12px;
            padding: 12px;
        }
        QLabel#limitLabel {
            color: #5c7182;
            font-size: 12px;
            font-weight: 600;
        }
        QLineEdit {
            background: white;
            border: 1px solid #c9dced;
            border-radius: 10px;
            padding: 9px 12px;
            min-height: 20px;
        }
        QLineEdit:focus {
            border: 2px solid #5495d8;
        }
        QPushButton {
            border-radius: 12px;
            padding: 10px 16px;
            font-size: 13px;
            font-weight: 700;
            min-width: 120px;
        }
        QPushButton#confirmButton {
            background: #2f80ed;
            color: white;
            border: none;
        }
        QPushButton#cancelButton {
            background: white;
            color: #29465b;
            border: 1px solid #d0dfeb;
        }
    )");
}

int TransactionDialog::amount() const
{
    return amountEdit->text().toInt();
}

int TransactionDialog::password() const
{
    return passwordEdit->text().toInt();
}
