
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#ifndef __ERROR_CODE_H_
#define __ERROR_CODE_H_

namespace opendnp3
{
	template<class T>
	class ErrorCode
	{
		public:

			static ErrorCode Success(T aValue)
			{
				return ErrorCode(aValue, 0);
			}

			static ErrorCode Failure(int aCode)
			{
				assert(aCode != 0);
				return ErrorCode(aCode);
			}

			bool IsError()
			{
				return code != 0;
			}
			
			int Code()
			{
				assert(code != 0);
				return code;
			}

			T Result()
			{
				assert(code == 0);
				return value;
			}

		private:
			ErrorCode(T aValue, int aCode): value(aValue), code(aCode)
			{}

			ErrorCode(int aCode): value(), code(aCode)
			{}

			T value;
			int code;
	};


}

#endif
