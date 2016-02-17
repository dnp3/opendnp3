using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    class MasterSessionProxy : IMasterSession
    {
        readonly UInt64 sessionid;
        readonly MasterSessionCache cache;

        public MasterSessionProxy(MasterSessionCache cache, UInt64 sessionid)
        {
            this.sessionid = sessionid;
            this.cache = cache;
        }

        void IMasterSession.BeginShutdown()
        {
            cache.Perform(sessionid, (m) => m.BeginShutdown());
        }

        IStackStatistics IMasterOperations.GetStackStatistics()
        {
            return cache.Perform(sessionid, (m) => m.GetStackStatistics());
        }

        IMasterScan IMasterOperations.AddAllObjectsScan(byte group, byte variation, TimeSpan period, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.AddAllObjectsScan(group, variation, period, config));
        }

        IMasterScan IMasterOperations.AddClassScan(ClassField field, TimeSpan period, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.AddClassScan(field, period, config));
        }

        IMasterScan IMasterOperations.AddRangeScan(byte group, byte variation, ushort start, ushort stop, TimeSpan period, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.AddRangeScan(group, variation, start, stop, period, config));
        }

        IMasterScan IMasterOperations.AddScan(IEnumerable<Header> headers, TimeSpan period, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.AddScan(headers, period, config));
        }

        Task<TaskCompletion> IMasterOperations.ScanAllObjects(byte group, byte variation, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.ScanAllObjects(group, variation, config));
        }

        Task<TaskCompletion> IMasterOperations.ScanClasses(ClassField field, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.ScanClasses(field, config));
        }

        Task<TaskCompletion> IMasterOperations.ScanRange(byte group, byte variation, ushort start, ushort stop, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.ScanRange(group, variation, start, stop, config));
        }

        Task<TaskCompletion> IMasterOperations.Scan(IEnumerable<Header> headers, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.Scan(headers, config));
        }

        Task<TaskCompletion> IMasterOperations.Write(TimeAndInterval value, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.Write(value, index, config));
        }

        Task<RestartResultType> IMasterOperations.Restart(RestartType restartType, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.Restart(restartType, config));
        }

        Task<TaskCompletion> IMasterOperations.PerformFunction(string name, FunctionCode func, IEnumerable<Header> headers, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.PerformFunction(name, func, headers, config));
        }

        Task<CommandTaskResult> ICommandProcessor.SelectAndOperate(ICommandHeaders headers, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.SelectAndOperate(headers, config));
        }

        Task<CommandTaskResult> ICommandProcessor.DirectOperate(ICommandHeaders headers, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.DirectOperate(headers, config));
        }

        Task<CommandTaskResult> ICommandProcessor.SelectAndOperate(ControlRelayOutputBlock command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.SelectAndOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.DirectOperate(ControlRelayOutputBlock command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.DirectOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.SelectAndOperate(AnalogOutputInt16 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.SelectAndOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.DirectOperate(AnalogOutputInt16 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.DirectOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.SelectAndOperate(AnalogOutputInt32 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.SelectAndOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.DirectOperate(AnalogOutputInt32 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.DirectOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.SelectAndOperate(AnalogOutputFloat32 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.SelectAndOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.DirectOperate(AnalogOutputFloat32 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.DirectOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.SelectAndOperate(AnalogOutputDouble64 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.SelectAndOperate(command, index, config));
        }

        Task<CommandTaskResult> ICommandProcessor.DirectOperate(AnalogOutputDouble64 command, ushort index, TaskConfig config)
        {
            return cache.Perform(sessionid, (m) => m.DirectOperate(command, index, config));
        }
    }
}
