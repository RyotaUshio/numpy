#pragma once
#include <string>
#include <sstream>

namespace python {

  class object {
    
  public:
    
    virtual std::string __repr__() const {
      std::stringstream ss;
      ss << "<object at" << this << ">";
      return ss.str();
    }
    
    virtual std::string __str__() const {
      return __repr__();
    }

    virtual operator std::string() const final {
      return __str__();
    }
    
  };
  
}
