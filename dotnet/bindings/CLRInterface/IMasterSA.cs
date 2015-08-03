
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Extends the normal master interface with SA features
    /// </summary>
    public interface IMasterSA : IMaster
    {
        /// <summary>
        /// Add a user to the outstation, specifing the users update key.
        /// If the user already exists, the user's session is immediately invalidated.
        /// </summary>
        /// <param name="user"></param>
        /// <param name="key"></param>
        void AddUser(User user, UpdateKey key);

        /// <summary>        
        /// Begin the task of changing a user's status on the outstation.
        /// No data is returned so the success or failure of the task can be established using the generic
        /// TaskCompletion enumeration.        
        /// </summary>
        /// <param name="change"></param>
        /// <param name="config"></param>
        /// <returns></returns>
        Task<TaskCompletion> ChangeUserStatus(UserStatusChange change, TaskConfig config);

        /// <summary>
        /// 
        /// Begin the process of changing a user's update keys. This first exchange does several things
        /// 
        /// 1) send a username and get back a user # that the outstation will assign if mutual authentication is established
        /// 2) Specify the nonce that the master is using and retrieve the nonce that the 
        /// 3) Get back a key change sequence number used by the outstation for this exchange
        /// 
        /// For now, the key change method is implicitly AES_256_SHA256_HMAC
        /// 
        /// </summary>
        /// <returns>An object that represents failure or succes + data</returns>        
        Task<BeginUpdateKeyChangeResult> BeginUpdateKeyChange(string username);
    }
}
