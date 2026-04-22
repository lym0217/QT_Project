#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "loginpage.h"
#include "mainpage.h"

/**
 * @brief MainWindow 클래스는 로그인 화면과 메인 화면 전환을 담당하는 최상위 창이다.
 *
 * 멤버 변수
 * - m_stack: 페이지 전환용 스택 위젯
 * - m_loginPage: 로그인 화면
 * - m_mainPage: 로그인 후 메인 화면
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    QStackedWidget *m_stack;
    LoginPage *m_loginPage;
    MainPage *m_mainPage;
};

#endif // MAINWINDOW_H
