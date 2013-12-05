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
namespace openpal
{

/**
* This is a bit of a hack to avoid copying large objects with lamdas
* borrowed from here:
* http://marcoarena.wordpress.com/2012/11/01/learn-how-to-capture-by-move/
*
* This should be unnecessary in C++14
*
*/
template<typename T>
struct move_on_copy
{
   move_on_copy(T&& aValue) : value(std::move(aValue)) {}
   move_on_copy(const move_on_copy& other) : value(std::move(other.value)) {}
 
   T& Value()
   {
      return value;
   }
 
   const T& Value() const
   {
      return value;
   }
 
private:
   mutable T value;
   move_on_copy& operator=(move_on_copy&& aValue); // not needed here
   move_on_copy& operator=(const move_on_copy& aValue); // not needed here
};

}
