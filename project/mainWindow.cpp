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
    connect(loginPage, &LoginPage::loginSuccess, this, [=](const QString &userName, const QString &username) {
        mainPage->setCurrentUser(userName, username);
        mainPage->showHomePage();
        resize(650, 552);
        stack->setCurrentIndex(1);
    });

    connect(mainPage, &MainPage::logoutRequested, this, [=]() {
        loginPage->clearInputs();
        resize(526, 574);
        stack->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow()
{
}
