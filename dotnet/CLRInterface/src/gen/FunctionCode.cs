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
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (https://stepfunc.io) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

namespace Step Function I/O.DNP3.Interface
{
  /// <summary>
  /// Application layer function code enumeration
  /// </summary>
  public enum FunctionCode : byte
  {
    /// <summary>
    /// Master sends this to an outstation to confirm the receipt of an Application Layer fragment
    /// </summary>
    CONFIRM = 0x0,
    /// <summary>
    /// Outstation shall return the data specified by the objects in the request
    /// </summary>
    READ = 0x1,
    /// <summary>
    /// Outstation shall store the data specified by the objects in the request
    /// </summary>
    WRITE = 0x2,
    /// <summary>
    /// Outstation shall select (or arm) the output points specified by the objects in the request in preparation for a subsequent operate command
    /// </summary>
    SELECT = 0x3,
    /// <summary>
    /// Outstation shall activate the output points selected (or armed) by a previous select function code command
    /// </summary>
    OPERATE = 0x4,
    /// <summary>
    /// Outstation shall immediately actuate the output points specified by the objects in the request
    /// </summary>
    DIRECT_OPERATE = 0x5,
    /// <summary>
    /// Same as DIRECT_OPERATE but outstation shall not send a response
    /// </summary>
    DIRECT_OPERATE_NR = 0x6,
    /// <summary>
    /// Outstation shall copy the point data values specified by the objects in the request to a separate freeze buffer
    /// </summary>
    IMMED_FREEZE = 0x7,
    /// <summary>
    /// Same as IMMED_FREEZE but outstation shall not send a response
    /// </summary>
    IMMED_FREEZE_NR = 0x8,
    /// <summary>
    /// Outstation shall copy the point data values specified by the objects in the request into a separate freeze buffer and then clear the values
    /// </summary>
    FREEZE_CLEAR = 0x9,
    /// <summary>
    /// Same as FREEZE_CLEAR but outstation shall not send a response
    /// </summary>
    FREEZE_CLEAR_NR = 0xA,
    /// <summary>
    /// Outstation shall copy the point data values specified by the objects in the request to a separate freeze buffer at the time and/or time intervals specified in a special time data information object
    /// </summary>
    FREEZE_AT_TIME = 0xB,
    /// <summary>
    /// Same as FREEZE_AT_TIME but outstation shall not send a response
    /// </summary>
    FREEZE_AT_TIME_NR = 0xC,
    /// <summary>
    /// Outstation shall perform a complete reset of all hardware and software in the device
    /// </summary>
    COLD_RESTART = 0xD,
    /// <summary>
    /// Outstation shall reset only portions of the device
    /// </summary>
    WARM_RESTART = 0xE,
    /// <summary>
    /// Obsolete-Do not use for new designs
    /// </summary>
    INITIALIZE_DATA = 0xF,
    /// <summary>
    /// Outstation shall place the applications specified by the objects in the request into the ready to run state
    /// </summary>
    INITIALIZE_APPLICATION = 0x10,
    /// <summary>
    /// Outstation shall start running the applications specified by the objects in the request
    /// </summary>
    START_APPLICATION = 0x11,
    /// <summary>
    /// Outstation shall stop running the applications specified by the objects in the request
    /// </summary>
    STOP_APPLICATION = 0x12,
    /// <summary>
    /// This code is deprecated-Do not use for new designs
    /// </summary>
    SAVE_CONFIGURATION = 0x13,
    /// <summary>
    /// Enables outstation to initiate unsolicited responses from points specified by the objects in the request
    /// </summary>
    ENABLE_UNSOLICITED = 0x14,
    /// <summary>
    /// Prevents outstation from initiating unsolicited responses from points specified by the objects in the request
    /// </summary>
    DISABLE_UNSOLICITED = 0x15,
    /// <summary>
    /// Outstation shall assign the events generated by the points specified by the objects in the request to one of the classes
    /// </summary>
    ASSIGN_CLASS = 0x16,
    /// <summary>
    /// Outstation shall report the time it takes to process and initiate the transmission of its response
    /// </summary>
    DELAY_MEASURE = 0x17,
    /// <summary>
    /// Outstation shall save the time when the last octet of this message is received
    /// </summary>
    RECORD_CURRENT_TIME = 0x18,
    /// <summary>
    /// Outstation shall open a file
    /// </summary>
    OPEN_FILE = 0x19,
    /// <summary>
    /// Outstation shall close a file
    /// </summary>
    CLOSE_FILE = 0x1A,
    /// <summary>
    /// Outstation shall delete a file
    /// </summary>
    DELETE_FILE = 0x1B,
    /// <summary>
    /// Outstation shall retrieve information about a file
    /// </summary>
    GET_FILE_INFO = 0x1C,
    /// <summary>
    /// Outstation shall return a file authentication key
    /// </summary>
    AUTHENTICATE_FILE = 0x1D,
    /// <summary>
    /// Outstation shall abort a file transfer operation
    /// </summary>
    ABORT_FILE = 0x1E,
    /// <summary>
    /// The master uses this function code when sending authentication requests to the outstation
    /// </summary>
    AUTH_REQUEST = 0x20,
    /// <summary>
    /// The master uses this function code when sending authentication requests to the outstation that do no require acknowledgement
    /// </summary>
    AUTH_REQUEST_NO_ACK = 0x21,
    /// <summary>
    /// Master shall interpret this fragment as an Application Layer response to an ApplicationLayer request
    /// </summary>
    RESPONSE = 0x81,
    /// <summary>
    /// Master shall interpret this fragment as an unsolicited response that was not prompted by an explicit request
    /// </summary>
    UNSOLICITED_RESPONSE = 0x82,
    /// <summary>
    /// The outstation uses this function code to issue authentication messages to the master
    /// </summary>
    AUTH_RESPONSE = 0x83,
    /// <summary>
    /// Unknown function code. Used internally in opendnp3 to indicate the code didn't match anything known
    /// </summary>
    UNKNOWN = 0xFF
  }
}
