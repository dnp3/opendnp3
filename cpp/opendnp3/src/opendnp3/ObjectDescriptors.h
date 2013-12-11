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
#ifndef __OBJECT_DESCRIPTORS_H_
#define __OBJECT_DESCRIPTORS_H_

#include <openpal/BufferWrapper.h>

#include <opendnp3/Uncopyable.h>
#include <opendnp3/DataTypes.h>
#include <opendnp3/ControlRelayOutputBlock.h>

#include "objects/Group1.h"
#include "objects/Group2.h"
#include "objects/Group12.h"

namespace opendnp3
{	
	struct Group1Var2Parser : private PureStatic
	{		
		typedef Binary Target;
		typedef Group1Var2 Underlying;
				
		static Binary Read(openpal::ReadOnlyBuffer& buffer) { 
			auto gv =  Group1Var2::Read(buffer);
			return Binary(gv.flags); 
		}
	};

	struct Group2Var1Parser : private PureStatic
	{		
		typedef Binary Target;
		typedef Group2Var1 Underlying;
				
		static Binary Read(openpal::ReadOnlyBuffer& buffer) { 
			auto gv =  Group2Var1::Read(buffer);
			return Binary(gv.flags); 
		}	
	};


	struct Group2Var2Parser : private PureStatic
	{		
		typedef Binary Target;
		typedef Group2Var2 Underlying;
				
		static Binary Read(openpal::ReadOnlyBuffer& buffer) { 
			auto gv =  Group2Var2::Read(buffer);
			return Binary(gv.flags, gv.time48); 
		}	
	};

	struct Group12Var1Parser : private PureStatic
	{		
		typedef ControlRelayOutputBlock Target;
		typedef Group12Var1 Underlying;
				
		static ControlRelayOutputBlock Read(openpal::ReadOnlyBuffer& buffer) { 
			auto gv =  Group12Var1::Read(buffer);
			return ControlRelayOutputBlock(gv.code, gv.count, gv.onTime, gv.offTime);
		}	
	};

}

#endif
