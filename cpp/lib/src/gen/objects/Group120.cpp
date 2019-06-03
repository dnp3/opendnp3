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

#include "Group120.h"

#include <ser4cpp/serialization/EndianHelpers.h>
#include "app/parsing/PrefixFields.h"
#include <ser4cpp/serialization/LittleEndian.h>

using namespace ser4cpp;

namespace opendnp3 {

// ------- Group120Var1 -------

Group120Var1::Group120Var1() : 
  challengeSeqNum(0), userNum(0), hmacAlgo(HMACType::UNKNOWN), challengeReason(ChallengeReason::UNKNOWN)
{}

Group120Var1::Group120Var1(
  uint32_t challengeSeqNum_,
  uint16_t userNum_,
  HMACType hmacAlgo_,
  ChallengeReason challengeReason_,
  const ser4cpp::rseq_t& challengeData_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  hmacAlgo(hmacAlgo_),
  challengeReason(challengeReason_),
  challengeData(challengeData_)
{}

uint32_t Group120Var1::Size() const
{
  return MIN_SIZE + challengeData.length();
}

bool Group120Var1::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var1::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  UInt32::read_from(copy, this->challengeSeqNum);
  UInt16::read_from(copy, this->userNum);
  uint8_t hmacAlgoRawValue;
  UInt8::read_from(copy, hmacAlgoRawValue);
  this->hmacAlgo = HMACTypeFromType(hmacAlgoRawValue);
  uint8_t challengeReasonRawValue;
  UInt8::read_from(copy, challengeReasonRawValue);
  this->challengeReason = ChallengeReasonFromType(challengeReasonRawValue);

  this->challengeData = copy; // whatever is left over
  return true;
}

bool Group120Var1::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  UInt32::write_to(buffer, this->challengeSeqNum);
  UInt16::write_to(buffer, this->userNum);
  UInt8::write_to(buffer, HMACTypeToType(this->hmacAlgo));
  UInt8::write_to(buffer, ChallengeReasonToType(this->challengeReason));

  buffer.copy_from(challengeData);
  return true;
}

// ------- Group120Var2 -------

Group120Var2::Group120Var2() : 
  challengeSeqNum(0), userNum(0)
{}

Group120Var2::Group120Var2(
  uint32_t challengeSeqNum_,
  uint16_t userNum_,
  const ser4cpp::rseq_t& hmacValue_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  hmacValue(hmacValue_)
{}

uint32_t Group120Var2::Size() const
{
  return MIN_SIZE + hmacValue.length();
}

bool Group120Var2::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var2::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  UInt32::read_from(copy, this->challengeSeqNum);
  UInt16::read_from(copy, this->userNum);

  this->hmacValue = copy; // whatever is left over
  return true;
}

bool Group120Var2::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  UInt32::write_to(buffer, this->challengeSeqNum);
  UInt16::write_to(buffer, this->userNum);

  buffer.copy_from(hmacValue);
  return true;
}

// ------- Group120Var3 -------

Group120Var3::Group120Var3() : challengeSeqNum(0), userNum(0)
{}

bool Group120Var3::Read(rseq_t& buffer, Group120Var3& output)
{
  bool result = true;

  result &= UInt32::read_from(buffer, output.challengeSeqNum);
  result &= UInt16::read_from(buffer, output.userNum);

  return result;
}

bool Group120Var3::Write(const Group120Var3& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt32::write_to(buffer, arg.challengeSeqNum);
  result &= UInt16::write_to(buffer, arg.userNum);

  return result;
}

// ------- Group120Var4 -------

Group120Var4::Group120Var4() : userNum(0)
{}

bool Group120Var4::Read(rseq_t& buffer, Group120Var4& output)
{
  bool result = true;

  result &= UInt16::read_from(buffer, output.userNum);

  return result;
}

bool Group120Var4::Write(const Group120Var4& arg, ser4cpp::wseq_t& buffer)
{
  bool result = true;

  result &= UInt16::write_to(buffer, arg.userNum);

  return result;
}

