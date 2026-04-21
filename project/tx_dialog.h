#ifndef TX_DIALOG_H
#define TX_DIALOG_H
#include <QDialog>

namespace Ui {
class tx_dialog;
}

class tx_dialog : public QDialog
{
    Q_OBJECT
public:
    // 입금으로 변경
    explicit tx_dialog(QWidget *parent = nullptr, QString type = "입금");
    ~tx_dialog();

    // 현재 계좌정보 출력
    void setCurrentAccount(QString bank, QString balance);
    int  getAmount();
    int  getPassword();
    void setBalance(int balance);

private slots:
    void onOkClicked();

private:
    Ui::tx_dialog *ui;
    QString type;
    int balance = 0;
};

#endif // TX_DIALOG_H