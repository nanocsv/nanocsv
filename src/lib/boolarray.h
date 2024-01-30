#ifndef BOOLARRAY_H
#define BOOLARRAY_H
#include <cassert>
#include <iostream>
#include <iso646.h>
#include <sstream>
#include <stdarg.h>
#include <stdexcept>
#include <vector>

#include "ewahutil.h"

namespace ewah {
template <class uword = uint32_t> class BoolArray {
public:
  BoolArray(const size_t n, const uword initval = 0)
      : buffer(n / wordinbits + (n % wordinbits == 0 ? 0 : 1), initval),
        sizeinbits(n) {}

  BoolArray() : buffer(), sizeinbits(0) {}

  BoolArray(const BoolArray &ba)
      : buffer(ba.buffer), sizeinbits(ba.sizeinbits) {}
  static BoolArray bitmapOf(size_t n, ...) {
    BoolArray ans;
    va_list vl;
    va_start(vl, n);
    for (size_t i = 0; i < n; i++) {
      ans.set(static_cast<size_t>(va_arg(vl, int)));
    }
    va_end(vl);
    return ans;
  }
  size_t sizeInBytes() const { return buffer.size() * sizeof(uword); }

  void read(std::istream &in) {
    sizeinbits = 0;
    in.read(reinterpret_cast<char *>(&sizeinbits), sizeof(sizeinbits));
    buffer.resize(sizeinbits / wordinbits +
                  (sizeinbits % wordinbits == 0 ? 0 : 1));
    if (buffer.size() == 0)
      return;
    in.read(reinterpret_cast<char *>(&buffer[0]),
            static_cast<std::streamsize>(buffer.size() * sizeof(uword)));
  }

  void readBuffer(std::istream &in, const size_t size) {
    buffer.resize(size);
    sizeinbits = size * sizeof(uword) * 8;
    if (buffer.empty())
      return;
    in.read(reinterpret_cast<char *>(&buffer[0]),
            buffer.size() * sizeof(uword));
  }

  void setSizeInBits(const size_t sizeib) { sizeinbits = sizeib; }

  void write(std::ostream &out) { write(out, sizeinbits); }

  void write(std::ostream &out, const size_t numberofbits) const {
    const size_t size =
        numberofbits / wordinbits + (numberofbits % wordinbits == 0 ? 0 : 1);
    out.write(reinterpret_cast<const char *>(&numberofbits),
              sizeof(numberofbits));
    if (numberofbits == 0)
      return;
    out.write(reinterpret_cast<const char *>(&buffer[0]),
              static_cast<std::streamsize>(size * sizeof(uword)));
  }

  void writeBuffer(std::ostream &out, const size_t numberofbits) const {
    const size_t size =
        numberofbits / wordinbits + (numberofbits % wordinbits == 0 ? 0 : 1);
    if (size == 0)
      return;
#ifdef EWAHASSERT
    assert(buffer.size() >= size);
#endif
    out.write(reinterpret_cast<const char *>(&buffer[0]), size * sizeof(uword));
  }

  size_t sizeOnDisk() const {
    size_t size =
        sizeinbits / wordinbits + (sizeinbits % wordinbits == 0 ? 0 : 1);
    return sizeof(sizeinbits) + size * sizeof(uword);
  }

  BoolArray &operator=(const BoolArray &x) {
    this->buffer = x.buffer;
    this->sizeinbits = x.sizeinbits;
    return *this;
  }

  bool operator==(const BoolArray &x) const {
    if (sizeinbits != x.sizeinbits)
      return false;
    for (size_t k = 0; k < buffer.size(); ++k)
      if (buffer[k] != x.buffer[k])
        return false;
    return true;
  }

  bool operator!=(const BoolArray &x) const { return !operator==(x); }

  void setWord(const size_t pos, const uword val) {
#ifdef EWAHASSERT
    assert(pos < buffer.size());
#endif
    buffer[pos] = val;
  }

  void addWord(const uword val) {
    if (sizeinbits % wordinbits != 0)
      throw std::invalid_argument("you probably didn't want to do this");
    sizeinbits += wordinbits;
    buffer.push_back(val);
  }

  uword getWord(const size_t pos) const {
#ifdef EWAHASSERT
    assert(pos < buffer.size());
#endif
    return buffer[pos];
  }

  void set(const size_t pos) {
    if (pos >= sizeinbits)
      padWithZeroes(pos + 1);
    buffer[pos / wordinbits] |= static_cast<uword>((static_cast<uword>(1) << (pos % wordinbits)));
  }

  void unset(const size_t pos) {
    if (pos < sizeinbits)
      buffer[pos / wordinbits] &=
          ~(static_cast<uword>(1) << (pos % wordinbits));
  }

  bool get(const size_t pos) const {
#ifdef EWAHASSERT
    assert(pos / wordinbits < buffer.size());
#endif
    return (buffer[pos / wordinbits] &
            (static_cast<uword>(1) << (pos % wordinbits))) != 0;
  }

  void reset() {
    if (buffer.size() > 0)
      memset(&buffer[0], 0, sizeof(uword) * buffer.size());
    sizeinbits = 0;
  }

  size_t sizeInBits() const { return sizeinbits; }

  ~BoolArray() {}

  void logicaland(const BoolArray &ba, BoolArray &out) const {
    if (ba.buffer.size() < buffer.size())
      out.setToSize(ba);
    else
      out.setToSize(*this);
    for (size_t i = 0; i < out.buffer.size(); ++i)
      out.buffer[i] = buffer[i] & ba.buffer[i];
  }

  BoolArray logicaland(const BoolArray &a) const {
    BoolArray answer;
    logicaland(a, answer);
    return answer;
  }

  void inplace_logicaland(const BoolArray &ba) {
    if (ba.buffer.size() < buffer.size())
      setToSize(ba);
    for (size_t i = 0; i < buffer.size(); ++i)
      buffer[i] = buffer[i] & ba.buffer[i];
  }

  void logicalandnot(const BoolArray &ba, BoolArray &out) const {
    out.setToSize(*this);
    size_t upto = out.buffer.size() < ba.buffer.size() ? out.buffer.size()
                                                       : ba.buffer.size();
    for (size_t i = 0; i < upto; ++i)
      out.buffer[i] = static_cast<uword>(buffer[i] & (~ba.buffer[i]));
    for (size_t i = upto; i < out.buffer.size(); ++i)
      out.buffer[i] = buffer[i];
    out.clearBogusBits();
  }

  BoolArray logicalandnot(const BoolArray &a) const {
    BoolArray answer;
    logicalandnot(a, answer);
    return answer;
  }

  void inplace_logicalandnot(const BoolArray &ba) {
    size_t upto =
        buffer.size() < ba.buffer.size() ? buffer.size() : ba.buffer.size();
    for (size_t i = 0; i < upto; ++i)
      buffer[i] = buffer[i] & (~ba.buffer[i]);
    clearBogusBits();
  }

  void logicalor(const BoolArray &ba, BoolArray &out) const {
    const BoolArray *smallest;
    const BoolArray *largest;
    if (ba.buffer.size() > buffer.size()) {
      smallest = this;
      largest = &ba;
      out.setToSize(ba);
    } else {
      smallest = &ba;
      largest = this;
      out.setToSize(*this);
    }
    for (size_t i = 0; i < smallest->buffer.size(); ++i)
      out.buffer[i] = buffer[i] | ba.buffer[i];
    for (size_t i = smallest->buffer.size(); i < largest->buffer.size(); ++i)
      out.buffer[i] = largest->buffer[i];
  }

  BoolArray logicalor(const BoolArray &a) const {
    BoolArray answer;
    logicalor(a, answer);
    return answer;
  }

  void inplace_logicalor(const BoolArray &ba) { logicalor(ba, *this); }

  void logicalxor(const BoolArray &ba, BoolArray &out) const {
    const BoolArray *smallest;
    const BoolArray *largest;
    if (ba.buffer.size() > buffer.size()) {
      smallest = this;
      largest = &ba;
      out.setToSize(ba);
    } else {
      smallest = &ba;
      largest = this;
      out.setToSize(*this);
    }
    for (size_t i = 0; i < smallest->buffer.size(); ++i)
      out.buffer[i] = buffer[i] ^ ba.buffer[i];
    for (size_t i = smallest->buffer.size(); i < largest->buffer.size(); ++i)
      out.buffer[i] = largest->buffer[i];
  }

  BoolArray logicalxor(const BoolArray &a) const {
    BoolArray answer;
    logicalxor(a, answer);
    return answer;
  }

  void inplace_logicalxor(const BoolArray &ba) { logicalxor(ba, *this); }

  void logicalnot(BoolArray &out) const {
    out.setToSize(*this);
    for (size_t i = 0; i < buffer.size(); ++i)
      out.buffer[i] = ~buffer[i];
    out.clearBogusBits();
  }

  BoolArray logicalandnot() const {
    BoolArray answer;
    logicalnot(answer);
    return answer;
  }

  void inplace_logicalnot() {
    for (size_t i = 0; i < buffer.size(); ++i)
      buffer[i] = ~buffer[i];
    clearBogusBits();
  }

  size_t numberOfOnes() const {
    size_t count = 0;
    for (size_t i = 0; i < buffer.size(); ++i) {
      count += countOnes(buffer[i]);
    }
    return count;
  }

  inline void printout(std::ostream &o = std::cout) {
    for (size_t k = 0; k < sizeinbits; ++k)
      o << get(k) << " ";
    o << std::endl;
  }

  void makeSameSize(BoolArray &a) {
    if (a.sizeinbits < sizeinbits)
      a.padWithZeroes(sizeinbits);
    else if (sizeinbits < a.sizeinbits)
      padWithZeroes(a.sizeinbits);
  }

  void setToSize(const BoolArray &a) {
    sizeinbits = a.sizeinbits;
    buffer.resize(a.buffer.size());
  }

  size_t padWithZeroes(const size_t totalbits) {
    size_t currentwordsize = (sizeinbits + wordinbits - 1) / wordinbits;
    size_t neededwordsize = (totalbits + wordinbits - 1) / wordinbits;
#ifdef EWAHASSERT
    assert(neededwordsize >= currentwordsize);
#endif
    buffer.resize(neededwordsize);
    sizeinbits = totalbits;
    return static_cast<size_t>(neededwordsize - currentwordsize);
  }

  void append(const BoolArray &a);

  enum { wordinbits = sizeof(uword) * 8 };

  std::vector<size_t> toArray() const {
    std::vector<size_t> ans;
    for (size_t k = 0; k < buffer.size(); ++k) {
      uword myword = buffer[k];
      while (myword != 0) {
        uint32_t ntz = numberOfTrailingZeros(myword);
        ans.push_back(sizeof(uword) * 8 * k + ntz);
        myword ^= (static_cast<uword>(1) << ntz);
      }
    }
    return ans;
  }

  operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }

