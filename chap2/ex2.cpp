#include <list>
#include <vector>
#include <iterator>

#include <iostream>
#include <cassert>

// 1. We need an iterrator to the middle of the container (without std::distance)
template <typename InputIt>
InputIt iterator_to_the_middle_wo_dist(InputIt begin, InputIt end) {
    if constexpr(std::is_base_of_v< std::random_access_iterator_tag,
                                    typename std::iterator_traits<InputIt>::iterator_category
                                  >) {
        return begin + (end - begin) / 2;
    }
    else {
        auto it = begin;
        std::ptrdiff_t dist{0};
        for (; end != it; it++, dist++) {}
        for (std::ptrdiff_t i = 0; i < dist / 2; i++, begin++) { }
        return begin;
    }
    throw std::runtime_error("[iterator_to_the_middle_wo_dist]: Invalid iterator tag!");
    return end;
}

// 2. We need an iterrator to the middle of the container (with std::distance)
template <typename InputIt>
InputIt iterator_to_the_middle(InputIt begin, InputIt end) {
    auto dist = std::distance(begin, end);
    if constexpr(std::is_base_of_v< std::random_access_iterator_tag,
                                    typename std::iterator_traits<InputIt>::iterator_category
                                  >) {
        return begin + dist / 2;
    }
    else {
        for (std::ptrdiff_t i = 0; i < dist / 2; i++, begin++) { }
        return begin;
    }
}

// somehow std::merge is found by default...
namespace detail {
template <typename InputIt, typename Compare>
void merge(InputIt begin, InputIt mid, InputIt end, Compare cmp) {
    using Type = typename InputIt::value_type;

    auto it1 = begin;
    auto it2 = mid; ++it2;
  
    std::vector<Type> Left;
    Left.reserve(std::distance(it1, it2));
    std::copy(it1, it2,
        std::back_insert_iterator<std::decay_t<decltype(Left)>>(Left));

#ifdef DEBUG
    std::cerr << "Left: ";
    std::copy(Left.begin(), Left.end(),
              std::ostream_iterator<Type>(std::cerr, " "));
    std::cerr << std::endl;
#endif

    auto real_end = end; ++real_end;

    std::vector<Type> Right;
    Right.reserve(std::distance(it2, real_end));
    std::copy(it2, real_end,
        std::back_insert_iterator<std::decay_t<decltype(Right)>>(Right));

#ifdef DEBUG
    std::cerr << "Right: ";
    std::copy(Right.begin(), Right.end(),
              std::ostream_iterator<Type>(std::cerr, " "));
    std::cerr << std::endl;
#endif

    auto il = Left.begin();
    auto ir = Right.begin();

    for ( ; (il != Left.end()) && (ir != Right.end()) ; ) {
        if (cmp(*il, *ir))
            *it1++ = *il++;
        else
            *it1++ = *ir++;
    }
    for ( ; il != Left.end() ; ) {
        *it1++ = *il++;
    }
    for ( ; ir != Right.end() ; ) {
        *it1++ = *ir++;
    }
}
}

template <typename InputIt, typename Compare>
void merge_sort(InputIt begin, InputIt end, Compare cmp) {
    using Type = typename InputIt::value_type;

    if (std::distance(begin, end) <= 0) return;

    auto mid = iterator_to_the_middle(begin, end);
    merge_sort(begin, mid, cmp);
    auto mid_plus = mid; ++mid_plus;
    merge_sort(mid_plus, end, cmp);
    detail::merge(begin, mid, end, cmp);
}

int main() {
    std::vector v = { 5, 2, 4, 7, 1, 3, 2, 6 };
    std::list   l = { 5, 2, 4, 7, 1, 3, 2, 6 };

    //auto it = iterator_to_the_middle_wo_dist(v.cbegin(), v.cend());
    auto it = iterator_to_the_middle(v.cbegin(), v.cend());
    assert(*it == 1);

    //auto it1 = iterator_to_the_middle_wo_dist(l.cbegin(), l.cend());
    auto it1 = iterator_to_the_middle(l.cbegin(), l.cend());
    assert(*it1 == 1);

    merge_sort(v.begin(), v.end() - 1, std::less{});
    std::cout << "final vec:" << std::endl;
    for (auto&& e : v) std::cout << e << "; ";
    std::cout << std::endl;

    merge_sort(l.begin(), l.end(), std::less{});
    std::cout << "final list:" << std::endl;
    for (auto&& e : l) std::cout << e << "; ";
}