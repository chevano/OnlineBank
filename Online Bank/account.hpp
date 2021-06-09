#ifndef _ACCOUNT_HPP_
#define _ACCOUNT_HPP_

#include <algorithm>
#include <vector>
#include <iterator>

#include "transaction.hpp"

#define BEGIN_ACCOUNT "BEGIN_ACCOUNT"

/*
   This class defines a money account which stores financial transactions
   and maintains a balance.
*/

class Account {
   private:
      string name;
      vector<Transaction> transactions;

   public:
      Account() : name({}), transactions({}) {}
      Account(const string& name) : name(name), transactions({}) {}

      const string& getName() const {
         return name;
      }

      const vector<Transaction>& getTransactions() const {
         return transactions;
      }

      void addTransaction(Transaction& transaction) {
         transactions.push_back(transaction);
      }

      long double balance() const {
         long double sum = 0;
         sum = accumulate(begin(transactions), end(transactions), sum, 
               [](const long double& x, const Transaction& y) {
                  return x + y.getAmount();
               });

         return sum;
      }

      friend ostream& operator<<(ostream& os, const Account& account);
      friend istream& operator>>(istream& is, Account& account);
};

ostream& operator<<(ostream& os, const Account& account) {
   os << BEGIN_ACCOUNT << " "
      << account.name << "\n";

   return os;
}

istream& operator>>(istream& is, Account& account) {
   is >> account.name;

   return is;
}


#endif