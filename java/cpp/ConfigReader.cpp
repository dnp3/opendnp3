
#include "ConfigReader.h"


#include "JNIHelpers.h"

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
	{
		jfieldID field = apEnv->GetFieldID(clazz, "outstationConfig", "Lcom/automatak/dnp3/OutstationConfig;");
		assert(field != nullptr);
		jobject obj = apEnv->GetObjectField(jCfg, field);
		assert(obj != nullptr);
		cfg.slave = ConvertOutstationConfig(apEnv, obj);
	}
	{
		jfieldID field = apEnv->GetFieldID(clazz, "databaseConfig", "Lcom/automatak/dnp3/DatabaseConfig;");
		assert(field != nullptr);
		jobject obj = apEnv->GetObjectField(jCfg, field);
		assert(obj != nullptr);
		cfg.device = ConvertDatabaseConfig(apEnv, obj);
	}

	return cfg;
}

SlaveConfig ConfigReader::ConvertOutstationConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);
	{

	}

/*
  public int maxControls;
    Signature: I
  public boolean disableUnsol;
    Signature: Z
  public int unsolMask;
    Signature: I
  public boolean allowTimeSync;
    Signature: Z
  public long timeSyncPeriodMs;
    Signature: J
  public long unsolPackDelayMs;
    Signature: J
  public long unsolRetryDelayMs;
    Signature: J
  public int maxFragSize;
    Signature: I
  public int vtoWriterQueueSize;
    Signature: I
  int maxBinaryEvents;
    Signature: I
  int maxAnalogEvents;
    Signature: I
  int maxCounterEvents;
    Signature: I
  int maxVtoEvents;
    Signature: I
  public final com.automatak.dnp3.GroupVariation staticBinaryInput;
    Signature: Lcom/automatak/dnp3/GroupVariation;
  public final com.automatak.dnp3.GroupVariation staticAnalogInput;
    Signature: Lcom/automatak/dnp3/GroupVariation;
  public final com.automatak.dnp3.GroupVariation staticCounter;
    Signature: Lcom/automatak/dnp3/GroupVariation;
  public final com.automatak.dnp3.GroupVariation staticAnalogOutputStatus;
    Signature: Lcom/automatak/dnp3/GroupVariation;
  public final com.automatak.dnp3.GroupVariation eventBinaryInput;
    Signature: Lcom/automatak/dnp3/GroupVariation;
  public final com.automatak.dnp3.GroupVariation eventAnalogInput;
    Signature: Lcom/automatak/dnp3/GroupVariation;
  public final com.automatak.dnp3.GroupVariation eventCounter;
    Signature: Lcom/automatak/dnp3/GroupVariation;
  public final com.automatak.dnp3.GroupVariation eventVto;
    Signature: Lcom/automatak/dnp3/GroupVariation;
*/		
	
	
	return cfg;
}

DeviceTemplate ConfigReader::ConvertDatabaseConfig(JNIEnv* apEnv, jobject jCfg)
{
	DeviceTemplate cfg;

	return cfg;
}


MasterConfig ConfigReader::ConvertMasterConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterConfig cfg;

	cfg.FragSize = JNIHelpers::GetIntField(apEnv, jCfg, "maxRequestFragmentSize");
	cfg.VtoWriterQueueSize = JNIHelpers::GetIntField(apEnv, jCfg, "vtoWriterQueueSize");
	cfg.UseNonStandardVtoFunction = JNIHelpers::GetBoolField(apEnv, jCfg, "useNonStandardVtoFunction");	
	cfg.AllowTimeSync = JNIHelpers::GetBoolField(apEnv, jCfg, "allowTimeSync");
	cfg.DoUnsolOnStartup = JNIHelpers::GetBoolField(apEnv, jCfg, "doUnsolOnStartup");
	cfg.EnableUnsol = JNIHelpers::GetBoolField(apEnv, jCfg, "enableUnsol");
	cfg.UnsolClassMask = JNIHelpers::GetIntField(apEnv, jCfg, "unsolClassMask");
	cfg.IntegrityRate = JNIHelpers::GetLongField(apEnv, jCfg, "integrityRateMs");
	cfg.TaskRetryRate = JNIHelpers::GetLongField(apEnv, jCfg, "taskRetryRateMs");
	
	jobject list = JNIHelpers::GetObjectField(apEnv, jCfg, "scans", "Ljava/util/List;");

	jmethodID sizeMID = JNIHelpers::GetMethodID(apEnv, list, "size", "()I");	
	jint size = apEnv->CallIntMethod(list, sizeMID);

	jmethodID getMID = JNIHelpers::GetMethodID(apEnv, list, "get", "(I)Ljava/lang/Object;");

	for(jint i=0; i< size; ++i)
	{
		jobject scan = apEnv->CallObjectMethod(list, getMID, i); 
		assert(scan != nullptr);
		
		int mask = JNIHelpers::GetIntField(apEnv, scan, "classMask");
		long rate = JNIHelpers::GetLongField(apEnv, scan, "scanRateMs"); 		

		cfg.AddExceptionScan(mask, rate);
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



