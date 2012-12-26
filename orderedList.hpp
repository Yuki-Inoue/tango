#ifndef YUKI_INOUE_TANGO_ORDERED_LIST_HPP
#define YUKI_INOUE_TANGO_ORDERED_LIST_HPP

#include <list>
#include <algorithm>
#include <utility>


namespace mystd {

template <class T, class Predicate>
void insertFirst(std::list<T> &l, const T &x, const Predicate &predicate){
  l.insert
    (std::find_if
     (l.begin(), l.end(), predicate), x);
}

template <class T, class Compare>
void addWithOrder(std::list<T> &l, const T &x, const Compare &compare){
  insertFirst(l, x, std::bind1st(compare, x));
}

template <class T, class Compare>
void addWithOrder(std::list<T> &l, T &&x, const Compare &compare){
  auto it = std::find_if
    (l.begin(), l.end(), std::bind1st(compare, x));
  l.insert(it, std::move(x));
}

template <class T, class Arg>
void addWithOrder(std::list<T> &l, Arg &&x){
  addWithOrder(l, std::forward<Arg>(x), std::less<T>());
}

}

#endif
