using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Automatak.DNP3.Interface
{
   public interface IStackStatistics
   {    
       /// <summary>
       /// Number of valid TPDUs received
       /// </summary>
       System.UInt32 NumTransportRx { get; }

       /// <summary>
       /// Number of TPDUs transmitted
       /// </summary>
       System.UInt32 NumTransportTx { get; }

       /// <summary>
       /// Number of TPDUs dropped due to error
       /// </summary>
       System.UInt32 NumTransportErrorRx { get; }       
   }
}
