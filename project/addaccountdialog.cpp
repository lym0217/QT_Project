#include "addaccountdialog.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

AddAccountDialog::AddAccountDialog(QWidget *parent)
    : QDialog(parent)
    , m_bankComboBox(new QComboBox(this))
    , m_accountNumberEdit(new QLineEdit(this))
    , m_balanceEdit(new QLineEdit(this))
    , m_balancePasswordEdit(new QLineEdit(this))
{
    setWindowTitle("계좌 추가");
    resize(360, 260);

    auto *titleLabel = new QLabel("새 계좌 정보 입력", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: 700; color: #17324d;");

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
    auto *bankListView = new QListView(m_bankComboBox);
    bankListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bankListView->setUniformItemSizes(true);
    bankListView->setSpacing(0);
    m_bankComboBox->setView(bankListView);
    m_bankComboBox->addItems(bankList);

    m_accountNumberEdit->setPlaceholderText("예: 100-200-3000");
    m_balanceEdit->setText("0");
    m_balanceEdit->setReadOnly(true);
    m_balancePasswordEdit->setPlaceholderText("예: 123456");
    m_balanceEdit->setValidator(new QIntValidator(0, 1000000000, this));
    m_balancePasswordEdit->setValidator(new QIntValidator(0, 999999, this));
    m_balancePasswordEdit->setEchoMode(QLineEdit::Password);

    auto *formLayout = new QFormLayout();
    formLayout->addRow("은행명", m_bankComboBox);
    formLayout->addRow("계좌번호", m_accountNumberEdit);
    formLayout->addRow("잔액", m_balanceEdit);
    formLayout->addRow("계좌 비밀번호", m_balancePasswordEdit);

    auto *okButton = new QPushButton("추가", this);
    auto *cancelButton = new QPushButton("취소", this);

    connect(okButton, &QPushButton::clicked, this, &AddAccountDialog::validateAndAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #f8fbff, stop:0.55 #eef8ff, stop:1 #f4fffb);
        }
        QLabel {
            color: #35546b;
            font-size: 13px;
            font-weight: 600;
        }
        QLineEdit, QComboBox {
            background: white;
            border: 1px solid #c9dced;
            border-radius: 10px;
            padding: 8px 10px;
            min-height: 18px;
        }
        QLineEdit[readOnly="true"] {
            background: #f2f6fa;
            color: #4d6476;
        }
        QLineEdit:focus, QComboBox:focus {
            border: 2px solid #5495d8;
        }
        QComboBox QAbstractItemView {
            background: white;
            border: 1px solid #c9dced;
            selection-background-color: #d9edff;
            selection-color: #17324d;
            outline: none;
        }
        QComboBox QAbstractItemView::item {
            min-height: 30px;
            max-height: 30px;
            padding: 4px 8px;
        }
        QPushButton {
            border: none;
            border-radius: 12px;
            padding: 9px 16px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton:hover {
            opacity: 0.92;
        }
    )");

    okButton->setStyleSheet("background:white; color:black; border:1px solid black;");
    cancelButton->setStyleSheet("background:white; color:black; border:1px solid black;");
}

QString AddAccountDialog::bank() const
{
    return m_bankComboBox->currentText().trimmed();
}

QString AddAccountDialog::accountNumber() const
{
    return m_accountNumberEdit->text().trimmed();
}

int AddAccountDialog::balance() const
{
    return m_balanceEdit->text().toInt();
}

int AddAccountDialog::balancePassword() const
{
    return m_balancePasswordEdit->text().toInt();
}

void AddAccountDialog::validateAndAccept()
{
    if (accountNumber().isEmpty() || m_balancePasswordEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "모든 계좌 정보를 입력해주세요.");
        return;
    }

    accept();
}
