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

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Various callbacks that can be received from the link layer
    /// </summary>
    public interface ILinkStatusListener
    {
        /// Called when a the reset/unreset status of the link layer changes
        void OnStateChange(LinkStatus value);

        /// Called when the link-layer receives any frame with an unknown destination address
        void OnUnknownDestinationAddress(ushort destination);

        /// Called when the link-layer receives any frame from an unknown source address
        void OnUnknownSourceAddress(ushort source);

        /// Called when a keep alive message is transmitted
        void OnKeepAliveInitiated();

        /// Called when a keep alive message (request link status) receives no response
        void OnKeepAliveFailure();

        /// Called when a keep alive message receives a valid response
        void OnKeepAliveSuccess();
    }
}
