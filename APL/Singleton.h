//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __STATE_SINGLETON_H_
#define __STATE_SINGLETON_H_

#define MACRO_SINGLETON_INSTANCE(type) \
	private:\
		static type mInstance;\
	protected: \
		type(){}; \
	public:\
		static type* Inst(){ return &mInstance; }

#define MACRO_NAME_SINGLETON_INSTANCE(type) \
	MACRO_SINGLETON_INSTANCE(type) \
	std::string Name() const { return #type; }

#define MACRO_STATE_SINGLETON_INSTANCE(type) MACRO_NAME_SINGLETON_INSTANCE(type)

#endif

