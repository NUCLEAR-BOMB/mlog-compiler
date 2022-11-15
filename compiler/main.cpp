#include "Parsing.hpp"
#include "Variables.hpp"
#include "Commands.hpp"

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
		//std::cerr << "\nWrong number of arguments\n";
		//std::exit(1);
		infilepath = "t.txt";
		outfilepath = "out.txt";
	}

	std::cout << "Opening file...\n";

	std::fstream infile(infilepath, std::fstream::in);
	if (!infile.is_open()) {
		std::cerr << "\nFile doesnt exists\n";
		std::exit(1);
	}
	std::fstream outfile(outfilepath, std::fstream::trunc | std::fstream::out);
	if (!outfile.is_open()) {
		std::cerr << "\nWrite file error\n";
		std::exit(1);
	}

	bool is_compilation_with_error = false;
	auto compilation_error = [&](std::string_view v) 
	{
		std::cerr << v << " -> " << "Compilation error\n";
		is_compilation_with_error = true;
		//std::exit(1);
	};

	mlc::VariablesPool varpool;

	std::cout << "Compilation...\n\n";

	

	// get line
	std::string line;
	while (std::getline(infile, line)) 
	{
		// skip a line if it is empty
		if (line.find_first_not_of(' ') == std::string::npos) continue;
		mlc::check_line_comment(line);

		// get string using a delimiter
		std::stringstream ss{line};
		while (std::getline(ss, line, ';'))
		{
			// skip a line if it is empty
			if (line.find_first_not_of(' ') == std::string::npos) continue;

			mlc::Command command;

			if (mlc::extract_command(line, command)) {
				// write mlog command to output file
				std::string rawcommand = command.convert();
				outfile << rawcommand << '\n';

				// print status
				std::cout << line << "\t -> " << rawcommand << '\n';
			}
			else {
				compilation_error(line);
				continue;
			}

			if (varpool.is_creating_var(command)) {
				if (varpool.add(command)) {
					//std::cout << "Adding var\n";
				} else {
					std::cout << "^^^ Error ^^^\n";
					is_compilation_with_error = true;
					//compilation_error(line);
				}
			}
		}
	}

	// save files
	infile.close();

	if (is_compilation_with_error) {
		outfile << "\nCOMPILATION FAILED WITH AN ERROR";
	}

	outfile.close();
	
	std::cin.get();
	return 0;
}
