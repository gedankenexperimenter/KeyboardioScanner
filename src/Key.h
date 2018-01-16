// -*- c++ -*-

#pragma once

namespace kaleidoscope {
namespace hardware {

// This is a terrible name; probably all these names should be redone with more
// descriptive versions, but at least the namespace separates this from
// kaleidoscope::Key...

// Same datastructure as on the other side ­ other side of what?
union Key {
  struct {
    uint8_t row: 2,
            col: 3,
            keyState: 1,
            keyEventsWaiting: 1,
            eventReported: 1;
  };
  uint8_t val;
};

} // namespace hardware {
} // namespace kaleidoscope {
