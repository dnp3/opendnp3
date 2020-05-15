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
    /// Callbacks for a channel
    /// </summary>
    public interface IChannelListener
    {
        /// <summary>
        /// Invoked when the ChannelState changes
        /// </summary>
        /// <param name="state">the new state of the channel</param>
        void OnStateChange(ChannelState state);
    }

    public static class ChannelListener
    {
        private class PrintingChannelListener : IChannelListener
        {
            void IChannelListener.OnStateChange(ChannelState state)
            {
                Console.WriteLine("Channel state: " + state);
            }
        }

        private class NullChannelListener : IChannelListener
        {
            void IChannelListener.OnStateChange(ChannelState state) { }            
        }

        public static IChannelListener Print()
        {
            return new PrintingChannelListener();
        }

        public static IChannelListener None()
        {
            return new NullChannelListener();
        }
    }
}
