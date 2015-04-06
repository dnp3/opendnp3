using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Collections.ObjectModel;

namespace Automatak.Simulator.DNP3.Commons
{
    public interface IMeasurementObserver
    { 
        void Refresh(IEnumerable<Measurement> rows);

        void Update(Measurement meas);   
    }   

    public class MeasurementCollection
    {
        private IDictionary<ushort, Measurement> map = new SortedDictionary<ushort, Measurement>();

        private IList<IMeasurementObserver> observers = new List<IMeasurementObserver>();

        public void AddObserver(IMeasurementObserver observer)
        {
            lock (observers)
            {
                this.observers.Add(observer);
                observer.Refresh(map.Values);
            }
        }

        public void RemoveObserver(IMeasurementObserver observer)
        {
            lock (observers)
            {
                this.observers.Remove(observer);
            }
        }

        public void Update(Measurement meas)
        {
            lock (observers)
            {
                map[meas.Index] = meas;
                foreach (var o in observers)
                {
                    o.Update(meas);
                }
            }
        }

        public void Update(IEnumerable<Measurement> meas)
        {
            lock (observers)
            {
                foreach (var m in meas)
                {
                    map[m.Index] = m;
                    foreach (var o in observers)
                    {
                        o.Update(m);
                    }
                }                
            }
        }
        
    }
}
