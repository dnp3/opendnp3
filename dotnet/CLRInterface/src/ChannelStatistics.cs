// Copyright 2013-2020 Automatak, LLC
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements. 
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Interface
{
   /// <summary>
   /// Statistics for a channel
   /// </summary>
   public class ChannelStatistics : IChannelStatistics
   {
       /// <summary>
       /// The number of times the channel has sucessfully opened
       /// </summary>
       public System.UInt64 numOpen = 0;

       /// <summary>
       /// The number of times the channel has failed to open
       /// </summary>
       public System.UInt64 numOpenFail = 0;

       /// <summary>
       /// The number of times the channel has closed either due to user intervention or an error
       /// </summary>
       public System.UInt64 numClose = 0;

       /// <summary>
       /// The number of bytes received
       /// </summary>
       public System.UInt64 numBytesRx = 0;

       /// <summary>
       /// The number of bytes transmitted
       /// </summary>
       public System.UInt64 numBytesTx = 0;

       /// <summary>
       /// Number of LPDUs discared due to CRC errors
       /// </summary>
       public System.UInt64 numCrcError = 0;

       /// <summary>
       /// Number of valid LPDUs received
       /// </summary>
       public System.UInt64 numLinkFrameRx = 0;

       /// <summary>
       /// Number of LPDUs transmitted
       /// </summary>
       public System.UInt64 numLinkFrameTx = 0;

       /// <summary>
       /// Number of LPDUs detected with bad / malformed contents
       /// </summary>
       public System.UInt64 numBadLinkFrameRx = 0;

       /// <summary>
       /// The number of times the channel has sucessfully opened
       /// </summary>
       UInt64 IChannelStatistics.NumOpen
       {
           get { return numOpen; }
       }

        /// <summary>
        /// The number of times the channel has failed to open
        /// </summary>
       UInt64 IChannelStatistics.NumOpenFail
       {
           get { return numOpenFail; }
       }

        /// <summary>
        /// The number of times the channel has closed either due to user intervention or an error
        /// </summary>
       UInt64 IChannelStatistics.NumClose
       {
           get { return numClose; }
       }

       /// <summary>
       /// The number of bytes received
       /// </summary>
       UInt64 IChannelStatistics.NumBytesRx
       {
           get { return numBytesRx; }
       }

       /// <summary>
       /// The number of bytes transmitted
       /// </summary>
       UInt64 IChannelStatistics.NumBytesTx
       {
           get { return numBytesTx; }
       }

       /// <summary>
       /// Number of frames discared due to CRC errors
       /// </summary>
       UInt64 IChannelStatistics.NumCrcError
       {
           get { return numCrcError; }
       }

       /// <summary>
       /// Number of valid LPDUs received
       /// </summary>
       UInt64 IChannelStatistics.NumLinkFrameRx
       {
           get { return numLinkFrameRx; }
       }

       /// <summary>
       /// Number of valid LPDUs transmitted
       /// </summary>
       UInt64 IChannelStatistics.NumLinkFrameTx
       {
           get { return numLinkFrameTx; }
       }

       /// <summary>
       /// Number of LPDUs detected with bad / malformed contents
       /// </summary>
       UInt64 IChannelStatistics.NumBadLinkFrameRx
       {
           get { return numBadLinkFrameRx; }
       }      
   }
}
