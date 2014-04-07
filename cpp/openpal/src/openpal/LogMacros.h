/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __LOG_MACROS_H_
#define __LOG_MACROS_H_

#include "Location.h"

//macro to remove boiler-plate code for logging messages
#define LOG_BLOCK(severity, message) \
	if(this->logger.IsEnabled(severity)){ \
		logger.Log(severity, LOCATION, message, -1); \
	}

/*
//macro to remove boiler-plate code for logging messages with an error code
#define //ERROR_BLOCK(severity, string, code) //ERROR_LOGGER_BLOCK(this->logger, severity, string, code)

#define //LOGGER_BLOCK(logger, severity, string) CODED_//LOGGER_BLOCK(logger, severity, string, -1)

#define P//LOGGER_BLOCK(pLogger, severity, string)\
	if(pLogger){\
	//LOGGER_BLOCK((*pLogger), severity, string)\
	}

#define CODED_FORMATED_//LOGGER_BLOCK(logger, severity, code, format, ...)\
	if(logger.IsEnabled(severity)){\		
		logger.Log(severity, LOCATION, format, code);\
	}


#define CODED_FORMATED_P//LOGGER_BLOCK(pLogger, severity, code, format, ...)\
	if(pLogger){\
		//ERROR_LOGGER_BLOCK((*pLogger), severity, string, code)\
	}
*/

#endif
