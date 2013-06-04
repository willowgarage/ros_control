///////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2012, hiDOF INC.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of hiDOF, Inc. nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//////////////////////////////////////////////////////////////////////////////

/*
 * Author: Wim Meeussen
 */

#ifndef HARDWARE_INTERFACE_JOINT_STATE_INTERFACE_H
#define HARDWARE_INTERFACE_JOINT_STATE_INTERFACE_H

#include <hardware_interface/internal/demangle_symbol.h>
#include <hardware_interface/internal/named_resource_manager.h>
#include <hardware_interface/hardware_interface.h>
#include <string>
#include <vector>

namespace hardware_interface
{

/** A handle used to read the state of a single joint. */
class JointStateHandle
{
public:
  JointStateHandle() {}
  JointStateHandle(const std::string& name, const double* pos, const double* vel, const double* eff)
    : name_(name), pos_(pos), vel_(vel), eff_(eff)
  {}

  std::string getName() const {return name_;}
  double getPosition() const {return *pos_;}
  double getVelocity() const {return *vel_;}
  double getEffort()   const {return *eff_;}

private:
  std::string name_;
  const double* pos_;
  const double* vel_;
  const double* eff_;
};


/** \brief Hardware interface to support reading the state of an array of joints
 *
 * This \ref HardwareInterface supports reading the state of an array of named
 * joints, each of which has some position, velocity, and effort (force or
 * torque).
 *
 */
class JointStateInterface: public HardwareInterface
{
public:
  /// Get the vector of joint names registered to this interface.
  std::vector<std::string> getJointNames() const
  {
    return handle_map_.getNames();
  }

  /** \brief Register a new joint with this interface.
   *
   * \param name The name of the new joint
   * \param pos A pointer to the storage for this joint's position
   * \param vel A pointer to the storage for this joint's velocity
   * \param eff A pointer to the storage for this joint's effort (force or torque)
   *
   */
  void registerJoint(const std::string& name, double* pos, double* vel, double* eff)
  {
    JointStateHandle handle(name, pos, vel, eff);
    handle_map_.insert(name, handle);
  }

  /** \brief Get a \ref JointStateHandle for accessing a joint's state
   *
   * \param name The name of the joint
   *
   */
  JointStateHandle getJointStateHandle(const std::string& name) const
  {
    try
    {
      return handle_map_.get(name);
    }
    catch(...)
    {
      throw HardwareInterfaceException("Could not find joint [" + name + "] in " + internal::demangledTypeName(*this));
    }
  }

private:
  internal::NamedResourceManager<JointStateHandle> handle_map_;
};

}

#endif