// ------- Group120Var5 -------

Group120Var5::Group120Var5() : 
  keyChangeSeqNum(0), userNum(0), keyWrapAlgo(KeyWrapAlgorithm::UNDEFINED), keyStatus(KeyStatus::UNDEFINED), hmacAlgo(HMACType::UNKNOWN)
{}

Group120Var5::Group120Var5(
  uint32_t keyChangeSeqNum_,
  uint16_t userNum_,
  KeyWrapAlgorithm keyWrapAlgo_,
  KeyStatus keyStatus_,
  HMACType hmacAlgo_,
  const ser4cpp::rseq_t& challengeData_,
  const ser4cpp::rseq_t& hmacValue_
) : 
  keyChangeSeqNum(keyChangeSeqNum_),
  userNum(userNum_),
  keyWrapAlgo(keyWrapAlgo_),
  keyStatus(keyStatus_),
  hmacAlgo(hmacAlgo_),
  challengeData(challengeData_),
  hmacValue(hmacValue_)
{}

uint32_t Group120Var5::Size() const
{
  return MIN_SIZE + challengeData.length() + hmacValue.length();
}

bool Group120Var5::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var5::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  UInt32::read_from(copy, this->keyChangeSeqNum);
  UInt16::read_from(copy, this->userNum);
  uint8_t keyWrapAlgoRawValue;
  UInt8::read_from(copy, keyWrapAlgoRawValue);
  this->keyWrapAlgo = KeyWrapAlgorithmFromType(keyWrapAlgoRawValue);
  uint8_t keyStatusRawValue;
  UInt8::read_from(copy, keyStatusRawValue);
  this->keyStatus = KeyStatusFromType(keyStatusRawValue);
  uint8_t hmacAlgoRawValue;
  UInt8::read_from(copy, hmacAlgoRawValue);
  this->hmacAlgo = HMACTypeFromType(hmacAlgoRawValue);

  if(!PrefixFields::Read(copy, challengeData))
  {
    return false;
  }

  this->hmacValue = copy; // whatever is left over
  return true;
}

bool Group120Var5::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  // All of the fields that have a uint16_t length must have the proper size
  if(!PrefixFields::LengthFitsInUInt16(challengeData))
  {
    return false;
  }

  UInt32::write_to(buffer, this->keyChangeSeqNum);
  UInt16::write_to(buffer, this->userNum);
  UInt8::write_to(buffer, KeyWrapAlgorithmToType(this->keyWrapAlgo));
  UInt8::write_to(buffer, KeyStatusToType(this->keyStatus));
  UInt8::write_to(buffer, HMACTypeToType(this->hmacAlgo));

  if(!PrefixFields::Write(buffer, challengeData))
  {
    return false;
  }

  buffer.copy_from(hmacValue);
  return true;
}

// ------- Group120Var6 -------

Group120Var6::Group120Var6() : 
  keyChangeSeqNum(0), userNum(0)
{}

Group120Var6::Group120Var6(
  uint32_t keyChangeSeqNum_,
  uint16_t userNum_,
  const ser4cpp::rseq_t& keyWrapData_
) : 
  keyChangeSeqNum(keyChangeSeqNum_),
  userNum(userNum_),
  keyWrapData(keyWrapData_)
{}

uint32_t Group120Var6::Size() const
{
  return MIN_SIZE + keyWrapData.length();
}

bool Group120Var6::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var6::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  UInt32::read_from(copy, this->keyChangeSeqNum);
  UInt16::read_from(copy, this->userNum);

  this->keyWrapData = copy; // whatever is left over
  return true;
}

bool Group120Var6::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  UInt32::write_to(buffer, this->keyChangeSeqNum);
  UInt16::write_to(buffer, this->userNum);

  buffer.copy_from(keyWrapData);
  return true;
}

// ------- Group120Var7 -------

Group120Var7::Group120Var7() : 
  challengeSeqNum(0), userNum(0), assocId(0), errorCode(AuthErrorCode::UNKNOWN), time(0)
{}

