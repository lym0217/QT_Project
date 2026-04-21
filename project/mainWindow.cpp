#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(526, 574);

    stack = new QStackedWidget(this);

    loginPage = new LoginPage();
    mainPage = new MainPage();

    stack->addWidget(loginPage);  // index 0
    stack->addWidget(mainPage);   // index 1

    setCentralWidget(stack);

    // 로그인 성공 시 페이지 전환
    connect(loginPage, &LoginPage::loginSuccess, this, [=](const QString &userName) {
        mainPage->setUserName(userName);
        mainPage->showHomePage();
        stack->setCurrentIndex(1);
    });

    connect(mainPage, &MainPage::logoutRequested, this, [=]() {
        stack->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow()
{
}
