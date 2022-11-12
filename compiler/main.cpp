#include "Parsing.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>

int main(int argc, char *argv[])
{
	//if (argc != 2) return 1;
	(void)argc;
	(void)argv;

	std::cout << "Opening file\n";

	std::string filepath = "t.txt";
	std::string outfilepath = "tout.txt";

	std::fstream infile(filepath, std::fstream::in);
	std::fstream outfile(outfilepath, std::fstream::trunc | std::fstream::out);

	if (!infile.is_open()) return 1;

	auto compilation_error = [](std::string_view v) {
		std::cerr << v << " -> " << "Compilation error\n";
		//std::exit(1);
	};

	std::cout << "Compilation...\n\n";
	std::string line;
	while (std::getline(infile, line)) 
	{
		std::stringstream ss{line};
		while (std::getline(ss, line, ';'))
		{
			mlc::Command command;
			if (!mlc::extract_command(line, command)) compilation_error(line);

			if (command.name().empty()) continue;

			if (mlc::verify_command(command)) {
				std::string rawcommand = command.convert();
				outfile << rawcommand << '\n';

				std::cout << line << "\t -> " << rawcommand << '\n';
			} else {
				compilation_error(line);
			}
		}
	}

	infile.close();
	outfile.close();
	
	std::cin.get();
	return 0;
}
