#include "Expression.h"
int main()
{
	Commands com;
	Settings set = com.get_settings();
	Expression full(set, false);
	Expression reals(set, true);
	std::string exp;

	do {
		com.print_welcome();
		std::cout << "\nEnter you expression below`\n  -> ";
		std::getline(std::cin, exp);
		com.set_command(exp);
		full.set_infix(exp);
		if (com.check_command())
			com.run_command();
		else if (full.check_expression())
		{
			set = com.get_settings();
			full.set_settings(set);
			reals.set_settings(set);
			full.set_infix(exp);
			full.find_postfix();
			full.find_truth_vector();
			full.find_fictive_variables();
			reals.set_postfix(full.get_real_postfix());
			reals.find_truth_vector();
			full.print_info();
			full.print_truth_table();
			full.print_fictive_variables();
			reals.print_truth_table();
			full.print_CDNF();
			full.print_CCNF();
			full.print_Zhegalkin();
		}
		else
			std::cout << "\nOops.... Something was wrong in you expression.\n";
		std::cout << "\n<-- Press any key to continue -->\n";
		system("pause > nul");
		system("cls");
	} while (true);
}

