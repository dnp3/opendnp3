
#include "ConfigReader.h"


using namespace opendnp3;

MasterStackConfig ConfigReader::ConvertMasterStackConfig(JNIEnv* apEnv, jobject jCfg)
{
	MasterStackConfig cfg;
	jclass clazz = apEnv->GetObjectClass(jCfg);
	jfieldID linkField = apEnv->GetFieldID(clazz, "linkConfig", "Lcom/automatak/dnp3/LinkLayerConfig");
	cfg.link = ConvertLinkConfig(apEnv, apEnv->GetObjectField(jCfg, linkField)); 
	return cfg;
}


LinkConfig ConfigReader::ConvertLinkConfig(JNIEnv* apEnv, jobject jCfg)
{
	LinkConfig cfg(true, false);
	return cfg;
}



