using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    public interface IMeasurementCollection<T>
    {
        void Foreach(Action<T> callback);

        UInt32 Count
        {
            get;
        }
    }
}
