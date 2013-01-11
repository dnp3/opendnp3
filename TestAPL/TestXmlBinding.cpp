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
#include <boost/test/unit_test.hpp>


#include <APL/Log.h>
#include <APL/Exception.h>
#include <tinyxml/tinyxml.h>
#include <XMLBindings/APLXML_Base.h>
#include <APLXML/tinybinding.h>
#include <APLXML/SingleNodeSaver.h>

using namespace apl;


BOOST_AUTO_TEST_SUITE(XmlTests)


BOOST_AUTO_TEST_CASE(ParseInt)
{
	TiXmlElement testNode("Test");
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_int(&testNode, "100"), 100);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_int(&testNode, "123456789"), 123456789);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_int(&testNode, "-100"), -100);

	//dont do implit casting to int
	BOOST_CHECK_THROW(IXMLDataBound::FromString_int(&testNode, "-1.00"), apl::Exception);
	//dont read a string as int
	BOOST_CHECK_THROW(IXMLDataBound::FromString_int(&testNode, "abcasdasd"), apl::Exception);
	//empty string where an int should fail
	BOOST_CHECK_THROW(IXMLDataBound::FromString_int(&testNode, ""), apl::Exception);

	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_int(&testNode, "2147483647"), 2147483647);
	BOOST_CHECK_THROW(IXMLDataBound::FromString_int(&testNode, "2147483648"), apl::Exception);
}
BOOST_AUTO_TEST_CASE(WriteInt)
{
	BOOST_CHECK_EQUAL(IXMLDataBound::ToString_int(100), "100");
	BOOST_CHECK_EQUAL(IXMLDataBound::ToString_int(-1), "-1");
}
BOOST_AUTO_TEST_CASE(ParseBool)
{
	TiXmlElement testNode("Test");
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_bool(&testNode, "true"), true);
	//can only use "true" and "false" any other casing is an error.
	BOOST_CHECK_THROW(IXMLDataBound::FromString_bool(&testNode, "TRUE"), apl::Exception);
	BOOST_CHECK_THROW(IXMLDataBound::FromString_bool(&testNode, "True"), apl::Exception);
	BOOST_CHECK_THROW(IXMLDataBound::FromString_bool(&testNode, "tRue"), apl::Exception);

	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_bool(&testNode, "false"), false);
	BOOST_CHECK_THROW(IXMLDataBound::FromString_bool(&testNode, "FALSE"), apl::Exception);
	BOOST_CHECK_THROW(IXMLDataBound::FromString_bool(&testNode, "False"), apl::Exception);
	BOOST_CHECK_THROW(IXMLDataBound::FromString_bool(&testNode, "faLse"), apl::Exception);

	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_bool(&testNode, "0"), false);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_bool(&testNode, "1"), true);
	BOOST_CHECK_THROW(IXMLDataBound::FromString_bool(&testNode, ""), apl::Exception);
}
BOOST_AUTO_TEST_CASE(WriteBool)
{
	BOOST_CHECK_EQUAL(IXMLDataBound::ToString_bool(true), "true");
	BOOST_CHECK_EQUAL(IXMLDataBound::ToString_bool(false), "false");
}
BOOST_AUTO_TEST_CASE(ParseDouble)
{
	TiXmlElement testNode("Test");
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "100"), 100);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "3.1456"), 3.1456);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "-100."), -100);

	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "-100.0e-7"), -1.0e-5);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "-100.0E-7"), -1.0e-5);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "2147483648"), 2147483648UL);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "-100.0E4"), -1.0e6);
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_double(&testNode, "-100.0E+4"), -1.0e6);
}
BOOST_AUTO_TEST_CASE(WriteDouble)
{
	BOOST_CHECK_EQUAL(IXMLDataBound::ToString_double(55), "55");
}
BOOST_AUTO_TEST_CASE(ParseString)
{
	TiXmlElement testNode("Test");
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_string(&testNode, "qwertyuiop"), "qwertyuiop");

	//consider an empty string to be a valid string
	BOOST_CHECK_EQUAL(IXMLDataBound::FromString_string(&testNode, ""), "");
}
BOOST_AUTO_TEST_CASE(WriteString)
{
	BOOST_CHECK_EQUAL(IXMLDataBound::ToString_string("qwertyuiop"), "qwertyuiop");
	BOOST_CHECK_EQUAL(IXMLDataBound::ToString_string(""), "");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SingleNodeSuite)

BOOST_AUTO_TEST_CASE(SaveAndLoad)
{
	EventLog log;
	SingleNodeSaver<APLXML_Base::TCPServer_t> s("TestSingle.tiny.xml", log.GetLogger(LEV_DEBUG, "SingleNode"));
	APLXML_Base::TCPServer_t node;

	node.Name = "HelloWorld";
	node.Port = 1000;

	BOOST_CHECK(s.SaveNode(node));

	APLXML_Base::TCPServer_t node2;
	BOOST_CHECK(s.LoadNode(node2));

	BOOST_CHECK_EQUAL(node2.Name, "HelloWorld");
	BOOST_CHECK_EQUAL(node2.Port, 1000);
}
BOOST_AUTO_TEST_CASE(LoadFails)
{
	EventLog log;
	SingleNodeSaver<APLXML_Base::Serial_t> s("TestSingleFailure.tiny.xml", log.GetLogger(LEV_DEBUG, "SingleNode"));
	APLXML_Base::Serial_t node;

	BOOST_CHECK_EQUAL(s.LoadNode(node), false);
}

BOOST_AUTO_TEST_SUITE_END()

