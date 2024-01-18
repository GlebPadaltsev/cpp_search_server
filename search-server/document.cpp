
#include "document.h"
using namespace std::literals;

Document::Document(int id, double relevance, int rating) : id(id)
, relevance(relevance)
, rating(rating) {
}

std::ostream& operator<<(std::ostream& cout, const Document& doc)
{
    return cout << "{ "s << "document_id = "s
        << doc.id << ", "s
        << "relevance = "s
        << doc.relevance << ", "s
        << "rating = "s << doc.rating << " }"s;
}
