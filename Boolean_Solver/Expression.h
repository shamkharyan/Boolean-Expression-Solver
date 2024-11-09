#pragma once
#include "Commands.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
struct Variable
{
	char name;
	bool realnes;
};
class Expression
{
private:
	Settings settings;
	int variables_count;
	int combinations;
	bool is_real;
	std::vector<Variable> variables;
	std::vector<std::vector<bool>> variables_values;
	std::vector<bool> truth_vector;

	std::string infix;
	std::string postfix;

	//checkers
	bool check_scopes();

	//finders
	int operator_precedence(char op);
	void find_variables(); //Finds all variables from postfix equation. Calls find_postfix()
	void find_variables_values(); //Finds all variables values. Calls find_variables()
public:

	Expression(Settings set, bool real);

	//getters
	std::string get_infix();
	std::string get_postfix();
	int get_variables_count();
	int get_combinations();
	std::string get_real_postfix();

	//setters
	void set_infix(std::string exp);
	void set_postfix(std::string exp);
	void set_settings(Settings set);

	//checkers
	bool check_expression();
	
	//finders
	void find_postfix(); //Converts infix to postfix.
	void find_fictive_variables();
	void find_truth_vector(); //Finds truth vector of expression. Calls find_variables_values()
	bool solve_postfix(std::string exp);

	//printers
	void print_info();
	void print_variables();
	void print_truth_vector();
	void print_truth_table();
	void print_fictive_variables();
	void print_CDNF();
	void print_CCNF();
	void print_Zhegalkin();
	
};
