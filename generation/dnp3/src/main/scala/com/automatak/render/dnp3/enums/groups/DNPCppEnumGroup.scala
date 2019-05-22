/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.render.dnp3.enums.groups

import com.automatak.render.cpp.EnumConfig
import com.automatak.render.dnp3.enums._


object DNPCppEnumGroup {

  def enums : List[EnumConfig] = List(fullEnums, simpleEnums, stringOnlyEnums).flatten

  private def fullEnums = List(
    FunctionCode(),
    QualifierCode(),
    LinkFunction(),
    IntervalUnit(),
    ControlCode(),
    GroupVariationEnum(),
    DoubleBit(),
    CommandStatus(),
    HMACType(),
    ChallengeReason(),
    KeyWrapAlgorithm(),
    KeyStatus(),
    AuthErrorCode(),
    CertificateType(),
    KeyChangeMethod(),
    UserOperation(),
    UserRole(),
    Parity(),
    StopBits(),
    FlowControl(),
    FlagsType()
  ).map(x => EnumConfig(x, true, true))

  private def simpleEnums = (List(
    AssignClassType(),
    StaticTypeBitmask(),
    PointClass(),
    TimeSyncMode(),
    RestartMode(),
    TimestampMode(),
    EventMode(),
    IndexMode(),
    ConfigAuthMode(),
    SecurityStatIndex(),
    RestartType(),
    OperateType(),
    ServerAcceptMode(),
    IndexQualifierMode()
  ) ::: DefaultVariations.enums ::: QualityMasks.enums).map(x => EnumConfig(x, false, false))

  private def stringOnlyEnums = List(
    MasterTaskType(),
    TaskCompletion(),
    ChannelState(),
    LinkStatus(),
    CommandPointState()).map(x => EnumConfig(x, false, true))

}
