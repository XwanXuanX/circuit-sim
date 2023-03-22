// The MIT License (MIT)

// Copyright (c) 2023 Wanxuan Li

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

// include guard for this header
#ifndef C_STRUCTS_H
#define C_STRUCTS_H

// includes for this header
#include <string>   // std::string
#include <cstdint>  // standard int types
#include <utility>  // std::pair
#include <vector>   // std::vector
#include <cassert>  // assert()

// namespace for entire project
namespace Cim {

// forward declaration of classes
class Component;
class Gate;

// define data structures
struct Pin {
  // define two directions of the pin
  enum class Dir : uint8_t {
    Input = 0, 
    Output
  };

  // link is the connection of one pin to any other pin
  typedef std::pair<Component*, uint32_t> Link;

  std::string   name;           // name of the pin
  uint32_t      index;          // local index of pin in device
  bool          state;          // the CURRENT state of pin
  bool          state_changed;  // has the pin state changed
  Dir           direction;      // IN / OUT

  // Pin -> which component's which pin index?
  // take into consideration for multiple input connect to one output and vice versa
  std::vector<Link> connections;
  
  // constructor of Pin struct
  inline Pin(const std::string& pin_name, const uint32_t pin_idx, Dir pin_dir) :
    name{pin_name},
    index{pin_idx}, 
    state{false},
    state_changed{false},
    direction{pin_dir},
    connections(1) {
    // default initialize the size of connections is one
    assert(connections.size() == 1 && "ERR: SIZE RESERVED FAILED! \n");
    connections.at(0) = std::make_pair<Component*, uint32_t>(nullptr, UINT32_MAX);
  }
};

}

#endif  // C_STRUCTS_H
