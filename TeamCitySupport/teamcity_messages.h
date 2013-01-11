/* Copyright 2009 JetBrains
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * $Revision: 50800 $
*/

#ifndef H_TEAMCITY_MESSAGES
#define H_TEAMCITY_MESSAGES

#include <string>
#include <iostream>

namespace JetBrains
{

bool underTeamcity();

class TeamcityMessages
{
	std::ostream* m_out;

protected:
	std::string escape(std::string s);

	void openMsg(const std::string& name);
	void writeProperty(std::string name, std::string value);
	void closeMsg();

public:
	TeamcityMessages();

	void setOutput(std::ostream&);

	void suiteStarted(std::string name);
	void suiteFinished(std::string name);

	void testStarted(std::string name);
	void testFailed(std::string name, std::string message, std::string details = "");
	void testIgnored(std::string name, std::string message = "");
	void testFinished(std::string name);
};

}

#endif /* H_TEAMCITY_MESSAGES */
