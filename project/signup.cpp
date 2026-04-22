#include "signup.h"
#include "ui_signup.h"
#include "signupmanager.h"


SignUp::SignUp(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignUp)
{
    ui->setupUi(this);

    setWindowTitle("회원가입");
    resize(440, 460);

    ui->name_lineEdit->setPlaceholderText("이름");
    ui->id_lineEdit->setPlaceholderText("아이디");
    ui->pw_lineEdit->setPlaceholderText("비밀번호");
    ui->pw_check_lineEdit->setPlaceholderText("비밀번호 확인");
    ui->phone_lineEdit->setPlaceholderText("010-0000-0000");
    ui->email_lineEdit->setPlaceholderText("example@email.com");

    ui->pw_lineEdit->setEchoMode(QLineEdit::Password);
    ui->pw_check_lineEdit->setEchoMode(QLineEdit::Password);

    setStyleSheet(R"(
        QDialog#SignUp {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #f8fbff, stop:0.5 #eef7ff, stop:1 #f7f4ff);
        }
        QLabel {
            color: #35546b;
            font-size: 13px;
            font-weight: 600;
        }
        QLabel#label {
            color: #17324d;
            font-size: 28px;
            font-weight: 700;
            padding: 10px 0 14px 0;
        }
        QLineEdit {
            background: #ffffff;
            color: #163447;
            border: 1px solid #c9dced;
            border-radius: 10px;
            padding: 9px 12px;
            min-height: 18px;
            selection-background-color: #75c7bc;
        }
        QLineEdit:focus {
            border: 2px solid #5495d8;
        }
        QPushButton {
            border-radius: 12px;
            padding: 10px 14px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton#id_check_btn {
            color: #1b4965;
            background: #e6f2fb;
            border: 1px solid #bfd6ea;
        }
        QPushButton#id_check_btn:hover {
            background: #d9ecfa;
        }
        QPushButton#ok_btn {
            color: white;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #3a7bd5, stop:1 #00c6a7);
            border: none;
        }
        QPushButton#ok_btn:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #316fc2, stop:1 #00b196);
        }
        QPushButton#cancel_btn {
            color: #4f6473;
            background: #eef3f7;
            border: 1px solid #d6e0e8;
        }
        QPushButton#cancel_btn:hover {
            background: #e4edf4;
        }
    )");
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
        accept();   // Dialog 닫기
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

