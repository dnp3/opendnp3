using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
   public class StackStatistics : IStackStatistics
   {
       public System.UInt32 numTransportRx = 0;
       public System.UInt32 numTransportTx = 0;
       public System.UInt32 numTransportErrorRx = 0;

       uint IStackStatistics.NumTransportRx
       {
           get { return numTransportRx; }
       }

       uint IStackStatistics.NumTransportTx
       {
           get { return numTransportTx; }
       }

       uint IStackStatistics.NumTransportErrorRx
       {
           get { return numTransportErrorRx; }
       }
   }
}
