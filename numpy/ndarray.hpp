#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <memory>
#include <utility>
#include <initializer_list>
#include <numpy/python.hpp>
#include <numpy/dtype.hpp>
#include <numpy/memory.hpp>
#include <numpy/viewinfo.hpp>
#include <numpy/iterator.hpp>
#include <numpy/utils.hpp>
#include <numpy/typename.hpp>
#include <numpy/ufunc.hpp>
#include <numpy/debug.hpp>

namespace numpy {
  
  template <typename Dtype>
  class ndarray {
    
  public:
    using iterator = array_iter<Dtype>;
    using viewinfo = array_view;
    using dtype = Dtype;
    
    template <typename AnotherDtype> friend class ndarray;
    friend array_iter<Dtype>;
    template <template <class> class UnaryOperation> friend struct ufunc_unary;
    template <template <class, class> class BinaryOperation> friend struct ufunc_binary;
    template <class Type1, class Type2> friend bool may_share_memory(const ndarray<Type1>& a, const ndarray<Type2>& b);
    friend struct debug;

  private:
    std::shared_ptr<shared_memory<Dtype>> memory_ptr;
    ndarray::viewinfo view;
    ndarray::viewinfo view_transpose;
    ndarray<Dtype>* base_ptr;
    
  public:
    
    // accessors
    inline const shape_type& shape() const noexcept {
      return view.shape;
    }
    
    inline const shape_elem_type& shape(axis_type axis) const noexcept(false) {
      if (axis > ndim())
	throw std::out_of_range("IndexError: shape index out of range");
      return view.shape[axis];
    }
    
    inline const size_type& size() const noexcept {
      return view.size;
    }
    
    inline const dim_type& ndim() const noexcept {
      return view.ndim;
    }
    
    // inline const std::type_info& dtype() const{
    //   return typeid(Dtype);
    // }

    inline const ndarray<Dtype> T() const {
      return ndarray<Dtype>(memory_ptr, view_transpose, base_ptr);
    }

    inline const ndarray<Dtype>& base() const noexcept {
      return *base_ptr;
    }
    
    /* Constructors */
    ndarray(): base_ptr(this) {}
    
  private:
    ndarray(const std::shared_ptr<shared_memory<Dtype>>& ptr, const viewinfo& view_, ndarray<Dtype>* base)
      : memory_ptr(ptr), view(view_), view_transpose(view.transpose()), base_ptr(base) {}

    ndarray(const std::shared_ptr<shared_memory<Dtype>>& ptr, viewinfo&& view_, ndarray<Dtype>* base)
      : memory_ptr(ptr), view(view_), view_transpose(view.transpose()), base_ptr(base) {}

    ndarray(shared_memory<Dtype> *ptr, const viewinfo& view_, ndarray<Dtype>* base_)
      : ndarray<Dtype>(std::shared_ptr<shared_memory<Dtype>>(ptr), view_, base_) {}

    ndarray(shared_memory<Dtype> *ptr, viewinfo&& view_, ndarray<Dtype>* base_)
      : ndarray<Dtype>(std::shared_ptr<shared_memory<Dtype>>(ptr), view_, base_) {}

    ndarray(const std::vector<Dtype>& data, const viewinfo& view_)
      : ndarray<Dtype>(new shared_memory<Dtype>(data), view_, this) {}

    ndarray(const std::vector<Dtype>& data, viewinfo&& view_)
      : ndarray<Dtype>(new shared_memory<Dtype>(data), view_, this) {}

  public:
    ndarray(const std::vector<Dtype>& data, const shape_type& shape_)
      : ndarray<Dtype>(data, viewinfo(shape_)) {}
    
    // ndarray(Dtype* first, Dtype* last, const shape_type& shape_)
    //   : ndarray<Dtype>(std::vector<Dtype>(first, last), shape_) {}

    ndarray(Dtype scolar)
      : ndarray<Dtype>(std::vector<Dtype>(1, scolar), shape_type()) {}

    friend void swap(ndarray<Dtype>& first, ndarray<Dtype>& second) {
      using std::swap;
      first.memory_ptr.swap(second.memory_ptr);
      swap(first.view, second.view);
      swap(first.view_transpose, second.view_transpose);
      auto tmp = first.base_ptr;
      first.base_ptr = second.base_ptr;
      second.base_ptr = tmp;
    }

    /**
     * Copy constructor & copy assignment operator
     * -------------------------------------------
     * The copied object should refer to the same memory address, but newly generate its view object.
     */
    
    ndarray(const ndarray<Dtype>& src)
      : ndarray<Dtype>(src.memory_ptr, src.view, src.base_ptr) {}

