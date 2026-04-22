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
    , m_myAccountRadio(new QRadioButton("<내 계좌로 보내기>", this))
    , m_otherAccountRadio(new QRadioButton("<다른 계좌로 보내기>", this))
    , m_myAccountComboBox(new QComboBox(this))
    , m_otherBankComboBox(new QComboBox(this))
    , m_otherAccountEdit(new QLineEdit(this))
{
    setWindowTitle("송금");
    resize(430, 320);

    auto *brandLabel = new QLabel("[T055 BANK]", this);
    brandLabel->setObjectName("brandLabel");

    auto *titleLabel = new QLabel("어디로 돈을 보낼까요?", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    m_myAccountComboBox->addItems(myAccounts);
    m_myAccountComboBox->setEnabled(!myAccounts.isEmpty());

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
    m_otherBankComboBox->addItems(bankList);
    m_otherAccountEdit->setPlaceholderText("계좌번호를 입력하세요");

    auto *otherAccountLayout = new QHBoxLayout();
    otherAccountLayout->addWidget(m_otherBankComboBox);
    otherAccountLayout->addWidget(m_otherAccountEdit);

    auto *okButton = new QPushButton("확인", this);
    auto *cancelButton = new QPushButton("취소", this);
    okButton->setObjectName("confirmButton");
    cancelButton->setObjectName("cancelButton");

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_myAccountRadio, &QRadioButton::toggled, this, [this]() { updateMode(); });
    connect(m_otherAccountRadio, &QRadioButton::toggled, this, [this]() { updateMode(); });

    m_myAccountRadio->setChecked(true);
    updateMode();

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(brandLabel, 0, Qt::AlignLeft);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_myAccountRadio);
    mainLayout->addWidget(m_myAccountComboBox);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(m_otherAccountRadio);
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
    const bool useMyAccount = m_myAccountRadio->isChecked();
    m_myAccountComboBox->setEnabled(useMyAccount);
    m_otherBankComboBox->setEnabled(!useMyAccount);
    m_otherAccountEdit->setEnabled(!useMyAccount);
}

bool TransferDialog::isMyAccountTransfer() const
{
    return m_myAccountRadio->isChecked();
}

QString TransferDialog::selectedMyAccountText() const
{
    return m_myAccountComboBox->currentText();
}

QString TransferDialog::targetBank() const
{
    return m_otherBankComboBox->currentText();
}

QString TransferDialog::targetAccountNumber() const
{
    return m_otherAccountEdit->text().trimmed();
}
