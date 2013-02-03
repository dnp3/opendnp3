using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    public interface IMaster: IStack
    {
        ICommandProcessor GetCommandProcessor();
    }
}
