using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;

namespace Automatak.Simulator.API
{
    public interface ISimulatorNodeCallbacks
    {
        void ChangeImage(int index);
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
