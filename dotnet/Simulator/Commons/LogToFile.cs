using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Text;
using System.Threading;


namespace Automatak.Simulator.Commons
{
    public class LogToFile : ILog
    {
        readonly BlockingCollection<Action> actions = new BlockingCollection<Action>();

        bool shutdown = false;
        readonly Thread thread;

        public LogToFile()
        {
            this.thread = new Thread(this.Run);
            thread.Start();
        }
        

        public void Shutdown()
        {
            this.actions.Add(() => this.shutdown = true);
            this.thread.Join();
        }
       
        void ILog.Log(DisplayHint hint, string message)
        {             
            this.EnqueueMessage(LogFormatter.GetBareString(message));            
        }

        void ILog.LogFull(DisplayHint hint, string filter, string alias, string message)
        {
            this.EnqueueMessage(LogFormatter.GetLogString(filter, alias, message));            
        }

        void Run()
        {
            while (!shutdown)
            {
                actions.Take().Invoke();
            }
        }        

        void LogMessage(string message)
        {
            Console.WriteLine(String.Format("Message from thread: {0}", message));
        }        

        void EnqueueMessage(string msg)
        {
            this.actions.Add(() => this.LogMessage(msg));
        }


        
    }
}
