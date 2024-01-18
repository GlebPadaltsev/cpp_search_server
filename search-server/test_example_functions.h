#pragma once

#include "paginator.h"
#include "document.h"
#include "remove_duplicates.h"
#include "search_server.h"

#include <vector>
#include <string>
#include <iostream>
#include <tuple>

using std::string_literals::operator""s;

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& out, const std::pair<Key, Value> container);

template <typename T>
void Print(std::ostream& out, const T& documents) {
    bool flag = true;
    for (const auto& element : documents) {
        if (!flag) {
            out << ", "s;
        }
        flag = false;
        out << element;
    }
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& container) {
    Print(out << '[', container);
    return out << ']';
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& container) {
    Print(out << '{', container);
    return out << '}';
}

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& out, const std::map<Key, Value> container) {
    out << '{';
    Print(out, container);
    return out << '}';
}

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& out, const std::pair<Key, Value> container) {
    return out << container.first << ": " << container.second;
}
//==================== Начало кода для тестов (макросы и функции) ============================ 

template <typename T>
void RunTestImpl(const T& func, const std::string& f) {
    func();
    std::cerr << f << " OK"s << std::endl;
}

#define RUN_TEST(function)  RunTestImpl((function), #function) 

//============================================================================================================ 

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const std::string& t_str, const std::string& u_str, const std::string& file,
    const std::string& func, unsigned line, const std::string& hint) {
    if (t != u) {
        std::cout << std::boolalpha;
        std::cout << file << "("s << line << "): "s << func << ": "s;
        std::cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        std::cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            std::cout << " Hint: "s << hint;
        }
        std::cout << std::endl;
        std::abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s) 
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint)) 

//================================================================================================================== 

void AssertImpl(bool value, const std::string& expr_str, const std::string& file, const std::string& func, unsigned line,
    const std::string& hint);

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s) 
#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint)) 

//=================================Конец кода для тестов (макросы и функции) ======================= 

//============================== Код тестов ======================= 

void TestExcludeStopWordsFromAddedDocumentContent();
void TestExcludeMinusTextWithMinusWordsInFindTopDocuments();
void TestFindTopDocumentsWithPredicate();
void TestFindTopDocumentsWithStatus();
void TestFindTopDocuments();
void TestGetDocumentCount();
void TestMatchDocument();
void TestSortFindedDocumentsByRelevance();
void TestComputngRating();

//======================= Конец кода для тестов ======================= 


//================ Вызов модульных тестов поисковой системы =========== 

void TestSearchServer();
// ============= Окончание модульных тестов поисковой системы ====================== 