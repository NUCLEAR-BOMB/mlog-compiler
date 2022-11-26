#include "Parsing.hpp"
#include "Variables.hpp"
#include "Commands.hpp"
#include "Line.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <regex>
#include <locale>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define IS_WINDOWS 1
#else
	#define IS_WINDOWS 0
#endif

#if IS_WINDOWS
#include <Windows.h>
#endif

int main(int argc, char* argv[])
{
#if IS_WINDOWS
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
#endif

	(void)mlc::COMMAND_LIST;

	std::string infilepath;
	std::string outfilepath;

	if (argc == 2) {
		infilepath = argv[1];
		outfilepath = "out.txt";
	} else if (argc == 3) {
		infilepath = argv[1];
		outfilepath = argv[2];
	} else {
		//std::wcerr << "\nWrong number of arguments\n";
		//std::exit(1);
		infilepath = "t.txt";
		outfilepath = "out.txt";
	}

	std::wclog << L"Opening (" << infilepath.c_str() << L")\n";
	std::wfstream infile(infilepath, std::fstream::in);
	if (!infile.is_open()) {
		std::wcerr << L"Input file open error\n";
		std::exit(1);
	}

	std::wfstream outfile(outfilepath, std::fstream::trunc | std::fstream::out);
	if (!outfile.is_open()) {
		std::wcerr << L"Output file open error\n";
		std::exit(1);
	}

	bool is_compilation_with_error = false;

	//auto compilation_error = [&](const mlc::Line& line) 
	//{
	//	std::wcerr << line.line() << " " << line.get() << " -> " << "Compilation error\n";
	//	is_compilation_with_error = true;
	//	//std::exit(1);
	//};

	mlc::VariablesPool varpool;
	mlc::ErrorTrace errortrace;

	std::wclog << L"Compilation...\n\n";

	// get line
	mlc::Line line;
	while (std::getline(infile, (++line).get()))
	{

		// skip a line if it is empty
		if (line.get().find_first_not_of(' ') == std::wstring::npos) continue;
		mlc::uncomment_line(line);

		// get string using a delimiter
		std::wstringstream ss{line.get()};
		while (std::getline(ss, line.get(), L';'))
		{
			// skip a line if it is empty
			if (line.get().find_first_not_of(' ') == std::wstring::npos) continue;

			std::vector<mlc::Command> commandarr; commandarr.reserve(3);
			mlc::Operator op;

			commandarr.clear();
				 if (!errortrace.push(mlc::extract_operator(line, commandarr)).critical());
			else if (!errortrace.push(mlc::extract_command(line, commandarr)).critical());
			else {
				errortrace.push(mlc::Error(line, L"Using unknown action"));
			}

			for (const auto& command : commandarr)
			{
				if (!mlc::is_command_variables_valid(varpool, command)) {
					//std::wcerr << "^^^ Using unknown variable ^^^\n";
					errortrace.push(mlc::Error(line, L"Using unknown variable"));
				}

				if (mlc::is_creating_var(command)) {
					if (varpool.add(command.out_arg())) {
						//std::wcout << "Adding var\n";
					}
					else {
						//std::wcerr << "^^^ Invalid variable ^^^\n";
						errortrace.push(mlc::Error(line, L"Invalid variable name"));
						//compilation_error(line);
					}
				}

				if (errortrace) {
					// Print error trace
					std::wcerr << errortrace << L'\n';
					is_compilation_with_error = true;

					errortrace.clear();
					continue;
				}

				mlc::raw_mlog_command_type rawcommand = command.convert();
				// write mlog command to output file
				outfile << rawcommand << L'\n';

				// print status
				std::wcout << line.get() << L"\t -> " << rawcommand << L'\n';
			}
		}
	}

	// save files
	infile.close();

	if (is_compilation_with_error) {
		outfile << "\n# COMPILATION FAILED WITH AN ERROR";
	}

	outfile.close();
	
	std::cin.get();
	return 0;
}