    /**
     * The elegant implementation of the copy assignment operator with the COPY-AND-SWAP IDIOM
     * cf) https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
     */
    
    // ndarray<Dtype>& operator=(ndarray<Dtype> rhs) {
    //   std::cout << "ndarray::(copy assignment operator) is called" << std::endl;
    //   swap(*this, rhs);
    //   return *this;
    // }

    /**
     * When a move assignment operator is also defined, however, IT WILL CAUSE A COMPLIE ERROR WHICH LOOKS LIKE BELOW:
     *
     * g++ -std=c++17 -O3 -Wall -I .  -o test/test_copy_move test/test_copy_move.cpp
     * test/test_copy_move.cpp:38:5: error: use of overloaded operator '=' is ambiguous (with operand types 'numpy::ndarray<double>' and 'ndarray<double>')
     *   c = b.transpose();
     *   ~ ^ ~~~~~~~~~~~~~
     * ./numpy/ndarray.hpp:104:21: note: candidate function
     *     ndarray<Dtype>& operator=(ndarray<Dtype> rhs) {
     *                     ^
     * ./numpy/ndarray.hpp:117:21: note: candidate function
     *     ndarray<Dtype>& operator=(ndarray<Dtype>&& rhs) noexcept {
     *                     ^
     * 1 error generated.
     * make: *** [test/test_copy_move] Error 1
     */

    /**
     * So, I'm obliged to throw away the graceful fasion above, and instead, choose the usual way of implmentation using a `const T&` parameter.
     */
        
    ndarray<Dtype>& operator=(const ndarray<Dtype>& rhs) & {
      ndarray<Dtype> tmp(rhs);
      swap(*this, tmp);
      return *this;
    }

    // https://cpprefjp.github.io/lang/cpp11/ref_qualifier_for_this.html
    ndarray<Dtype>& operator=(const ndarray<Dtype>& rhs) && {
      auto rhs_view = rhs.view;
      array_view::broadcast(view, rhs_view);
      ndarray<Dtype> rhs_broadcasted(rhs.memory_ptr, rhs_view, rhs.base_ptr);
      
      std::copy(rhs_broadcasted.begin(), rhs_broadcasted.end(), begin());
      // std::copy(rhs.begin(), rhs.end(), begin());
      return *this;
    }
 
    /* Move constructor & move assignment operator */
    
    ndarray(ndarray<Dtype>&& src) noexcept
      : ndarray<Dtype>() {
      swap(*this, src);
    }
      
    ndarray<Dtype>& operator=(ndarray<Dtype>&& rhs) & noexcept {
      swap(*this, rhs);
      return *this;
    }
    
    ndarray<Dtype> reshape(const shape_type& newshape) const {
      return ndarray<Dtype>(memory_ptr, viewinfo(newshape), base_ptr);
    }

    template <class... Integer>
    ndarray<Dtype> reshape(int head, Integer... tail) const {
      return reshape(shape_type({head, tail...}));
    }

    ndarray<Dtype> transpose(const std::vector<dim_type>& axes) const {
      return ndarray<Dtype>(memory_ptr, view.transpose(axes), base_ptr);
    }
    
    ndarray<Dtype> transpose() const {
      return T();
    }

    ndarray<Dtype> ravel() const {
      return reshape(shape_type(1, size()));
    }

    ndarray<Dtype> flatten() const {
      return ravel().copy();
    }
    
    /* operators */

    // https://numpy.org/doc/stable/reference/ufuncs.html
    // -- "Some of these ufuncs are called automatically on arrays when the relevant infix notation is used (e.g., add(a, b) is called internally when a + b is written and a or b is an ndarray)."

    auto operator+() const {
      return positive(*this);
    }
    
    auto operator-() const {
      return negative(*this);
    }
    
    ndarray<Dtype>& operator+=(const ndarray<Dtype>& rhs) {
      return add(*this, rhs, *this);
    }

    ndarray<Dtype>& operator-=(const ndarray<Dtype>& rhs) {
      return subtract(*this, rhs, *this);
    }

    ndarray<Dtype>& operator*=(const ndarray<Dtype>& rhs) {
      return multiply(*this, rhs, *this);
    }

    ndarray<Dtype>& operator/=(const ndarray<Dtype>& rhs) {
      return divide(*this, rhs, *this);
    }
    
    array_iter<Dtype> begin() const {
      return array_iter<Dtype>(this);
    };
    
    array_iter<Dtype> end() const {
      return begin() + view.size;
    }

