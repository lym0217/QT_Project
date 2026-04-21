#include "mainpage.h"
#include "ui_mainpage.h"

#include <QHeaderView>

MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainPage)
{
    ui->setupUi(this);

    setWindowTitle("계좌 관리 대시보드");

    ui->label_2->setText("TO55 BANK");
    ui->label->setText("총 자산 12,480,000원");
    ui->label_3->setText("TO55 BANK");
    ui->label_4->setText("대표 계좌 : 4,820,000원");
    ui->home_user_name_label->setText("OOO님");

    showHomePage();

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget_2->setAlternatingRowColors(true);
    ui->tableWidget->setShowGrid(false);
    ui->tableWidget_2->setShowGrid(false);

    setStyleSheet(R"(
        QWidget#MainPage {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                                        stop:0 #f5f9ff, stop:0.55 #eefbf7, stop:1 #fff8ed);
        }
        QLabel {
            color: #284b63;
        }
        QLabel#label_2, QLabel#label_3 {
            color: #2a6f97;
            font-size: 14px;
            font-weight: 700;
            letter-spacing: 1px;
            padding: 10px 14px;
            background: rgba(255, 255, 255, 0.72);
            border: 1px solid #d4e4f1;
            border-radius: 14px;
            max-width: 140px;
        }
        QLabel#label {
            color: #17324d;
            font-size: 28px;
            font-weight: 700;
            padding: 16px 8px 14px 8px;
        }
        QLabel#label_4 {
            color: #42657d;
            font-size: 16px;
            font-weight: 600;
            padding: 10px 6px;
        }
        QLabel#home_user_name_label {
            color: #35546b;
            font-size: 14px;
            font-weight: 700;
            padding: 10px 14px;
            background: rgba(255, 255, 255, 0.72);
            border: 1px solid #d4e4f1;
            border-radius: 14px;
            min-width: 88px;
        }
        QTableWidget {
            background: rgba(255, 255, 255, 0.88);
            alternate-background-color: rgba(235, 246, 255, 0.72);
            border: 1px solid #d6e5f0;
            border-radius: 16px;
            color: #213c4f;
            gridline-color: transparent;
            selection-background-color: #bfe9df;
            selection-color: #163447;
        }
        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                        stop:0 #5b86e5, stop:1 #36d1dc);
            color: white;
            border: none;
            padding: 10px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton {
            border: none;
            border-radius: 14px;
            padding: 11px 18px;
            font-size: 13px;
            font-weight: 700;
            color: white;
        }
        QPushButton#pushButton_4, QPushButton#back_btn {
            color: #29465b;
            background: rgba(255, 255, 255, 0.78);
            border: 1px solid #d0dfeb;
        }
        QPushButton#pushButton_4:hover, QPushButton#back_btn:hover {
            background: rgba(255, 255, 255, 0.95);
        }
        QPushButton#logout_btn {
            color: #fffaf7;
            background: #f26b5e;
        }
        QPushButton#logout_btn:hover {
            background: #df5b4f;
        }
        QPushButton#pushButton_2 {
            background: #1fbf8f;
        }
        QPushButton#pushButton_2:hover {
            background: #15ab7f;
        }
        QPushButton#pushButton_3 {
            background: #ff8a5b;
        }
        QPushButton#pushButton_3:hover {
            background: #f07a49;
        }
        QPushButton#pushButton {
            background: #4f7cff;
        }
        QPushButton#pushButton:hover {
            background: #416de6;
        }
    )");
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::setUserName(const QString &userName)
{
    const QString displayName = userName.isEmpty() ? "OOO님" : userName + "님";
    ui->home_user_name_label->setText(displayName);
}

void MainPage::showHomePage()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainPage::showDetailPage()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainPage::on_logout_btn_clicked()
{
    showHomePage();
    emit logoutRequested();
}

void MainPage::on_back_btn_clicked()
{
    showHomePage();
}
