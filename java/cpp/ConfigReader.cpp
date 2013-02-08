
#include "ConfigReader.h"


#include "JNIHelpers.h"

#include <iostream>
#include <assert.h>

using namespace opendnp3;

MasterStackConfig ConfigReader::ConvertMasterStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterStackConfig cfg;
	
	cfg.link = ConvertLinkConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;"));
	cfg.app = ConvertAppConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;"));
	cfg.master = ConvertMasterConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "masterConfig", "Lcom/automatak/dnp3/MasterConfig;"));

	return cfg;
}

SlaveStackConfig ConfigReader::ConvertSlaveStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveStackConfig cfg;

	cfg.link = ConvertLinkConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig;"));
	cfg.app = ConvertAppConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "appConfig", "Lcom/automatak/dnp3/AppLayerConfig;")); 
	cfg.slave = ConvertOutstationConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "outstationConfig", "Lcom/automatak/dnp3/OutstationConfig;"));
	cfg.device = ConvertDatabaseConfig(apEnv, JNIHelpers::GetObjectField(apEnv, jCfg, "databaseConfig", "Lcom/automatak/dnp3/DatabaseConfig;"));	

	return cfg;
}

SlaveConfig ConfigReader::ConvertOutstationConfig(JNIEnv* apEnv, jobject jCfg)
{
	SlaveConfig cfg;
	

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

	JNIHelpers::IterateOverListOfObjects(apEnv, list, [&](jobject scan) {
		int mask = JNIHelpers::GetIntField(apEnv, scan, "classMask");
		long rate = JNIHelpers::GetLongField(apEnv, scan, "scanRateMs");
		cfg.AddExceptionScan(mask, rate); 
	});	

	return cfg;
}

AppConfig ConfigReader::ConvertAppConfig(JNIEnv* apEnv, jobject jCfg)
{
	AppConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);

	cfg.RspTimeout = JNIHelpers::GetLongField(apEnv, jCfg, "rspTimeoutMs");
	cfg.NumRetry = JNIHelpers::GetIntField(apEnv, jCfg, "numRetry"); 
	cfg.FragSize = JNIHelpers::GetIntField(apEnv, jCfg, "maxFragSize");
	
	return cfg;	
}

LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* apEnv, jobject jCfg)
{
	LinkConfig cfg(true, false);
	
	cfg.IsMaster = JNIHelpers::GetBoolField(apEnv, jCfg, "isMaster");
	cfg.IsMaster = JNIHelpers::GetBoolField(apEnv, jCfg, "useConfirms");
	cfg.UseConfirms = JNIHelpers::GetIntField(apEnv, jCfg, "numRetry");
	cfg.LocalAddr = JNIHelpers::GetIntField(apEnv, jCfg, "localAddr");	
	cfg.RemoteAddr = JNIHelpers::GetIntField(apEnv, jCfg, "remoteAddr");		

	return cfg;
}



