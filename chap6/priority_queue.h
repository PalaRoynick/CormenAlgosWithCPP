#pragma once

#include <deque>
#include <stdexcept>

#include "make_heap.h"

namespace my_adapters {

template <typename T,
          template <typename> class Container = std::deque,
          typename Comparator = std::less<T>
         >
class priority_queue {
    //using Iterator = std::decay_t< decltype(Container<T>().begin()) >;

    Container<T> c_;
    Comparator cmp_;

public:
    /*
        Creates a binary heap with max (cmp == std::less) or min (cmp == std::greater)
    */

    template <typename Iterator>
    priority_queue(Iterator begin, Iterator end, Comparator cmp = Comparator());

    void insert(const T& elem);
    void insert(T&& elem);

    const T& top();

    void extract_top();

    // ???
    // void increase_key(const T& elem, const Key& key);
};

template <typename T,
          template <typename> class Container,
          typename Comparator
         >
template <typename Iterator>
priority_queue<T, Container, Comparator>::priority_queue(Iterator begin,Iterator end, Comparator cmp)
    : c_(begin, end)
    , cmp_(cmp) {
    make_heap(c_.begin(), c_.end(), cmp_);
}

template <typename T,
          template <typename> class Container,
          typename Comparator
         >
const T& priority_queue<T, Container, Comparator>::top() {
    /* heap invariant must be satisfied! */
    if (c_.size() > 0)
        return *c_.cbegin();
    throw std::runtime_error("[priority_queue::top] Empty heap!");
}

template <typename T,
          template <typename> class Container,
          typename Comparator
         >
void priority_queue<T, Container, Comparator>::extract_top() {
    if (c_.size() < 1)
        throw std::runtime_error("[priority_queue::extract_top] Empty heap!");
    pop_heap(c_.begin(), c_.end(), cmp_);
    c_.pop_back();
}

template <typename T,
          template <typename> class Container,
          typename Comparator
         >
void priority_queue<T, Container, Comparator>::insert(const T& value) {
    c_.push_back(value);
    push_heap(c_.begin(), c_.end(), cmp_);
}

template <typename T,
          template <typename> class Container,
          typename Comparator
         >
void priority_queue<T, Container, Comparator>::insert(T&& value) {
    c_.push_back(std::move(value));
    push_heap(c_.begin(), c_.end(), cmp_);
}

}