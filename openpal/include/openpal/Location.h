
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __LOCATION_H_
#define __LOCATION_H_

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifndef OPENDNP3_SUPPRESS_LOG_LOCATION
#define LOCATION __FILE__ "(" TOSTRING(__LINE__) ")"
#else
#define LOCATION ""
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION(type, message) { throw type(LOCATION, message); }
#else
#define MACRO_THROW_EXCEPTION(type, message) throw type(LOCATION, "");
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_INDEX_OUT_OF_BOUNDS(index) { throw openpal::IndexOutOfBoundsException(LOCATION, index); }
#else
#define MACRO_THROW_INDEX_OUT_OF_BOUNDS(index) { throw  openpal::IndexOutOfBoundsException(LOCATION, index); }
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION_WITH_CODE(type, message, code) { throw type(LOCATION, message, code); }
#else
#define MACRO_THROW_EXCEPTION_WITH_CODE(type, message, code) throw type(LOCATION, "", code);
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION_COMPLEX(type, message) {\
	std::ostringstream oss;\
	oss << message;\
	throw type(LOCATION, oss.str()); }
#else
#define MACRO_THROW_EXCEPTION_COMPLEX(type, message) throw type(LOCATION, "");
#endif

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
#define MACRO_THROW_EXCEPTION_COMPLEX_WITH_CODE(type, message, code) {\
	std::ostringstream oss; \
	oss << message; \
	throw type(LOCATION, oss.str(), code); }
#else
#define MACRO_THROW_EXCEPTION_COMPLEX_WITH_CODE(type, message, code) throw type(LOCATION, "", code);
#endif

#endif
