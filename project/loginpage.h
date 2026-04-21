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
    void clearInputs();

signals:
    void loginSuccess(const QString &userName, const QString &username);

private slots:
    void on_login_btn_clicked();

    void on_pw_checkBox_checkStateChanged(const Qt::CheckState &arg1);

    void on_pushButton_clicked();

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H
