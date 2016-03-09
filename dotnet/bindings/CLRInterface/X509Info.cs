using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public class X509Info
    {
        public X509Info(byte[] sha1fingerprint, string subjectName)
        {
            this.sha1fingerprint = sha1fingerprint;
            this.subjectName = subjectName;
        }

        readonly byte[] sha1fingerprint;
        readonly string subjectName;
    }
}
