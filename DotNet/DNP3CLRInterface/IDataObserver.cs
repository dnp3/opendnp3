using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace DNP3.Interface
{
    public interface IDataObserver
	{
		void Start();
		void Update(Binary update, System.UInt32 index);
		void Update(Analog update, System.UInt32 index);
		void Update(Counter update, System.UInt32 index);
		void Update(ControlStatus update, System.UInt32 index);
		void Update(SetpointStatus update, System.UInt32 index);
		void End();
	}
}
