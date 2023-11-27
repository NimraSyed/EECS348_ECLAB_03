#include <iomanip>
#include <iostream>
#include <string>

class Account {
protected:
  std::string accountNumber;
  std::string accountHolder;
  double balance;

public:
  // Constructor for Account class
  Account(const std::string &number, const std::string &holder,
          double initialBalance)
      : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

  // Virtual function to display account details
  virtual void displayDetails() const {
    std::cout << "Account Details for " << getKind()
              << " (ID: " << accountNumber << "):" << std::endl;
    std::cout << "   Holder: " << accountHolder << std::endl;
    std::cout << "   Balance: $" << std::fixed << std::setprecision(2)
              << balance << std::endl;
  }

  // Virtual function to deposit money into the account
  virtual void deposit(double amount) { balance += amount; }

  // Virtual function to withdraw money from the account (ensuring sufficient
  // balance)
  virtual void withdraw(double amount) {
    if (amount > balance) {
      std::cout << "Amount greater than the balance!" << std::endl;
    } else {
      balance -= amount;
    }
  }

  // Virtual function to get the type of account
  virtual std::string getKind() const { return "Account"; }

  // Function to get the current balance
  double getBalance() const { return balance; }

  // Friend function to overload the << operator for account details
  friend std::ostream &operator<<(std::ostream &os, const Account &acc);
};

// Overloading the << operator to display account details
std::ostream &operator<<(std::ostream &os, const Account &acc) {
  acc.displayDetails();
  return os;
}

// Derived class from Account: SavingsAccount
class SavingsAccount : public Account {
private:
  double interestRate;

public:
  // Constructor for SavingsAccount class
  SavingsAccount(const std::string &number, const std::string &holder,
                 double initialBalance, double rate)
      : Account(number, holder, initialBalance), interestRate(rate) {}

  // Overriding the displayDetails function to include interest rate
  void displayDetails() const override {
    Account::displayDetails();
    std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2)
              << (interestRate * 100) << "%" << std::endl;
  }

  // Overriding the withdraw function to enforce a minimum balance
  void withdraw(double amount) override {
    const double minimumBalance =
        100; // example minimum balance for savings account
    if (balance - amount < minimumBalance) {
      std::cout << "Not possible. Minimum balance should be maintained."
                << std::endl;
    } else {
      balance -= amount;
    }
  }

  // Overriding the getKind function to specify the account type
  std::string getKind() const override { return "Savings Account"; }
};

// Derived class from Account: CurrentAccount
class CurrentAccount : public Account {
private:
  double overdraftLimit;

public:
  // Constructor for CurrentAccount class
  CurrentAccount(const std::string &number, const std::string &holder,
                 double initialBalance, double limit)
      : Account(number, holder, initialBalance), overdraftLimit(limit) {}

  // Overriding the displayDetails function to include overdraft limit
  void displayDetails() const override {
    Account::displayDetails();
    std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2)
              << overdraftLimit << std::endl;
  }

  // Overriding the withdraw function to allow overdrafts up to the specified
  // limit
  void withdraw(double amount) override {
    if (amount > (balance + overdraftLimit)) {
      std::cout << "Not possible. Amount exceeds overdraft limit." << std::endl;
    } else {
      balance -= amount;
    }
  }

  // Overriding the getKind function to specify the account type
  std::string getKind() const override { return "Current Account"; }
};

// Overloading the + operator to transfer money between accounts
CurrentAccount operator+(CurrentAccount &current, SavingsAccount &savings) {
  double transferAmount = 300; // example transfer amount
  if (savings.getBalance() >= transferAmount) {
    current.deposit(transferAmount);
    savings.withdraw(transferAmount);
  } else {
    std::cout
        << "Transfer not possible. Insufficient amount in savings account."
        << std::endl;
  }

  return current;
}

int main() {
  // Creating instances of SavingsAccount and CurrentAccount
  SavingsAccount savings("S123", "John Doe", 1000, 0.02);
  CurrentAccount current("C456", "Jane Doe", 2000, 500);

  // Displaying initial account details
  savings.displayDetails();
  current.displayDetails();

  // Performing deposit and withdrawal operations
  savings.deposit(500);
  current.withdraw(1000);

  // Displaying account details after transactions
  savings.displayDetails();
  current.displayDetails();

  // Transfer 300 from savings to current using operator overloading
  current = current + savings;

  // Displaying account details after transfer
  savings.displayDetails();
  current.displayDetails();

  return 0;
}
