#include "priority_queue.h"
#include <vector>
#include <iostream>
#include <iterator>

int main() {
    std::vector<int> v = { 2, 5, 7, 1, 10, 8, 3, 4, 6, 9 };

    //my_adapters::make_heap(v.begin(), v.end(), std::greater<int>{});

    //my_adapters::heap_sort(v.begin(), v.end(), std::less<int>{});

    auto q = my_adapters::priority_queue<int, std::vector, std::less<int>>(v.begin(), v.end(), std::less<int>{});

    std::cout << q.top() << std::endl;

    q.extract_top();

    std::cout << q.top() << std::endl;

    q.insert(11);

    std::cout << q.top() << std::endl;

    q.extract_top();

    std::cout << q.top() << std::endl;

    std::copy(v.cbegin(), v.cend(), std::ostream_iterator<int>(std::cout, " -> "));
}