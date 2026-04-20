#include "account_manager.h"

AccountManager::AccountManager()
{
    currentIndex = 0;
    loadDummyData();
}

void AccountManager::loadDummyData()
{
    accounts << Account("KD은행",     1000000)
             << Account("헌마을금고",  500000)
             << Account("신헙은행",   30000000)
             << Account("좌체국예금",  2000000);
}

QList<Account>& AccountManager::getAccounts()
{
    return accounts;
}

Account& AccountManager::getCurrentAccount()
{
    return accounts[currentIndex];
}

void AccountManager::setCurrentIndex(int index)
{
    currentIndex = index;
}

int AccountManager::getCurrentIndex() const
{
    return currentIndex;
}

int AccountManager::getTotalBalance() const
{
    int total = 0;
    for(const auto& a : accounts) total += a.getBalance();  // ← getter 사용
    return total;
}

bool AccountManager::deposit(int amount)
{
    if(amount > 100000000) return false;
    accounts[currentIndex].deposit(amount);                             // ← Account 메서드 사용
    accounts[currentIndex].addTransaction("입금", amount, "본인");
    return true;
}

bool AccountManager::withdraw(int amount)
{
    if(amount > 100000000) return false;
    if(amount > accounts[currentIndex].getBalance()) return false;      // ← getter 사용
    accounts[currentIndex].withdraw(amount);                            // ← Account 메서드 사용
    accounts[currentIndex].addTransaction("출금", amount, "본인");
    return true;
}

bool AccountManager::transfer(int amount, QString targetBank, bool isMyAccount, QString fromBank)
{
    if(amount <= 0 || amount > 100000000) return false;
    if(amount > accounts[currentIndex].getBalance()) return false;      // ← getter 사용

    accounts[currentIndex].withdraw(amount);                            // ← Account 메서드 사용

    if(isMyAccount) {
        for(auto& a : accounts) {
            if(a.getBank() == targetBank) {                             // ← getter 사용
                a.deposit(amount);                                      // ← Account 메서드 사용
                a.addTransaction("입금", amount, "[My] " + fromBank);
                break;
            }
        }
        accounts[currentIndex].addTransaction("송금", amount, "[My] " + targetBank);
    }
    else {
        accounts[currentIndex].addTransaction("송금", amount, targetBank);
    }
    return true;
}