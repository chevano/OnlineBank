#ifndef _TRANSACTION_HPP_
#define _TRANSACTION_HPP_

#include <iostream>
#include <ctime>
#include <string>

using namespace std;

#define BEGIN_TRANSACTION "BEGIN_TRANSACTION"

   // Helper function to remove leading and trailing spaces
   string& trim(string& inputString) {
      inputString.erase(0, inputString.find_first_not_of(" \t\n"));
      inputString.erase(inputString.find_last_not_of(" \t\n") + 1);

      return inputString;
   }

/*
  This class defines a single financial transaction
  time(0) doesn't allow the user to modify the timestamp once its created
*/


class Transaction {
   private:
      string accountName;
      long double amount;
      string memo;
      time_t transactionDate;

   public:
      Transaction() : accountName({}), amount{}, memo({}), transactionDate(time(0)) {}  
											
      Transaction(const string& accountName, long double amount, const string& memo) :
         accountName(accountName), amount(amount), memo(memo), 
         transactionDate(std::time(0)) {}

      const string& getAccountName() const {
         return accountName;
      }

      long double getAmount() const {
         return amount;
      }

      const string& getMemo() const {
         return memo;
      }

      time_t getTransactionDate() const {
         return transactionDate;
      }

      friend ostream& operator<<(ostream& os, const Transaction& transaction);
      friend istream& operator>>(istream& is, Transaction& transaction);
};

ostream& operator<<(ostream& os, const Transaction& transaction) {
   os << BEGIN_TRANSACTION << " "
      << transaction.accountName << " "
      << transaction.transactionDate << " "
      << transaction.amount << " "
      << transaction.memo << "\n";

   return os;
}

istream& operator>>(istream& is, Transaction& transaction) {
   is >> transaction.accountName;
   is >> transaction.transactionDate;
   is >> transaction.amount;
   getline(is, transaction.memo);

   trim(transaction.memo);

   return is;
}

#endif