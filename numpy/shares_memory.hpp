namespace numpy {

  template <class Dtype>
  bool may_share_memory(const ndarray<Dtype>& a, const ndarray<Dtype>& b) {
    // This implementation must be improved
    return (&a.base == &b.base);
  }
  
}
