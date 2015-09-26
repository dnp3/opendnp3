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
   
        public TLSConfig(
            string peerCertFilePath,
            string localCertFilePath,
            string privateKeyFilePath
        )
        {
            this.peerCertFilePath = peerCertFilePath;
            this.localCertFilePath = localCertFilePath;
            this.privateKeyFilePath = privateKeyFilePath;

            this.allowTLSv10 = true;
            this.allowTLSv11 = true;
            this.allowTLSv12 = true;
        }

        public readonly string peerCertFilePath;
        public readonly string localCertFilePath;
        public readonly string privateKeyFilePath;
       

        #region private members

        /// Allow TLS version 1.0 (default true)
        public bool allowTLSv10;

        /// Allow TLS version 1.1 (default true)
        public bool allowTLSv11;

        /// Allow TLS version 1.2 (default true)
        public bool allowTLSv12;

        #endregion
    }
}
