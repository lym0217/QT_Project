#ifndef SIGNUP_MANAGER_H
#define SIGNUP_MANAGER_H

#include <QString>
#include <QJsonArray>

/**
 * @brief SignupManager 클래스는 회원가입 입력 검증과 사용자 JSON 저장을 담당한다.
 *
 * 멤버 변수
 * - m_filePath: 사용자 정보가 저장된 JSON 파일 경로
 *
 * 주요 함수
 * - registerUser(): 회원가입 검증 후 새 사용자를 저장한다.
 * - isUsernameAvailable(): 아이디 중복 여부를 확인한다.
 * - loadJson(), saveJson(): JSON 파일 입출력
 * - isValidPhone(), isValidEmail(): 입력 형식 검증
 */
class SignupManager
{
public:
    explicit SignupManager(QString filePath = "users.json");

    bool registerUser(const QString& username,
                      const QString& password,
                      const QString& confirmPassword,
                      const QString& name,
                      const QString& phone,
                      const QString& email,
                      QString& message);

    bool isUsernameAvailable(const QString& username);

private:
    QString m_filePath;

    bool loadJson(QJsonArray& users);
    bool saveJson(const QJsonArray& users);

    bool isValidPhone(const QString& phone);
    bool isValidEmail(const QString& email);
};

#endif // SIGNUP_MANAGER_H
