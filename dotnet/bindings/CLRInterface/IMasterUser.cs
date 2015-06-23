
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

namespace Automatak.DNP3.Interface
{
    public class UpdateKey
    {
        public UpdateKey(byte[] key)
        {
            this.keyMode = GetModeFromKeyLength(key.Length);
            this.key = key;            
        }

        static UpdateKeyMode GetModeFromKeyLength(int length)
        {
            switch (length)
            { 
                case(16):
                    return UpdateKeyMode.AES128;
                case(32):
                    return UpdateKeyMode.AES128;
                default:
                    throw new ArgumentException(String.Format("Invalid update key length: {0}", length));
            }
        }
        
        public readonly UpdateKeyMode keyMode;
        public readonly byte[] key;        
    };


    /// <summary>
    /// Straw-man interface for a single-user SA enabled master
    /// </summary>
    public interface IMasterUser
    {
        User UserNumber
        {
            get;
        }

        UpdateKey UpdateKey
        {
            get;
        }
    }

    public class SimpleMasterUser : IMasterUser
    {
        readonly UpdateKey key;
        readonly User user;

        public SimpleMasterUser(User user, byte[] key)
        {
            this.user = user;
            this.key = new UpdateKey(key);
        }

        UpdateKey IMasterUser.UpdateKey
        {
            get { return key; }
        }     

        User IMasterUser.UserNumber
        {
            get { return user; }
        }
    }

   
}
