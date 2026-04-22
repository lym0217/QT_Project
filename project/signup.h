#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include <QString>
#include <QJsonArray>
#include <QMessageBox>

namespace Ui {
class SignUp;
}

/**
 * @brief SignUp 클래스는 회원가입 입력 UI와 버튼 동작을 담당한다.
 *
 * 멤버 변수
 * - m_ui: 회원가입 다이얼로그 UI 포인터
 *
 * 주요 함수
 * - onCheckIdButtonClicked(): 아이디 중복 확인
 * - onConfirmButtonClicked(): 회원가입 요청
 * - onCancelButtonClicked(): 다이얼로그 닫기
 */
class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(QWidget *parent = nullptr);
    ~SignUp();

private slots:
    void onCheckIdButtonClicked();
    void onConfirmButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::SignUp *m_ui;
};

#endif // SIGNUP_H
