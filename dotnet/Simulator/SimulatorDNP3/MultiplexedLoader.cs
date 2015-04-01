using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    class MultiplexedLoader : IMeasurementLoader
    {
        IEnumerable<IMeasurementLoader> loaders;

        public MultiplexedLoader(params IMeasurementLoader[] loaders)
        {
            this.loaders = new List<IMeasurementLoader>(loaders);
        }              

        void IMeasurementLoader.Load(IChangeSet updates)
        {
            foreach (var loader in loaders)
            {
                loader.Load(updates);
            }
        }
    }
}
