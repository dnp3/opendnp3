//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef OPENDNP3_GROUP120_H
#define OPENDNP3_GROUP120_H

#include "opendnp3/app/GroupVariationID.h"
#include <openpal/container/ReadBufferView.h>
#include <openpal/container/WriteBufferView.h>
#include "opendnp3/Types.h"
#include "opendnp3/app/IVariableLength.h"
#include "opendnp3/gen/HMACType.h"
#include "opendnp3/gen/ChallengeReason.h"
#include "opendnp3/gen/KeyWrapAlgorithm.h"
#include "opendnp3/gen/KeyStatus.h"
#include "opendnp3/gen/AuthErrorCode.h"

namespace opendnp3 {

// Authentication - Challenge
struct Group120Var1 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,1); }
  Group120Var1();
  virtual uint32_t Size() const override final;
  virtual bool Read(openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 8;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  HMACType macAlgo;
  ChallengeReason challengeReason;
  openpal::ReadBufferView challengeData;
};

// Authentication - Reply
struct Group120Var2 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,2); }
  Group120Var2();
  virtual uint32_t Size() const override final;
  virtual bool Read(openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 6;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  openpal::ReadBufferView hmac;
};

// Authentication - Aggressive Mode Request
struct Group120Var3
{
  static GroupVariationID ID() { return GroupVariationID(120,3); }
  static uint32_t Size() { return 6; }
  static bool Read(openpal::ReadBufferView&, Group120Var3&);
  static bool Write(const Group120Var3&, openpal::WriteBufferView&);

  uint32_t challengeSeqNum;
  uint16_t userNum;
};

// Authentication - Session Key Status Request
struct Group120Var4
{
  static GroupVariationID ID() { return GroupVariationID(120,4); }
  static uint32_t Size() { return 2; }
  static bool Read(openpal::ReadBufferView&, Group120Var4&);
  static bool Write(const Group120Var4&, openpal::WriteBufferView&);

  uint16_t userNum;
};

// Authentication - Session Key Status
struct Group120Var5 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,5); }
  Group120Var5();
  virtual uint32_t Size() const override final;
  virtual bool Read(openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 11;

  // member variables
  uint32_t keyChangeSeqNum;
  uint16_t userNum;
  KeyWrapAlgorithm keyWrapAlgo;
  KeyStatus keyStatus;
  HMACType macAlgo;
  openpal::ReadBufferView challengeData;
  openpal::ReadBufferView hmac;
};

// Authentication - Session Key Change
struct Group120Var6 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,6); }
  Group120Var6();
  virtual uint32_t Size() const override final;
  virtual bool Read(openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 6;

  // member variables
  uint32_t keyChangeSeqNum;
  uint16_t userNum;
  openpal::ReadBufferView keyWrapData;
};

// Authentication - Error
struct Group120Var7 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,7); }
  Group120Var7();
  virtual uint32_t Size() const override final;
  virtual bool Read(openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 15;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  uint16_t assocId;
  AuthErrorCode errorCode;
  DNPTime time;
  openpal::ReadBufferView errorText;
};

// Authentication - User Certificate
struct Group120Var8
{
  static GroupVariationID ID() { return GroupVariationID(120,8); }
};

// Authentication - HMAC
struct Group120Var9 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,9); }
  Group120Var9();
  virtual uint32_t Size() const override final;
  virtual bool Read(openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 0;

  // member variables
  openpal::ReadBufferView hmac;
};

// Authentication - User Status Change
struct Group120Var10
{
  static GroupVariationID ID() { return GroupVariationID(120,10); }
};

// Authentication - Update Key Change Request
struct Group120Var11
{
  static GroupVariationID ID() { return GroupVariationID(120,11); }
};

// Authentication - Update Key Change Reply
struct Group120Var12
{
  static GroupVariationID ID() { return GroupVariationID(120,12); }
};

// Authentication - Update Key Change
struct Group120Var13
{
  static GroupVariationID ID() { return GroupVariationID(120,13); }
};

// Authentication - Update Key Change Signature
struct Group120Var14
{
  static GroupVariationID ID() { return GroupVariationID(120,14); }
};

// Authentication - Update Key Change Confirmation
struct Group120Var15
{
  static GroupVariationID ID() { return GroupVariationID(120,15); }
};


}

#endif
