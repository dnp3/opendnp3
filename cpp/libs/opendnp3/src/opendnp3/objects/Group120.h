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
#include "opendnp3/gen/KeyChangeMethod.h"
#include "opendnp3/gen/CertificateType.h"

namespace opendnp3 {

// Authentication - Challenge
struct Group120Var1 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,1); }

  virtual GroupVariationID InstanceID() const override final { return Group120Var1::ID(); }

  Group120Var1();

  Group120Var1(
    uint32_t challengeSeqNum,
    uint16_t userNum,
    HMACType hmacAlgo,
    ChallengeReason challengeReason,
    const openpal::ReadBufferView& challengeData
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 8;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  HMACType hmacAlgo;
  ChallengeReason challengeReason;
  openpal::ReadBufferView challengeData;
};

// Authentication - Reply
struct Group120Var2 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,2); }

  virtual GroupVariationID InstanceID() const override final { return Group120Var2::ID(); }

  Group120Var2();

  Group120Var2(
    uint32_t challengeSeqNum,
    uint16_t userNum,
    const openpal::ReadBufferView& hmacValue
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 6;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  openpal::ReadBufferView hmacValue;
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

  virtual GroupVariationID InstanceID() const override final { return Group120Var5::ID(); }

  Group120Var5();

  Group120Var5(
    uint32_t keyChangeSeqNum,
    uint16_t userNum,
    KeyWrapAlgorithm keyWrapAlgo,
    KeyStatus keyStatus,
    HMACType hmacAlgo,
    const openpal::ReadBufferView& challengeData,
    const openpal::ReadBufferView& hmacValue
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 11;

  // member variables
  uint32_t keyChangeSeqNum;
  uint16_t userNum;
  KeyWrapAlgorithm keyWrapAlgo;
  KeyStatus keyStatus;
  HMACType hmacAlgo;
  openpal::ReadBufferView challengeData;
  openpal::ReadBufferView hmacValue;
};

// Authentication - Session Key Change
struct Group120Var6 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,6); }

  virtual GroupVariationID InstanceID() const override final { return Group120Var6::ID(); }

  Group120Var6();

  Group120Var6(
    uint32_t keyChangeSeqNum,
    uint16_t userNum,
    const openpal::ReadBufferView& keyWrapData
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const openpal::ReadBufferView&) override final;
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

  virtual GroupVariationID InstanceID() const override final { return Group120Var7::ID(); }

  Group120Var7();

  Group120Var7(
    uint32_t challengeSeqNum,
    uint16_t userNum,
    uint16_t assocId,
    AuthErrorCode errorCode,
    DNPTime time,
    const openpal::ReadBufferView& errorText
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const openpal::ReadBufferView&) override final;
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
struct Group120Var8 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,8); }

  virtual GroupVariationID InstanceID() const override final { return Group120Var8::ID(); }

  Group120Var8();

  Group120Var8(
    KeyChangeMethod keyChangeMethod,
    CertificateType certificateType,
    const openpal::ReadBufferView& certificate
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 2;

  // member variables
  KeyChangeMethod keyChangeMethod;
  CertificateType certificateType;
  openpal::ReadBufferView certificate;
};

// Authentication - HMAC
struct Group120Var9 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,9); }

  virtual GroupVariationID InstanceID() const override final { return Group120Var9::ID(); }

  Group120Var9();

  Group120Var9(
    const openpal::ReadBufferView& hmacValue
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const openpal::ReadBufferView&) override final;
  virtual bool Write(openpal::WriteBufferView&) const override final;

  static const uint32_t MIN_SIZE = 0;

  // member variables
  openpal::ReadBufferView hmacValue;
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
