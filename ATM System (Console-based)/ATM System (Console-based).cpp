#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string ClientFileName = "ClientDataFile.txt";

/// @brief Options available in the ATM main menu.
enum enATMMainMenuOptions { enQuickWithdraw = 1, enNormalWithdraw = 2, enDeposit = 3, enCheckBalance = 4, enLogout = 5 };

/// @brief Represents a client’s account information.
struct stClient {
	string AccountNumber, PinCode, FullName, PhoneNumber;
	double AccountBalance;
	bool MarkForDelete = false;
};

///< Holds the information of the currently logged-in client.
stClient CurrentClient;

void Login();
void ShowATMMainMenuScreen();
bool DepositAmount(string AccountNumber, double DepositAmount, vector <stClient> vClients);

//================= Utility Functions =================//

/// @brief Splits a string into substrings based on a delimiter.
vector <string> SplitString(string Text, string delim) {

	vector <string> vString{};
	short pos = 0;
	string sWord;

	while ((pos = Text.find(delim)) != std::string::npos) {
		sWord = Text.substr(0, pos);
		if (sWord != "")
			vString.push_back(sWord);
		Text.erase(0, pos + delim.length());
	}
	if (Text != "")
		vString.push_back(Text);

	return vString;
}

/// @brief Converts a line from file into a stClient record.
stClient ConvertClientsLineDataToRecord(string Line, string Seperator = "#//#") {

	stClient ClientData;
	vector <string> vClient;
	vClient = SplitString(Line, Seperator);

	ClientData.AccountNumber = vClient[0];
	ClientData.PinCode = vClient[1];
	ClientData.FullName = vClient[2];
	ClientData.PhoneNumber = vClient[3];
	ClientData.AccountBalance = stod(vClient[4]);

	return ClientData;
}

/// @brief Loads all clients from file.
vector <stClient> LoadClientsDataFromFile(string FileName) {

	vector <stClient> vFileContent;
	fstream MyFile;

	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string Line;
		stClient Client;

		while (getline(MyFile, Line)) {
			Client = ConvertClientsLineDataToRecord(Line);
			vFileContent.push_back(Client);
		}

		MyFile.close();
	}

	return vFileContent;
}

/// @brief Converts a stClient record into a formatted string line.
string ConvertRecordToLine(stClient& ClientData, string Seprator) {

	string stClientRecord = "";

	stClientRecord += ClientData.AccountNumber + Seprator;
	stClientRecord += ClientData.PinCode + Seprator;
	stClientRecord += ClientData.FullName + Seprator;
	stClientRecord += ClientData.PhoneNumber + Seprator;
	stClientRecord += to_string(ClientData.AccountBalance);

	return stClientRecord;
}

/// @brief Saves all clients to file (excluding those marked for delete).
vector <stClient> SaveClientDataToFile(string FileName, vector <stClient> vClients) {
	fstream MyFile;

	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open()) {

		for (stClient& C : vClients) {
			if (C.MarkForDelete != true) {
				DataLine = ConvertRecordToLine(C, "#//#");
				MyFile << DataLine << endl;
			}
		}

		MyFile.close();
	}

	return vClients;
}

/// @brief Searches for client by account number and PIN code.
bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& Client) {
	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);

	for (stClient& C : vClients) {
		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode) {
			Client = C;
			return true;
		}
	}

	return false;
}

