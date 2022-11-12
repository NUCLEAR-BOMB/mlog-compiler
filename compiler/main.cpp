#include "Parsing.hpp"
#include <fstream>
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	//if (argc != 2) return 1;
	(void)argc;
	(void)argv;

	std::cout << "Opening file\n";

	std::string file = "t.txt";

	std::fstream infile(file, std::fstream::in);
	if (!infile.is_open()) return 1;

	std::cout << "Reading\n";
	std::string line;
	while (std::getline(infile, line)) {
		mlc::Command command = mlc::extract_command(line);

		std::cout << command.convert() << '\n';
	}
	
	std::cin.get();
	return 0;
}
