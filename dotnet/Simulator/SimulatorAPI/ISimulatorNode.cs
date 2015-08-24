using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;

namespace Automatak.Simulator.API
{
    public enum NodeState
    {
        Inactive,
        Waiting,
        Active,
        Failed
    }

    public interface ISimulatorNodeCallbacks
    {
        

        void ChangeImage(int index);

        void ChangeState(NodeState state);
    }

    public interface ISimulatorNodeAction
    {
        string DisplayName
        {
            get;
        }

        void Invoke();
    }

    public interface ISimulatorNodeFactory
    {
        string DisplayName
        {
            get;
        }

        ISimulatorNode Create(ISimulatorNodeCallbacks callbacks);
    }
    
    public interface ISimulatorNode
    {
        void Remove();

        string Alias
        {
            get;
        }       

        IEnumerable<Metric> Metrics
        {
            get;
        }
                
        String DisplayName
        {
            get;
        }

        IEnumerable<ISimulatorNodeFactory> Children
        {
            get;
        }

        IEnumerable<ISimulatorNodeAction> Actions
        {
            get;
        }
    }
    
}
