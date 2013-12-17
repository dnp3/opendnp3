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
#ifndef __DYNAMIC_COLLECTION_H_
#define __DYNAMIC_COLLECTION_H_



namespace openpal
{

template <class T>
class Indexable : public HasSize 
{

	public:

		Indexable(uint32_t aSize) : size(aSize)
		{}
		
		T& operator[](uint32 index) { return Get(index); }

		T& operator[](uint32 index) const { return Get(index); }

	protected:

		virtual T& Get(uin32_t index) = 0;	
};

}

#endif
