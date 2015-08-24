using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Automatak.DNP3.Interface;
using Automatak.Simulator.DNP3.API;

namespace Automatak.Simulator.DNP3
{
    interface IDNP3Config
    {
        void AddTemplate(string alias, DatabaseTemplate template);

        DatabaseTemplate GetTemplateMaybeNull(string alias);

        IEnumerable<KeyValuePair<string, DatabaseTemplate>> Templates
        {
            get;
        }

        IEnumerable<IOutstationModule> OutstationModules
        {
            get;
        }

        bool ContainsTemplateId(string alias);
        
    }
}
