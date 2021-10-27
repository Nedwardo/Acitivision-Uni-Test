#include "Dictionary.h"
#include <iostream>
#include <fstream>

using namespace std;

int get_all_words(const Dictionary& dictionary, const vector<string>& wheel)
{
	int count = 0;
	auto current_lookups = vector<pair<string, const Dictionary*>>(0);

	for (unsigned wheel_index = 0; wheel_index < wheel.size(); wheel_index++)
	{
		auto new_lookups = vector<pair<string, const Dictionary*>>(0);
		for (unsigned string_index = 0; string_index < wheel[0].size(); string_index++)
		{
			const char current_char = tolower(wheel[wheel_index][string_index]);
			for (const auto& current_lookup : current_lookups)
			{
				auto next_dictionary = current_lookup.second->search_char(current_char);
				if (next_dictionary)
				{
					if (next_dictionary->is_full_word())
					{
						count += 1;
						cout << current_lookup.first + static_cast<char>(toupper(current_char)) << '\n';
					}
					if (next_dictionary->has_children())
						new_lookups.emplace_back(make_pair(
							current_lookup.first + static_cast<char>(toupper(current_char)), next_dictionary));
				}
			}
			if (wheel_index < wheel.size() - 1) // No one letter words
				new_lookups.emplace_back(make_pair(string(1, static_cast<char>(toupper(current_char))),
				                                   dictionary.search_char(current_char)));
		}
		current_lookups = new_lookups;
	}
	return count;
}

int main(const int argc, char* argv[])
{
	if (argc != 3)
	{
		cout << "Expecting only two arguments:\n 1) Location of Dictionary file\n 2) Location of wheels file";
		return 1;
	}
	ifstream infile(argv[1]);
	if (infile.fail())
	{
		cout << "Could not access wheels file\n Address given = " << argv[1] << '\n';
		return 1;
	}
	string line;
	unsigned wheel_count;
	unsigned wheel_width;
	try
	{
		getline(infile, line);
		wheel_count = stoi(line);
		getline(infile, line);
		wheel_width = stoi(line);
	}
	catch (invalid_argument&)
	{
		cout << "In wheels file (" << argv[1] << ", non-numerical characters are included in the wheels dimensions";
		return 1;
	}
	auto wheels = vector<string>(wheel_count);
	for (unsigned wheel_index = 0; getline(infile, line) && wheel_index < wheel_count; wheel_index++)
	{
		if (line.size() != wheel_width)
		{
			cout << "In wheels file (" << argv[1] << ", number of letters per wheel is incorrect";
			return 1;
		}
		for (auto letter : line)
		{
			if (!isalpha(letter))
			{
				cout << "In wheels file (" << argv[1] << ", non-alphabet characters are included in the wheels";
				return 1;
			}
		}

		wheels[wheel_index] = line;
	}
	infile.close();
	if (wheels[wheels.size() - 1].empty())
	{
		cout << "In wheels file (" << argv[1] << ", number of wheels is less than stated";
		return 1;
	}
	if (getline(infile, line))
	{
		cout << "In wheels file (" << argv[1] << ", number of wheels is more than stated";
		return 1;
	}

	auto dictionary = Dictionary();
	infile.close();
	infile.open(argv[2]);
	if (infile.fail())
	{
		cout << "Could not access dictionary file\n Address given = " << argv[2] << '\n';
		return 1;
	}
	cout << "Generating dictionary from file" << '\n';
	while (getline(infile, line))
	{
		for (auto letter : line)
		{
			if (!isalpha(letter))
			{
				cout << "In the dictionary file (" << argv[2] << ", non-alphabet characters are included in words";
				return 1;
			}
		}
		dictionary.add(line);
	}
	cout << "Finished generating dictionary from file" << '\n';
	infile.close();
	cout << "Found " << get_all_words(dictionary, wheels) << " words \n";

	return 0;
}
