#include "loginpage.h"
#include "ui_loginpage.h"
#include<QMessageBox>

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

    if(id == "test" && pw == "1234")
    {
        emit loginSuccess();  // MainWindow로 신호 전달
    }
    else
    {
        QMessageBox::warning(this, "오류", "로그인 실패");
    }
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

