#pragma once
#include <iostream>
#include <vector>

template <typename Iterator>
class IteratorRange {
public:
    explicit IteratorRange(Iterator begin = nullptr, Iterator end = nullptr) : first_(begin), last_(end) {}
    Iterator begin()const { return first_; }
    Iterator end() const { return last_; }
    size_t size() { return distance(first_, last_); }
private:
    Iterator first_;
    Iterator last_;
};

template <typename Iterator>
class Paginator {
    // тело класса
public:
    explicit Paginator() = default;
    explicit Paginator(Iterator first, Iterator end, const size_t& page_size) {
        int num_pages = 0;
        if (page_size != 0) {
            num_pages = (distance(first, end) + page_size - 1) / page_size;
        }
        auto ptr_first_it = first;
        for (int page = 0; page < num_pages; page++)
        {
            int row = 0;
            auto temp_ptr = ptr_first_it;
            while (row != page_size && ptr_first_it != end) {
                row++;
                ptr_first_it++;
            }
            pages_.push_back(IteratorRange< Iterator>(temp_ptr, --ptr_first_it));
            ptr_first_it++;
        }
    }
    auto begin() const { return pages_.begin(); }
    auto end() const { return pages_.end(); }

private:
    std::vector< IteratorRange<Iterator>> pages_;
};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}


template <typename T>
std::ostream& operator <<(std::ostream& cout, const IteratorRange<T>& page) {

    for (T ptr = page.begin(); ptr <= page.end(); ptr++) {
        cout << *ptr;
    }
    return cout;
}