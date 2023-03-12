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
#ifndef C_GATE_H
#define C_GATE_H

// includes for this header
#include <iostream>       // std::cout, std::endl
#include "c_component.h"  // class Component
#include "c_structs.h"    // Pin, forward declaration

// namespace for entire project
namespace Cim {

// Gate Class (Derived class from Component Interface)
// Gate is the most basic component -> building block for other devices
template <uint32_t IN, uint32_t OUT>
class Gate : public Component {
 public:

 private:
  // IDs
  std::string name;     // gate name
  std::string fullname; // gate fullname = name + parent name
  std::string type;     // gate type

  // Property
  bool monitored;       // if output of gate is monitored
  Component* pParent;   // parent device
  uint32_t index;       // index in local device
  uint32_t nesting_lvl; // level of nesting

  // I/O
  const uint32_t InPinsNum = IN;    // number of input pins
  const uint32_t OutPinsNum = OUT;  // number of output pins
  std::vector<Pin> InPins;          // input pin list
  std::vector<Pin> OutPins;         // output pin list
};

}

#endif  // C_GATE_H
