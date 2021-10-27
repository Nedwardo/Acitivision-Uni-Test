#include "Dictionary.h"
#include <stdexcept>
using namespace std;

Dictionary::Dictionary(const char letter)
{
	letter_ = letter;
	end_of_word_ = false;
	std::unordered_map<char, Dictionary*> children{};
}

Dictionary::~Dictionary()
{
	children_.clear();
}

const Dictionary* Dictionary::search_char(const char char_) const
{
	const auto next_letter = children_.find(char_);
	return next_letter != children_.end() ? next_letter->second : nullptr;
}

void Dictionary::add(const string& word, const unsigned index)
{
	if (index < word.size())
	{
		if (!children_[word[index]])
		{
			children_[word[index]] = new Dictionary(word[index]);
		}
		children_[word[index]]->add(word, index + 1);
	}
	else
		end_of_word_ = true;
}
