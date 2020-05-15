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
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public class TLSConfig
    {
        /// <summary>
        /// Construct a TLS configuration. All files are in PEM format.	    
        /// </summary>
        /// <param name="peerCertFilePath">Certificate file used to verify the server. Can be CA file or a self - signed cert provided by other party</param>
        /// <param name="localCertFilePath">File that contains the certificate that will be presented to the remote side of the connection</param>
        /// <param name="privateKeyFilePath">File that contains the private key corresponding to the local certificate</param>        
        /// <param name="allowTLSv10">Allow TLS version 1.0 (default true)</param>
        /// <param name="allowTLSv11">Allow TLS version 1.1 (default true)</param>
        /// <param name="allowTLSv12">Allow TLS version 1.2 (default true)</param>
        /// <param name="allowTLSv13">Allow TLS version 1.3 (default true)</param>
        /// <param name="cipherList">List of ciphers in openssl's format. Defaults to empty string which will use the default cipher list</param>

        public TLSConfig(
            string peerCertFilePath,
            string localCertFilePath,
            string privateKeyFilePath,            
            bool allowTLSv10 = false,
            bool allowTLSv11 = false,
            bool allowTLSv12 = true,
            bool allowTLSv13 = true,
            string cipherList = ""
        )
        {
            this.peerCertFilePath = peerCertFilePath;
            this.localCertFilePath = localCertFilePath;
            this.privateKeyFilePath = privateKeyFilePath;            
            this.allowTLSv10 = allowTLSv10;
            this.allowTLSv11 = allowTLSv11;
            this.allowTLSv12 = allowTLSv12;
            this.allowTLSv13 = allowTLSv13;
            this.cipherList = cipherList;            
        }

        public readonly string peerCertFilePath;
        public readonly string localCertFilePath;
        public readonly string privateKeyFilePath;               

        /// Allow TLS version 1.0 (default false)
        public readonly bool allowTLSv10;

        /// Allow TLS version 1.1 (default false)
        public readonly bool allowTLSv11;

        /// Allow TLS version 1.2 (default true)
        public readonly bool allowTLSv12;

        /// Allow TLS version 1.3 (default true)
        public readonly bool allowTLSv13;

        public readonly string cipherList;
    }
}
