using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.Linq;
using System.Text;
using System.Threading;

using System.IO;

namespace Automatak.Simulator.Commons
{
    public class LogToFile : ILog
    {
        readonly BlockingCollection<Action> actions = new BlockingCollection<Action>();
        readonly BlockingCollection<string> messages = new BlockingCollection<string>();

        bool shutdown = false;
        bool enabled = false;
        readonly Thread thread;
        string logFilePath = null;
        
        public LogToFile()
        {
            this.thread = new Thread(this.Run);
            thread.Start();
        }

        public String LogFilePath
        {
            set
            {
                this.actions.Add(() => this.ChangeFilePath(value));
            }
        }

        public bool Enabled
        {
            set
            {
                this.actions.Add(() => this.ChangeEnabledState(value));
            }
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
       
        void ChangeFilePath(string path)
        {
            this.logFilePath = path;
        }

        void ChangeEnabledState(bool value)
        {
            this.enabled = value;
        }

        void ReadLogMessages()
        {
            var lines = GetMessages();
            if(enabled && (logFilePath != null) && lines.Any())
            {
                WriteToFile(logFilePath, lines);
            }           
        }

        static bool WriteToFile(string path, IEnumerable<string> lines)
        {
            try
            {
                File.AppendAllLines(path, lines);
                return true;
            }
            catch(IOException)
            {
                return false;
            }
        }

        IEnumerable<string> GetMessages()
        {            
            if (messages.Count > 0)
            {
                var list = new List<string>(messages.Count);
                string msg;
                while (messages.TryTake(out msg))
                {
                    list.Add(msg);
                }
                return list;
            }
            else
            {
                return Enumerable.Empty<string>();
            }              
        }

        void EnqueueMessage(string msg)
        {
            this.messages.Add(msg);
            this.actions.Add(() => this.ReadLogMessages());
        }


        
    }
}
