#pragma once
#include <inttypes.h>
#include <stdlib.h>

template <uint8_t Size, typename T> class PtrArray {
  T* data_[Size];

public:
  PtrArray() { memset(data_, 0, sizeof(data_)); }

  void replace(T *oldValue, T *newValue) {
    for (uint8_t i = 0; i < Size; ++i) {
      if (data_[i] == oldValue) {
        data_[i] = newValue;
        return;
      }
    }
  }

  inline void append(T *newValue) {
    replace(static_cast<T*>(NULL), newValue);
  }

  void insertOrReplace(T *newValue) {
    for (uint8_t i = 0; i < Size; ++i) {
      if (data_[i] == newValue) {
        return;
      }
    }
    append(newValue);
  }

  /** Apply functor to each non-null element.
   * Func is some type which can be called as func(T *ptr)
   */
  template <typename Func> void apply(Func &&func) {
    for (uint8_t i = 0; i < Size && data_[i] != NULL; ++i) {
      func(data_[i]);
    }
  }
};
