using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

using Automatak.DNP3.Interface;

namespace Automatak.Simulator.DNP3
{
    class StringCallback : ITaskCallback
    {
        readonly TaskCompletionSource<TaskCompletion> tcs = new TaskCompletionSource<TaskCompletion>();

        public Task<TaskCompletion> Task
        {
            get
            {
                return tcs.Task;
            }
        }

        void ITaskCallback.OnStart()
        {
            
        }

        void ITaskCallback.OnComplete(TaskCompletion result)
        {
            tcs.SetResult(result);
        }

        void ITaskCallback.OnDestroyed()
        {
            
        }
    }
}
