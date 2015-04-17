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

#include "Group120.h"

#include <openpal/serialization/Serialization.h>
#include "opendnp3/app/parsing/PrefixFields.h"
#include <openpal/serialization/Format.h>
#include <openpal/serialization/Parse.h>

using namespace openpal;

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
  const openpal::ReadBufferView& challengeData_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  hmacAlgo(hmacAlgo_),
  challengeReason(challengeReason_),
  challengeData(challengeData_)
{}

uint32_t Group120Var1::Size() const
{
  return MIN_SIZE + challengeData.Size();
}

bool Group120Var1::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var1::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->challengeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);
  this->hmacAlgo = HMACTypeFromType(UInt8::ReadBuffer(copy));
  this->challengeReason = ChallengeReasonFromType(UInt8::ReadBuffer(copy));

  this->challengeData = copy; // whatever is left over

  return true;
}

bool Group120Var1::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->challengeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);
  UInt8::WriteBuffer(buffer, HMACTypeToType(this->hmacAlgo));
  UInt8::WriteBuffer(buffer, ChallengeReasonToType(this->challengeReason));

  challengeData.CopyTo(buffer);

  return true;
}

// ------- Group120Var2 -------

Group120Var2::Group120Var2() : 
  challengeSeqNum(0), userNum(0)
{}

Group120Var2::Group120Var2(
  uint32_t challengeSeqNum_,
  uint16_t userNum_,
  const openpal::ReadBufferView& hmacValue_
) : 
  challengeSeqNum(challengeSeqNum_),
  userNum(userNum_),
  hmacValue(hmacValue_)
{}

uint32_t Group120Var2::Size() const
{
  return MIN_SIZE + hmacValue.Size();
}

bool Group120Var2::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var2::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->challengeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);

  this->hmacValue = copy; // whatever is left over

  return true;
}

bool Group120Var2::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->challengeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);

  hmacValue.CopyTo(buffer);

  return true;
}

// ------- Group120Var3 -------

bool Group120Var3::Read(ReadBufferView& buffer, Group120Var3& output)
{
  return Parse::Many(buffer, output.challengeSeqNum, output.userNum);
}

bool Group120Var3::Write(const Group120Var3& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.challengeSeqNum, arg.userNum);
}

// ------- Group120Var4 -------

bool Group120Var4::Read(ReadBufferView& buffer, Group120Var4& output)
{
  return Parse::Many(buffer, output.userNum);
}

bool Group120Var4::Write(const Group120Var4& arg, openpal::WriteBufferView& buffer)
{
  return Format::Many(buffer, arg.userNum);
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
  const openpal::ReadBufferView& challengeData_,
  const openpal::ReadBufferView& hmacValue_
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
  return MIN_SIZE + challengeData.Size() + hmacValue.Size();
}

bool Group120Var5::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var5::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->keyChangeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);
  this->keyWrapAlgo = KeyWrapAlgorithmFromType(UInt8::ReadBuffer(copy));
  this->keyStatus = KeyStatusFromType(UInt8::ReadBuffer(copy));
  this->hmacAlgo = HMACTypeFromType(UInt8::ReadBuffer(copy));

  if(!PrefixFields::Read(copy, challengeData))
  {
    return false;
  }

  this->hmacValue = copy; // whatever is left over

  return true;
}

bool Group120Var5::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  if(challengeData.Size() > openpal::MaxValue<uint16_t>())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->keyChangeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);
  UInt8::WriteBuffer(buffer, KeyWrapAlgorithmToType(this->keyWrapAlgo));
  UInt8::WriteBuffer(buffer, KeyStatusToType(this->keyStatus));
  UInt8::WriteBuffer(buffer, HMACTypeToType(this->hmacAlgo));

  if(!PrefixFields::Write(buffer, challengeData))
  {
    return false;
  }

  hmacValue.CopyTo(buffer);

  return true;
}

// ------- Group120Var6 -------

Group120Var6::Group120Var6() : 
  keyChangeSeqNum(0), userNum(0)
{}

Group120Var6::Group120Var6(
  uint32_t keyChangeSeqNum_,
  uint16_t userNum_,
  const openpal::ReadBufferView& keyWrapData_
) : 
  keyChangeSeqNum(keyChangeSeqNum_),
  userNum(userNum_),
  keyWrapData(keyWrapData_)
{}

uint32_t Group120Var6::Size() const
{
  return MIN_SIZE + keyWrapData.Size();
}

bool Group120Var6::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var6::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->keyChangeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);

  this->keyWrapData = copy; // whatever is left over

  return true;
}

bool Group120Var6::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->keyChangeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);

  keyWrapData.CopyTo(buffer);

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
  const openpal::ReadBufferView& errorText_
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
  return MIN_SIZE + errorText.Size();
}

