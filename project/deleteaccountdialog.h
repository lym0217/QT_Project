#ifndef DELETEACCOUNTDIALOG_H
#define DELETEACCOUNTDIALOG_H

#include <QDialog>

class QLineEdit;

/**
 * @brief DeleteAccountDialog 클래스는 계정 삭제 확인용 연락처 정보를 입력받는다.
 *
 * 멤버 변수
 * - m_phoneEdit: 전화번호 입력창
 * - m_emailEdit: 이메일 입력창
 *
 * 주요 함수
 * - phone(), email(): 입력된 확인 정보 반환
 * - validateAndAccept(): 입력 여부를 검사한 뒤 다이얼로그를 닫는다.
 */
class DeleteAccountDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteAccountDialog(QWidget *parent = nullptr);

    QString phone() const;
    QString email() const;

private slots:
    void validateAndAccept();

private:
    QLineEdit *m_phoneEdit;
    QLineEdit *m_emailEdit;
};

#endif // DELETEACCOUNTDIALOG_H
