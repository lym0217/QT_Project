#include "deleteaccountdialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

DeleteAccountDialog::DeleteAccountDialog(QWidget *parent)
    : QDialog(parent)
    , phoneEdit(new QLineEdit(this))
    , emailEdit(new QLineEdit(this))
{
    setWindowTitle("계정 삭제");
    resize(360, 220);

    auto *brandLabel = new QLabel("[T055 BANK]", this);
    brandLabel->setObjectName("brandLabel");

    auto *titleLabel = new QLabel("전화번호와 이메일을 모두 입력하세요", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("titleLabel");

    phoneEdit->setPlaceholderText("전화번호");
    emailEdit->setPlaceholderText("이메일");

    auto *okButton = new QPushButton("확인", this);
    auto *cancelButton = new QPushButton("취소", this);
    okButton->setObjectName("confirmButton");
    cancelButton->setObjectName("cancelButton");

    connect(okButton, &QPushButton::clicked, this, &DeleteAccountDialog::validateAndAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(brandLabel, 0, Qt::AlignLeft);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(phoneEdit);
    mainLayout->addWidget(emailEdit);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #fffaf9, stop:0.55 #fff5f2, stop:1 #fff8f6);
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
            font-size: 18px;
            font-weight: 700;
        }
        QLineEdit {
            background: white;
            border: 1px solid #d8c6c1;
            border-radius: 10px;
            padding: 9px 12px;
            min-height: 20px;
        }
        QLineEdit:focus {
            border: 2px solid #cf7c75;
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
    )");
}

QString DeleteAccountDialog::phone() const
{
    return phoneEdit->text().trimmed();
}

QString DeleteAccountDialog::email() const
{
    return emailEdit->text().trimmed();
}

void DeleteAccountDialog::validateAndAccept()
{
    if (phone().isEmpty() || email().isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "전화번호와 이메일을 모두 입력해주세요.");
        return;
    }

    accept();
}
