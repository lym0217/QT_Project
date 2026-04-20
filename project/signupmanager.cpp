#include "signupmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QRegularExpression>

SignupManager::SignupManager(QString filePath)
    : m_filePath(filePath)
{
}

// 중복 체크
bool SignupManager::isUsernameAvailable(QString username)
{
    QJsonArray users;

    if(!loadJson(users))
        return false;

    for(auto u : users)
    {
        QJsonObject obj = u.toObject();
        if(obj["username"].toString() == username)
            return false;
    }
    return true;
}

// 회원가입
bool SignupManager::registerUser(QString username,
                                 QString password,
                                 QString confirmPassword,
                                 QString name,
                                 QString phone,
                                 QString email,
                                 QString& message)
{
    QJsonArray users;

    if(!loadJson(users))
    {
        message = "파일 오류";
        return false;
    }

    // 입력 검사
    if(username.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()
        || name.isEmpty() || phone.isEmpty() || email.isEmpty())
    {
        message = "모든 항목을 입력해주세요.";
        return false;
    }

    if(password != confirmPassword)
    {
        message = "비밀번호가 일치하지 않습니다.";
        return false;
    }

    if(!isValidPhone(phone))
    {
        message = "전화번호 형식 오류";
        return false;
    }

    if(!isValidEmail(email))
    {
        message = "이메일 형식 오류";
        return false;
    }

    if(!isUsernameAvailable(username))
    {
        message = "이미 사용중인 아이디입니다.";
        return false;
    }

    // 비밀번호 해시
    QString hashedPw = QCryptographicHash::hash(
                           password.toUtf8(),
                           QCryptographicHash::Sha256
                           ).toHex();

    // 사용자 추가
    QJsonObject newUser;
    newUser["username"] = username;
    newUser["password"] = hashedPw;
    newUser["name"] = name;
    newUser["phone"] = phone;
    newUser["email"] = email;
    newUser["accounts"] = QJsonArray();

    users.append(newUser);

    if(!saveJson(users))
    {
        message = "저장 실패";
        return false;
    }

    message = "회원가입 성공";
    return true;
}

// JSON 읽기
bool SignupManager::loadJson(QJsonArray& users)
{
    QFile file(m_filePath);

    if(!file.exists())
    {
        QJsonObject root;
        root["users"] = QJsonArray();

        QFile newFile(m_filePath);
        newFile.open(QIODevice::WriteOnly);
        newFile.write(QJsonDocument(root).toJson());
        newFile.close();
    }

    if(!file.open(QIODevice::ReadOnly))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    users = doc.object()["users"].toArray();

    file.close();
    return true;
}

// JSON 저장
bool SignupManager::saveJson(const QJsonArray& users)
{
    QJsonObject root;
    root["users"] = users;

    QFile file(m_filePath);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    file.write(QJsonDocument(root).toJson());
    file.close();

    return true;
}

// 전화번호 검사
bool SignupManager::isValidPhone(QString phone)
{
    QRegularExpression regex("^010-\\d{4}-\\d{4}$");
    return regex.match(phone).hasMatch();
}

// 이메일 검사
bool SignupManager::isValidEmail(QString email)
{
    QRegularExpression regex("^[\\w\\.]+@[\\w\\.]+$");
    return regex.match(email).hasMatch();
}