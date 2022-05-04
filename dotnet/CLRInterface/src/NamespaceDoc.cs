// Copyright 2013-2022 Step Function I/O, LLC
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (www.automatak.com) under one or more contributor license agreements. 
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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
using System.Threading.Tasks;

namespace Step Function I/O.DNP3.Interface
{
    /// <summary>
    ///   <para>
    ///     Public API of the opendnp3 CLR bindings.
    ///     
    ///     The DNP3.Adapter namespace contains 1 exposed factory class for obtaining a concrete implementation of IDNP3Manager.
    ///     
    ///     <code>
    /// using DNP3.Adapter;
    /// using DNP3.Interface;
    /// 
    /// class Program
    /// {
    ///    static void Main(string[] args)
    ///    {
    ///        //obtain an IDNP3Manager by using the factory class from the DNP3.Adapter namespace
    ///        IDNP3Manager mgr = DNP3ManagerFactory.CreateManager();
    ///        
    ///        // proceed to add channels and stacks .... checkout the wiki for details
    ///    }
    /// }
    ///     </code>
    ///   </para>
    /// </summary>        
    internal class NamespaceDoc
    {
    }
}
