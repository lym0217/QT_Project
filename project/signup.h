#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QString>
#include <QJsonArray>
#include <QMessageBox>

namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private slots:
    void on_id_check_btn_clicked();
    void on_ok_btn_clicked();

    void on_cancel_btn_clicked();

private:
    Ui::SignUp *ui;
};

#endif // SIGNUP_H
