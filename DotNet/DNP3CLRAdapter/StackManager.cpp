// This is the main DLL file.

#include "StackManager.h"

#include "Conversions.h"
#include "MasterDataObserverAdapter.h"
#include "CommandAcceptorAdapter.h"

#include "SlaveCommandAcceptorAdapter.h"
#include "SlaveDataObserverAdapter.h"

#include "LogAdapter.h"

#include <DNP3/MasterStackConfig.h>
#include <DNP3/SlaveStackConfig.h>
#include <DNP3/StackManager.h>

namespace DNP3
{	
namespace Adapter
{

	StackManager::StackManager() :
		pMgr(new apl::dnp::StackManager())
	{
		
	}

	void StackManager::AddTCPClient(System::String^ name, FilterLevel level, System::UInt64 retryMs, System::String^ address, System::UInt16 port)
	{
		
		std::string stdName = Conversions::convertString(name);
		std::string stdAddress = Conversions::convertString(address);
		uint16_t stdPort = port;
		apl::PhysLayerSettings pls(Conversions::convertFilterLevel(level), retryMs);

		try {
			pMgr->AddTCPClient(stdName, pls, stdAddress, stdPort);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	void StackManager::AddTCPServer(System::String^ name, FilterLevel level, System::UInt64 retryMs, System::String^ endpoint, System::UInt16 port)
	{
		std::string stdName = Conversions::convertString(name);
		std::string stdEndpoint = Conversions::convertString(endpoint);		
		uint16_t stdPort = port;

		apl::PhysLayerSettings pls(Conversions::convertFilterLevel(level), retryMs);
		
		try {
			pMgr->AddTCPServer(stdName, pls, stdEndpoint, stdPort);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	void StackManager::AddSerial(System::String^ name, FilterLevel level, System::UInt64 retryMs, SerialSettings^ settings)
	{
		std::string stdName = Conversions::convertString(name);
		apl::PhysLayerSettings pls(Conversions::convertFilterLevel(level), retryMs);
		apl::SerialSettings s = Conversions::convertSerialSettings(settings);

		pMgr->AddSerial(stdName, pls, s);
	}
		
	ICommandAcceptor^ StackManager::AddMaster(	System::String^ portName,
												System::String^ stackName,	                            
												FilterLevel level,
												DNP3::Interface::IDataObserver^ observer,
												DNP3::Interface::MasterStackConfig^ config)	
	{
		std::string stdPortName = Conversions::convertString(portName);
		std::string stdStackName = Conversions::convertString(stackName);
		apl::FilterLevel stdLevel = Conversions::convertFilterLevel(level);

		MasterDataObserverAdapterWrapper^ wrapper = gcnew MasterDataObserverAdapterWrapper(observer);		
		apl::dnp::MasterStackConfig cfg = Conversions::convertConfig(config);

		try {
			apl::ICommandAcceptor* pCmdAcceptor = pMgr->AddMaster(stdPortName, stdStackName, stdLevel, wrapper->GetDataObserver(), cfg);
			ICommandAcceptor^ ca = gcnew CommandAcceptorAdapter(pCmdAcceptor);
			return ca;
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	
	IDataObserver^	StackManager::AddSlave(	System::String^ portName,
									System::String^ stackName,
									FilterLevel level,
									ICommandAcceptor^ cmdAcceptor,
									SlaveStackConfig^ config)
	{
		std::string stdPortName = Conversions::convertString(portName);
		std::string stdStackName = Conversions::convertString(stackName);
		apl::FilterLevel stdLevel = Conversions::convertFilterLevel(level);

		SlaveCommandAcceptorAdapterWrapper^ wrapper = gcnew SlaveCommandAcceptorAdapterWrapper(cmdAcceptor);
		apl::dnp::SlaveStackConfig cfg = Conversions::convertConfig(config);

		try {
			apl::IDataObserver* pDataObs = pMgr->AddSlave(stdPortName, stdStackName, stdLevel, wrapper->GetCommandAcceptor(), Conversions::convertConfig(config));
			return gcnew SlaveDataObserverAdapter(pDataObs);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}



	void StackManager::RemovePort(System::String^ portName)
	{
		try {
			std::string stdPortName = Conversions::convertString(portName);		 		
			pMgr->RemovePort(stdPortName);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	void StackManager::RemoveStack(System::String^ stackName)
	{
		try {
			std::string stdStackName = Conversions::convertString(stackName);
			pMgr->RemoveStack(stdStackName);
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}

	void StackManager::AddLogHandler(ILogHandler^ logHandler)
	{
		try {
			LogAdapterWrapper^ wrapper = gcnew LogAdapterWrapper(logHandler);
			pMgr->AddLogHook(wrapper->GetLogAdapter());
		} 
		catch(apl::Exception ex){
			throw Conversions::convertException(ex);
		}
	}
}}
