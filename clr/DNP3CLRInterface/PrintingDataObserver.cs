using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    public class PrintingDataObserver : IDataObserver
    {
        private static readonly IDataObserver instance = new PrintingDataObserver();

        public static IDataObserver Instance
        {
            get
            {
                return instance;
            }
        }

        private PrintingDataObserver()
        { }

        public void End()
        {
            Console.WriteLine("End");
        }

        public void Start()
        {
            Console.WriteLine("Start");
        }

        public void Update(Binary update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(Analog update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(Counter update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(ControlStatus update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }

        public void Update(SetpointStatus update, System.UInt32 index)
        {
            Console.WriteLine(update.value);
        }
    }    
}
