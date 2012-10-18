#ifndef YUKI_INOUE_TANGO_ORDERED_LIST_HPP
#define YUKI_INOUE_TANGO_ORDERED_LIST_HPP

#include <list>
#include <algorithm>


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

template <class T>
void addWithOrder(std::list<T> &l, const T &x){
  addWithOrder(l, x, std::less<T>());
}

}

#endif