    // indexing
    template <class... Indexer>
    ndarray<Dtype> operator()(Indexer... indices) const {
      viewinfo newview = view.indexer(indices...);
      return ndarray<Dtype>(memory_ptr, newview, base_ptr);
    }

    // // yields compilation error
    // template <typename... Indexer>
    // auto operator()(Indexer... indices) const
    //   -> typename std::conditional<sizeof...(indices) == view.ndim, Dtype, ndarray<Dtype>>::type {
    //   viewinfo newview = view.indexer(indices...);
    //   auto result = ndarray<Dtype>(memory_ptr, newview, base_ptr);
    //   if constexpr(sizeof...(indices) == view.ndim) {
    // 	return *result.begin();
    //   } else {
    //     return result;
    //   }
    // }

    auto __getindex__(coord_type&& coord) const
      -> decltype(utils::dot(view.stride, coord, view.offset)) {
      if (coord.size() != view.ndim)
      	throw std::invalid_argument("IndexError: too many indices for array: "
      				    "array is " + python::str(view.ndim)
      				    + "-dimensional, but "
      				    + python::str(coord.size()) + " were indexed");

      for(axis_type ax = 0; ax<view.ndim; ax++) {
      	try {
      	  coord[ax] =  python::slice::abs_index(shape(ax), coord[ax]);
      	} catch (const std::out_of_range& e) {
      	  throw std::out_of_range("IndexError: index " + python::str(coord[ax]) +
      				  " is out of bounds for axis " + python::str(ax) +
      				  " with size " + python::str(shape(ax)));
      	}
      }
      auto index = utils::dot(view.stride, coord, view.offset);
      return index;
    }

    Dtype& __getitem__(coord_type&& coord) {
      auto index = __getindex__(std::move(coord));
      return *(memory_ptr->data.begin() + index);
    }

    Dtype __getitem__(coord_type&& coord) const {
      auto index = __getindex__(std::move(coord));
      return *(memory_ptr->data.begin() + index);      
    }
      
    Dtype& operator[](coord_type&& coord) {
      return __getitem__(std::move(coord));
    }

    Dtype operator[](coord_type&& coord) const {
      return __getitem__(std::move(coord));
    }

    Dtype& operator[](intp index) {
      return *(begin() + index);
    }

    Dtype operator[](intp index) const {
      return *(begin() + index);
    }

    template <class AnotherDtype>
    ndarray<AnotherDtype> astype() const {
      // なんでだめ?
      // return ndarray<AnotherDtype>(std::vector<AnotherDtype>(begin(), end()), viewinfo<AnotherDtype>(view));
      std::vector<AnotherDtype> newdata(view.size);
      std::transform(begin(), end(), newdata.begin(), [](Dtype e){return static_cast<AnotherDtype>(e);});
      return ndarray<AnotherDtype>(newdata, viewinfo(view));
    }

    ndarray<Dtype> copy() const {
      // ndarray<Dtype> tmp;
      // std::copy(begin(), end(), std::back_inserter(tmp))
      std::vector<Dtype> tmp(size());
      std::copy(begin(), end(), tmp.begin());
      ndarray<Dtype> copied(tmp, shape());
      return copied;
    }

    const viewinfo& get_view() const {
      return view;
    }

    std::string memory_info() const {
      std::stringstream ss;
      ss << "<shared_memory at ";
      ss << memory_ptr;
      ss << ">";
      return ss.str();
    }

    // methods relevant to Python's built-in functions

    shape_elem_type __len__() const {
      return shape(0);
    }
    
    std::string __repr__() const // override
    {
      return "array(" + _str_rec(',') + ")";
    }

    std::string __str__() const // override
    {
      return _str_rec('\0');
    }
    
    std::string _str_rec(const char delimiter) const {
      if (view.ndim <= 0) 
	return python::str(memory_ptr->data[view.offset]);
      std::stringstream ss;
      ss << "[";
      for (int i=0; i<__len__(); i++) {
	if (i >= 1)
	  ss << "\t";
	ss << operator()(i)._str_rec(delimiter);
	if (i < __len__() - 1) {
	  ss << delimiter;
	  if (view.ndim >= 2)
	    ss << "\n";
	}
      }
      ss << "]";
      return ss.str();
    }

  };


  namespace {
    /**
     * operator+, -, *, /の引数の少なくとも一方がndarrayのときに
     * ufuncが呼び出されるようにしたいので、SFINAEのための準備をする。
     * このくらいならオーバーロードをたくさん用意すればよいだけな気がするが、
     * 勉強の一環としてやっておく。
     **/

