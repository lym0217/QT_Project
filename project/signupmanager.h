#ifndef SIGNUP_MANAGER_H
#define SIGNUP_MANAGER_H

#include <QString>
#include <QJsonArray>

class SignupManager
{
public:
    SignupManager(QString filePath = "users.json");

    bool registerUser(QString username,
                      QString password,
                      QString confirmPassword,
                      QString name,
                      QString phone,
                      QString email,
                      QString& message);

    bool isUsernameAvailable(QString username);

private:
    QString m_filePath;

    bool loadJson(QJsonArray& users);
    bool saveJson(const QJsonArray& users);

    bool isValidPhone(QString phone);
    bool isValidEmail(QString email);
};

#endif