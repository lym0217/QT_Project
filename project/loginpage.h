#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

signals:
    void loginSuccess();

private slots:
    void on_login_btn_clicked();

    void on_pw_checkBox_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
