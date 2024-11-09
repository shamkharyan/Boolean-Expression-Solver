#include "Expression.h"

Expression::Expression(Settings set, bool real)
{
	settings = set;
	is_real = real;
}

//getters
std::string Expression::get_postfix()
{
	return postfix;
}
std::string Expression::get_infix()
{
	return infix;
}
int Expression::get_variables_count()
{
	return variables_count;
}
int Expression::get_combinations()
{
	return combinations;
}
std::string Expression::get_real_postfix()
{
	std::string real_postfix = postfix;
	for (int i = 0; i < variables_count; i++)
		if (!variables[i].realnes)
		std::replace(real_postfix.begin(), real_postfix.end(), (int)variables[i].name, (int)'1');
	return real_postfix;
}


//setters
void Expression::set_infix(std::string exp)
{
	infix = "";
	if (!exp.empty())
	{
		exp.erase(remove_if(exp.begin(), exp.end(), isspace), exp.end());
		if (!settings.case_sensitive)
			std::transform(exp.begin(), exp.end(), exp.begin(), tolower);
		for (int i = 0; i < exp.length() - 1; i++)
		{
			infix += exp[i];
			if ((isalpha(exp[i]) || exp[i] == ')' || exp[i] == ')') 
				&& (isalpha(exp[i + 1]) || exp[i + 1] == '(' || exp[i + 1] == '~'))
				infix += '*';
		}
		infix += exp[exp.length() - 1];
	}
}
void Expression::set_postfix(std::string exp)
{
	postfix = exp;
}
void Expression::set_settings(Settings set)
{
	settings = set;
}


//checkers
bool Expression::check_scopes()
{
	int counter = 0;
	for (char c : infix)
	{
		if (c == '(')
			counter++;
		else if (c == ')')
			counter--;
		if (counter < 0)
			return false;
	}
	if (counter == 0)
		return true;
	return false;
}
bool Expression::check_expression()
{
	if (!check_scopes())
		return false;
	bool is_var = false;
	bool is_op = true;
	bool is_scope = false;
	for (char c : infix)
	{
		if (isalpha(c) || c == '0' || c == '1')
		{
			if (is_var && !is_scope)
				return false;
			is_var = true;
			is_scope = false;
			is_op = false;
		}
		else if (c == '(')
		{
			is_var = false;
			is_scope = true;
			is_op = false;
		}
		else if (c == ')')
		{
			if (is_op || is_scope)
				return false;
			is_var = true;
			is_scope = false;
			is_op = false;
		}
		else if (c != '~' && operator_precedence(c) != -1)
		{
			if (!is_var || is_scope || is_op)
				return false;
			is_var = false;
			is_scope = false;
			is_op = true;
		}
		else if (c == '~')
		{
			if (is_var)
				return false;
			is_op = true;
			is_scope = false;
		}
		else
			return false;
	}
	if (is_scope || is_op)
		return false;
	return true;
}


