/* This program does all the reading and writing to the objects
*/

#ifndef _VIEW_HELPER_HPP_
#define _VIEW_HELPER_HPP_

#include "account.hpp"
#include "transaction.hpp"
#include "centerHelper.hpp"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <sstream>
#include <string>

using namespace std;

// Welcome message to the user
void printWelcomeBanner() {

    cout << setfill('-');
    // display raw text where x is the delimiter
    cout << setw(80) << centered(R"x(-----------------------------------)x") << "\n";
    cout << setw(80) << centered(R"x(                                   )x") << "\n";
    cout << setw(80) << centered(R"x( ____                              )x") << "\n";
    cout << setw(80) << centered(R"x(|     \    /\      |\     | |   /  )x") << "\n";
    cout << setw(80) << centered(R"x(|     /   /  \     | \    | |  /   )x") << "\n";
    cout << setw(80) << centered(R"x(|____/   /    \    |  \   | | /    )x") << "\n";
    cout << setw(80) << centered(R"x(|    \  /------\   |   \  | |/\    )x") << "\n";
    cout << setw(80) << centered(R"x(|    / /        \  |    \ | |  \   )x") << "\n";
    cout << setw(80) << centered(R"x(|___/ /          \ |     \| |   \  )x") << "\n";
    cout << setw(80) << centered(R"x(                                   )x") << "\n";
    cout << setw(80) << centered(R"x(-----------------------------------)x") << "\n";
    cout << setfill(' ');
    cout << "\n\n\n\n\n\n\n\n\n\n";
}

// Converts a string to a long double
long double strToDouble(string& number) {

   stringstream ss(number);
   long double d1;

   ss >> d1;
 
   return d1;
}

// Checks whether a string is a number(double)
bool isNumeric(string& str) {

   string numbers = "0123456789.";
   int counter = 0;

   int numberOfDots = count( begin(str), end(str), '.'); 

   if(numberOfDots > 1)
      return false;

   if(str[0] == '-' || str[0] == '+' || str[0] == '.') {
      for(int i = 1; i < (int)str.length(); i++) {
         for(int j = 0; j < (int)numbers.length(); j++) { 
            if(str[i] == numbers[j]) {
               counter++;
            }
         }
         if(counter == 0)
            return false;

         counter = 0;
      }
   }

   else {
      for(int i = 0; i < (int)str.length(); i++) {
         for(int j = 0; j < (int)numbers.length(); j++) { 
            if(str[i] == numbers[j]) {
               counter++;
            }
         }
         if(counter == 0)
            return false;

         counter = 0;
      }
   }

   return true;
} 

// Gets the prompt from the user
string prompt() {
   string input;

   cout << "Please enter command(type help for more info): ";
   getline(cin, input);
 
   return trim(input);
}

// Creates a new transaction
// Sets the default transaction to a deposit
Transaction readTransaction(bool withdraw = false) {
   string account;
   long double amount;
   string memo;

   string amountStr;

   cout << setw(15) << std::left << "Account name" << std::right << ": ";
   cin >> account;

   cout << setw(15) << std::left << "Amount" << std::right << ": ";
   cin >> amountStr;

   if(isNumeric(amountStr) )
      amount = strToDouble(amountStr);
   else {
      cout << "Amount field should be a number " << endl;
      exit(1);
   }
      

   amount = (long double) (amount * 100);
   get_money(amount);

   cout << setw(15) << std::left << "Memo" << std::right << ": ";

   if ( withdraw && amount > 0 ) {
    // Deduct the requested amount
      amount = -amount;
   }

   // Reads until it sees a new line character and discard all those characters 
   cin.ignore(numeric_limits<streamsize>::max(), '\n');
   getline(cin, memo);

   trim(memo);
   trim(account);

   Transaction transaction(account, amount, memo);

   return transaction;
} 

// Creates a new account
Account readAccount() {
  string accountName;

  cout << setw(20) << std::left << "New account name" << std::right << ": ";
  cin >> accountName;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  Account account(accountName);

  return account;
}

void deleteAccount(vector<Account>& accounts) {
   string accountName;
   cout << "Accounts avilable: \n";

   // Displays a list of all the accounts by their name
   for (const Account& account : accounts) {
      cout << setw(30) << account.getName() << ": " << account.getTransactions().size() 
           << " transactions\n";
   }

   cout << setw(15) << std::left << "Account name" << std::right << ": ";
   cin >> accountName;
   cin.ignore(numeric_limits<streamsize>::max(), '\n');

   // Removes the account with the selected name
   for ( auto it = begin(accounts); it != end(accounts); ) {
      if ( equal(begin((*it).getName()), end((*it).getName()), begin(accountName)) )
         it = accounts.erase(it);

      else
         ++it;
   }
}

void print(int setw1, string option, int setw2, string message) {
   cout << setw(setw1) << option << setw(setw2) << message << endl;
}

void printHelp() {
   string line(80, '-');

   cout << setw(80) << centered("Commands") << endl;
   cout << line << "\n";

   print(20, "Help",     50, "Print Help Message");
   print(20, "New",      50, "Create a new Account");
   print(20, "Deposit",  50, "Deposit the requested amount");
   print(20, "Withdraw", 50, "Withdraw the requested amount");
   print(20, "Ledger",   50, "Display Account Ledger");
   print(20, "Delete",   50, "Delete an Account and its Transactions");
   print(20, "Quit",     50, "Writes out the activity then exit the program");

   cout << endl << endl << endl;
}

// Adds a transaction to an account
void addToAccount(Transaction& transaction, vector<Account>& accounts) {
  for_each(begin(accounts), end(accounts), [&](Account& account) {
      string accountName = transaction.getAccountName();

      if ( equal(begin(accountName), end(accountName), begin(account.getName())) )
         account.addTransaction(transaction);
    });
}

// Prints the activity
void printAccountLedger(const Account& account) {
  string line(80, '-');
  std::ostringstream oss;

  oss << "Ledger for " << account.getName();

  cout << endl << endl;
  cout << setw(80) << centered(oss.str()) << endl;
  cout << line << endl;

  for (const Transaction& transaction : account.getTransactions()) {
    time_t time = transaction.getTransactionDate();
    struct tm* date = localtime(&time);

    // mm/dd/yyyy, 12 hour format
    cout << setw(20) << std::left  << std::put_time(date, "%D %r");
    cout << setw(40) << std::right << transaction.getMemo();
    cout << setw(20) << std::right << std::put_money(transaction.getAmount(), true);
    cout << "\n";
  }

  oss.str("");
  oss.clear();
  oss.imbue(locale(""));
  oss << "Total: $" << std::put_money(account.balance());

  string dashes(oss.str().size(), '-');

  cout << setw(80) << dashes << endl;
  cout << setw(80) << oss.str() << endl;
}

#endif
