#include "mainpage.h"
#include "ui_mainpage.h"

MainPage::MainPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainPage)
{
    ui->setupUi(this);

    // mainpage에서 테이블 수정 불가하게
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // mainpage 셀이 전체 다 클릭되게
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // JSON 로드
    manager.loadFromJson("data.json", 1);  // userId = 1

    // 총자산 라벨
    int total = manager.getTotalBalance();
    ui->label->setText("총 자산 : " +
                       QString("%L1").arg(total) + "원");

    // 테이블 세팅
    QList<Account>& accounts = manager.getAccounts();
    ui->tableWidget->setRowCount(accounts.size());
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels({"은행명", "계좌번호", "잔액"});

    for (int i = 0; i < accounts.size(); i++) {
        ui->tableWidget->setItem(i, 0,
                                 new QTableWidgetItem(accounts[i].getBank()));
        ui->tableWidget->setItem(i, 1,
                                 new QTableWidgetItem(accounts[i].getAccountNumber()));
        ui->tableWidget->setItem(i, 2,
                                 new NumericTableWidgetItem(
                                     QString("%L1").arg(accounts[i].getBalance()) + "원"));
    }

}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::on_tableWidget_cellClicked(int row, int column)
{
    manager.setCurrentIndex(row);
}


void MainPage::on_tableWidget_cellDoubleClicked(int row, int column)
{
    manager.setCurrentIndex(row);
    // stackedWidget에서 mainpage2로 전환
    ui->stackedWidget->setCurrentIndex(1);
}


void MainPage::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

