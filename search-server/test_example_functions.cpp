#include "test_example_functions.h"

using std::string_literals::operator""s;

void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line, const std::string& hint)
{
    if (!value) {
        std::cout << file << "("s << line << "): "s << func << ": "s;
        std::cout << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            std::cout << " Hint: "s << hint;
        }
        std::cout << std::endl;
        std::abort();
    }
}

void TestExcludeStopWordsFromAddedDocumentContent()
{
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
    {
        SearchServer server("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(),
            "Stop words must be excluded from documents"s);
    }
}
void TestExcludeMinusTextWithMinusWordsInFindTopDocuments()
{
    {
        const int doc_id = 42;
        const std::string content = "cat in the city"s;
        const std::vector<int> ratings = { 1, 2, 3 };
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(1, "mouse without war"s, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in -war"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
}

void TestFindTopDocumentsWithPredicate()
{
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.AddDocument(321, "cat "s, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(11, "cat in content"s, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s, [](int document_id, DocumentStatus status, int rating) { return document_id % 2 == 0; });
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
}

void TestFindTopDocumentsWithStatus()
{
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.AddDocument(321, "cat "s, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id, content, DocumentStatus::BANNED, ratings);
        server.AddDocument(11, "cat in content"s, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s, DocumentStatus::BANNED);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
}
void TestFindTopDocuments()
{
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.AddDocument(321, "cat "s, DocumentStatus::BANNED, ratings);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(11, "cat in content"s, DocumentStatus::BANNED, ratings);
        const auto found_docs = server.FindTopDocuments("cat"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }
}

void TestGetDocumentCount()
{
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.GetDocumentCount();
        ASSERT_EQUAL(found_docs, 1u);
        server.AddDocument(321, "cat "s, DocumentStatus::BANNED, ratings);
        server.AddDocument(11, "cat in content"s, DocumentStatus::BANNED, ratings);
        const auto found_docs2 = server.GetDocumentCount();
        ASSERT_EQUAL(found_docs2, 3u);
    }
}

void TestMatchDocument()
{
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = { 1, 2, 3 };
    std::tuple<std::vector<std::string>, DocumentStatus> focus_doc = { {"cat"}, DocumentStatus::ACTUAL };
    SearchServer server("in"s);
    server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(1, "mouse without war"s, DocumentStatus::ACTUAL, ratings);
    const auto found_docs = server.MatchDocument("cat", doc_id);
    ASSERT(found_docs == focus_doc);
}

void TestSortFindedDocumentsByRelevance()
{
    const int count_docs = 3;
    const std::vector<int> doc_id = { 6, 1, 4 };
    const std::vector<std::string> content = { "big mouse lives on the roof"s,
                                    "strong bee flying near the ear"s,
                                    "happy elephant shakes a tree in front of children"s };
    const std::vector<std::vector<int>> ratings = { {4, 2},
                                          {1},
                                          {9, 10, 9} };
    SearchServer server;
    for (int i = 0; i < count_docs; ++i) {
        server.AddDocument(doc_id[i], content[i], DocumentStatus::ACTUAL, ratings[i]);
    }
    const std::vector<Document> found_docs = server.FindTopDocuments("mouse bee shakes roof flying big"s);
    for (int i = 1; i < count_docs; ++i) {
        ASSERT(found_docs[i].relevance < found_docs[i - 1].relevance);
    }
}

void TestComputngRating()
{
    const int doc_id = 43;
    const std::string content = { "nice snake stand upright"s };
    const std::vector<int> rating = { 4, 5, 10, 1 };
    SearchServer server;
    server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, rating);
    const auto found_docs = server.FindTopDocuments("snake"s);
    ASSERT_EQUAL(found_docs[0].rating, 5u);
}

void TestSearchServer()
{
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestExcludeMinusTextWithMinusWordsInFindTopDocuments);
    RUN_TEST(TestFindTopDocumentsWithPredicate);
    RUN_TEST(TestFindTopDocumentsWithStatus);
    RUN_TEST(TestFindTopDocuments);
    RUN_TEST(TestGetDocumentCount);
    RUN_TEST(TestMatchDocument);
    RUN_TEST(TestSortFindedDocumentsByRelevance);
    RUN_TEST(TestComputngRating);
}

