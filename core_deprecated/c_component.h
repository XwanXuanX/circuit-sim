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
#ifndef C_COMPONENT_H
#define C_COMPONENT_H

// includes for this header
#include <string>     // std::string
#include <vector>     // std::vector
#include <utility>    // std::pair
#include <cstdint>    // standard int types

#include "c_structs.h"  // useful structure definitions

// namespace for entire project
namespace Cim {

// Interface Component class (Gates & Device & Simulation)
class Component {
 public:
  virtual ~Component() { /* DN */ };
  
  // initialize components to initial state
  virtual void Initialize() = 0;

  // Set a pin to new state and update the state_changed flag
  virtual void Set(const uint32_t pin_idx, const bool new_state) = 0;
  virtual void Set(const std::string& pin_name, const bool new_state) = 0;

  // // propagate signal across the circuit
  // virtual void Propagate() = 0;

  // return true if component is monitored
  virtual bool IsMonitored() const noexcept = 0;

  // return true if component is a device
  virtual bool IsDevice() const noexcept = 0;

  // return the pointer to parent device
  virtual const Component* const GetParentDevice() const noexcept = 0;

  // return local index
  virtual uint32_t GetLocalIndex() const noexcept = 0;

  // set local index
  virtual void SetLocalIndex(const uint32_t new_idx) noexcept = 0;

  // get inpins and outpins number
  virtual std::pair<uint32_t, uint32_t> GetIONum() const noexcept = 0;

  // return component name
  virtual std::string GetName() const noexcept = 0;

  // return component full name
  virtual std::string GetFullName() const noexcept = 0;

  // return component type
  virtual std::string GetType() const noexcept = 0;

  // return nesting level
  virtual uint32_t GetNestingLvl() const noexcept = 0;

  // return pin state
  virtual bool GetPinState(const uint32_t pin_idx) const = 0;

  // return if state changed after the last evaluation
  virtual bool IsPinStateChanged(const uint32_t pin_idx) const = 0;

  // return pin name
  virtual std::string GetPinName(const uint32_t pin_idx) const = 0;

  // return list of input pins
  virtual const std::vector<Pin> GetInPins() const = 0;

  // return list of output pins
  virtual const std::vector<Pin> GetOutPins() const = 0;

  // return the pin's direction
  virtual Pin::Dir GetPinDirection(const uint32_t pin_idx) const = 0;
  virtual Pin::Dir GetPinDirection(const std::string& pin_name) const = 0;

  // return pin index
  virtual uint32_t GetPinIndex(const std::string& pin_name) const = 0;

  // return & modify the connection of that pin
  virtual std::vector<Pin::Link>& GetPinConnection(const uint32_t pin_idx) = 0;
  virtual std::vector<Pin::Link>& GetPinConnection(const std::string& pin_name) = 0;

  // return whether a pin exist
  virtual bool DoesPinExist(const std::string& pin_name) const = 0;

  // check if pin is connected
  virtual bool IsPinConnected(const uint32_t pin_idx) const = 0;
  virtual bool IsPinConnected(const std::string& pin_name) const = 0;

  // print input pins' states
  virtual void PrintInPinStates() const noexcept = 0;

  // print output pins' states
  virtual void PrintOutPinStates() const noexcept = 0;
};

}

#endif  // C_COMPONENT_H
