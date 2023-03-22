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
class Gate : public Component {
 public:
  // constructor
  Gate(const Component* const ParentDevice, const std::string& GateName, const std::string& GateType,
      const std::vector<std::string>& InPinNames, bool isMonitored = false);

  // destructor
  ~Gate() { /* DN */ }
  
  // initialize components to initial state
  virtual void Initialize() override;

  // Set a pin to new state and update the state_changed flag
  virtual void Set(const uint32_t pin_idx, const bool new_state) override;
  virtual void Set(const std::string& pin_name, const bool new_state) override;

  // // propagate signal across the circuit
  // virtual void Propagate() override;

  // return true if component is monitored
  virtual bool IsMonitored() const noexcept override;

  // return true if component is a device
  virtual bool IsDevice() const noexcept override;

  // return the pointer to parent device
  virtual const Component* const GetParentDevice() const noexcept override;

  // return local index
  virtual uint32_t GetLocalIndex() const noexcept override;

  // set local index
  virtual void SetLocalIndex(const uint32_t new_idx) noexcept override;

  // get inpins and outpins number
  virtual std::pair<uint32_t, uint32_t> GetIONum() const noexcept override;

  // return component name
  virtual std::string GetName() const noexcept override;

  // return component full name
  virtual std::string GetFullName() const noexcept override;

  // return component type
  virtual std::string GetType() const noexcept override;

  // return nesting level
  virtual uint32_t GetNestingLvl() const noexcept override;

  // return pin state
  virtual bool GetPinState(const uint32_t pin_idx) const override;

  // return if state changed after the last evaluation
  virtual bool IsPinStateChanged(const uint32_t pin_idx) const override;

  // return pin name
  virtual std::string GetPinName(const uint32_t pin_idx) const override;

  // return list of input pins
  virtual const std::vector<Pin> GetInPins() const override;

  // return list of output pins
  virtual const std::vector<Pin> GetOutPins() const override;

  // return the pin's direction
  virtual Pin::Dir GetPinDirection(const uint32_t pin_idx) const override;
  virtual Pin::Dir GetPinDirection(const std::string& pin_name) const override;

  // return pin index
  virtual uint32_t GetPinIndex(const std::string& pin_name) const override;

  // return & modify the connection of that pin
  virtual std::vector<Pin::Link>& GetPinConnection(const uint32_t pin_idx) override;
  virtual std::vector<Pin::Link>& GetPinConnection(const std::string& pin_name) override;

  // return whether a pin exist
  virtual bool DoesPinExist(const std::string& pin_name) const override;

  // check if pin is connected
  virtual bool IsPinConnected(const uint32_t pin_idx) const override;
  virtual bool IsPinConnected(const std::string& pin_name) const override;

  // print input pins' states
  virtual void PrintInPinStates() const noexcept override;

  // print output pins' states
  virtual void PrintOutPinStates() const noexcept override;

 private:
  // search for a pin by name
  Pin* const search(const std::string& pin_name) const;

  // evaluation of each type of gate
  static bool AND(const std::vector<Pin>& pins);    // AND
  static bool OR(const std::vector<Pin>& pins);     // OR
  static bool XOR(const std::vector<Pin>& pins);    // XOR
  static bool NOT(const std::vector<Pin>& pins);    // NOT
  static bool NAND(const std::vector<Pin>& pins);   // NAND
  static bool NOR(const std::vector<Pin>& pins);    // NOR
  static bool XNOR(const std::vector<Pin>& pins);   // XNOR

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
  std::vector<Pin> Pins; // In & Out pin list
};

}

#endif  // C_GATE_H