bool Group120Var7::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var7::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->challengeSeqNum = UInt32::ReadBuffer(copy);
  this->userNum = UInt16::ReadBuffer(copy);
  this->assocId = UInt16::ReadBuffer(copy);
  this->errorCode = AuthErrorCodeFromType(UInt8::ReadBuffer(copy));
  this->time = UInt48::ReadBuffer(copy);

  this->errorText = copy; // whatever is left over

  return true;
}

bool Group120Var7::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt32::WriteBuffer(buffer, this->challengeSeqNum);
  UInt16::WriteBuffer(buffer, this->userNum);
  UInt16::WriteBuffer(buffer, this->assocId);
  UInt8::WriteBuffer(buffer, AuthErrorCodeToType(this->errorCode));
  UInt48::WriteBuffer(buffer, this->time);

  errorText.CopyTo(buffer);

  return true;
}

// ------- Group120Var8 -------

Group120Var8::Group120Var8() : 
  keyChangeMethod(KeyChangeMethod::UNDEFINED), certificateType(CertificateType::UNKNOWN)
{}

Group120Var8::Group120Var8(
  KeyChangeMethod keyChangeMethod_,
  CertificateType certificateType_,
  const openpal::ReadBufferView& certificate_
) : 
  keyChangeMethod(keyChangeMethod_),
  certificateType(certificateType_),
  certificate(certificate_)
{}

uint32_t Group120Var8::Size() const
{
  return MIN_SIZE + certificate.Size();
}

bool Group120Var8::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var8::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->keyChangeMethod = KeyChangeMethodFromType(UInt8::ReadBuffer(copy));
  this->certificateType = CertificateTypeFromType(UInt8::ReadBuffer(copy));

  this->certificate = copy; // whatever is left over

  return true;
}

bool Group120Var8::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  UInt8::WriteBuffer(buffer, KeyChangeMethodToType(this->keyChangeMethod));
  UInt8::WriteBuffer(buffer, CertificateTypeToType(this->certificateType));

  certificate.CopyTo(buffer);

  return true;
}

// ------- Group120Var9 -------

Group120Var9::Group120Var9()
{}

Group120Var9::Group120Var9(
  const openpal::ReadBufferView& hmacValue_
) : 
  hmacValue(hmacValue_)
{}

uint32_t Group120Var9::Size() const
{
  return MIN_SIZE + hmacValue.Size();
}

bool Group120Var9::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var9::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->hmacValue = copy; // whatever is left over

  return true;
}

bool Group120Var9::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  hmacValue.CopyTo(buffer);

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
  const openpal::ReadBufferView& userName_,
  const openpal::ReadBufferView& userPublicKey_,
  const openpal::ReadBufferView& certificationData_
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
  return MIN_SIZE + userName.Size() + userPublicKey.Size() + certificationData.Size();
}

bool Group120Var10::Read(const ReadBufferView& buffer)
{
  if(buffer.Size() < Group120Var10::MIN_SIZE)
  {
    return false;
  }

  ReadBufferView copy(buffer); //mutable copy for parsing

  this->keyChangeMethod = KeyChangeMethodFromType(UInt8::ReadBuffer(copy));
  this->userOperation = UserOperationFromType(UInt8::ReadBuffer(copy));
  this->statusChangeSeqNum = UInt32::ReadBuffer(copy);
  this->userRole = UInt16::ReadBuffer(copy);
  this->userRoleExpDays = UInt16::ReadBuffer(copy);

  if(!PrefixFields::Read(copy, userName, userPublicKey, certificationData))
  {
    return false;
  }

  return true;
}

bool Group120Var10::Write(openpal::WriteBufferView& buffer) const
{
  if(this->Size() > buffer.Size())
  {
    return false;
  }

  if(userName.Size() > openpal::MaxValue<uint16_t>())
  {
    return false;
  }

  if(userPublicKey.Size() > openpal::MaxValue<uint16_t>())
  {
    return false;
  }

  if(certificationData.Size() > openpal::MaxValue<uint16_t>())
  {
    return false;
  }

  UInt8::WriteBuffer(buffer, KeyChangeMethodToType(this->keyChangeMethod));
  UInt8::WriteBuffer(buffer, UserOperationToType(this->userOperation));
  UInt32::WriteBuffer(buffer, this->statusChangeSeqNum);
  UInt16::WriteBuffer(buffer, this->userRole);
  UInt16::WriteBuffer(buffer, this->userRoleExpDays);

  if(!PrefixFields::Write(buffer, userName, userPublicKey, certificationData))
  {
    return false;
  }


  return true;
}


}
