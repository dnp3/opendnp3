using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
   public class ChannelStatistics : IChannelStatistics
   {
       /// <summary>
       /// The number of times the channel has sucessfully opened
       /// </summary>
       public System.UInt32 numOpen = 0;

       /// <summary>
       /// The number of times the channel has failed to open
       /// </summary>
       public System.UInt32 numOpenFail = 0;

       /// <summary>
       /// The number of times the channel has closed either due to user intervention or an error
       /// </summary>
       public System.UInt32 numClose = 0;

       /// <summary>
       /// The number of bytes received
       /// </summary>
       public System.UInt32 numBytesRx = 0;

       /// <summary>
       /// The number of bytes transmitted
       /// </summary>
       public System.UInt32 numBytesTx = 0;

       /// <summary>
       /// Number of LPDUs discared due to CRC errors
       /// </summary>
       public System.UInt32 numCrcError = 0;

       /// <summary>
       /// Number of valid LPDUs received
       /// </summary>
       public System.UInt32 numLinkFrameRx = 0;

       /// <summary>
       /// Number of LPDUs transmitted
       /// </summary>
       public System.UInt32 numLinkFrameTx = 0;

       /// <summary>
       /// Number of LPDUs detected with bad / malformed contents
       /// </summary>
       public System.UInt32 numBadLinkFrameRx = 0;     

       uint IChannelStatistics.NumOpen
       {
           get { return numOpen; }
       }

       uint IChannelStatistics.NumOpenFail
       {
           get { return numOpenFail; }
       }

       uint IChannelStatistics.NumClose
       {
           get { return numClose; }
       }

       uint IChannelStatistics.NumBytesRx
       {
           get { return numBytesRx; }
       }

       uint IChannelStatistics.NumBytesTx
       {
           get { return numBytesTx; }
       }

       uint IChannelStatistics.NumCrcError
       {
           get { return numCrcError; }
       }

       uint IChannelStatistics.NumLinkFrameRx
       {
           get { return numLinkFrameRx; }
       }

       uint IChannelStatistics.NumLinkFrameTx
       {
           get { return numLinkFrameTx; }
       }

       uint IChannelStatistics.NumBadLinkFrameRx
       {
           get { return numBadLinkFrameRx; }
       }      
   }
}
