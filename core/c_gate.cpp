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

// includes for this file
#include "c_gate.h"
#include <stdexcept>
#include <cassert>

// using namespace for this project
using namespace Cim;

// constructor
Gate::Gate(const Component* const ParentDevice, const std::string& GateName, const std::string& GateType,
    const std::vector<std::string>& InPinNames, bool isMonitored) : 
  // ID
  name{GateName}, type{GateType},
  // Properties
  monitored{isMonitored}, pParent{const_cast<Component*>(ParentDevice)}, index{UINT32_MAX},
  // IO
  InPins(InPinNames.size()), OutPin() {
  // codes & error-handling
  // 1. check the number of inputs
  const int input_size = InPinNames.size();
  if (GateType == "NOT") {
    if (input_size != 1) {
      throw std::invalid_argument("ERR: NOT GATE MUST ONLY HAVE 1 INPUT! \n");
    }
  } else {
    if (input_size < 2) {
      throw std::invalid_argument("ERR: AT LEAST 2 INPUTS FOR THIS LOGIC GATES! \n");
    }
  }
  // 2. check nullptr
  if (ParentDevice == nullptr) {
    nesting_lvl = 0;
    fullname = GateName;
  } else {
    nesting_lvl = pParent->GetNestingLvl() + 1;
    fullname = pParent->GetFullName() + "::" + GateName;
  }




}


// print output pins' states
void Gate::PrintOutPinStates() noexcept {
  
}
