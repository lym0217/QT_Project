#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(526, 574);

    m_stack = new QStackedWidget(this);

    m_loginPage = new LoginPage();
    m_mainPage = new MainPage();

    m_stack->addWidget(m_loginPage);
    m_stack->addWidget(m_mainPage);

    setCentralWidget(m_stack);

    connect(m_loginPage, &LoginPage::loginSuccess, this, [=](const QString &userName, const QString &username) {
        m_mainPage->setCurrentUser(userName, username);
        m_mainPage->showHomePage();
        resize(650, 552);
        m_stack->setCurrentIndex(1);
    });

    connect(m_mainPage, &MainPage::logoutRequested, this, [=]() {
        m_loginPage->clearInputs();
        resize(526, 574);
        m_stack->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow()
{
}
