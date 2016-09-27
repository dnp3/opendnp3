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

