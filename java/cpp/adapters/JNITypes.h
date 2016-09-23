//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2016 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef OPENDNP3JAVA_JNITYPES_H
#define OPENDNP3JAVA_JNITYPES_H

#include <jni.h>

struct MethodInfo
{ 
	const char* name; const char* sig;
};

// fully qualified class name
struct FQCN
{
	const char* value;
};

struct FieldId
{
	const char* value;
};

struct ClassMethodPair
{	
	ClassMethodPair(jclass clazz, jmethodID method) : clazz(clazz), method(method) {}
	ClassMethodPair() {}

	jclass clazz = nullptr;
	jmethodID method = nullptr;
};

#endif
