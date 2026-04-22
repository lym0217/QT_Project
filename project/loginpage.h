#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>

namespace Ui {
class LoginPage;
}

/**
 * @brief LoginPage 클래스는 로그인 입력과 회원가입 진입을 담당한다.
 *
 * 멤버 변수
 * - m_ui: 로그인 화면 UI 포인터
 *
 * 주요 함수
 * - clearInputs(): 로그아웃 후 아이디/비밀번호 입력창 초기화
 * - onLoginButtonClicked(): 로그인 검증 수행
 * - onPasswordVisibilityChanged(): 비밀번호 보이기/가리기 전환
 * - onSignupButtonClicked(): 회원가입 다이얼로그 실행
 */
class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
    void clearInputs();

signals:
    void loginSuccess(const QString &userName, const QString &username);

private slots:
    void onLoginButtonClicked();
    void onPasswordVisibilityChanged(const Qt::CheckState &state);
    void onSignupButtonClicked();

private:
    Ui::LoginPage *m_ui;
};

#endif // LOGINPAGE_H
