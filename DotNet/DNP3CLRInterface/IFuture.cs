using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{    
    public delegate void FutureCallback<T>(T value);

    public interface IFuture<T>
    {
        T Await();
        void Listen(FutureCallback<T> callback);
    }

    public class Future<T> : IFuture<T>
    {
        private bool set = false;
        private T value = default(T);
        private Object mutex = new Object();
        private List<FutureCallback<T>> listeners = new List<FutureCallback<T>>();

        public T Await()
        {
            lock (mutex)
            {
                if (set) return value;
                else 
                {
                    System.Threading.Monitor.Wait(mutex);
                    return value;
                }
            }
        }

        public void Listen(FutureCallback<T> callback)
        {
            lock (mutex)
            {
                if (set) callback(value);
                else {
                    listeners.Add(callback);
                }
            }
        }

        public void Set(T value)
        {
            lock (mutex)
            {
                if (set) throw new Exception("Value has already been set");
                else {
                    set = true;
                    this.value = value;
                    System.Threading.Monitor.PulseAll(mutex);
                    foreach (FutureCallback<T> fc in listeners) { fc(this.value); }
                }
            }
        }

    }
}