Group120Var7::Group120Var7(
  uint32_t challengeSeqNum_,
  uint16_t userNum_,
  uint16_t assocId_,
  AuthErrorCode errorCode_,
  DNPTime time_,
  const ser4cpp::rseq_t& errorText_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  assocId(assocId_),
  errorCode(errorCode_),
  time(time_),
  errorText(errorText_)
{}

uint32_t Group120Var7::Size() const
{
  return MIN_SIZE + errorText.length();
}

bool Group120Var7::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var7::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  UInt32::read_from(copy, this->challengeSeqNum);
  UInt16::read_from(copy, this->userNum);
  UInt16::read_from(copy, this->assocId);
  uint8_t errorCodeRawValue;
  UInt8::read_from(copy, errorCodeRawValue);
  this->errorCode = AuthErrorCodeFromType(errorCodeRawValue);
  UInt48Type timeTemp;
  UInt48::read_from(copy, timeTemp);
  this->time = timeTemp.Get();

  this->errorText = copy; // whatever is left over
  return true;
}

bool Group120Var7::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  UInt32::write_to(buffer, this->challengeSeqNum);
  UInt16::write_to(buffer, this->userNum);
  UInt16::write_to(buffer, this->assocId);
  UInt8::write_to(buffer, AuthErrorCodeToType(this->errorCode));
  UInt48::write_to(buffer, UInt48Type(this->time));

  buffer.copy_from(errorText);
  return true;
}

// ------- Group120Var8 -------

Group120Var8::Group120Var8() : 
  keyChangeMethod(KeyChangeMethod::UNDEFINED), certificateType(CertificateType::UNKNOWN)
{}

Group120Var8::Group120Var8(
  KeyChangeMethod keyChangeMethod_,
  CertificateType certificateType_,
  const ser4cpp::rseq_t& certificate_
) : 
  keyChangeMethod(keyChangeMethod_),
  certificateType(certificateType_),
  certificate(certificate_)
{}

uint32_t Group120Var8::Size() const
{
  return MIN_SIZE + certificate.length();
}

bool Group120Var8::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var8::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  uint8_t keyChangeMethodRawValue;
  UInt8::read_from(copy, keyChangeMethodRawValue);
  this->keyChangeMethod = KeyChangeMethodFromType(keyChangeMethodRawValue);
  uint8_t certificateTypeRawValue;
  UInt8::read_from(copy, certificateTypeRawValue);
  this->certificateType = CertificateTypeFromType(certificateTypeRawValue);

  this->certificate = copy; // whatever is left over
  return true;
}

bool Group120Var8::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  UInt8::write_to(buffer, KeyChangeMethodToType(this->keyChangeMethod));
  UInt8::write_to(buffer, CertificateTypeToType(this->certificateType));

  buffer.copy_from(certificate);
  return true;
}

// ------- Group120Var9 -------

Group120Var9::Group120Var9()
{}

Group120Var9::Group120Var9(
  const ser4cpp::rseq_t& hmacValue_
) : 
  hmacValue(hmacValue_)
{}

uint32_t Group120Var9::Size() const
{
  return MIN_SIZE + hmacValue.length();
}

bool Group120Var9::Read(const rseq_t& buffer)
{
  this->hmacValue = buffer; // the object is just the remainder field
  return true;
}

bool Group120Var9::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  buffer.copy_from(hmacValue);
  return true;
}

// ------- Group120Var10 -------

Group120Var10::Group120Var10() : 
  keyChangeMethod(KeyChangeMethod::UNDEFINED), userOperation(UserOperation::OP_UNDEFINED), statusChangeSeqNum(0), userRole(0), userRoleExpDays(0)
{}

