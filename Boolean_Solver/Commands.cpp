#include "Commands.h"
#include <iostream>
Commands::Commands()
{
	settings.main_info = true;
	settings.truth_table = true;
	settings.fictive_search = true;
	settings.real_table = true;
	settings.CDNF = true;
	settings.CCNF = true;
	settings.Zhegalkin = true;
	settings.conjuction_sign = true;
	settings.case_sensitive = true;
}
void Commands::set_command(std::string com)
{
	int i = 0;
	command = "";
	if (!com.empty())
	{
		while (isspace(com[i]) && i < com.length())
			++i;
		while (!isspace(com[i]) && i < com.length())
			command += com[i++];
	}
}
Settings Commands::get_settings()
{
	return settings;
}
bool Commands::check_command()
{
	if (!command.compare("/exit"))
		return true;
	if (!command.compare("/settings"))
		return true;
	if (!command.compare("/help"))
		return true;
	return false;
}
void Commands::run_command()
{
	if (!command.compare("/exit"))
		exit(0);
	if (!command.compare("/settings"))
		set_settings();
	if (!command.compare("/help"))
		print_help();
}
void Commands::set_settings()
{
	std::string choice;
	do
	{
		system("cls");
		std::cout << "<-- Settings -->\n";
		std::cout << "  1. Information about expression: ";
		if (settings.main_info)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  2. Full truth table: ";
		if (settings.truth_table)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  3. Fictive variables search: ";
		if (settings.fictive_search)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  4. Truth table without fictives: ";
		if (settings.real_table)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  5. Complete disjunctive normal form: ";
		if (settings.CDNF)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  6. Complete conjuctive normal form: ";
		if (settings.CCNF)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  7. Zhegalkin polynomial: ";
		if (settings.Zhegalkin)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  8. Conjuction sign: ";
		if (settings.conjuction_sign)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  9. Case sensitivity: ";
		if (settings.case_sensitive)
			std::cout << "Enabled\n";
		else
			std::cout << "Disabled\n";
		std::cout << "  0. Exit.\n";
		std::cout << "\nEnter you setting number below\n  -> ";

		std::getline(std::cin, choice);
		
		if (!choice.compare("1"))
			settings.main_info = !settings.main_info;
		else if (!choice.compare("2"))
			settings.truth_table = !settings.truth_table;
		else if (!choice.compare("3"))
			settings.fictive_search = !settings.fictive_search;
		else if (!choice.compare("4"))
			settings.real_table = !settings.real_table;
		else if (!choice.compare("5"))
			settings.CDNF = !settings.CDNF;
		else if (!choice.compare("6"))
			settings.CCNF = !settings.CCNF;
		else if (!choice.compare("7"))
			settings.Zhegalkin = !settings.Zhegalkin;
		else if (!choice.compare("8"))
			settings.conjuction_sign = !settings.conjuction_sign;
		else if (!choice.compare("9"))
			settings.case_sensitive = !settings.case_sensitive;
		else if (!choice.compare("0"))
			break;
	} while (true);
}
void Commands::print_help()
{
	system("cls");
	std::cout << "<-- How to use -->\n";
	std::cout << " - Enter the logical function in the field and press \"Enter\" (for example, a+b*c).\n";
	std::cout << " - Enter the /settings command in the field to change settings.\n";
	std::cout << " - Enter the /exit command to exit the program.\n";
	std::cout << " - Enter the /help command to open this menu.\n";
	std::cout << "\n<-- What this solver can? -->\n";
	std::cout << " - Create truth table and find truth vector of the function.\n";
	std::cout << " - Find the fictive variables of the function.\n";
	std::cout << " - Remove fictive variables from truth table and truth vector.\n";
	std::cout << " - Create complete disjunctive form of the function.\n";
	std::cout << " - Create complete conjuctive form of the function.\n";
	std::cout << " - Create Zhegalkin polynomial of the function.\n";
	std::cout << "\n<-- Rules -->\n";
	std::cout << " - Use only round scopes ().\n";
	std::cout << " - Every variable must be a single letter.\n";
	std::cout << " - Expression can contain spaces and values.\n";
	std::cout << " - You can put down conjuction sign.\n";
}
void Commands::print_welcome()
{
	system("cls");
	std::cout << "<-- Welcome to Boolean Expression Solver -->\n";
	std::cout << "  ~ Negation (NOT)\n";
	std::cout << "  * Conjuction (AND)\n";
	std::cout << "  + Disjunction (OR)\n";
	std::cout << "  > Implication (IMP)\n";
	std::cout << "  = Equivalence (XNOR)\n";
	std::cout << "  @ Exclusive disjunction (XOR)\n";
	std::cout << "  ! Peirce arrow (NOR)\n";
	std::cout << "  | Sheffer stroke (NAND)\n";
	std::cout << "\n - Type /settings to open settings menu.\n";
	std::cout << " - Type /exit to exit the programm.\n";
	std::cout << " - Type /help for more information.\n";
}