/// @brief Loads client info into CurrentClient.
bool LoadClientInfo(string AccountNumber, string PinCode) {

	if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

//================= ATM Menu Functions =================//

/// @brief Reads user’s choice for main menu.
enATMMainMenuOptions ReadATMMainMenuOption() {
	short ATMMainMenuOption = 0;

	cout << "Choose What do you want to do? [1 to 5]? ";
	cin >> ATMMainMenuOption;

	return (enATMMainMenuOptions)ATMMainMenuOption;

}

/// @brief Reads user’s choice for quick withdraw menu.
short ReadQuickWithdrawOption() {
	short QuickWithdrawOption = 0;

	while (QuickWithdrawOption < 1 || QuickWithdrawOption > 9) {
		cout << "\nChoose What do you want to do? [1 to 9]? ";
		cin >> QuickWithdrawOption;
	}

	return QuickWithdrawOption;

}

/// @brief Reads a withdraw amount (multiple of 5).
int ReadWithdrawAmount() {
	int WithdrawAmount = 0;

	do {
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> WithdrawAmount;
	} while ((WithdrawAmount % 5) != 0);

	return WithdrawAmount;
}

/// @brief Reads a deposit amount (must be positive).
int ReadDepositAmount() {
	int DepositAmount = 0;

	do {
		cout << "\nEnter a positive deposit amount ? ";
		cin >> DepositAmount;
	} while (DepositAmount < 0);

	return DepositAmount;
}

/// @brief Displays balance of CurrentClient.
bool CheckBalance() {
	vector <stClient> vClients;
	vClients = LoadClientsDataFromFile(ClientFileName);

	cout << "========================================\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "========================================\n";

	cout << "\n\nYour Balance is " << CurrentClient.AccountBalance << endl;


	return true;
}

/// @brief Returns back to the ATM main menu after pressing a key.
void GoBackToATMMainMenuScreen() {
	cout << "\n\nPress any key to back to ATM Main Menu..." << endl;
	system("pause>0");
	ShowATMMainMenuScreen();
}

/// @brief Maps quick withdraw option to fixed withdraw amount
short GetQuickWithdrawAmount(short QuickWithdrawAmount) {

	switch (QuickWithdrawAmount) {
	case 1: {
		return 20;
	}
	case 2: {
		return 50;
	}
	case 3: {
		return 100;
	}
	case 4: {
		return 200;
	}
	case 5: {
		return 400;
	}
	case 6: {
		return 600;
	}
	case 7: {
		return 800;
	}
	case 8: {
		return 1000;
	}
	case 9: {
		return 0;
	}
	}
}

/// @brief Performs quick withdraw operation.
void PerformQuickWithdraw(short WithdrawOption) {

	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	char Answer = 'N';

	short WithdrawAmount = GetQuickWithdrawAmount(WithdrawOption);

	if (WithdrawAmount == 0) {
		return;
	}

	if (WithdrawAmount > CurrentClient.AccountBalance) {
		cout << "Amoount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
		return;
	}

	DepositAmount(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
	CurrentClient.AccountBalance -= WithdrawAmount;
}

/// @brief Displays quick withdraw menu screen.
void ShowQuickWithdrawMenuScreen() {
	system("cls");
	cout << "========================================\n";
	cout << "\t\tQuick Withdraw\n";
	cout << "========================================\n";
	cout << "\t[1] 20 \t\t [2] 50\n";
	cout << "\t[3] 100\t\t [4] 200\n";
	cout << "\t[5] 400\t\t [6] 600\n";
	cout << "\t[7] 800\t\t [8] 1000\n";
	cout << "\t[9] Exit\n";
	cout << "========================================\n" << endl;

	cout << "\nYour Balance is " << CurrentClient.AccountBalance << endl;

	PerformQuickWithdraw(ReadQuickWithdrawOption());
}

/// @brief Performs a normal withdraw (custom amount).
void PerformNormalWithdraw() {

	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);

	int WithdrawAmount = ReadWithdrawAmount();

	if (WithdrawAmount > CurrentClient.AccountBalance) {
		cout << "Amoount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
		return;
	}

	DepositAmount(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
	CurrentClient.AccountBalance -= WithdrawAmount;
}

/// @brief Displays normal withdraw menu screen.
void ShowNormalWithdrawMenuScreen() {
	system("cls");
	cout << "========================================\n";
	cout << "\t\Normal Withdraw Screen\n";
	cout << "========================================\n";

	PerformNormalWithdraw();
}

/// @brief Performs a deposit transaction
bool DepositAmount(string AccountNumber, double DepositAmount, vector <stClient> vClients) {

	char Answer = 'N';

	cout << "Are you Sure you want perform this transaction? y/n ? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y') {
		for (stClient& C : vClients) {
			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += DepositAmount;
				cout << "\n\nDone Successfully.  New Balance is " << C.AccountBalance << endl;
				break;
			}
		}
		SaveClientDataToFile(ClientFileName, vClients);

		return true;
	}

	return false;
}

/// @brief Performs deposit of a positive amount for CurrentClient.
void PerformDepositAmount() {
	
	int AmountToDeposit = ReadDepositAmount();

	vector <stClient> vClients = LoadClientsDataFromFile(ClientFileName);
	DepositAmount(CurrentClient.AccountNumber, AmountToDeposit, vClients);
	CurrentClient.AccountBalance += AmountToDeposit;
}

/// @brief Displays deposit screen.
void ShowDepositScreen() {
	cout << "\n---------------------------------------------------------------\n";
	cout << "\t\tDeposit Screen\n";
	cout << "---------------------------------------------------------------\n\n";

	PerformDepositAmount();
}

/// @brief Executes the selected ATM menu option.
void PerformATMMainMenuoption(enATMMainMenuOptions ATMMainMenuOptions) {
	switch (ATMMainMenuOptions) {
	case enQuickWithdraw: {
		system("cls");
		ShowQuickWithdrawMenuScreen();
		GoBackToATMMainMenuScreen();
		break;
	}
	case enNormalWithdraw: {
		system("cls");
		ShowNormalWithdrawMenuScreen();
		GoBackToATMMainMenuScreen();
		break;
	}
	case enDeposit: {
		system("cls");
		ShowDepositScreen();
		GoBackToATMMainMenuScreen();
		break;
	}
	case enCheckBalance: {
		system("cls");
		CheckBalance();
		GoBackToATMMainMenuScreen();
		break;
	}
	case enLogout: {
		system("cls");
		Login();
		break;
	}
	}
}

/// @brief Displays the main ATM menu and processes selected option.
void ShowATMMainMenuScreen() {

	system("cls");
	cout << "========================================\n";
	cout << "\t\ATM Main Menu Screen\n";
	cout << "========================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "========================================\n" << endl;

	PerformATMMainMenuoption(ReadATMMainMenuOption());
}

/// @brief Handles the login process (loops until correct account number & PIN).
void Login() {

	bool LoginFaild = false;
	string AccountNumber, PinCode;

	do {
		system("cls");
		cout << "========================================\n";
		cout << "\t\Login Screen\n";
		cout << "========================================\n";

		if (LoginFaild)
			cout << "Invalid Account Number/PinCode!\n";

		cout << "Enter Account Number?: ";
		cin >> AccountNumber;

		cout << "Enter PinCode?: ";
		cin >> PinCode;

		LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFaild);

	ShowATMMainMenuScreen();
}

//================= Main Function =================//

int main()
{

	Login();
    return 0;
}