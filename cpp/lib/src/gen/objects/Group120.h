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
// Copyright 2013-2019 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef OPENDNP3_GROUP120_H
#define OPENDNP3_GROUP120_H

#include "opendnp3/app/GroupVariationID.h"
#include <ser4cpp/container/SequenceTypes.h>
#include "opendnp3/app/DNPTime.h"
#include "app/IVariableLength.h"
#include "gen/HMACTypeSerialization.h"
#include "gen/ChallengeReasonSerialization.h"
#include "gen/KeyWrapAlgorithmSerialization.h"
#include "gen/KeyStatusSerialization.h"
#include "gen/AuthErrorCodeSerialization.h"
#include "gen/KeyChangeMethodSerialization.h"
#include "gen/CertificateTypeSerialization.h"
#include "gen/UserOperationSerialization.h"

namespace opendnp3 {

// Authentication - Challenge
struct Group120Var1 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,1); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var1();

  Group120Var1(
    uint32_t challengeSeqNum,
    uint16_t userNum,
    HMACType hmacAlgo,
    ChallengeReason challengeReason,
    const ser4cpp::rseq_t& challengeData
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 8;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  HMACType hmacAlgo;
  ChallengeReason challengeReason;
  ser4cpp::rseq_t challengeData;
};

// Authentication - Reply
struct Group120Var2 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,2); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var2();

  Group120Var2(
    uint32_t challengeSeqNum,
    uint16_t userNum,
    const ser4cpp::rseq_t& hmacValue
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 6;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  ser4cpp::rseq_t hmacValue;
};

// Authentication - Aggressive Mode Request
struct Group120Var3
{
  static GroupVariationID ID() { return GroupVariationID(120,3); }

  Group120Var3();

  static uint32_t Size() { return 6; }
  static bool Read(ser4cpp::rseq_t&, Group120Var3&);
  static bool Write(const Group120Var3&, ser4cpp::wseq_t&);

  uint32_t challengeSeqNum;
  uint16_t userNum;
};

// Authentication - Session Key Status Request
struct Group120Var4
{
  static GroupVariationID ID() { return GroupVariationID(120,4); }

  Group120Var4();

  static uint32_t Size() { return 2; }
  static bool Read(ser4cpp::rseq_t&, Group120Var4&);
  static bool Write(const Group120Var4&, ser4cpp::wseq_t&);

  uint16_t userNum;
};

// Authentication - Session Key Status
struct Group120Var5 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,5); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var5();

  Group120Var5(
    uint32_t keyChangeSeqNum,
    uint16_t userNum,
    KeyWrapAlgorithm keyWrapAlgo,
    KeyStatus keyStatus,
    HMACType hmacAlgo,
    const ser4cpp::rseq_t& challengeData,
    const ser4cpp::rseq_t& hmacValue
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 11;

  // member variables
  uint32_t keyChangeSeqNum;
  uint16_t userNum;
  KeyWrapAlgorithm keyWrapAlgo;
  KeyStatus keyStatus;
  HMACType hmacAlgo;
  ser4cpp::rseq_t challengeData;
  ser4cpp::rseq_t hmacValue;
};

// Authentication - Session Key Change
struct Group120Var6 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,6); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var6();

  Group120Var6(
    uint32_t keyChangeSeqNum,
    uint16_t userNum,
    const ser4cpp::rseq_t& keyWrapData
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 6;

  // member variables
  uint32_t keyChangeSeqNum;
  uint16_t userNum;
  ser4cpp::rseq_t keyWrapData;
};

// Authentication - Error
struct Group120Var7 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,7); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var7();

  Group120Var7(
    uint32_t challengeSeqNum,
    uint16_t userNum,
    uint16_t assocId,
    AuthErrorCode errorCode,
    DNPTime time,
    const ser4cpp::rseq_t& errorText
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 15;

  // member variables
  uint32_t challengeSeqNum;
  uint16_t userNum;
  uint16_t assocId;
  AuthErrorCode errorCode;
  DNPTime time;
  ser4cpp::rseq_t errorText;
};

// Authentication - User Certificate
struct Group120Var8 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,8); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var8();

  Group120Var8(
    KeyChangeMethod keyChangeMethod,
    CertificateType certificateType,
    const ser4cpp::rseq_t& certificate
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 2;

  // member variables
  KeyChangeMethod keyChangeMethod;
  CertificateType certificateType;
  ser4cpp::rseq_t certificate;
};

// Authentication - HMAC
struct Group120Var9 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,9); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var9();

  explicit Group120Var9(
    const ser4cpp::rseq_t& hmacValue
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 0;

  // member variables
  ser4cpp::rseq_t hmacValue;
};

// Authentication - User Status Change
struct Group120Var10 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,10); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var10();

  Group120Var10(
    KeyChangeMethod keyChangeMethod,
    UserOperation userOperation,
    uint32_t statusChangeSeqNum,
    uint16_t userRole,
    uint16_t userRoleExpDays,
    const ser4cpp::rseq_t& userName,
    const ser4cpp::rseq_t& userPublicKey,
    const ser4cpp::rseq_t& certificationData
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 16;

  // member variables
  KeyChangeMethod keyChangeMethod;
  UserOperation userOperation;
  uint32_t statusChangeSeqNum;
  uint16_t userRole;
  uint16_t userRoleExpDays;
  ser4cpp::rseq_t userName;
  ser4cpp::rseq_t userPublicKey;
  ser4cpp::rseq_t certificationData;
};

// Authentication - Update Key Change Request
struct Group120Var11 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,11); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var11();

  Group120Var11(
    KeyChangeMethod keyChangeMethod,
    const ser4cpp::rseq_t& userName,
    const ser4cpp::rseq_t& challengeData
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 5;

  // member variables
  KeyChangeMethod keyChangeMethod;
  ser4cpp::rseq_t userName;
  ser4cpp::rseq_t challengeData;
};

// Authentication - Update Key Change Reply
struct Group120Var12 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,12); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var12();

  Group120Var12(
    uint32_t keyChangeSeqNum,
    uint16_t userNum,
    const ser4cpp::rseq_t& challengeData
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 8;

  // member variables
  uint32_t keyChangeSeqNum;
  uint16_t userNum;
  ser4cpp::rseq_t challengeData;
};

// Authentication - Update Key Change
struct Group120Var13 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,13); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var13();

  Group120Var13(
    uint32_t keyChangeSeqNum,
    uint16_t userNum,
    const ser4cpp::rseq_t& encryptedUpdateKey
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 8;

  // member variables
  uint32_t keyChangeSeqNum;
  uint16_t userNum;
  ser4cpp::rseq_t encryptedUpdateKey;
};

// Authentication - Update Key Change Signature
struct Group120Var14 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,14); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var14();

  explicit Group120Var14(
    const ser4cpp::rseq_t& digitalSignature
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 0;

  // member variables
  ser4cpp::rseq_t digitalSignature;
};

// Authentication - Update Key Change Confirmation
struct Group120Var15 : public IVariableLength
{
  static GroupVariationID ID() { return GroupVariationID(120,15); }

  virtual GroupVariationID InstanceID() const override final { return ID(); }

  Group120Var15();

  explicit Group120Var15(
    const ser4cpp::rseq_t& hmacValue
  );

  virtual uint32_t Size() const override final;
  virtual bool Read(const ser4cpp::rseq_t&) override final;
  virtual bool Write(ser4cpp::wseq_t&) const override final;

  static const uint32_t MIN_SIZE = 0;

  // member variables
  ser4cpp::rseq_t hmacValue;
};


}

#endif