    /* Check if the type `T` is an instance of ClassTemplate */
    // https://stackoverflow.com/questions/16337610/how-to-know-if-a-type-is-a-specialization-of-stdvector
    template <class T, template <class...> class ClassTemplate>
    struct is_instance : public std::false_type {};
    // ちょっと自信がないが、これはたぶん「特定の場合に継承元のクラスを変更する」という明示的特殊化。
    template <template <class...> class ClassTemplate, class... Args>
    struct is_instance<ClassTemplate<Args...>, ClassTemplate> : std::true_type {};


    /* Check if at least one of the template arguments is an ndarray */
    template <class Dtype> using is_array = is_instance<Dtype, ndarray>;
    template <class Dtype1, class Dtype2>
    constexpr bool at_least_one_is_array() {
      return is_array<Dtype1>::value or is_array<Dtype2>::value;
    }

    /* Temporary type alias for SFINAE */
    template <class Dtype1, class Dtype2, class ReturnType>
    using __sfinae__ = typename std::enable_if<at_least_one_is_array<Dtype1, Dtype2>(),
					       ReturnType>::type;
  }

  template <class Dtype1, class Dtype2>
  auto operator+(const Dtype1& lhs, const Dtype2& rhs)
    -> __sfinae__<Dtype1, Dtype2, decltype(add(lhs, rhs))> {
    return add(lhs, rhs);
  }
  
  template <class Dtype1, class Dtype2>
  auto operator-(const Dtype1& lhs, const Dtype2& rhs)
    -> __sfinae__<Dtype1, Dtype2, decltype(subtract(lhs, rhs))> {
    return subtract(lhs, rhs);
  }

  template <class Dtype1, class Dtype2>
  auto operator*(const Dtype1& lhs, const Dtype2& rhs)
    -> __sfinae__<Dtype1, Dtype2, decltype(multiply(lhs, rhs))> {
    return multiply(lhs, rhs);
  }

  template <class Dtype1, class Dtype2>
  auto operator/(const Dtype1& lhs, const Dtype2& rhs) 
    -> __sfinae__<Dtype1, Dtype2, decltype(divide(lhs, rhs))> {
    return divide(lhs, rhs);
  }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator+=(ndarray<Dtype1>& lhs, const Dtype2& rhs) {
  //   return add(lhs, rhs, lhs);
  // }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator-=(ndarray<Dtype1>& lhs, const Dtype2& rhs) {
  //   return subtract(lhs, rhs, lhs);
  // }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator*=(ndarray<Dtype1>& lhs, const Dtype2& rhs) {
  //   return multiply(lhs, rhs, lhs);
  // }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator/=(ndarray<Dtype1>& lhs, const Dtype2& rhs) {
  //   return divide(lhs, rhs, lhs);
  // }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator+=(ndarray<Dtype1>&& lhs, const Dtype2& rhs) {
  //   return add(lhs, rhs, lhs);
  // }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator-=(ndarray<Dtype1>&& lhs, const Dtype2& rhs) {
  //   return subtract(lhs, rhs, lhs);
  // }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator*=(ndarray<Dtype1>&& lhs, const Dtype2& rhs) {
  //   return multiply(lhs, rhs, lhs);
  // }

  // template <class Dtype1, class Dtype2>
  // ndarray<Dtype1>& operator/=(ndarray<Dtype1>&& lhs, const Dtype2& rhs) {
  //   return divide(lhs, rhs, lhs);
  // }



  
  template <class Dtype>
  inline bool _may_share_memory_impl(const Dtype* a_begin, const Dtype* a_end,
				     const Dtype* b_begin, const Dtype* b_end) {
    if (a_begin >= b_end)
      return false;
    if (a_end <= b_begin)
      return false;
    return true;
  }

  template <class Dtype1, class Dtype2>
  bool may_share_memory(const ndarray<Dtype1>& a, const ndarray<Dtype2>& b) {
    
    if constexpr (std::is_same<Dtype1, Dtype2>::value) {
  	
      if (a.memory_ptr != b.memory_ptr)
        return false;

      auto a_begin = &(*a.begin());
      auto a_end = &(*a.end());
      auto b_begin = &(*b.begin());
      auto b_end = &(*b.end());

      if (a_begin < a_end) {
        if (b_begin < b_end)
  	return _may_share_memory_impl(a_begin, a_end, b_begin, b_end);
        else
  	return _may_share_memory_impl(a_begin, a_end, b_end, b_begin);
      } else {
        if (b_begin < b_end)
  	return _may_share_memory_impl(a_end, a_begin, b_begin, b_end);
        else
  	return _may_share_memory_impl(a_end, a_begin, b_end, b_begin);
      }
      } else {
    return false;
    }
  }
     
}
