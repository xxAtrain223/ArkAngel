//
// Created by Anthony on 5/16/2016.
//

#ifndef ARKANGEL_RANGES_HPP
#define ARKANGEL_RANGES_HPP

#include <boost/range.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/iterator/iterator_facade.hpp>

#include <iterator>

namespace _ranges_detail {

    using std::begin;
    using std::end;

// Compilers are dumb.
#if defined(__GNUC__) && !defined(__APPLE__)
    using boost::rbegin;
    using boost::rend;
#else
    using std::rbegin;
using std::rend;
#endif

    template<typename Iter>
    class deferred_iterator final
            : public boost::iterator_facade<deferred_iterator<Iter>, const Iter, boost::bidirectional_traversal_tag>
    {
        Iter iter;
    public:
        deferred_iterator() = default;
        deferred_iterator(Iter iter) : iter(std::move(iter)) { }
        Iter dereference() const { return iter; }
        bool equal(const deferred_iterator& other) const { return iter == other.iter; }
        void increment() { ++iter; }
        void decrement() { --iter; }
    };

    template<typename Iter>
    deferred_iterator<Iter> make_deferred_iterator(Iter iter)
    {
        return deferred_iterator<Iter>(std::move(iter));
    }

    template<typename Container>
    auto defer(Container& container)
    -> boost::iterator_range<deferred_iterator<decltype(begin(container))>>
    {
        auto db = make_deferred_iterator(begin(container));
        auto de = make_deferred_iterator(end(container));
        return boost::make_iterator_range(db, de);
    }

} // namespace _ranges_detail

using _ranges_detail::begin;
using _ranges_detail::end;
using _ranges_detail::rbegin;
using _ranges_detail::rend;

using _ranges_detail::make_deferred_iterator;
using _ranges_detail::defer;
using boost::make_iterator_range;
using boost::adaptors::reverse;
using boost::adaptors::reversed;

#endif //ARKANGEL_RANGES_HPP
