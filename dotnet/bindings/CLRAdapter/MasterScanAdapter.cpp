
#include "Stdafx.h"
#include "MasterScanAdapter.h"

#include "ScanListenerAdapter.h"

using namespace Automatak::DNP3::Interface;
using namespace System::Collections::Generic;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			MasterScanAdapter::MasterScanAdapter(const opendnp3::MasterScan& scan)
			{
				pScan = new opendnp3::MasterScan(scan);
				pAdapter = new ScanListenerAdapter(this);
				pScan->SetStateListener(*pAdapter);
				listeners = gcnew List<System::Action<PollState>^>();
			}

			MasterScanAdapter::~MasterScanAdapter()
			{
				delete pScan;
				delete pAdapter;
			}

			void MasterScanAdapter::AddScanCallback(System::Action<PollState>^ callback)
			{
				listeners->Add(callback);
			}

			void MasterScanAdapter::OnStateChane(Automatak::DNP3::Interface::PollState state)
			{
				for each(auto callback in listeners)
				{
					callback->Invoke(state);
				}
			}

			void MasterScanAdapter::Demand()
			{
				pScan->Demand();
			}

		}
	}
}