Group120Var10::Group120Var10(
  KeyChangeMethod keyChangeMethod_,
  UserOperation userOperation_,
  uint32_t statusChangeSeqNum_,
  uint16_t userRole_,
  uint16_t userRoleExpDays_,
  const ser4cpp::rseq_t& userName_,
  const ser4cpp::rseq_t& userPublicKey_,
  const ser4cpp::rseq_t& certificationData_
) : 
  keyChangeMethod(keyChangeMethod_),
  userOperation(userOperation_),
  statusChangeSeqNum(statusChangeSeqNum_),
  userRole(userRole_),
  userRoleExpDays(userRoleExpDays_),
  userName(userName_),
  userPublicKey(userPublicKey_),
  certificationData(certificationData_)
{}

uint32_t Group120Var10::Size() const
{
  return MIN_SIZE + userName.length() + userPublicKey.length() + certificationData.length();
}

bool Group120Var10::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var10::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  uint8_t keyChangeMethodRawValue;
  UInt8::read_from(copy, keyChangeMethodRawValue);
  this->keyChangeMethod = KeyChangeMethodFromType(keyChangeMethodRawValue);
  uint8_t userOperationRawValue;
  UInt8::read_from(copy, userOperationRawValue);
  this->userOperation = UserOperationFromType(userOperationRawValue);
  UInt32::read_from(copy, this->statusChangeSeqNum);
  UInt16::read_from(copy, this->userRole);
  UInt16::read_from(copy, this->userRoleExpDays);

  if(!PrefixFields::Read(copy, userName, userPublicKey, certificationData))
  {
    return false;
  }

  // object does not have a remainder field so it should be fully consumed
  // The header length disagrees with object encoding so abort
  if(copy.is_not_empty())
  {
    return false;
  }

  return true;
}

bool Group120Var10::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  // All of the fields that have a uint16_t length must have the proper size
  if(!PrefixFields::LengthFitsInUInt16(userName, userPublicKey, certificationData))
  {
    return false;
  }

  UInt8::write_to(buffer, KeyChangeMethodToType(this->keyChangeMethod));
  UInt8::write_to(buffer, UserOperationToType(this->userOperation));
  UInt32::write_to(buffer, this->statusChangeSeqNum);
  UInt16::write_to(buffer, this->userRole);
  UInt16::write_to(buffer, this->userRoleExpDays);

  if(!PrefixFields::Write(buffer, userName, userPublicKey, certificationData))
  {
    return false;
  }

  return true;
}

// ------- Group120Var11 -------

Group120Var11::Group120Var11() : 
  keyChangeMethod(KeyChangeMethod::UNDEFINED)
{}

Group120Var11::Group120Var11(
  KeyChangeMethod keyChangeMethod_,
  const ser4cpp::rseq_t& userName_,
  const ser4cpp::rseq_t& challengeData_
) : 
  keyChangeMethod(keyChangeMethod_),
  userName(userName_),
  challengeData(challengeData_)
{}

uint32_t Group120Var11::Size() const
{
  return MIN_SIZE + userName.length() + challengeData.length();
}

bool Group120Var11::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var11::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  uint8_t keyChangeMethodRawValue;
  UInt8::read_from(copy, keyChangeMethodRawValue);
  this->keyChangeMethod = KeyChangeMethodFromType(keyChangeMethodRawValue);

  if(!PrefixFields::Read(copy, userName, challengeData))
  {
    return false;
  }

  // object does not have a remainder field so it should be fully consumed
  // The header length disagrees with object encoding so abort
  if(copy.is_not_empty())
  {
    return false;
  }

  return true;
}

bool Group120Var11::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  // All of the fields that have a uint16_t length must have the proper size
  if(!PrefixFields::LengthFitsInUInt16(userName, challengeData))
  {
    return false;
  }

  UInt8::write_to(buffer, KeyChangeMethodToType(this->keyChangeMethod));

  if(!PrefixFields::Write(buffer, userName, challengeData))
  {
    return false;
  }

  return true;
}

// ------- Group120Var12 -------

Group120Var12::Group120Var12() : 
  keyChangeSeqNum(0), userNum(0)
{}

Group120Var12::Group120Var12(
  uint32_t keyChangeSeqNum_,
  uint16_t userNum_,
  const ser4cpp::rseq_t& challengeData_
) : 
  keyChangeSeqNum(keyChangeSeqNum_),
  userNum(userNum_),
  challengeData(challengeData_)
{}

