#ifndef __MASTER_SCAN_H_
#define __MASTER_SCAN_H_

namespace openpal { class IExecutor; }

namespace opendnp3
{
	class AsyncTaskBase;

	class MasterScan
	{
		public:
			MasterScan(openpal::IExecutor* apExecutor, AsyncTaskBase* apTask);
			
			// Request that the scan be performed ASAP
			void Demand();

		private:
			MasterScan();
			openpal::IExecutor* mpExecutor;
			AsyncTaskBase* mpTask;
	};
}

#endif
