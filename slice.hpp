#pragma once

#include <string>
#include <regex>
#include <limits>
#include <stdexcept>

namespace numpy {
  
  class slice {
    static std::string _r_int, _r_colon_int;
    static std::regex re;
  
    const int m_start;
    const int m_stop;
    const int m_step;

  public:
    slice(int start, int stop, int step)
      : m_start(start), m_stop(stop), m_step(step) {}
    slice(int idx)
      : slice(idx, idx+1, 0) {}
    slice(std::string str) { // ":"とか"1:-1"とか
      std::smatch m;
      if (std::regex_match(str, m, re)) {
	m_start = (m[1].str().empty()) ? 0 : std::stoi(m[1]);
	m_stop = (m[3].str().empty()) ? std::numeric_limits<int>::max() : std::stoi(m[3]);
	m_step = (m[5].str().empty()) ? 1 : std::stoi(m[5]);
      }
      else
	throw std::invalid_argument("Broken slice \"" + str + "\"");
    }
    slice(const char* str)
      : slice(std::string(str)) {}

    int size(const std::size_t length) const {
      return (stop(length) - start(length)) / step;
    }

    int start(const std::size_t length) const {
      return (m_start < 0) ? length + m_start : m_start;
    }

    int stop(const std::size_t length) const {
      return (m_stop < 0) ? length + m_stop : m_stop;
    }

    int step() const {
      return m_step;
    }
  };

  std::string slice::_r_int = "[+-]?[1-9]\\d*|[+-]?0";
  std::string slice::_r_colon_int = ":(" + slice::_r_int + ")?";
  std::regex slice::re("(" + slice::_r_int + ")?(" + slice::_r_colon_int + ")?(" + slice::_r_colon_int + ")?");
  
}