//finders
void Expression::find_variables()
{
	variables_count = 0;
	std::vector<char> temp;
	for (char c : postfix)
	{
		if (isalpha(c) && std::find(temp.begin(), temp.end(), c) == temp.end())
		{
			temp.push_back(c);
			variables_count++;
		}
	}
	combinations = 1 << variables_count;
	sort(temp.begin(), temp.end());
	if (!variables.empty())
		variables.clear();
	for (int i = 0; i < variables_count; i++)
		variables.push_back({ temp[i], 0 });
}
void Expression::find_postfix()
{
	postfix = "";
	std::stack<char> st;
	for (char c : infix)
	{
		if (isalpha(c) || c == '0' || c == '1')
			postfix += c;
		else if (c == '(')
			st.push(c);
		else if (operator_precedence(c) != -1 && c != '~')
		{
			while (!st.empty() && operator_precedence(st.top()) >= operator_precedence(c))
			{
				postfix += st.top();
				st.pop();
			}
			st.push(c);
		}
		else if (c == ')')
		{
			while (st.top() != '(')
			{
				postfix += st.top();
				st.pop();
			}
			st.pop();
		}
		else if (c == '~')
			st.push(c);
	}
	while (!st.empty())
	{
		postfix += st.top();
		st.pop();
	}
}
void Expression::find_variables_values()
{
	find_variables();
	if (!variables_values.empty())
	{
		for (int i = 0; i < variables_values.size(); i++)
			variables_values[i].clear();
		variables_values.clear();
	}
	for (int i = 0; i < combinations; i++)
	{
		int rows_copy = combinations / 2;
		variables_values.push_back({});
		while (rows_copy != 0)
		{
			variables_values[i].push_back((bool)((i / rows_copy) % 2));
			rows_copy /= 2;
		}
	}
}
void Expression::find_truth_vector()
{
	find_variables_values();
	if (!truth_vector.empty())
		truth_vector.clear();
	for (int i = 0; i < combinations; i++)
	{
		std::string copy_postfix = postfix;
		for (int j = 0; j < variables_count; j++)
			std::replace(copy_postfix.begin(), copy_postfix.end(), (int)variables[j].name, variables_values[i][j] + '0');
		truth_vector.push_back(solve_postfix(copy_postfix));
	}
}
bool Expression::solve_postfix(std::string exp)
{
	std::stack<bool> st;
	bool op1, op2;
	for (char c : exp)
	{
		if (c == '0' || c == '1')
			st.push(c == '1');
		else
		{
			if (c == '~')
			{
				op1 = st.top();
				st.pop();
				st.push(!op1);
			}
			else
			{
				op2 = st.top();
				st.pop();
				op1 = st.top();
				st.pop();
				switch (c)
				{
				case '*':
					st.push(op1 && op2);
					break;
				case '+':
					st.push(op1 || op2);
					break;
				case '>':
					st.push(!op1 || op2);
					break;
				case '=':
					st.push(op1 == op2);
					break;
				case '@':
					st.push(op1 != op2);
					break;
				case '!':
					st.push(!(op1 || op2));
					break;
				case '|':
					st.push(!(op1 && op2));
					break;
				}
			}
		}
	}
	return st.top();
}
void Expression::find_fictive_variables()
{
	if (variables_count != 0)
	{
		int copy_combinations = combinations;
		for (int i = 0; i < variables_count; i++)
		{
			copy_combinations /= 2;
			for (int j = 0; j < combinations; j++)
			{
				if ((j / copy_combinations) % 2 == 0)
				{
					if (truth_vector[j] != truth_vector[j + copy_combinations])
					{
						variables[i].realnes = true;
						break;
					}
				}
				else
					j += copy_combinations - 1;
			}
		}
	}
}

//printers

