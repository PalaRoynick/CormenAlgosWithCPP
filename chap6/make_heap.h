#pragma once

#include <iostream>

namespace my_adapters {

template <typename RandomIt, typename Comparator>
void heapify(RandomIt begin, RandomIt end, RandomIt it, Comparator cmp = Comparator()) {
    auto size = end - begin;
    auto i = it - begin;
    auto left  = begin + 2 * i + 1;
    auto right = left + 1;
    auto largest = ( (left - begin < size) && cmp(*it, *left) ) ? left : it;
    if ( (right - begin < size) && cmp(*largest, *right) ) {
        largest = right;
    }
    if (largest != it) {
        std::swap(*largest, *it);
        heapify(begin, end, largest, cmp);
    }
}

template <typename RandomIt, typename Comparator>
void make_heap(RandomIt begin, RandomIt end, Comparator cmp = Comparator()) {
    auto size = end - begin;
    for (auto i = size / 2 - 1; i >= 0 ; --i) {
        heapify(begin, end, begin + i, cmp);
    }
}

template <typename RandomIt, typename Comparator>
void pop_heap(RandomIt begin, RandomIt end, Comparator cmp) {
    std::swap(*begin, *(end - 1));
    make_heap(begin, end - 1, cmp);
}

/*
    Inserts the element at position (end - 1) into the heap [begin, end - 1).
    The resulting heap is [begin, end).
*/
template <typename RandomIt, typename Comparator>
void push_heap(RandomIt begin, RandomIt end, Comparator cmp) {
    std::swap(*(end - 1), *begin);
    heapify(begin, end, begin, cmp);
}

template <typename RandomIt, typename Comparator>
void heap_sort(RandomIt begin, RandomIt end, Comparator cmp) {
    make_heap(begin, end, cmp);
    for (auto it = end - 1; it != begin; --it) {
        std::swap(*begin, *it);
        heapify(begin, it, begin, cmp);
    }
}

}