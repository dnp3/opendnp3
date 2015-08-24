using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.Simulator.DNP3.Commons
{
    public static class InfiniteEnumerable
    {        
        public static IEnumerable<T> Create<T>(params T[] values)
        {
            int pos = 0;
            if (values.Length == 0)
            {
                yield break;
            }
            else
            {
                while (true)
                {
                    yield return values[pos];                    
                    pos = (pos+1) % values.Length;
                }
            }
        }
    


}
}
