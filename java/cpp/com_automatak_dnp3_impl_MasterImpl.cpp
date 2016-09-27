#include "com_automatak_dnp3_impl_MasterImpl.h"

#include "jni/JCache.h"
#include "adapters/GlobalRef.h"
#include "adapters/JNI.h"

#include "asiodnp3/IMaster.h"
#include "opendnp3/master/CommandSet.h"

#include <memory>

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_enable_1native
(JNIEnv* env, jobject, jlong native)
{
	auto master = (asiodnp3::IMaster*) native;
	master->Enable();
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_disable_1native
(JNIEnv* env, jobject, jlong native)
{
	auto master = (asiodnp3::IMaster*) native;
	master->Disable();
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_shutdown_1native
(JNIEnv* env, jobject, jlong native)
{
	auto master = (asiodnp3::IMaster*) native;
	master->Shutdown();
}

template <class Fun>
void operate(JNIEnv* env, jlong native, jobject headers, jobject future, const Fun& operate)
{
	auto builder = jni::JCache::CommandBuilderImpl.init0(env);
	jni::JCache::CommandHeaders.build(env, headers, builder); // send the commands to the builder
	auto& set = *(opendnp3::CommandSet*) jni::JCache::CommandBuilderImpl.getnativePointer(env, builder);

	auto sharedf = std::make_shared<GlobalRef>(future);
	auto callback = [sharedf](const opendnp3::ICommandTaskResult& result)
	{
		const auto env = JNI::GetEnv();
		const auto jsummary = jni::JCache::TaskCompletion.fromType(env, static_cast<jint>(result.summary));
		const auto jlist = jni::JCache::ArrayList.init1(env, result.Count());

		auto addToJList = [&](const opendnp3::CommandPointResult& cpr) {
			const auto jstate = jni::JCache::CommandPointState.fromType(env, static_cast<jint>(cpr.state));
			const auto jstatus = jni::JCache::CommandStatus.fromType(env, static_cast<jint>(cpr.status));
			const auto jres = jni::JCache::CommandPointResult.init4(env, cpr.headerIndex, cpr.index, jstate, jstatus);
			jni::JCache::ArrayList.add(env, jlist, jres);
		};

		result.ForeachItem(addToJList);

		const auto jtaskresult = jni::JCache::CommandTaskResult.init2(env, jsummary, jlist);

		jni::JCache::CompletableFuture.complete(env, *sharedf, jtaskresult); // invoke the future		
	};

	auto& master = *(asiodnp3::IMaster*) native;
	operate(master, set, callback);	
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_select_1and_1operate_1native
(JNIEnv* env, jobject, jlong native, jobject headers, jobject future)
{
	auto sbo = [](asiodnp3::IMaster& master, opendnp3::CommandSet& commandset, const opendnp3::CommandCallbackT& callback) -> void 
	{
		master.SelectAndOperate(std::move(commandset), callback);
	};

	operate(env, native, headers, future, sbo);
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_direct_1operate_1native
(JNIEnv*  env, jobject, jlong native, jobject headers, jobject future)
{
	auto directOp = [](asiodnp3::IMaster& master, opendnp3::CommandSet& commandset, const opendnp3::CommandCallbackT& callback) -> void
	{
		master.DirectOperate(std::move(commandset), callback);
	};

	operate(env, native, headers, future, directOp);
}

bool ConvertJHeader(JNIEnv* env, jobject jheader, opendnp3::Header& header)
{
	const auto group = jni::JCache::Header.getgroup(env, jheader);
	const auto var = jni::JCache::Header.getvariation(env, jheader);
	const auto qualifier = opendnp3::QualifierCodeFromType(static_cast<uint8_t>(jni::JCache::QualifierCode.toType(env, jni::JCache::Header.getqualifier(env, jheader))));
	const auto count = jni::JCache::Header.getcount(env, jheader);
	const auto start = jni::JCache::Header.getstart(env, jheader);
	const auto stop = jni::JCache::Header.getstop(env, jheader);

	switch (qualifier)
	{
	case(opendnp3::QualifierCode::ALL_OBJECTS):
		header = opendnp3::Header::AllObjects(group, var);		
		return true;
	case(opendnp3::QualifierCode::UINT8_CNT):
		header = opendnp3::Header::Count8(group, var, static_cast<uint8_t>(count));
		return true;
	case(opendnp3::QualifierCode::UINT16_CNT):
		header = opendnp3::Header::Count16(group, var, static_cast<uint16_t>(count));
		return true;
	case(opendnp3::QualifierCode::UINT8_START_STOP):
		header = opendnp3::Header::Range8(group, var, static_cast<uint8_t>(start), static_cast<uint8_t>(stop));
		return true;
	case(opendnp3::QualifierCode::UINT16_START_STOP):
		header = opendnp3::Header::Range16(group, var, static_cast<uint16_t>(start), static_cast<uint16_t>(stop));
		return true;
	default:
		return false;
	}
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_scan_1native
(JNIEnv* env, jobject, jlong native, jobject jheaders)
{
	auto& master = *(asiodnp3::IMaster*) native;

	std::vector<opendnp3::Header> headers;

	auto process = [&](jobject jheader) {
		opendnp3::Header header;
		if (ConvertJHeader(env, jheader, header))
		{
			headers.push_back(header);
		}
	};

	JNI::Iterate(env, jheaders, process);

	master.Scan(headers);
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_MasterImpl_add_1periodic_1scan_1native
(JNIEnv* env, jobject, jlong native, jobject jduration, jobject jheaders)
{
	auto& master = *(asiodnp3::IMaster*) native;

	std::vector<opendnp3::Header> headers;

	auto process = [&](jobject jheader) {
		opendnp3::Header header;
		if (ConvertJHeader(env, jheader, header))
		{
			headers.push_back(header);
		}
	};

	JNI::Iterate(env, jheaders, process);

	auto period = openpal::TimeDuration::Milliseconds(jni::JCache::Duration.toMillis(env, jduration));

	master.AddScan(period, headers);
}