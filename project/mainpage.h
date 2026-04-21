#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

    void setUserName(const QString &userName);
    void showHomePage();
    void showDetailPage();

signals:
    void logoutRequested();

private slots:
    void on_logout_btn_clicked();
    void on_back_btn_clicked();

private:
    Ui::MainPage *ui;
};

#endif // MAINPAGE_H
