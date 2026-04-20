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
}

LoginPage::~LoginPage()
{
    delete ui;
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
            emit loginSuccess();   // 나중에는 사용자에 따른 계좌 리스트 출력
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

