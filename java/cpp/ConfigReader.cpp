
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
	{
		jfieldID field = apEnv->GetFieldID(clazz, "masterConfig", "Lcom/automatak/dnp3/MasterConfig;");
		assert(field != nullptr);
		jobject obj = apEnv->GetObjectField(jCfg, field);
		assert(obj != nullptr);
		cfg.master = ConvertMasterConfig(apEnv, obj);
	}
	return cfg;
}

SlaveStackConfig ConfigReader::ConvertSlaveStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveStackConfig cfg;
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


MasterConfig ConfigReader::ConvertMasterConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);

	{
		jfieldID field = apEnv->GetFieldID(clazz, "maxRequestFragmentSize", "I");
		assert(field != nullptr);
		cfg.FragSize = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "vtoWriterQueueSize", "I");
		assert(field != nullptr);
		cfg.VtoWriterQueueSize = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "useNonStandardVtoFunction", "Z");
		assert(field != nullptr);
		cfg.UseNonStandardVtoFunction = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "allowTimeSync", "Z");
		assert(field != nullptr);
		cfg.AllowTimeSync = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "doUnsolOnStartup", "Z");
		assert(field != nullptr);
		cfg.DoUnsolOnStartup = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "doUnsolOnStartup", "Z");
		assert(field != nullptr);
		cfg.DoUnsolOnStartup = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "enableUnsol", "Z");
		assert(field != nullptr);
		cfg.EnableUnsol = apEnv->GetBooleanField(jCfg, field);
	}	
	{
		jfieldID field = apEnv->GetFieldID(clazz, "unsolClassMask", "I");
		assert(field != nullptr);
		cfg.UnsolClassMask = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "integrityRateMs", "J");
		assert(field != nullptr);
		cfg.IntegrityRate = apEnv->GetLongField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "taskRetryRateMs", "J");
		assert(field != nullptr);
		cfg.TaskRetryRate = apEnv->GetLongField(jCfg, field);
	}

	{
		jfieldID field = apEnv->GetFieldID(clazz, "scans", "Ljava/util/List;");
		assert(field != nullptr);
		jobject list = apEnv->GetObjectField(jCfg, field);
		assert(list != nullptr);
		jclass clazz = apEnv->GetObjectClass(list);
		assert(clazz != nullptr);
		jmethodID sizeMID = apEnv->GetMethodID(clazz, "size", "()I");
		jint size = apEnv->CallIntMethod(list, sizeMID);

		jmethodID getMID = apEnv->GetMethodID(clazz, "get", "(I)Ljava/lang/Object;");
		assert(getMID != nullptr);

		for(jint i=0; i< size; ++i)
		{
			jobject scan = apEnv->CallObjectMethod(list, getMID); 
			assert(scan != nullptr);
			jclass exScanClass = apEnv->GetObjectClass(scan);
			assert(exScanClass != nullptr);
			jfieldID fieldMask = apEnv->GetFieldID(exScanClass, "classMask", "I");
			assert(fieldMask != nullptr);
			jfieldID fieldScanRateMs = apEnv->GetFieldID(exScanClass, "scanRateMs", "J");
			assert(fieldScanRateMs != nullptr);

			jint classMask = apEnv->GetIntField(scan, fieldMask);
			jlong scanRateMs = apEnv->GetIntField(scan, fieldMask);

			cfg.AddExceptionScan(classMask, scanRateMs);
		}
	}


	return cfg;
}

AppConfig ConfigReader::ConvertAppConfig(JNIEnv* apEnv, jobject jCfg)
{
	AppConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);

	{
		jfieldID field = apEnv->GetFieldID(clazz, "rspTimeoutMs", "J");
		assert(field != nullptr);
		cfg.RspTimeout = apEnv->GetLongField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "numRetry", "I");
		assert(field != nullptr);
		cfg.NumRetry = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "maxFragSize", "I");
		assert(field != nullptr);
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
		assert(field != nullptr);
		cfg.IsMaster = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "useConfirms", "Z");
		assert(field != nullptr);
		cfg.UseConfirms = apEnv->GetBooleanField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "numRetry", "I");
		assert(field != nullptr);
		cfg.UseConfirms = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "localAddr", "I");
		assert(field != nullptr);
		cfg.LocalAddr = apEnv->GetIntField(jCfg, field);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "remoteAddr", "I");
		assert(field != nullptr);
		cfg.RemoteAddr = apEnv->GetIntField(jCfg, field);
	}	
	return cfg;
}



