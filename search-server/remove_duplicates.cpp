#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server)
{
	std::set<int> to_remove_duplicats_id;
	std::map<std::set<std::string>, std::vector<int>> original_words;
	for (const int id_words : search_server) {
		std::map<std::string, double > document_to_word = search_server.GetWordFrequencies(id_words);
		std::set<std::string> orig_word;
		for (auto& [word, freq] : document_to_word) {
			orig_word.insert(word);
		}
		if (original_words.count(orig_word) == 0) {
			original_words[orig_word].push_back(id_words);
		}
		else {
			to_remove_duplicats_id.insert(id_words);
		}
	}
	for (const int& remove_id : to_remove_duplicats_id) {
		search_server.RemoveDocument(remove_id);
		std::cout << "Found duplicate document id "s << remove_id << std::endl;
	}
}
