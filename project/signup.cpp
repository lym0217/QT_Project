#include "signup.h"
#include "ui_signup.h"
#include "signupmanager.h"


SignUp::SignUp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_id_check_btn_clicked()
{

    QString id = ui->id_lineEdit->text().trimmed();

    if(id.isEmpty())
    {
        QMessageBox::warning(this, "오류", "아이디를 입력해주세요.");
        return;
    }

    SignupManager manager;

    if(manager.isUsernameAvailable(id))
    {
        QMessageBox::information(this, "확인", "사용 가능한 아이디입니다.");
    }
    else
    {
        QMessageBox::warning(this, "확인", "사용중인 아이디입니다.");
    }
}

void SignUp::on_ok_btn_clicked()
{
    QString id = ui->id_lineEdit->text().trimmed();
    QString pw = ui->pw_lineEdit->text();
    QString pw_check = ui->pw_check_lineEdit->text();
    QString name = ui->name_lineEdit->text().trimmed();
    QString phone = ui->phone_lineEdit->text().trimmed();
    QString email = ui->email_lineEdit->text().trimmed();

    SignupManager manager;
    QString message;

    bool success = manager.registerUser(
        id,
        pw,
        pw_check,
        name,
        phone,
        email,
        message
        );

    if(success)
    {
        QMessageBox::information(this, "성공", message);
        accept();   // ⭐ Dialog 닫기
    }
    else
    {
        QMessageBox::warning(this, "실패", message);
    }
}
void SignUp::on_cancel_btn_clicked()
{
    reject();
}

