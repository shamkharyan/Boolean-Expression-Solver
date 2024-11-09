#pragma once
#include <string>
struct Settings
{
	bool main_info;
	bool truth_table;
	bool fictive_search;
	bool real_table;
	bool CDNF;
	bool CCNF;
	bool Zhegalkin;
	bool conjuction_sign;
	bool case_sensitive;
};
class Commands
{
private:
	Settings settings;
	std::string command;
public:
	Commands();
	Settings get_settings();
	void set_command(std::string);
	void run_command();
	bool check_command();
	void print_welcome();
	void print_help();
	void set_settings();
};