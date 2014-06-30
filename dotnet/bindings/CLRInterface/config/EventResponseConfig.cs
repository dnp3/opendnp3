using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    public class EventResponseConfig
    {
        /// <summary>
        /// The default group/variation to use for event binary responses
        /// </summary>	    
        public EventBinaryResponse binary = EventBinaryResponse.Group2Var1;

        /// <summary>
        /// The default group/variation to use for event double binary responses
        /// </summary>	    
        public EventDoubleBinaryResponse doubleBinary = EventDoubleBinaryResponse.Group4Var1;

        /// <summary>
        /// The default group/variation to use for event binary output status responses
        /// </summary>	    
        public EventBinaryOutputStatusResponse binaryOutputStatus = EventBinaryOutputStatusResponse.Group11Var1;

        /// <summary>
        /// The default group/variation to use for event analog responses
        /// </summary>
        public EventCounterResponse counter = EventCounterResponse.Group22Var1;

        /// <summary>
        /// The default group/variation to use for event analog responses
        /// </summary>
        public EventFrozenCounterResponse frozenCounter = EventFrozenCounterResponse.Group23Var1;

        /// <summary>
        /// The default group/variation to use for event analog responses
        /// </summary>
        public EventAnalogResponse analog = EventAnalogResponse.Group32Var1;

        /// <summary>
        /// The default group/variation to use for event binary output status responses
        /// </summary>	    
        public EventAnalogOutputStatusResponse analogOutputStatus = EventAnalogOutputStatusResponse.Group42Var1;
    }
}