void Expression::print_info()
{
	if (settings.main_info)
	{
		std::cout << "\n<-- Information about expression -->\n";
		std::cout << "  Infix notation -> " << infix << '\n';
		std::cout << "  Postfix notation -> " << postfix << '\n';
		std::cout << "  Variables -> ";
		print_variables();
		std::cout << " (" << variables_count << ")\n";
		std::cout << "  Combinations -> " << combinations << '\n';
	}
}
void Expression::print_variables()
{
	if (variables.empty())
		std::cout << "No variables";
	else
	{
		int i;
		for (i = 0; i < variables_count - 1; i++)
			std::cout << variables[i].name << ", ";
		std::cout << variables[i].name;
	}
}
void Expression::print_truth_vector()
{
	std::cout << '(';
	for (int i = 0; i < combinations; i++)
		std::cout << truth_vector[i];
	std::cout << ')';
}
void Expression::print_truth_table()
{
	if ((settings.truth_table && !is_real) || (settings.real_table && is_real))
	{
		if (settings.truth_table && !is_real)
			std::cout << "\n<-- Full truth table -->\n";
		else if (settings.real_table && is_real)
			std::cout << "\n<-- Truth table without fictives-->\n";
		if (variables_count != 0)
		{
			int variables_width = 4 * variables_count + 1;
			int vector_width = 3 * (variables_count + 1) + 1;
			int truth_table_width = variables_width + vector_width + 1;

			std::cout
				<< std::setw(truth_table_width)
				<< std::setfill('-') << '\n';

			for (int i = 0; i < variables_count; i++)
				std::cout << "| " << variables[i].name << " ";
			std::cout << "| f(";
			print_variables();
			std::cout << ") |" << '\n';
			std::cout
				<< std::setw(truth_table_width)
				<< std::setfill('-') << '\n';
			for (int i = 0; i < combinations; i++)
			{
				for (int j = 0; j < variables_count; j++)
					std::cout << "| " << variables_values[i][j] << " ";
				std::cout << "| ";
				std::cout
					<< std::right
					<< std::setw(vector_width / 2 - 1)
					<< std::setfill(' ')
					<< truth_vector[i]
					<< std::setw(vector_width / 2 + vector_width % 2 + 1)
					<< std::setfill(' ')
					<< "|\n";
			}
			std::cout
				<< std::setw(truth_table_width)
				<< std::setfill('-') << '\n';
			std::cout << "  f(";
			print_variables();
			std::cout << ") = ";
			print_truth_vector();
			std::cout << '\n';
		}
		else
		{
			std::cout << "  Result = ";
			print_truth_vector();
			std::cout << '\n';
		}
	}
}
void Expression::print_fictive_variables()
{
	if (settings.fictive_search)
	{
		std::cout << "\n<-- Fictive variables search -->\n";
		if (variables_count != 0)
		{
			int copy_combinations = combinations;
			for (int i = 0; i < variables_count; i++)
			{
				std::cout << "Checking " << variables[i].name << '\n';
				copy_combinations /= 2;
				for (int j = 0; j < combinations; j++)
				{
					if ((j / copy_combinations) % 2 == 0)
					{
						std::cout << "  " << j << " <-> " << j + copy_combinations;
						if (truth_vector[j] != truth_vector[j + copy_combinations])
						{
							std::cout << " Not equal.\n";
							break;
						}
						else
							std::cout << " Equal.\n";
					}
					else
						j += copy_combinations - 1;
				}
				std::cout << variables[i].name << " is ";
				if (variables[i].realnes)
					std::cout << "real.\n";
				else
					std::cout << "fictive.\n";
			}
		}
		else
			std::cout << "  No variables to find fictive ones.\n";
	}
}
void Expression::print_CDNF()
{
	if (settings.CDNF)
	{
		std::cout << "\n<-- Complete disjunctive normal form -->\n";
		if (variables_count != 0)
		{
			bool first = true;
			for (int i = 0; i < combinations; i++)
			{
				if (truth_vector[i] == 1)
				{
					if (first)
					{
						first = false;
						std::cout << "  ";
					}
					else
						std::cout << " + ";
					std::cout << '(';
					for (int j = 0; j < variables_count - 1; j++)
					{
						if (variables_values[i][j] == 0)
							std::cout << '~';
						if (settings.conjuction_sign)
							std::cout << variables[j].name << " * ";
					}
					if (variables_values[i][variables_count - 1] == 0)
						std::cout << '~';
					std::cout << variables[variables_count - 1].name << ")";
				}
			}
			if (first)
				std::cout << "  No CDNF for this expression.\n";
			else
				std::cout << '\n';
		}
		else
			std::cout << "  No variables to create CDNF.\n";
	}
}
void Expression::print_CCNF()
{
	if (settings.CCNF)
	{
		std::cout << "\n<-- Complete conjunctive normal form -->\n";
		if (variables_count != 0)
		{
			bool first = true;
			for (int i = 0; i < combinations; i++)
			{
				if (truth_vector[i] == 0)
				{
					if (first)
					{
						first = false;
						std::cout << "  ";
					}
					else
					{
						if (settings.conjuction_sign)
							std::cout << " * ";
					}
					std::cout << '(';
					for (int j = 0; j < variables_count - 1; j++)
					{
						if (variables_values[i][j] == 1)
							std::cout << '~';
						std::cout << variables[j].name << " + ";
					}
					if (variables_values[i][variables_count - 1] == 1)
						std::cout << '~';
					std::cout << variables[variables_count - 1].name << ")";
				}
			}
			if (first)
				std::cout << "  No CCNF for this expression.\n";
			else
				std::cout << '\n';
		}
		else
			std::cout << "  No variables to create CCNF.\n";
	}
}
void Expression::print_Zhegalkin()
{
	if (settings.Zhegalkin)
	{
		std::cout << "\n<-- Zhegalkin polynomial -->\n";
		if (variables_count != 0)
		{
			std::vector<bool> jegalkin(truth_vector);
			bool is_first = true;
			int last;
			if (jegalkin[0] == 1)
			{
				is_first = false;
				std::cout << "  1";
			}
			for (int i = 1; i < combinations; i++)
			{
				for (int j = 0; j < combinations - i; j++)
					jegalkin[j] = (jegalkin[j] != jegalkin[j + 1]);
				jegalkin.pop_back();
				if (jegalkin[0] == 1)
				{
					if (is_first)
					{
						std::cout << "  ";
						is_first = false;
					}
					else
						std::cout << " @ ";
					for (int j = 0; j < variables_count; j++)
						if (variables_values[i][j])
							last = j;
					std::cout << '(';
					for (int j = 0; j < last; j++)
					{
						if (variables_values[i][j] && settings.conjuction_sign)
							std::cout << variables[j].name << " * ";
						else if (variables_values[i][j])
							std::cout << variables[j].name;
					}
					std::cout << variables[last].name << ')';
				}
			}
			if (is_first)
				std::cout << "  No Zhegalkin polynomial for this expression.\n";
			else
				std::cout << '\n';
		}
		else
			std::cout << "  No variables to create Zhegalkin polynomial.\n";
	}
}
//other
int Expression::operator_precedence(char op)
{
	if (op == '~')
		return 6;
	if (op == '*')
		return 5;
	if (op == '+')
		return 4;
	if (op == '>')
		return 3;
	if (op == '=')
		return 2;
	if (op == '@')
		return 1;
	if (op == '!')
		return 1;
	if (op == '|')
		return 1;
	if (op == '(')
		return 0;
	return -1;
}

