#include <iostream>
#include <fstream>
#include <string>
#include "IniWriter.h"
#include "IniReader.h"

int main(int argc, char * argv[])
{

	std::string iniName = "Settings.ini";

	std::ifstream iniExists(iniName);

	if (!iniExists) {
		CIniWriter iniWriter(".\\Settings.ini");
		iniWriter.WriteString("Settings", "Email", "avery627@gmail.com");
		iniWriter.WriteString("Settings", "Liscense", "9912312gydg21dyu1gd");
	}

	CIniReader iniReader(".\\Settings.ini");
	char *szEmail = iniReader.ReadString("Settings", "Email", "");
	char *szLiscense = iniReader.ReadString("Settings", "Liscense", "");

	std::cout << "Email:" << szEmail << std::endl
		<< "Liscense:" << szLiscense << std::endl;
	delete szEmail;

	system("PAUSE");
	return 0;
}