uint32_t Group120Var12::Size() const
{
  return MIN_SIZE + challengeData.length();
}

bool Group120Var12::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var12::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  UInt32::read_from(copy, this->keyChangeSeqNum);
  UInt16::read_from(copy, this->userNum);

  if(!PrefixFields::Read(copy, challengeData))
  {
    return false;
  }

  // object does not have a remainder field so it should be fully consumed
  // The header length disagrees with object encoding so abort
  if(copy.is_not_empty())
  {
    return false;
  }

  return true;
}

bool Group120Var12::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  // All of the fields that have a uint16_t length must have the proper size
  if(!PrefixFields::LengthFitsInUInt16(challengeData))
  {
    return false;
  }

  UInt32::write_to(buffer, this->keyChangeSeqNum);
  UInt16::write_to(buffer, this->userNum);

  if(!PrefixFields::Write(buffer, challengeData))
  {
    return false;
  }

  return true;
}

// ------- Group120Var13 -------

Group120Var13::Group120Var13() : 
  keyChangeSeqNum(0), userNum(0)
{}

Group120Var13::Group120Var13(
  uint32_t keyChangeSeqNum_,
  uint16_t userNum_,
  const ser4cpp::rseq_t& encryptedUpdateKey_
) : 
  keyChangeSeqNum(keyChangeSeqNum_),
  userNum(userNum_),
  encryptedUpdateKey(encryptedUpdateKey_)
{}

uint32_t Group120Var13::Size() const
{
  return MIN_SIZE + encryptedUpdateKey.length();
}

bool Group120Var13::Read(const rseq_t& buffer)
{
  if(buffer.length() < Group120Var13::MIN_SIZE)
  {
    return false;
  }

  rseq_t copy(buffer); //mutable copy for parsing

  UInt32::read_from(copy, this->keyChangeSeqNum);
  UInt16::read_from(copy, this->userNum);

  if(!PrefixFields::Read(copy, encryptedUpdateKey))
  {
    return false;
  }

  // object does not have a remainder field so it should be fully consumed
  // The header length disagrees with object encoding so abort
  if(copy.is_not_empty())
  {
    return false;
  }

  return true;
}

bool Group120Var13::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  // All of the fields that have a uint16_t length must have the proper size
  if(!PrefixFields::LengthFitsInUInt16(encryptedUpdateKey))
  {
    return false;
  }

  UInt32::write_to(buffer, this->keyChangeSeqNum);
  UInt16::write_to(buffer, this->userNum);

  if(!PrefixFields::Write(buffer, encryptedUpdateKey))
  {
    return false;
  }

  return true;
}

// ------- Group120Var14 -------

Group120Var14::Group120Var14()
{}

Group120Var14::Group120Var14(
  const ser4cpp::rseq_t& digitalSignature_
) : 
  digitalSignature(digitalSignature_)
{}

uint32_t Group120Var14::Size() const
{
  return MIN_SIZE + digitalSignature.length();
}

bool Group120Var14::Read(const rseq_t& buffer)
{
  this->digitalSignature = buffer; // the object is just the remainder field
  return true;
}

bool Group120Var14::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  buffer.copy_from(digitalSignature);
  return true;
}

// ------- Group120Var15 -------

Group120Var15::Group120Var15()
{}

Group120Var15::Group120Var15(
  const ser4cpp::rseq_t& hmacValue_
) : 
  hmacValue(hmacValue_)
{}

uint32_t Group120Var15::Size() const
{
  return MIN_SIZE + hmacValue.length();
}

bool Group120Var15::Read(const rseq_t& buffer)
{
  this->hmacValue = buffer; // the object is just the remainder field
  return true;
}

bool Group120Var15::Write(ser4cpp::wseq_t& buffer) const
{
  if(this->Size() > buffer.length())
  {
    return false;
  }

  buffer.copy_from(hmacValue);
  return true;
}


}
