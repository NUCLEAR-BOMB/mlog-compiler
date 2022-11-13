#include "Parsing.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>

int main(int argc, char* argv[])
{
	std::string infilepath;
	std::string outfilepath;

	if (argc == 2) {
		infilepath = argv[1];
		outfilepath = "out.txt";
	} else if (argc == 3) {
		infilepath = argv[1];
		outfilepath = argv[2];
	} else {
		std::cerr << "\nWrong number of arguments\n";
		std::exit(1);
	}

	std::cout << "Opening file...\n";

	std::fstream infile(infilepath, std::fstream::in);
	if (!infile.is_open()) {
		std::cerr << "\nFile doesnt exists\n";
		std::exit(1);
	}
	std::fstream outfile(outfilepath, std::fstream::trunc | std::fstream::out);

	auto compilation_error = [](std::string_view v) {
		std::cerr << v << " -> " << "Compilation error\n";
		//std::exit(1);
	};

	std::cout << "Compilation...\n\n";
	// get line
	std::string line;
	while (std::getline(infile, line)) 
	{
		// get string using a delimiter
		std::stringstream ss{line};
		while (std::getline(ss, line, ';'))
		{
			mlc::Command command;
			if (!mlc::extract_command(line, command)) compilation_error(line);

			// continue if the command is an empty line
			if (command.name().empty()) continue;

			if (mlc::verify_command(command)) {
				// write mlog command to output file
				std::string rawcommand = command.convert();
				outfile << rawcommand << '\n';

				// print status
				std::cout << line << "\t -> " << rawcommand << '\n';
			} else {
				compilation_error(line);
			}
		}
	}

	// save files
	infile.close();
	outfile.close();
	
	//std::cin.get();
	return 0;
}
