#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <locale>
#include <cctype>

#include "transaction.hpp"
#include "account.hpp"
#include "viewHelper.hpp"

#define FILE "bank.txt"

using namespace std;

int read(vector<Account>&, istream&);
void write(vector<Account>& accounts, ostream& os);
void makeDepositORWithdrawal(Transaction& transaction, vector<Account>& accounts, string message);

int main() {
  
   //Set default locale to system locale
   locale loc("");

   //Set cout and cin to use the system locale
   cout.imbue(loc);
   cin.imbue(loc);

   // flag to determine when to terminate the program
   bool quit = false;
   vector<Account> accounts;
   ifstream inFile(FILE);

   if (inFile) {
      int numOfTransactions = read(accounts, inFile);

      cout << "Read in " << numOfTransactions << " Transactions" << endl;
   }

   inFile.close();

   printWelcomeBanner();
 
   while(!quit) {
      string command = prompt();

      trim(command);
      transform(command.begin(), command.end(), command.begin(), ::tolower);

      if (command.empty())
         continue;
 
      else if ( command == "quit" )
         quit = true;

      else if ( command == "ledger" )  {

         if(accounts.size() == 0)
            cout << "No Account found, use the new command to create an Account\n\n\n";

         else {
            for(const Account& account : accounts)
               printAccountLedger(account);
         }
      }

      else if ( command == "deposit" ) { 
         Transaction transaction = readTransaction();
         makeDepositORWithdrawal(transaction, accounts, "deposit");
      }

      else if ( command == "withdraw" ) {
         cout << endl << endl << endl;
         long double balance;

         Transaction transaction = readTransaction(true);
         
         for(Account account : accounts) {
            if(transaction.getAccountName() == account.getName()) {
               balance = account.balance();

            }
         }

         if(balance >= (transaction.getAmount() * -1) ) 
            makeDepositORWithdrawal(transaction, accounts, "withdrawal");
         
         else
            cout << "\n\n\nInsufficient Funds, your balance is " << balance << endl << endl;

      }

      else if ( command == "new" ) {
         Account account = readAccount();
         accounts.push_back(account);

         cout << "Account created" << endl << endl << endl;
      }

      else if ( command == "delete" ) {
         deleteAccount(accounts);
         cout << "Account deleted" << endl << endl << endl;
      }

      else if ( command == "help" ) 
         printHelp();
     
      else 
        cerr << "Please enter a command from the given list";
   }

  
   ofstream  outFile(FILE);
   write(accounts, outFile);
   outFile.close();

   return 0;
}

/**
   Loads all the accounts and their corresponding transactions back into memory
   from the output file. Then return the number of transactions found 
**/
int read(vector<Account>& accounts, istream& is) {
   string header;
   int transactionCount = 0;

   while(is) {
      is >> header;

      trim(header);
      transform(header.begin(), header.end(), header.begin(), ::toupper);

      if (header.empty())
         continue;

      else if ( header == BEGIN_ACCOUNT ) {
         Account account;
         is >> account;
         accounts.push_back(account);
      }

      else if ( header == BEGIN_TRANSACTION ) {
         Transaction transaction;
         is >> transaction;
         ++transactionCount;

         for_each( begin(accounts), end(accounts), [&transaction](Account& account) {
            string accountName = transaction.getAccountName(); 

            if ( equal(begin(accountName), end(accountName), begin(account.getName())) )
               account.addTransaction(transaction);
         });
      }

      else
         cerr << "Unknown header field: \"" << header << "\"" << endl;

      header.clear();
   }

   return transactionCount;
}

/**
   Output all accounts and their corresponding transactions to a file
   so that it can be loaded at a later time. 
**/
void write(vector<Account>& accounts, ostream& os) {
   for (Account& account : accounts) {
      os << account;

      for (const Transaction& transaction : account.getTransactions())
         os << transaction;
  }
}


// Makes a deposit or a withdrawal
void makeDepositORWithdrawal(Transaction& transaction, vector<Account>& accounts, string message) {
   bool accountExist = false;

   for(Account account : accounts) {
      if(transaction.getAccountName() == account.getName())
         accountExist = true;
   }

      if(accountExist) {
         addToAccount(transaction, accounts);
         cout << message << " completed" << endl << endl << endl;
      }

      else {
         cout << "There is no record of \"" << transaction.getAccountName() << "\"" 
              << "\nUse the new command to create an Account" << endl << endl << endl; 
      }
}