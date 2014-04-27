using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Collections.ObjectModel;

namespace Automatak.DNP3.Simulator
{
    interface IMeasurementObserver
    { 
        void Refresh(IEnumerable<Measurement> rows);

        void Update(Measurement meas);   
    }   

    class MeasurementCollection
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
                foreach (var o in observers) o.Update(meas);
            }
        }  
        
        
    }
}
