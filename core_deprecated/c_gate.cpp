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
#include <algorithm>  // std::sort()

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
  Pins() {
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
  // initialize input and output Pins
  Pins.reserve(InPinNames.size() + 1);
  // 1. INPUTS
  uint32_t cur_pin_index = 0;
  for (const std::string& each_name : InPinNames) {
    Pins.emplace_back(Pin{each_name, cur_pin_index, Pin::Dir::Input});
    cur_pin_index++;
  }
  // 2. OUTPUT
  Pins.emplace_back(Pin{"out", cur_pin_index, Pin::Dir::Output});
}

// initialize components to initial state
void Gate::Initialize() {
  // Methodology: 3 cases -
  //  - input of the component connected to parent component's input pin      -> treat as 1
  //  - input of the component is not connected (dangling connection)         -> treat as 0
  //  - input of component is connected to other component in the same device -> fetch the state of that pin
  // Assume all inputs are zero

  

}


// return component full name
std::string Gate::GetFullName() const noexcept {
  return this->fullname;
}

// return nesting level
uint32_t Gate::GetNestingLvl() const noexcept {
  return this->nesting_lvl;
}

// return component type
std::string Gate::GetType() const noexcept {
  return this->type;
}

// return component name
std::string Gate::GetName() const noexcept {
  return this->name;
}

// get inpins and outpins number
std::pair<uint32_t, uint32_t> Gate::GetIONum() const noexcept {
  return std::make_pair<uint32_t, uint32_t>(Pins.size() - 1, 1);
}

// return true if component is a device
bool Gate::IsDevice() const noexcept {
  return false;
}

// return true if component is monitored
bool Gate::IsMonitored() const noexcept {
  return this->monitored;
}

// set local index
void Gate::SetLocalIndex(const uint32_t new_idx) noexcept {
  this->index = new_idx;
}

// return local index
uint32_t Gate::GetLocalIndex() const noexcept {
  return this->index;
}

// return the pointer to parent device
const Component* const Gate::GetParentDevice() const noexcept {
  return this->pParent;
}

// return pin state
bool Gate::GetPinState(const uint32_t pin_idx) const {
  return this->Pins.at(pin_idx).state;
}

// return if state changed after the last evaluation
bool Gate::IsPinStateChanged(const uint32_t pin_idx) const {
  return this->Pins.at(pin_idx).state_changed;
}

// return pin name
std::string Gate::GetPinName(const uint32_t pin_idx) const {
  return this->Pins.at(pin_idx).name;
}

// return whether a pin exist
bool Gate::DoesPinExist(const std::string& pin_name) const {
  return search(pin_name) != nullptr;
}

// return pin index
uint32_t Gate::GetPinIndex(const std::string& pin_name) const {
  Pin* const pin = search(pin_name);
  if (pin == nullptr) {
    throw std::invalid_argument("ERR: PIN DOES NOT EXIST! \n");
    return UINT32_MAX;
  }
  return pin->index;
}

// check if pin is connected
bool Gate::IsPinConnected(const uint32_t pin_idx) const {
  const auto& connections = this->Pins.at(pin_idx).connections;
  // might be multiple connections, check all
  bool is_connected = false;
  for (const auto& each_connection : connections) {
    // sanity check
    assert(((each_connection.first == nullptr && each_connection.second == UINT32_MAX) || 
            (each_connection.first != nullptr && each_connection.second != UINT32_MAX)) &&
            "ERR: CONNECTION.FIRST AND CONNECTION.SECOND NOT CONSISTENT! \n");
    if (each_connection.first != nullptr) {
      is_connected = true;
      break;
    }
  }
  return is_connected;
}

// check if pin is connected
bool Gate::IsPinConnected(const std::string& pin_name) const {
  Pin* const pin = search(pin_name);
  if (pin == nullptr) {
    throw std::invalid_argument("ERR: PIN DOES NOT EXIST! \n");
    return false;
  }
  const auto& connections = pin->connections;
  bool is_connected = false;
  for (const auto& each_connection : connections) {
    // sanity check
    assert(((each_connection.first == nullptr && each_connection.second == UINT32_MAX) || 
            (each_connection.first != nullptr && each_connection.second != UINT32_MAX)) &&
            "ERR: CONNECTION.FIRST AND CONNECTION.SECOND NOT CONSISTENT! \n");
    if (each_connection.first != nullptr) {
      is_connected = true;
      break;
    }
  }
  return is_connected;
}

// return & modify the connection of that pin
std::vector<Pin::Link>& Gate::GetPinConnection(const uint32_t pin_idx) {
  return this->Pins.at(pin_idx).connections;
}

// return & modify the connection of that pin
std::vector<Pin::Link>& Gate::GetPinConnection(const std::string& pin_name) {
  Pin* const pin = search(pin_name);
  if (pin == nullptr) {
    throw std::invalid_argument("ERR: PIN DOES NOT EXIST! \n");
  }
  return pin->connections;
}

// return the pin's direction
Pin::Dir Gate::GetPinDirection(const uint32_t pin_idx) const {
  return this->Pins.at(pin_idx).direction;
}

// return the pin's direction
Pin::Dir Gate::GetPinDirection(const std::string& pin_name) const {
  Pin* const pin = search(pin_name);
  if (pin == nullptr) {
    throw std::invalid_argument("ERR: PIN DOES NOT EXIST! \n");
    return static_cast<Pin::Dir>(-1);
  }
  return pin->direction;
}

