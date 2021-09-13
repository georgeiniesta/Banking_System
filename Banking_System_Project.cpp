/*Menu option
1. Open account
    Name First, Last, Init Balance
2. Balance
3. Deposit
4. Withdrawal
5. Close Account
6. Show All Accounts
7. Quit.
*/
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

class insufficient_Funds{};
class Account
{
private:
    long accNumber;
    string firstName;
    string lastName;
    float accBalance;
    static long nextAccNo;
public:
    Account(){}
    Account(string f_name, string l_name, float accBalance);
    string getFirstName(){return firstName;}
    string getLastName(){return lastName;}
    long getAccNumber(){return accNumber;}
    float getBalance(){return accBalance;}

    void Deposit(float amount);
    void Withdraw(float amount);
    static void setLastAccNo(long accNumber);
    static long getLastAccNo();

    friend ofstream &operator<<(ofstream &ofst, Account &ac);
    friend ifstream &operator>>(ifstream &ifst, Account &ac);
    friend ostream &operator<<(ostream &ost, Account &ac);

};

long Account::nextAccNo=0;
Account::Account(string f_name, string l_name, float accBalance)
{
    nextAccNo++;
    accNumber=nextAccNo;
    firstName=f_name;
    lastName=l_name;
    this->accBalance=accBalance;
}
void Account::Deposit(float amount)
{
    accBalance+=amount;
}
void Account::Withdraw(float amount)
{
    if(accBalance-amount>=0)
    {
        accBalance-=amount;
    }
    else
    {
        throw insufficient_Funds();
    }
}

void Account::setLastAccNo(long accNumber)
{
    nextAccNo=accNumber;
}
long Account::getLastAccNo()
{
    return nextAccNo;
}

ofstream &operator<<(ofstream &ofst,Account &ac)
{
    ofst<<ac.accNumber<<endl;
    ofst<<ac.firstName<<endl;
    ofst<<ac.lastName<<endl;
    ofst<<ac.accBalance<<endl;
    return ofst;
}

ostream &operator<<(ostream &ost,Account &ac)
{
    ost<<ac.accNumber<<endl;
    ost<<ac.firstName<<endl;
    ost<<ac.lastName<<endl;
    ost<<ac.accBalance<<endl;
    return ost;
}

ifstream &operator>>(ifstream &ifst,Account &ac)
{
    ifst>>ac.accNumber;
    ifst>>ac.firstName;
    ifst>>ac.lastName;
    ifst>>ac.accBalance;
    return ifst;
}

class Bank
{
private:
    map<long, Account> Accounts;
public:
    Bank();
    Account openAcc(string f_name, string l_name, float accBalance);
    Account balanceCheck(long accNumber);
    Account deposit(long accNumber, float amount);
    Account withdraw(long accNumber, float amount);
    void closeAcc(long accNumber, float amount);
    void showAllAcc();
    ~Bank();
};

Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("bankData.txt");
    if(!infile)
    {
        return;
    }
    while (!infile.eof())
    {
        infile>>account;
        Accounts.insert(pair<long,Account>(account.getAccNumber(),account));
    }
    Account::setLastAccNo(account.getAccNumber());
    infile.close();
}

Account Bank::openAcc(string f_name,string l_name, float accBalance)
{
    ofstream outfile;
    Account account(f_name,l_name, accBalance);
    Accounts.insert(pair<long,Account>(account.getAccNumber(),account));

    outfile.open("bankData.txt", ios::trunc);

    map<long, Account>::iterator itr;
    for(itr=Accounts.begin();itr!=Accounts.end();itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
    return account;
}

Account Bank::balanceCheck(long accNumber)
{
    map<long,Account>::iterator itr=Accounts.find(accNumber);
    return itr->second;
}

Account Bank::deposit(long accNumber,float amount)
{
    map<long,Account>::iterator itr=Accounts.find(accNumber);
    itr->second.Deposit(amount);
    return itr->second;
}

Account Bank::withdraw(long accNumber,float amount)
{
    map<long, Account>::iterator itr=Accounts.find(accNumber);
    itr->second.Withdraw(amount);
    return itr->second;
}

void Bank::closeAcc( long accNumber,float amount)
{
    if(amount==0)
    {
        map<long,Account>::iterator itr=Accounts.find(accNumber);
        cout<<"Account Deleted"<<itr->second;
        Accounts.erase(accNumber);
    }
    else
    {
        cout<<"Please set Account balance to zero in order to proceed"<<endl;
    }
}

void Bank::showAllAcc()
{
    map<long,Account>::iterator itr;
    for(itr=Accounts.begin();itr!=Accounts.end();itr++)
    {
        cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
    }
}

Bank::~Bank()
{
    ofstream outfile;
    outfile.open("bankData.txt",ios::trunc);

    map<long,Account>::iterator itr;
    for(itr=Accounts.begin();itr!=Accounts.end();itr++)
    {
        outfile<<itr->second;
    }
    outfile.close();
}

int main()
{
    Bank bank;
    Account acc;

    int choice;
    bool sure;
    string f_name,l_name;
    long accountNumber;
    float balance, amount;

    cout<<"***Banking System***"<<endl;
    do
    {
        cout<<"\n\t Select one of the following options\n";
        cout<<"\n\t1 Open Bank Account";
        cout<<"\n\t2 Check Account Balance";
        cout<<"\n\t3 Deposit to Account";
        cout<<"\n\t4 Withdrawal";
        cout<<"\n\t5 Close an Account";
        cout<<"\n\t6 Show All Bank Accounts";
        cout<<"\n\t7 Quit";
        cout<<"\n Please enter your choice: ";
        cin>>choice;
        switch (choice)
        {
        case 1:
            cout<<"Enter client First Name: "<<endl;
            cin>>f_name;
            cout<<"Enter client Last Name: "<<endl;
            cin>>l_name;
            cout<<"Enter initial balance: "<<endl;
            cin>>balance;
            acc=bank.openAcc(f_name,l_name,balance);
            cout<<"\n Congratulations! Account has been created"<<endl;
            cout<<acc;
            break;
        case 2:
            cout<<"Enter Account Number: ";
            cin>>accountNumber;
            acc=bank.balanceCheck(accountNumber);
            cout<<endl<<"Your Account Detail"<<endl;
            cout<<acc;
            break;
        case 3:
            cout<<"Enter Account Number: "<<endl;
            cin>>accountNumber;
            cout<<"Enter Amount to Deposit: "<<endl;
            cin>>amount;
            acc=bank.deposit(accountNumber,amount);
            cout<<"Amount has been deposited "<<endl;
            cout<<acc;
            break;
        case 4:
            cout<<"Enter Account Number: "<<endl;
            cin>>accountNumber;
            cout<<"Enter Amount to Withdraw"<<endl;
            cin>>amount;
            acc=bank.withdraw(accountNumber, amount);
            cout<<"Amount has been withdrawn from Bank Account"<<endl;
            cout<<acc;
            break;
        case 5:
            cout<<"Enter Bank Account to close: "<<endl;
            cin>>accountNumber;
            cout<<"Are you sure to close the Account?"<<endl;
            cin>>sure;
            if(sure==1)
            {
                bank.closeAcc(accountNumber,amount);
                cout<<endl<<"Account has been closed"<<endl;
                cout<<acc;
            }
            else
            {
                return 0;
            }
            break;
        case 6:
            bank.showAllAcc();
            break;
        case 7:
            break;
        default:
            cout<<"\n Enter another choice"<<endl;
            exit(0);
        }
        }while (choice!=7);
        return 0;
}
