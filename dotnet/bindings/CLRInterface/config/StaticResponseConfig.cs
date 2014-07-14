using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    public class StaticResponseConfig
    {
        /// <summary>
        /// The default group/variation to use for static binary responses
        /// </summary>	    
        public StaticBinaryResponse binary = StaticBinaryResponse.Group1Var2;

        /// <summary>
        /// The default group/variation to use for static double binary responses
        /// </summary>	    
        public StaticDoubleBinaryResponse doubleBinary = StaticDoubleBinaryResponse.Group3Var2;

        /// <summary>
        /// The default group/variation to use for static binary output status responses
        /// </summary>	    
        public StaticBinaryOutputStatusResponse binaryOutputStatus = StaticBinaryOutputStatusResponse.Group10Var2;

        /// <summary>
        /// The default group/variation to use for static analog responses
        /// </summary>
        public StaticCounterResponse counter = StaticCounterResponse.Group20Var1;

        /// <summary>
        /// The default group/variation to use for static analog responses
        /// </summary>
        public StaticFrozenCounterResponse frozenCounter = StaticFrozenCounterResponse.Group21Var1;

        /// <summary>
        /// The default group/variation to use for static analog responses
        /// </summary>
        public StaticAnalogResponse analog = StaticAnalogResponse.Group30Var1;

        /// <summary>
        /// The default group/variation to use for static binary output status responses
        /// </summary>	    
        public StaticAnalogOutputStatusResponse analogOutputStatus = StaticAnalogOutputStatusResponse.Group40Var1;   
    }
}
