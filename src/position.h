/* Copyright 2017-2018 ccls Authors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#pragma once

#include "maybe.h"
#include "utils.h"

#include <stdint.h>
#include <string>

struct Position {
  int16_t line = -1;
  int16_t column = -1;

  static Position FromString(const std::string &encoded);

  bool Valid() const { return line >= 0; }
  std::string ToString();

  // Compare two Positions and check if they are equal. Ignores the value of
  // |interesting|.
  bool operator==(const Position &o) const {
    return line == o.line && column == o.column;
  }
  bool operator<(const Position &o) const {
    if (line != o.line)
      return line < o.line;
    return column < o.column;
  }
  bool operator<=(const Position &o) const { return !(o < *this); }
};
MAKE_HASHABLE(Position, t.line, t.column);

struct Range {
  Position start;
  Position end;

  static Range FromString(const std::string &encoded);

  bool Valid() const { return start.Valid(); }
  bool Contains(int line, int column) const;
  Range RemovePrefix(Position position) const;

  std::string ToString();

  bool operator==(const Range &o) const {
    return start == o.start && end == o.end;
  }
  bool operator<(const Range &o) const {
    return !(start == o.start) ? start < o.start : end < o.end;
  }
};

namespace std {
template <> struct hash<Range> {
  std::size_t operator()(Range x) const {
    union U {
      Range range = {};
      uint64_t u64;
    } u;
    static_assert(sizeof(Range) == 8);
    u.range = x;
    return hash<uint64_t>()(u.u64);
  }
};
} // namespace std

// Reflection
class Reader;
class Writer;
void Reflect(Reader &visitor, Position &value);
void Reflect(Writer &visitor, Position &value);
void Reflect(Reader &visitor, Range &value);
void Reflect(Writer &visitor, Range &value);