  friend std::ostream &operator<<(std::ostream &out, const BoolArray &a) {
    std::vector<size_t> v = a.toArray();
    out << "{";
    for (std::vector<size_t>::const_iterator i = v.begin(); i != v.end();) {
      out << *i;
      ++i;
      if (i != v.end())
        out << ",";
    }
    out << "}";
    return out;

    return (out << static_cast<std::string>(a));
  }

private:
  void clearBogusBits() {
    if ((sizeinbits % wordinbits) != 0) {
      const uword maskbogus =
          static_cast<uword>((static_cast<uword>(1) << (sizeinbits % wordinbits)) - 1);
      buffer[buffer.size() - 1] &= maskbogus;
    }
  }

  std::vector<uword> buffer;
  size_t sizeinbits;
};

template <class uword>
void fast_logicalor_tocontainer(size_t n, const BoolArray<uword> **inputs,
                                BoolArray<uword> &container) {
  if (n == 0) {
    container.reset();
    return;
  }
  container = *inputs[0];
  for (size_t i = 0; i < n; i++) {
    container.inplace_logicalor(*inputs[i]);
  }
}

template <class uword>
BoolArray<uword> fast_logicalor(size_t n, const BoolArray<uword> **inputs) {
  BoolArray<uword> answer;
  fast_logicalor_tocontainer(n, inputs, answer);
  return answer;
}

template <class uword> void BoolArray<uword>::append(const BoolArray &a) {
  if (sizeinbits % wordinbits == 0) {
    buffer.insert(buffer.end(), a.buffer.begin(), a.buffer.end());
  } else {
    throw std::invalid_argument(
        "Cannot append if parent does not meet boundary");
  }
  sizeinbits += a.sizeinbits;
}
}
#endif
