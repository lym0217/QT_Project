#include "loginpage.h"
#include "ui_loginpage.h"
#include "signup.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::LoginPage)
{
    m_ui->setupUi(this);
    m_ui->pw_checkBox->setIcon(QIcon(":/resources/hidden1.png"));

    setWindowTitle("간편 입출금 계좌 관리");

    m_ui->label->setText("T055");
    m_ui->label_2->setText("아이디");
    m_ui->label_3->setText("비밀번호");
    m_ui->id_lineEdit->setPlaceholderText("아이디를 입력하세요");
    m_ui->pw_lineEdit->setPlaceholderText("비밀번호를 입력하세요");
    m_ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
    m_ui->pw_checkBox->setCursor(Qt::PointingHandCursor);
    m_ui->label_2->setFixedWidth(84);
    m_ui->label_3->setFixedWidth(84);
    m_ui->id_lineEdit->setFixedWidth(220);
    m_ui->pw_lineEdit->setFixedWidth(220);
    m_ui->pw_checkBox->setFixedSize(24, 24);
    m_ui->login_btn->setFixedWidth(328);
    m_ui->pushButton->setFixedWidth(328);
    m_ui->gridLayout->setAlignment(Qt::AlignHCenter);
    m_ui->gridLayout->setHorizontalSpacing(10);
    m_ui->gridLayout->setVerticalSpacing(12);

    connect(m_ui->login_btn, &QPushButton::clicked, this, &LoginPage::onLoginButtonClicked);
    connect(m_ui->pw_checkBox, &QCheckBox::checkStateChanged, this, &LoginPage::onPasswordVisibilityChanged);
    connect(m_ui->pushButton, &QPushButton::clicked, this, &LoginPage::onSignupButtonClicked);

    setStyleSheet(R"(
        QWidget#LoginPage {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #eef6ff, stop:0.55 #dff4ee, stop:1 #fff3dc);
        }
        QLabel {
            color: #284b63;
        }
        QLabel#label {
            color: #10324a;
            font-size: 38px;
            font-weight: 700;
            letter-spacing: 2px;
            margin-top: 28px;
            margin-bottom: 18px;
        }
        QLabel#label_2, QLabel#label_3 {
            color: #47657a;
            font-size: 14px;
            font-weight: 600;
            padding: 8px 0;
        }
        QLineEdit {
            background: rgba(255, 255, 255, 0.92);
            color: #163447;
            border: 2px solid #c8deeb;
            border-radius: 12px;
            padding: 10px 14px;
            min-height: 20px;
            selection-background-color: #63c3b2;
        }
        QLineEdit:focus {
            border-color: #4f9fd8;
            background: #ffffff;
        }
        QPushButton {
            border: none;
            border-radius: 14px;
            padding: 12px 18px;
            font-size: 14px;
            font-weight: 700;
        }
        QPushButton#login_btn {
            color: white;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #2f80ed, stop:1 #3bb2b8);
        }
        QPushButton#login_btn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #2472db, stop:1 #32a2a8);
        }
        QPushButton#pushButton {
            color: #1b4965;
            background: rgba(255, 255, 255, 0.78);
            border: 1px solid #bcd4e6;
        }
        QPushButton#pushButton:hover {
            background: rgba(255, 255, 255, 0.96);
        }
        QCheckBox {
            spacing: 0px;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
    )");
}

LoginPage::~LoginPage()
{
    delete m_ui;
}

void LoginPage::clearInputs()
{
    m_ui->id_lineEdit->clear();
    m_ui->pw_lineEdit->clear();
    m_ui->pw_checkBox->setChecked(false);
    m_ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
    m_ui->id_lineEdit->setFocus();
}

void LoginPage::onLoginButtonClicked()
{
    const QString id = m_ui->id_lineEdit->text();
    const QString password = m_ui->pw_lineEdit->text();

    if (id.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "오류", "아이디와 비밀번호를 입력하세요.");
        return;
    }

    QFile file("users.json");

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "오류", "파일을 열 수 없습니다.");
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray users = doc.object()["users"].toArray();

    file.close();

    // 입력 비밀번호 해시
    const QString hashedPassword = QCryptographicHash::hash(
                           password.toUtf8(),
                           QCryptographicHash::Sha256
                           ).toHex();

    // 사용자 검사
    for (const auto& userValue : users)
    {
        const QJsonObject userObject = userValue.toObject();

        if (userObject["username"].toString() == id &&
            userObject["password"].toString() == hashedPassword)
        {
            emit loginSuccess(userObject["name"].toString(), userObject["username"].toString());
            return;
        }
    }

    // 실패
    QMessageBox::warning(this, "로그인 실패", "아이디 또는 비밀번호가 틀렸습니다.");
}
void LoginPage::onPasswordVisibilityChanged(const Qt::CheckState &state)
{
    if (state == Qt::Checked)
    {
        m_ui->pw_lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        m_ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
    }
}


void LoginPage::onSignupButtonClicked()
{
    SignUp dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, "완료", "회원가입이 완료되었습니다.");
    }
}

