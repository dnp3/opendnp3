
#ifndef __DNP3_MANAGER_WRAPPER_H_
#define __DNP3_MANAGER_WRAPPER_H_

#include <jni.h>
#include <DNP3/DNP3Manager.h>
#include <vector>

namespace apl {
namespace dnp {

class DNP3ManagerWrapper
{
	public:
		DNP3ManagerWrapper(
			uint32_t aConcurrency,
			JavaVM* apJVM
		);

		~DNP3ManagerWrapper();

		void AddLogSubscriber(JNIEnv* apEnv, jobject subscriber);

		void Shutdown();

		IChannel* AddTCPClient(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arAddr, uint16_t aPort);
		IChannel* AddTCPServer(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arEndpoint, uint16_t aPort);
		IChannel* AddSerial(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, SerialSettings aSettings);

	private:
		void ReleaseObject(jobject obj);

		JavaVM* mpJVM;
		DNP3Manager mProxy;
		std::vector<std::function<void ()>> mOnDestruction;
		
};

}}

#endif