// return list of output pins
const std::vector<Pin> Gate::GetOutPins() const {
  assert(Pins.rbegin()->direction == Pin::Dir::Output);
  return std::vector<Pin>{*this->Pins.rbegin()};
}

// return list of input pins
const std::vector<Pin> Gate::GetInPins() const {
  std::vector<Pin> in_pins;
  in_pins.reserve(this->Pins.size() - 1);
  for (uint32_t i = 0; i < Pins.size() - 1; i++) {
    if (Pins.at(i).direction != Pin::Dir::Input) {
      throw std::logic_error("ERR: OUTPUT PIN MISPLACED! CHECK OUTPUT PIN! \n");
    } else {
      in_pins.push_back(Pins.at(i));
    }
  }
  assert(in_pins.size() == this->Pins.size() - 1);
  return in_pins;
}

// search for a pin by name
Pin* const Gate::search(const std::string& pin_name) const {
  for (uint32_t i = 0; i < Pins.size(); i++) {
    if (Pins[i].name == pin_name) {
      return const_cast<Pin* const>(&Pins[i]);
    }
  }
  // if not found -> nullptr
  return nullptr;
}

// print input pins' states
void Gate::PrintInPinStates() const noexcept {
  // print component full name
  std::cout << "[ " << GetFullName() << " ] \n";
  // print each pin state
  for (const Pin& each_pin : this->Pins) {
    if (each_pin.direction != Pin::Dir::Input) {
      continue;
    }
    std::cout << "> " << each_pin.name << ": ";
    if (each_pin.state) {
      std::cout << "T \n";
    } else {
      std::cout << "F \n";
    }
  }
  std::cout << '\n';
}

// print output pins' states
void Gate::PrintOutPinStates() const noexcept {
  std::cout << "[ " << GetFullName() << " ] \n";
  for (const Pin& each_pin : this->Pins) {
    if (each_pin.direction != Pin::Dir::Output) {
      continue;
    }
    std::cout << "> " << each_pin.name << ": ";
    if (each_pin.state) {
      std::cout << "T \n";
    } else {
      std::cout << "F \n";
    }
  }
  std::cout << '\n';
}

// Set a pin to new state and update the state_changed flag
void Gate::Set(const uint32_t pin_idx, const bool new_state) {
  // update the state_changed flag
  if (Pins.at(pin_idx).state != new_state) {
    Pins.at(pin_idx).state_changed = true;
  } else {
    Pins.at(pin_idx).state_changed = false;
  }
  this->Pins.at(pin_idx).state = new_state;
}

// Set a pin to new state and update the state_changed flag
void Gate::Set(const std::string& pin_name, const bool new_state) {
  Pin* const pin = search(pin_name);
  if (pin == nullptr) {
    throw std::invalid_argument("ERR: PIN DOES NOT EXIST! \n");
  }
  // update the state_changed flag
  if (pin->state != new_state) {
    pin->state_changed = true;
  } else {
    pin->state_changed = false;
  }
  pin->state = new_state;
}

// evaluation of each type of gate
bool Gate::AND(const std::vector<Pin>& pins) {
  int assert_counter = 0;
  bool output = true;
  for (const Pin& each_pin : pins) {
    if (each_pin.direction == Pin::Dir::Input) {
      output &= each_pin.state;
      assert_counter++;
    }
  }
  assert(assert_counter == pins.size() - 1);
  return output;
} // AND

bool Gate::OR(const std::vector<Pin>& pins) {
  int assert_counter = 0;
  bool output = false;
  for (const Pin& each_pin : pins) {
    if (each_pin.direction == Pin::Dir::Input) {
      output |= each_pin.state;
      assert_counter++;
    }
  }
  assert(assert_counter == pins.size() - 1);
  return output;
} // OR

bool Gate::NOT(const std::vector<Pin>& pins) {
  int assert_counter = 0;
  bool output = false;
  for (const Pin& each_pin : pins) {
    if (each_pin.direction == Pin::Dir::Input) {
      output = !each_pin.state;
      assert_counter++;
    }
  }
  assert(assert_counter == 1 && pins.size() == 2);
  return output;
} // NOT

bool Gate::NAND(const std::vector<Pin>& pins) {
  return !AND(pins);
} // NAND

bool Gate::NOR(const std::vector<Pin>& pins) {
  return !OR(pins);
} // NOR

// Special Note for XOR:
// For 3 or more inputs, the XOR gate has a value of 1, when there is an odd number of
// 1's in the inputs, otherwise, it is a 0.
// This also applies when there are only two inputs.
bool Gate::XOR(const std::vector<Pin>& pins) {
  int input_counter = 0;
  int num_of_1 = 0;
  for (const Pin& each_pin : pins) {
    if (each_pin.direction != Pin::Dir::Input) {
      continue;
    }
    input_counter++;
    if (each_pin.state == true) {
      num_of_1++;
    }
  }
  assert(input_counter == pins.size() - 1);
  return (num_of_1 % 2 != 0) ? true : false;
} // XOR

bool Gate::XNOR(const std::vector<Pin>& pins) {
  return !XOR(pins);
} // XNOR
