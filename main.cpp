#include <assert.h>
#include <iostream>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "Utils.h"
#include "..\..\sqlite\sqlite3.h"

using namespace sf;
using namespace std;

static int callback(void* pNotUsed, int numF, char** arrVals, char** arrNames) 
{
	stringstream ss;
	int i;
	for (i = 0; i < numF; i++) {
		ss << arrNames[i] << " = " << (arrVals[i] ? arrVals[i] : "NULL") << "\n";
	}
	DebugPrint(ss.str().c_str(), "");
	return 0;
}

void RunSQL(const string& sql, sqlite3* db, int (*pCallback)(void*, int, char**, char**) = nullptr, const string& okPrompt = "RunSQL", const string& errorPrompt = "RunSQL")
{
	assert(!sql.empty() && db);
	char* zErrMsg = nullptr;
	int rc = sqlite3_exec(db, sql.c_str(), pCallback, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		DebugPrint("SQL error: " + errorPrompt, zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		DebugPrint("OK: " + okPrompt);
	}
}


int main()
{
	char* zErrMsg = nullptr;
	sqlite3* db;
	int rc = sqlite3_open("data/chinook.db", &db);
	if (rc) {
		DebugPrint("Can’t open database : ", sqlite3_errmsg(db));
		assert(false);
	}




	cout << "\t \t \t[Tables of Chinook]\n";

	const string TablenamesQuery = "SELECT name FROM sqlite_master WHERE type = 'table' AND name NOT LIKE 'sqlite_%'; ";
	 RunSQL(TablenamesQuery, db, callback, "\nTable names");

	cout << "\n \t \t\t [Fields in Invoices]\n";


	const string Fieldsininvoices = "SELECT * FROM sqlite_master WHERE type = 'table' AND name = 'invoices'";
	RunSQL(Fieldsininvoices, db, callback, "\ninvoices fields");

	cout << "\n \t \t\t [Customer info for Julia Barnett]\n";

	const string CustomerInfoJB = "SELECT * FROM customers WHERE FirstName='Julia' AND LastName='Barnett'";
	RunSQL(CustomerInfoJB, db, callback, "\nCustomer info");

	cout << "\n \t \t\t [Money spent by Julia]\n";

	const string JuliaRecords = "SELECT InvoiceDate, Total FROM invoices WHERE CustomerId=28; SELECT SUM(Total) From invoices WHERE CustomerId=28;";
	RunSQL(JuliaRecords, db, callback, "all Julias purchase info");


	system("pause");

	return EXIT_SUCCESS;
}
