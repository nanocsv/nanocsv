#ifndef EWAH_H
#define EWAH_H

#include <algorithm>
#include <queue>
#include <vector>

#include "boolarray.h"
#include "ewahutil.h"

#include "runninglengthword.h"

namespace ewah {
template <class uword> class EWAHBoolArrayIterator;
template <class uword> class EWAHBoolArraySetBitForwardIterator;
class BitmapStatistics;
template <class uword> class EWAHBoolArrayRawIterator;
template <class uword = uint32_t> class EWAHBoolArray {
public:
  EWAHBoolArray() : buffer(1, 0), sizeinbits(0), lastRLW(0) {}

  static EWAHBoolArray bitmapOf(size_t n, ...) {
    EWAHBoolArray ans;
    va_list vl;
    va_start(vl, n);
    for (size_t i = 0; i < n; i++) {
      ans.set(static_cast<size_t>(va_arg(vl, int)));
    }
    va_end(vl);
    return ans;
  }

  void trim() { buffer.shrink_to_fit(); }

  bool get(const size_t pos) const {
    if (pos >= static_cast<size_t>(sizeinbits))
      return false;
    const size_t wordpos = pos / wordinbits;
    size_t WordChecked = 0;
    EWAHBoolArrayRawIterator<uword> j = raw_iterator();
    while (j.hasNext()) {
      BufferedRunningLengthWord<uword> &rle = j.next();
      WordChecked += static_cast<size_t>(rle.getRunningLength());
      if (wordpos < WordChecked)
        return rle.getRunningBit();
      if (wordpos < WordChecked + rle.getNumberOfLiteralWords()) {
        const uword w = j.dirtyWords()[wordpos - WordChecked];
        return (w & (static_cast<uword>(1) << (pos % wordinbits))) != 0;
      }
      WordChecked += static_cast<size_t>(rle.getNumberOfLiteralWords());
    }
    return false;
  }

  bool empty() const {
    size_t pointer(0);
    while (pointer < buffer.size()) {
      ConstRunningLengthWord<uword> rlw(buffer[pointer]);
      if (rlw.getRunningBit()) {
        if (rlw.getRunningLength() > 0)
          return false;
      }
      ++pointer;
      for (size_t k = 0; k < rlw.getNumberOfLiteralWords(); ++k) {
        if (buffer[pointer] != 0)
          return false;
        ++pointer;
      }
    }
    return true;
  }

  bool set(size_t i);

  operator std::string() const {
    std::stringstream ss;
    ss << *this;
    return ss.str();
  }
  friend std::ostream &operator<<(std::ostream &out, const EWAHBoolArray &a) {
    out << "{";
    for (EWAHBoolArray::const_iterator i = a.begin(); i != a.end();) {
      out << *i;
      ++i;
      if (i != a.end())
        out << ",";
    }
    out << "}";
    return out;
  }

  void makeSameSize(EWAHBoolArray &a) {
    if (a.sizeinbits < sizeinbits)
      a.padWithZeroes(sizeinbits);
    else if (sizeinbits < a.sizeinbits)
      padWithZeroes(a.sizeinbits);
  }

  enum { RESERVEMEMORY = true }; // for speed

  typedef EWAHBoolArraySetBitForwardIterator<uword> const_iterator;

  const_iterator begin() const {
    return EWAHBoolArraySetBitForwardIterator<uword>(&buffer);
  }

  const_iterator &end() const {
    return EWAHBoolArraySetBitForwardIterator<uword>::end();
  }

  std::vector<size_t> toArray() const;

  void logicaland(const EWAHBoolArray &a, EWAHBoolArray &container) const;

  EWAHBoolArray logicaland(const EWAHBoolArray &a) const {
    EWAHBoolArray answer;
    logicaland(a, answer);
    return answer;
  }

  EWAHBoolArray operator&(const EWAHBoolArray &a) const {
    return logicaland(a);
  }

  void logicalandnot(const EWAHBoolArray &a, EWAHBoolArray &container) const;

  EWAHBoolArray operator-(const EWAHBoolArray &a) const {
    return logicalandnot(a);
  }

  EWAHBoolArray logicalandnot(const EWAHBoolArray &a) const {
    EWAHBoolArray answer;
    logicalandnot(a, answer);
    return answer;
  }

  bool intersects(const EWAHBoolArray &a) const;

  void logicalor(const EWAHBoolArray &a, EWAHBoolArray &container) const;

  size_t logicalorcount(const EWAHBoolArray &a) const;

  size_t logicalandcount(const EWAHBoolArray &a) const;

  size_t logicalandnotcount(const EWAHBoolArray &a) const;

  size_t logicalxorcount(const EWAHBoolArray &a) const;

  EWAHBoolArray logicalor(const EWAHBoolArray &a) const {
    EWAHBoolArray answer;
    logicalor(a, answer);
    return answer;
  }

  EWAHBoolArray operator|(const EWAHBoolArray &a) const { return logicalor(a); }

  void logicalxor(const EWAHBoolArray &a, EWAHBoolArray &container) const;

  EWAHBoolArray logicalxor(const EWAHBoolArray &a) const {
    EWAHBoolArray answer;
    logicalxor(a, answer);
    return answer;
  }

  EWAHBoolArray operator^(const EWAHBoolArray &a) const {
    return logicalxor(a);
  }

  void reset() {
    buffer.clear();
    buffer.push_back(0);
    sizeinbits = 0;
    lastRLW = 0;
  }

  inline size_t addWord(const uword newdata,
                        const uint32_t bitsthatmatter = 8 * sizeof(uword));

  inline void printout(std::ostream &o = std::cout) {
    toBoolArray().printout(o);
  }

  void debugprintout() const;

  inline size_t sizeInBits() const { return sizeinbits; }

  inline size_t sizeInBytes() const { return buffer.size() * sizeof(uword); }

  size_t addStreamOfEmptyWords(const bool v, size_t number);

  size_t addStreamOfDirtyWords(const uword *v, const size_t number);

  size_t addStreamOfNegatedDirtyWords(const uword *v, const size_t number);

  size_t padWithZeroes(const size_t totalbits);

  size_t sizeOnDisk(const bool savesizeinbits = true) const;

  size_t write(std::ostream &out, const bool savesizeinbits = true) const;

  size_t write(char *out, size_t capacity,
               const bool savesizeinbits = true) const;

  void writeBuffer(std::ostream &out) const;

  size_t bufferSize() const { return buffer.size(); }

  size_t read(std::istream &in, const bool savesizeinbits = true);

  size_t read(const char *in, size_t capacity,
              const bool savesizeinbits = true);

  void readBuffer(std::istream &in, const size_t buffersize);

  bool operator==(const EWAHBoolArray &x) const;

  bool operator!=(const EWAHBoolArray &x) const;

  bool operator==(const BoolArray<uword> &x) const;

  bool operator!=(const BoolArray<uword> &x) const;

  EWAHBoolArrayIterator<uword> uncompress() const;

  EWAHBoolArrayRawIterator<uword> raw_iterator() const;

  void append(const EWAHBoolArray &x);

  BitmapStatistics computeStatistics() const;

  BoolArray<uword> toBoolArray() const;

  template <class container>
  void appendSetBits(container &out, const size_t offset = 0) const;

  std::vector<size_t> toVector() const { return toArray(); }

  size_t numberOfOnes() const;

  void swap(EWAHBoolArray &x);

  const std::vector<uword> &getBuffer() const { return buffer; }

  enum { wordinbits = sizeof(uword) * 8 };

  EWAHBoolArray(const EWAHBoolArray &other)
      : buffer(other.buffer), sizeinbits(other.sizeinbits),
        lastRLW(other.lastRLW) {}

  EWAHBoolArray &operator=(const EWAHBoolArray &x) {
    buffer = x.buffer;
    sizeinbits = x.sizeinbits;
    lastRLW = x.lastRLW;
    return *this;
  }

  EWAHBoolArray(EWAHBoolArray &&other)
      : buffer(std::move(other.buffer)), sizeinbits(other.sizeinbits),
        lastRLW(other.lastRLW) {}

  EWAHBoolArray &operator=(EWAHBoolArray &&x) {
    buffer = std::move(x.buffer);
    sizeinbits = x.sizeinbits;
    lastRLW = x.lastRLW;
    return *this;
  }

  void expensive_copy(const EWAHBoolArray &x) {
    buffer = x.buffer;
    sizeinbits = x.sizeinbits;
    lastRLW = x.lastRLW;
  }

  void logicalnot(EWAHBoolArray &x) const;

  EWAHBoolArray<uword> logicalnot() const {
    EWAHBoolArray answer;
    logicalnot(answer);
    return answer;
  }

  void inplace_logicalnot();

  inline void setSizeInBits(const size_t size) { sizeinbits = size; }

  inline void fastaddStreamOfEmptyWords(const bool v, size_t number);

  inline void fastaddStreamOfDirtyWords(const uword *v, const size_t number);

private:
  void assertWordCount(std::string message) const;
  void correctWordCount();
  size_t numberOfWords() const;
  inline size_t addLiteralWord(const uword newdata);
  size_t addEmptyWord(const bool v);

  std::vector<uword> buffer;
  size_t sizeinbits;
  size_t lastRLW;
};
}
#include "ewah-inl.h"

#endif
