#include "loginpage.h"
#include "ui_loginpage.h"
#include "signup.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QMessageBox>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    ui->pw_checkBox->setIcon(QIcon(":/resources/hidden1.png"));

    setWindowTitle("간편 입출금 계좌 관리");

    ui->label->setText("T055");
    ui->label_2->setText("아이디");
    ui->label_3->setText("비밀번호");
    ui->id_lineEdit->setPlaceholderText("아이디를 입력하세요");
    ui->pw_lineEdit->setPlaceholderText("비밀번호를 입력하세요");
    ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
    ui->pw_checkBox->setCursor(Qt::PointingHandCursor);
    ui->label_2->setFixedWidth(84);
    ui->label_3->setFixedWidth(84);
    ui->id_lineEdit->setFixedWidth(220);
    ui->pw_lineEdit->setFixedWidth(220);
    ui->pw_checkBox->setFixedSize(24, 24);
    ui->login_btn->setFixedWidth(328);
    ui->pushButton->setFixedWidth(328);
    ui->gridLayout->setAlignment(Qt::AlignHCenter);
    ui->gridLayout->setHorizontalSpacing(10);
    ui->gridLayout->setVerticalSpacing(12);

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
    delete ui;
}

void LoginPage::clearInputs()
{
    ui->id_lineEdit->clear();
    ui->pw_lineEdit->clear();
    ui->pw_checkBox->setChecked(false);
    ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
    ui->id_lineEdit->setFocus();
}

void LoginPage::on_login_btn_clicked()
{
    QString id = ui->id_lineEdit->text();
    QString pw = ui->pw_lineEdit->text();

    if(id.isEmpty() || pw.isEmpty())
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
    QString hashedPw = QCryptographicHash::hash(
                           pw.toUtf8(),
                           QCryptographicHash::Sha256
                           ).toHex();

    // 사용자 검사
    for(auto u : users)
    {
        QJsonObject obj = u.toObject();

        if(obj["username"].toString() == id &&
            obj["password"].toString() == hashedPw)
        {
            // 로그인 성공
            emit loginSuccess(obj["name"].toString(), obj["username"].toString());
            return;
        }
    }

    // 실패
    QMessageBox::warning(this, "로그인 실패", "아이디 또는 비밀번호가 틀렸습니다.");
}
void LoginPage::on_pw_checkBox_checkStateChanged(const Qt::CheckState &arg1)
{
    if(arg1 == Qt::Checked)
    {
        ui->pw_lineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
    }
}


void LoginPage::on_pushButton_clicked()
{
    SignUp dlg(this);
    if(dlg.exec() == QDialog::Accepted)
    {
        QMessageBox::information(this, "완료", "회원가입이 완료되었습니다.");
    }
}

