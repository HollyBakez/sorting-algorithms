///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the
// alternating disks problem.
//
// As provided, this header has four functions marked with
// TODO comments.
// You need to write in your own implementation of these
// functions.
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  //constructor to build row of disks with alternating light/dark disks
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(dark_count() > 0);

      for (size_t i = 1; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  size_t total_count() const {
    return _colors.size();
  }

  size_t dark_count() const {
    return total_count() / 2;
  }

  size_t light_count() const {
    return dark_count();
  }

  bool is_index(size_t i) const {
    return (i < total_count());
  }

  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  //swaps the given disk with the disk to the right
  void swap(size_t left_index) {
    assert(is_index(left_index));
    auto right_index = left_index + 1;
    assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is light, the second disk at index 1
  // is dark, and so on for the entire row of disks.
  bool is_alternating() const {
    // B/C of definition at top, return false if the first disk is dark
    if (_colors[0] == DISK_DARK) return false;

    for (int i = 0; i < _colors.size() - 1; ++i) {
    // to make sure it doesn't go out of boundary 
      if(i+1 != _colors.size()){
      if (_colors[i] == DISK_LIGHT && _colors[i+1] == DISK_LIGHT)
        return false;
      else if (_colors[i] == DISK_DARK && _colors[i+1] == DISK_DARK)
        return false;
    }
    }
    return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
    // Checks the light disks on the left side
    // If all light disks are accounted, then disks are sorted
    for(int i = 0; i < light_count(); i++){
      // if not all light disks are counted then it is not sorted
      if(_colors[i] != DISK_LIGHT){
        return false;
      }
    }
   return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
  // check that the input is in alternating format
  assert(before.is_alternating());

  disk_state temp = before;

  unsigned count = 0;
  for (int i = 0; i < temp.light_count(); i++) {
    for (int i = 0; i < temp.total_count() - 1; ++i) {
      if (temp.get(i) == DISK_DARK && temp.get(i+1) == DISK_LIGHT) {
        temp.swap(i);
        count++;
      }
    }
  }

  return sorted_disks(temp, count);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
  // check that the input is in alternating format
  assert(before.is_alternating());
  //Return after_ state along with the swap count
  // creating a disk state for after sorting by lawnmower
  disk_state after = before;
  int swap_count_after = 0;
 // The lawnmower method to swap is repeated n/2 times
  for(int i = 0; i < after.light_count()/2; i++){
  // swaps disks from left to right
    for(int i = 0; i < after.total_count()-1; i++){
      // n = 8
      // 0, 1, 2 ,3 ,4, 5, 6,7 
      // if i = 7, and i +1 == after.total_count() which is 8 then we do not run this block of code
      if(i+1 != after.total_count()){
        if(after.get(i) == DISK_DARK && after.get(i+1) == DISK_LIGHT){
          after.swap(i);
          swap_count_after++;
        }
      }
    }
  // swaps disks from right to left
    for(int i = after.total_count()-2; i > 0; i--){

      if(after.get(i) == DISK_DARK && after.get(i+1) == DISK_LIGHT){
        after.swap(i);
        swap_count_after++;
      }

    }
  }
  
  return sorted_disks(after, swap_count_after);
}
