#pragma once
#include <unordered_map>
#include <string>

class Dictionary
{
public:
	explicit Dictionary(char letter = 33);

	~Dictionary();

	void add(const std::string&, unsigned index = 0);
	const Dictionary* search_char(char) const;

	bool is_full_word() const { return end_of_word_; }
	bool has_children() const { return !children_.empty(); }

private:
	char letter_;
	std::unordered_map<char, Dictionary*> children_;
	bool end_of_word_;
};
