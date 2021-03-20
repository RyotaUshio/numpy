#pragma once

namespace python {

  class range_iterator;
  
  class range {

    using iterator = range_iterator;
  public:
    int start;
    int stop;
    int step;

  public:
    constexpr range(int start, int stop, int step=1);
    constexpr explicit range(int stop);
    iterator begin() const;
    iterator end() const;
    std::string __repr__() const;
    std::string __str__() const;
    
  };

  class range_iterator {

    friend range;
    
    range range_obj;
    int index;
    
    range_iterator(const range& range_obj_);

  public:
    int operator*() const;
    range_iterator& operator++();
    bool operator!=(const range_iterator& rhs) const;
    
  };


  
  constexpr range::range(int start, int stop, int step)
    : start(start), stop(stop), step(step) {}
    
  constexpr range::range(int stop)
    : range(0, stop, 1) {}

  range::iterator range::begin() const {
    return range::iterator(*this);
  }

  range::iterator range::end() const {
    return range::iterator(range(stop, stop, step));
  }

  std::string range::__repr__() const {
    std::stringstream ss;
    ss << "range(";
    ss << start << ", " << stop;
    if (step != 1)
      ss << ", " << step;
    ss << ")";
    return ss.str();
  }

  std::string range::__str__() const {
    return __repr__();
  }

  range_iterator::range_iterator(const range& range_obj_)
    : range_obj(range_obj_), index(range_obj.start) {}

  int range_iterator::operator*() const {
    return index;
  }

  range_iterator& range_iterator::operator++() {
    index += range_obj.step;
    return *this;
  }

  bool range_iterator::operator!=(const range_iterator& rhs) const {
    return (index != rhs.index);
  }
  
}
