
#include "ConfigReader.h"


#include <iostream>
#include <assert.h>

using namespace opendnp3;

MasterStackConfig ConfigReader::ConvertMasterStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterStackConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);
	{
		jfieldID field = apEnv->GetFieldID(clazz, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;");
		assert(field != nullptr);
		jobject obj = apEnv->GetObjectField(jCfg, field);
		assert(obj != nullptr);
		cfg.link = ConvertLinkConfig(apEnv, obj);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;");
		assert(field != nullptr);
		jobject obj = apEnv->GetObjectField(jCfg, field);
		assert(obj != nullptr);
		cfg.app = ConvertAppConfig(apEnv, obj);
	}
	return cfg;
}

AppConfig ConfigReader::ConvertAppConfig(JNIEnv* apEnv, jobject jCfg)
{
	AppConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);

	{
		jfieldID field = apEnv->GetFieldID(clazz, "rspTimeoutMs", "J");
		cfg.RspTimeout = apEnv->GetLongField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "numRetry", "I");
		cfg.NumRetry = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "maxFragSize", "I");
		cfg.FragSize = apEnv->GetIntField(jCfg, field);
	}
	
	return cfg;	
}

LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* apEnv, jobject jCfg)
{
	LinkConfig cfg(true, false);
	jclass clazz = apEnv->GetObjectClass(jCfg);

	
	{
		jfieldID field = apEnv->GetFieldID(clazz, "isMaster", "Z");
		cfg.IsMaster = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "useConfirms", "Z");
		cfg.UseConfirms = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "numRetry", "I");
		cfg.UseConfirms = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "localAddr", "I");
		cfg.LocalAddr = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "remoteAddr", "I");
		cfg.RemoteAddr = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "timeoutMs", "J");
		cfg.Timeout = apEnv->GetLongField(jCfg, field);
	}
	return cfg;
}



