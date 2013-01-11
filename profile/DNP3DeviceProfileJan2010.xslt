<?xml version="1.0"?>
<!-- *****************************  Instructions for use
       Copy this file to the same directory as the XML instance file
       add as 2nd line of xml: *<?xml-stylesheet type="text/xsl" href="DNP3DeviceProfileJan2010.xslt"?>* to autorun XSLT 
       Make sure that the directory also contains the DNP logo file dnp_logo.jpg and the translation UserData.xslt 

       _____________________________________________________

       DNP3 Device Profile transformation file Version 2-07

       operating with schema file DNP3DeviceProfileVersion 2-07
       _____________________________________________________
 -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:dnp="http://www.dnp3.org/DNP3/DeviceProfile/Jan2010">
<xsl:include href="UserData.xslt"/>

	<!-- The following two parameters can be changed to manage the presentation of the output
		The parameter <notes> can be set to yes or no to manage the display of notes
		The parameter <IEC61850Map> can be set to:
			- "none" for no presentation of the contents of section 2
			- "table" for presentation of section 2 in a tabular form
			- "tree" for the presentation of section 2 as a tree structure
	-->
	<xsl:param name="notes" select="'yes'"/>
	<xsl:param name="IEC61850Map" select="'tree'"/>

	<xsl:output method="html" encoding="ISO-8859-1"/>
	<xsl:variable name="evtext" select="'When responding with event data and more than one event has occurred for a data point, 
	                                                      an Outstation may include all events or only the most recent event.'"/>
	<xsl:variable name="holdTimeText" select="'A configurable value of 0 indicates that responses are not delayed due to this parameter.'"/>
	<xsl:variable name="pointListText" select="'List of addressable points. Points that do not exist (for example, because an option is
	                                                                not installed) are omitted from the table.'"/>
	
	<xsl:template match="/">
		<html>
			<body>
				<!-- call up the standard DNP3 header template for the revision history page -->
				<xsl:if test="/*/dnp:documentHeader">
					<xsl:apply-templates select="/*/dnp:documentHeader"/>
				</xsl:if>
				<!-- Reference Device -->
				<p/>	<br/><br/><br/>
				<xsl:if test="/*/dnp:referenceDevice">
					<xsl:apply-templates select="/*/dnp:referenceDevice"/>
				</xsl:if>
				<!-- Repeat for each occurrence of the auxiliary info -->
				<p/>	<br/><br/><br/>
				<xsl:if test="/*/dnp:auxiliaryInfo">
					<xsl:for-each select="/*/dnp:auxiliaryInfo">
						<p/>
						<table border="0" width="90%" align="center">
							<tbody>
								<tr><big>	<big><b>
									<font face="Times New Roman" style="font-variant: small-caps;">Auxiliary Information
										<xsl:value-of select="position()"/>
										: &#160;&#160;&#160;<xsl:value-of select="@description"/>
									</font>
								</b>	</big></big></tr>
							</tbody>
						</table>
						<xsl:call-template name="deviceProfile">
							<xsl:with-param name="path" select="."/>
						</xsl:call-template>
						<center>
							<b>---------- End of Device Profile for Auxiliary Information <xsl:value-of select="position()"/> ----------</b>
						</center>
					</xsl:for-each>
				</xsl:if>
				<br/>
				<center>
					<b>
						<big>------------------------------- End of Complete Device Profile -------------------------------</big>
					</b>
				</center>
			</body>
		</html>
	</xsl:template>
	
	<xsl:template match="/*/dnp:referenceDevice">
		<p/>
		<table border="0" width="90%" align="center">
			<tbody>
				<tr><big>	<big><b>
					<font face="Times New Roman" style="font-variant: small-caps;">
						Reference Device: &#160;&#160;&#160;<xsl:value-of select="@description"/>
					</font>
				</b>	</big></big></tr>
			</tbody>
		</table>
		<!-- Output a device profile  -->
		<xsl:call-template name="deviceProfile">
			<xsl:with-param name="path" select="/*/dnp:referenceDevice"/>
		</xsl:call-template>
		<br/>
		<br/>
		<center>
			<b>---------- End of Device Profile for Reference Device ----------</b>
		</center>
	</xsl:template>
	
	<xsl:template name="deviceProfile">
		<xsl:param name="path"/>
		<!-- Part 1 Device Properties -->
		<p/>
		<xsl:if test="$path/dnp:configuration">
			<xsl:call-template name="configuration">
				<xsl:with-param name="path" select="$path/dnp:configuration"/>
			</xsl:call-template>
		</xsl:if>
		<!-- Part 2 IEC61850 mapping -->
		<p/>	<br/><br/><br/>
		<xsl:if test="$path/dnp:iec61850DeviceMapping">
			<xsl:if test="$IEC61850Map='table'">
				<xsl:call-template name="iec61850DeviceMappingTable">
					<xsl:with-param name="path" select="$path/dnp:iec61850DeviceMapping"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:if test="$IEC61850Map='tree'">
				<xsl:call-template name="iec61850DeviceMappingTree">
					<xsl:with-param name="path" select="$path/dnp:iec61850DeviceMapping"/>
				</xsl:call-template>
			</xsl:if>
			<xsl:if test="$IEC61850Map='none'">
				<xsl:call-template name="iec61850DeviceMappingNone">
					<xsl:with-param name="path" select="$path/dnp:iec61850DeviceMapping"/>
				</xsl:call-template>
			</xsl:if>
		</xsl:if>
		<!-- Part 3 Start of the database area formatting  -->
		<p/>	<br/><br/><br/>
		<xsl:if test="$path/dnp:database">
			<xsl:call-template name="database">
				<xsl:with-param name="path" select="$path/dnp:database"/>
			</xsl:call-template>
		</xsl:if>
		<!-- Part 4 The implementation table entries -->
		<p/>	<br/><br/><br/>
		<xsl:if test="$path/dnp:implementationTable">
			<xsl:call-template name="implementationTable">
				<xsl:with-param name="path" select="$path/dnp:implementationTable"/>
			</xsl:call-template>
		</xsl:if>
		<!-- Part 5 The database points entries -->
		<p/>	<br/><br/><br/>
		<xsl:if test="$path/dnp:dataPointsList">
			<xsl:call-template name="dataPointsList">
				<xsl:with-param name="path" select="$path/dnp:dataPointsList"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>
	
	<xsl:template match="/*/dnp:documentHeader">
		<p/>
		<table border="0" width="90%" align="center">
			<tbody>
				<tr>
					<td align="center">
						<img src="dnp_logo.jpg" width="40%"/>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<big><big><b>
							DNP3  Device Profile<br/><i>Based on DNP XML Schema version <xsl:value-of select="/*/@schemaVersion"/></i><br/><br/>
						</b></big></big>	
					</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<big><b>Document Name: <xsl:value-of select="dnp:documentName"/>
							<br/><br/><br/>
						</b></big>
					</td>
				</tr>
				<tr>
					<td align="center" colspan="2">
						<big><b>Document Description: <xsl:value-of select="dnp:documentDescription"/>
							<br/><br/><br/>
						</b></big>
					</td>
				</tr>
			</tbody>
		</table>
		<!-- Produce the revision history table structure and headings and fill it in -->
		<p/>
		<p/>
		<p/>
		<table border="1" borderColor="lightgrey" align="center" width="90%" cellspacing="0" cellpadding="5">
			<caption align="left">
				<big>
					<b>Revision History</b>
				</big>
			</caption>
			<thead>
				<tr align="left">
					<th>Date</th>
					<th>Time</th>
					<th>Version</th>
					<th>Reason for change</th>
					<th>Edited by</th>
				</tr>
			</thead>
			<tbody>
				<xsl:for-each select="dnp:revisionHistory">
					<xsl:sort select="@dnp:date" order="ascending" data-type="text"/>
					<tr>
						<td valign="top" width="12%">
							<xsl:value-of select="dnp:date"/>
						</td>
						<td valign="top" width="12%">
							<xsl:value-of select="dnp:time"/>&#160;
						</td>
						<td valign="top" width="10%">
							<xsl:value-of select="@version"/>
						</td>
						<td valign="top" width="50%">
							<xsl:value-of select="dnp:reason"/>
						</td>
						<td valign="top" width="16%">
							<xsl:value-of select="dnp:author"/>
						</td>
					</tr>
				</xsl:for-each>
			</tbody>
		</table>
	</xsl:template>
	
	<xsl:template name="configuration">
		<xsl:param name="path"/>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<big>
					<b>1. Device Properties</b>
				</big>
				<p/>This document is intended to be used for several purposes, including: <br/><br/>
				- Identifying the capabilities of a DNP3 device (Master Station or Outstation) <br/><br/>
				- Recording the settings of a specific instance of a device (parameter settings for a specific instance of the device in the user's total DNP3 estate)
				<br/><br/>
        - Matching user requirements to product capabilities when procuring a DNP3 device <br/><br/>
				The document is therefore structured to show, for each technical feature, the capabilities of the device (or capabilities required by the device 
        when procuring). <br/><br/>
				It is also structured to show the current value (or setting) of each of the parameters that describe a specific instance of the device.
				This "current value" may also show a functional limitation of the device. For example when implementing secure authentication it is not
        required that all DNP3 devices accept aggressive mode requests during critical exchanges (see Device Profile 1.12.4), in which case a vendor would
        mark this current value as "No - does not accept aggressive mode requests". <br/><br/>
				Additionally, the current value may sometimes be used to show a value that a device can achieve because of hardware or software dependencies.
				An example of this is in section 1.6.8 of the Device Profile (Maximum error in the time that the Master issues freeze requests)
				 where the value may well depend upon tolerances of hardware components and interactions between software tasks. 
				 When the Device Profile current value is used in this way the corresponding entry in the capabilities column is grayed-out. Users
				should note that if an entry in the capabilities column of the Device Profile is grayed-out then there may be information in the current value
				column that is pertinent to the device's capabilities. <br/><br/>
				Unless otherwise noted, multiple boxes in the second column below are selected for each parameter to indicate all capabilities supported or required.
			    Parameters without checkboxes in the second column do not have capabilities and are included so that the current value may be shown in the third column.
				<br/><br/>The items listed in the capabilities column below may be configurable to any of the options selected, or set to a fixed value when the device was designed.
				Item 1.1.10 contains a list of abbreviations for the possible ways in which the configurable parameters may be set. 
				Since some parameters may not be accessible by each of these methods supported, an abbreviation for the configuration method supported by
				each parameter is shown in the fourth column of the tables below.
				<br/><br/>If this document is used to show the current values, the third column should be filled in even if a fixed parameter is selected in the 
				capabilities section ("NA" may be entered for parameters that are Not Applicable).
				<br/><br/>If the document is used to show the current values of parameters, then column 3 applies to a single connection between a master and an outstation.
			</caption>
		</table>
		<!-- 1.1 Device Identification -->
		<xsl:if test="$path/dnp:deviceConfig">
			<xsl:call-template name="deviceConfig">
				<xsl:with-param name="path" select="$path/dnp:deviceConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.2 Serial Connections -->
		<xsl:if test="$path/dnp:serialConfig">
			<xsl:call-template name="serialConfig">
				<xsl:with-param name="path" select="$path/dnp:serialConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.3 IP Networking -->
		<xsl:if test="$path/dnp:networkConfig">
			<xsl:call-template name="networkConfig">
				<xsl:with-param name="path" select="$path/dnp:networkConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.4 Link Layer -->
		<xsl:if test="$path/dnp:linkConfig">
			<xsl:call-template name="linkConfig">
				<xsl:with-param name="path" select="$path/dnp:linkConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.5 Application Layer -->
		<xsl:if test="$path/dnp:applConfig">
			<xsl:call-template name="applConfig">
				<xsl:with-param name="path" select="$path/dnp:applConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.6 Masters Capabilities -->
		<xsl:if test="$path/dnp:masterConfig">
			<xsl:call-template name="masterConfig">
				<xsl:with-param name="path" select="$path/dnp:masterConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.7 Outstations Capabilities -->
		<xsl:if test="$path/dnp:outstationConfig">
			<xsl:call-template name="outstationConfig">
				<xsl:with-param name="path" select="$path/dnp:outstationConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.8 Outstations Unsolicited Responses -->
		<xsl:if test="$path/dnp:unsolicitedConfig">
			<xsl:call-template name="unsolicitedConfig">
				<xsl:with-param name="path" select="$path/dnp:unsolicitedConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.9 Outstations Unsolicited Response Triggers -->
		<xsl:if test="$path/dnp:unsolicitedResponseTriggerConditions">
			<xsl:call-template name="unsolicitedResponseTriggerConditions">
				<xsl:with-param name="path" select="$path/dnp:unsolicitedResponseTriggerConditions"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.10 Outstation Performance -->
		<xsl:if test="$path/dnp:outstationPerformance">
			<xsl:call-template name="outstationPerformance">
				<xsl:with-param name="path" select="$path/dnp:outstationPerformance"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.11 Individual Field Outstation Parameters -->
		<xsl:if test="$path/dnp:fieldConfig">
			<xsl:call-template name="fieldConfig">
				<xsl:with-param name="path" select="$path/dnp:fieldConfig"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 1.12 Security Parameters -->
		<xsl:if test="$path/dnp:securityConfig"><xsl:call-template name="securityConfig">
			<xsl:with-param name="path" select="$path/dnp:securityConfig"/></xsl:call-template>
		</xsl:if>
		<!-- 1.13 User Data -->
		<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1">
			<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
		</xsl:if>
	</xsl:template>
	
	<xsl:template name="iec61850DeviceMappingNone">
		<xsl:param name="path"/>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<big>
					<b>2. Mapping to IEC 61850 Object Models</b>
				</big>
				<p/>This optional section allows each configuration parameter or point in the DNP Data map to be tied to an attribute in the IEC
			61850 object models. The IEC 61850 mappings are stored in the XML version of the Device Profile Document as a list of XPath
			references to the tags representing real-time data from DNP under each point (for example value, timestamp, and quality for
			Analog inputs) paired with an IEC 61850 Object Reference in the form of a flattened ACSI (Abstract Communications Service
			Interface) name of the object and attributes as specified in IEC 61850 parts 7-4 and 7-3. The Xpath reference into the DNP XML
			file may also contain a reference to a constant value, a formula or conditional expression involving one or more XML tags,
			or a reference to a configuration parameter that is not associated with a particular data point.
			<p/> A tree or table representation may be generated from the XML and shown here in the Device Profile Document. The 
			selection has been made not to show any representation.
			</caption>
		</table>
	</xsl:template>

	<xsl:template name="iec61850DeviceMappingTable">
		<xsl:param name="path"/>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<big>
					<b>2. Mapping to IEC 61850 Object Models</b>
				</big>
				<p/>This optional section allows each configuration parameter or point in the DNP Data map to be tied to an attribute in the IEC
			61850 object models. The IEC 61850 mappings are stored in the XML version of the Device Profile Document as a list of XPath
			references to the tags representing real-time data from DNP under each point (for example value, timestamp, and quality for
			Analog inputs) paired with an IEC 61850 Object Reference in the form of a flattened ACSI (Abstract Communications Service
			Interface) name of the object and attributes as specified in IEC 61850 parts 7-4 and 7-3. The Xpath reference into the DNP XML
			file may also contain a reference to a constant value, a formula or conditional expression involving one or more XML tags,
			or a reference to a configuration parameter that is not associated with a particular data point.
			<p/> A tree or table representation may be generated from the XML and shown here in the Device Profile Document. The 
			following is an example table format.
			</caption>
		</table>
		<!-- 2.1 Mapping to IEC 61850 Object Models: -->
		<xsl:if test="$path">
			<p/>
			<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
				<!-- first line of table heading -->
				<tr bgcolor="yellow">
					<b>
						<th align="left" width="100%" colspan="6">
							<font face="Times New Roman" style="font-variant: small-caps;">
							Mapping to IEC 61850 Object Models</font>
						</th>
					</b>
				</tr>
				<tbody>
					<!--Access Point: -->
					<xsl:if test="$path/dnp:accessPoint">
						<tr>
							<td width="100%" colspan="6" valign="top">Access Point: <xsl:value-of select="$path/dnp:accessPoint"/>
							</td>
						</tr>
					</xsl:if>
					<!--Mappings: -->
					<xsl:if test="$path/dnp:iec61850Mapping">
						<tr>
							<td width="30%" bgcolor="yellow" valign="top" align="center">DNP Xpath Reference</td>
							<td width="70%" colspan="5" bgcolor="yellow" valign="top" align="center">IEC 61850 Object</td>
						</tr>
						<tr>
							<td width="30%" bgcolor="yellow" valign="top" align="center">
								<font color="yellow">.</font>
							</td>
							<td width="25%" bgcolor="yellow" valign="top" align="center">Path</td>
							<td width="10%" bgcolor="yellow" valign="top" align="center">FC</td>
							<td width="10%" bgcolor="yellow" valign="top" align="center">Common Data Class</td>
							<td width="15%" bgcolor="yellow" valign="top" align="center">Data Type</td>
							<td width="10%" bgcolor="yellow" valign="top" align="center">Trigger</td>
						</tr>
						<xsl:for-each select="$path/dnp:iec61850Mapping">
							<tr>
								<td width="30%" valign="top">
									<xsl:value-of select="./dnp:dnp3XPath"/>
									<xsl:call-template name="showNotesGeneral">
										<xsl:with-param name="path" select="."/>
									</xsl:call-template>
								</td>
								<td width="20%" valign="top">
									<xsl:value-of select="./dnp:iec61850Path"/>
								</td>
								<td width="10%" valign="top" align="center">
									<xsl:value-of select="./dnp:functionalConstraint"/>
								</td>
								<td width="10%" valign="top" align="center">
									<xsl:value-of select="./dnp:commonDataClass"/>
								</td>
								<td width="15%" valign="top" align="center">
									<xsl:value-of select="./dnp:dataType"/>
									<xsl:if test="(./dnp:dataType) = 'Enum'"> (<xsl:value-of select="./dnp:enumTypeId"/>)</xsl:if>
								</td>
								<td width="15%" valign="top" align="center">
									<xsl:if test="(./dnp:triggerOptions) = 'dchg'">Data Change</xsl:if>
									<xsl:if test="(./dnp:triggerOptions) = 'qchg'">Qual Change</xsl:if>
									<xsl:if test="(./dnp:triggerOptions) = 'dupd'">Data Update</xsl:if>
									<xsl:if test="(./dnp:triggerOptions) = 'period'">Periodic</xsl:if>
									<xsl:if test="(./dnp:triggerOptions) = 'none'">None</xsl:if>
									&#160;
								</td>
							</tr>
						</xsl:for-each>
						<!--Enumerations -->
						<xsl:if test="$path/dnp:enum">
							<tr>
								<td width="100%" bgcolor="yellow" colspan="6" valign="top">Enumeration details</td>
							</tr>
						</xsl:if>
						<xsl:for-each select="$path/dnp:enum">
							<tr>
								<td width="100%" bgcolor="Khaki" colspan="6" valign="top">Enumerations for: <xsl:value-of select="@id"/>
								</td>
							</tr>
							<xsl:for-each select="./dnp:enumVal">
								<tr>
									<td width="30" valign="top">
										<xsl:value-of select="@ord"/>
										<br/>
									</td>
									<td width="70" colspan="5" valign="top">
										<xsl:value-of select="."/>
										<br/>
									</td>
								</tr>
							</xsl:for-each>
						</xsl:for-each>
					</xsl:if>
				</tbody>
			</table>
		</xsl:if>
	</xsl:template>

	<xsl:template name="iec61850DeviceMappingTree">
		<xsl:param name="path"/>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<big>
					<b>2. Mapping to IEC 61850 Object Models</b>
				</big>
				<p/>This optional section allows each configuration parameter or point in the DNP Data map to be tied to an attribute in the IEC
			61850 object models. The IEC 61850 mappings are stored in the XML version of the Device Profile Document as a list of XPath
			references to the tags representing real-time data from DNP under each point (for example value, timestamp, and quality for
			Analog inputs) paired with an IEC 61850 Object Reference in the form of a flattened ACSI (Abstract Communications Service
			Interface) name of the object and attributes as specified in IEC 61850 parts 7-4 and 7-3. The Xpath reference into the DNP XML
			file may also contain a reference to a constant value, a formula or conditional expression involving one or more XML tags,
			or a reference to a configuration parameter that is not associated with a particular data point.
			<p/> A tree or table representation may be generated from the XML and shown here in the Device Profile Document. The 
			following is an example tree format.
			</caption>
		</table>
		<!-- 2.1 Mapping to IEC 61850 Object Models: -->
		<xsl:if test="$path">
			<p/>
			<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
				<!-- first line of table heading -->
				<tr bgcolor="yellow">
					<b>
						<th align="left" width="100%" colspan="7">
							<font face="Times New Roman" style="font-variant: small-caps;">
							Mapping to IEC 61850 Object Models</font>
						</th>
					</b>
				</tr>
				<tbody>
					<!--Access Point: -->
					<xsl:if test="$path/dnp:accessPoint">
						<tr>
							<td width="100%" colspan="7" valign="top">Access Point: <xsl:value-of select="$path/dnp:accessPoint"/>
							</td>
						</tr>
					</xsl:if>
					<!--Mappings: -->
					<tr>
						<td width="25%" colspan="5" bgcolor="yellow" valign="top" align="left">IEC 61850 Object</td>
						<td width="75%" colspan="2" bgcolor="yellow" valign="top" align="left">DNP Xpath Reference</td>
					</tr>
					<tr>
					<td width="100%" colspan="7">
					<xsl:if test="$path/dnp:iec61850Mapping">
					<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">

					<!-- process each mapping in turn -->
					<xsl:for-each select="$path/dnp:iec61850Mapping">
						<xsl:choose>
							<xsl:when test="substring-before(./dnp:iec61850Path,'\') = substring-before(preceding::dnp:iec61850Path[1],'\')">
								<xsl:choose>
									<!-- first level name the same - check the second -->
									<xsl:when test="substring-before(substring-after(./dnp:iec61850Path,'\'),'.') = 
										substring-before(substring-after(preceding::dnp:iec61850Path[1],'\'),'.')">
										<xsl:choose>
											<!-- second level name the same - check the third -->
											<xsl:when test="(substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.') = 
												substring-before(substring-after(substring-after(preceding::dnp:iec61850Path[1],'\'),'.'),'.'))
												and (string-length(substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.')) &gt; 0)">
												<xsl:choose>
													<!-- third level the same - check the fourth level name -->
													<xsl:when test="(substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.') = 
														substring-before(substring-after(substring-after(substring-after(preceding::dnp:iec61850Path[1],'\'),'.'),'.'),'.'))
														and (string-length(substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')) &gt; 0)">
														<xsl:choose>
															<!-- fourth level the same - check the fifth level name -->
															<xsl:when test="(substring-after(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.') = 
																substring-after(substring-after(substring-after(substring-after(preceding::dnp:iec61850Path[1],'\'),'.'),'.'),'.'))
																and (string-length(substring-before(substring-after(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.'),'.')) &gt; 0)">
															</xsl:when>
															<xsl:otherwise>
																<!-- fifth level exists and has changed -->
																<xsl:call-template name="fifthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
															</xsl:otherwise>
														</xsl:choose>
													</xsl:when>
													<xsl:otherwise>
														<!-- fourth level has changed if it exists -->
														<xsl:choose>
															<xsl:when test="string-length(substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')) &gt; 0">
																<!-- the fifth level name exists - so output a fourth level name -->
																<tr>
																	<td width="2%"></td><td width="2%"></td><td width="2%"></td>
																	<td width="94%" valign="top" colspan="4">
																		- <xsl:value-of select="substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')"/><br/>
																	</td>
																</tr>
																<!-- and output the fifth level as a lowest level entry -->
																<xsl:call-template name="fifthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
															</xsl:when>
															<xsl:otherwise>
																<!-- no fifth level so output the fourth level as a lowest level entry -->
																<xsl:call-template name="fourthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
															</xsl:otherwise>
														</xsl:choose>
													</xsl:otherwise>
												</xsl:choose>
											</xsl:when>
											<!-- third level name has changed if it exists -->
											<xsl:otherwise>
												<xsl:choose>
													<xsl:when test="string-length(substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.')) &gt; 0">
														<!-- the fourth level name exists - so output a third level name -->
														<tr>
															<td width="2%"></td><td width="2%"></td>
															<td width="96%" valign="top" colspan="5">
																- <xsl:value-of select="substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.')"/><br/>
															</td>
														</tr>
														<xsl:choose>
															<xsl:when test="string-length(substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')) &gt; 0">
																<!-- the fifth level name exists - so output a fourth level name -->
																<tr>
																	<td width="2%"></td><td width="2%"></td><td width="2%"></td>
																	<td width="94%" valign="top" colspan="4">
																		- <xsl:value-of select="substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')"/><br/>
																	</td>
																</tr>
																<!-- and output the fifth level as a lowest level entry -->
																<xsl:call-template name="fifthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
															</xsl:when>
															<xsl:otherwise>
																<!-- the fifth level does not exist - so output a fourth level name as the lowest entry -->
																<xsl:call-template name="fourthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
															</xsl:otherwise>
														</xsl:choose>
													</xsl:when>
												</xsl:choose>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<!-- second level name has changed -->
									<xsl:otherwise>
										<!-- output the second level name -->
										<tr>
											<td width="2%"></td>
											<td width="98%" valign="top" colspan="6">
												- <xsl:value-of select="substring-before(substring-after(./dnp:iec61850Path,'\'),'.')"/><br/>
											</td>
										</tr>
										<xsl:choose>
											<xsl:when test="string-length(substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.')) &gt; 0">
												<!-- the fourth level name exists - so output a third level name -->
												<tr>
													<td width="2%"></td><td width="2%"></td>
													<td width="96%" valign="top" colspan="5">
														- <xsl:value-of select="substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.')"/><br/>
													</td>
												</tr>
												<xsl:choose>
													<xsl:when test="string-length(substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')) &gt; 0">
														<!-- the fifth level name exists - so output a fourth level name -->
														<tr>
															<td width="2%"></td><td width="2%"></td><td width="2%"></td>
															<td width="94%" valign="top" colspan="4">
																- <xsl:value-of select="substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')"/><br/>
															</td>
														</tr>
														<!-- and output the fifth level as a lowest level entry -->
														<xsl:call-template name="fifthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
													</xsl:when>
													<xsl:otherwise>
														<!-- the fifth level does not exist - so output a fourth level name as the lowest entry -->
														<xsl:call-template name="fourthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
													</xsl:otherwise>
												</xsl:choose>
											</xsl:when>
										</xsl:choose>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
							<!-- first level name has changed -->
								<!-- output the first level name -->
								<tr>
									<td valign="top" colspan="7">
										<xsl:value-of select="substring-before(./dnp:iec61850Path,'\')"/><br/>
									</td>
								</tr>
								<tr>
								<!-- output the second level name -->
									<td width="2%"></td>
									<td width="98%" valign="top" colspan="6">
										- <xsl:value-of select="substring-before(substring-after(./dnp:iec61850Path,'\'),'.')"/><br/>
									</td>
								</tr>
								<xsl:choose>
									<xsl:when test="string-length(substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.')) &gt; 0">
									<!-- the fourth level name exists - so output a third level name -->
										<tr>
											<td width="2%"></td><td width="2%"></td>
											<td width="96%" valign="top" colspan="5">
												- <xsl:value-of select="substring-before(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.')"/><br/>
											</td>
										</tr>
										<xsl:choose>
											<xsl:when test="string-length(substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')) &gt; 0">
												<!-- the fifth level name exists - so output a fourth level name -->
												<tr>
													<td width="2%"></td><td width="2%"></td><td width="2%"></td>
													<td width="94%" valign="top" colspan="4">
														- <xsl:value-of select="substring-before(substring-after(substring-after(substring-after(./dnp:iec61850Path,'\'),'.'),'.'),'.')"/><br/>
													</td>
												</tr>
												<!-- and output the fifth level as a lowest level entry -->
												<xsl:call-template name="fifthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
											</xsl:when>
											<xsl:otherwise>
												<!-- the fifth level does not exist - so output a fourth level name as the lowest entry -->
												<xsl:call-template name="fourthLayer"><xsl:with-param name="path" select="."/></xsl:call-template>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:for-each>
				</table>
				</xsl:if></td></tr>
				</tbody>
			</table>
		</xsl:if>
	</xsl:template>

	<xsl:template name="fifthLayer">
		<xsl:param name="path"/>
		<tr>
			<td width="2%"></td><td width="2%"></td><td width="2%"></td><td width="2%"></td>
			<td width="17%" valign="top">
				- <xsl:value-of select="substring-after(substring-after(substring-after(substring-after($path/dnp:iec61850Path,'\'),'.'),'.'),'.')"></xsl:value-of>
			</td>
			<td width="50%" valign="top">
				<xsl:value-of select="$path/dnp:dnp3XPath"></xsl:value-of>
			</td>
			<xsl:if test="$notes = 'yes'">
				<xsl:if test="$path/dnp:note">
					<td width="25%" bgcolor="PaleGreen">Note: <xsl:value-of select="$path/dnp:note"/>
					</td>
				</xsl:if>
			</xsl:if>
		</tr>
	</xsl:template>
	<xsl:template name="fourthLayer">
		<xsl:param name="path"/>
		<tr>
			<td width="2%"></td><td width="2%"></td><td width="2%"></td>
			<td width="19%" valign="top" colspan="2">
				- <xsl:value-of select="substring-after(substring-after(substring-after($path/dnp:iec61850Path,'\'),'.'),'.')"></xsl:value-of>
			</td>
			<td width="50%" valign="top">
				<xsl:value-of select="$path/dnp:dnp3XPath"></xsl:value-of>
			</td>
			<xsl:if test="$notes = 'yes'">
				<xsl:if test="$path/dnp:note">
					<td width="25%" bgcolor="PaleGreen">Note: <xsl:value-of select="$path/dnp:note"/>
					</td>
				</xsl:if>
			</xsl:if>
		</tr>
	</xsl:template>

	<xsl:template name="database">
		<xsl:param name="path"/>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<big>
					<b>3. Capabilities and Current Settings for Device Database (Outstation only)</b>
				</big>
				<p/>The following tables identify the capabilities and current settings for each DNP3 data type. 
				Details defining the data points available in the device are shown in part 5 of this Device Profile.
			</caption>
		</table>
		<!--  3.1 put out the Binary Input database -->
		<xsl:if test="$path/dnp:binaryInputGroup">
			<xsl:call-template name="binaryInputGroup">
				<xsl:with-param name="path" select="$path/dnp:binaryInputGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.2 put out the Double-bit Input database -->
		<xsl:if test="$path/dnp:doubleBitInputGroup">
			<xsl:call-template name="doubleBitInputGroup">
				<xsl:with-param name="path" select="$path/dnp:doubleBitInputGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.3 put out the Binary Output Status and CROB database -->
		<xsl:if test="$path/dnp:binaryOutputGroup">
			<xsl:call-template name="binaryOutputGroup">
				<xsl:with-param name="path" select="$path/dnp:binaryOutputGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.4 put out the Counter database -->
		<xsl:if test="$path/dnp:counterGroup">
			<xsl:call-template name="counterGroup">
				<xsl:with-param name="path" select="$path/dnp:counterGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.5 put out the Analogue Input database -->
		<xsl:if test="$path/dnp:analogInputGroup">
			<xsl:call-template name="analogInputGroup">
				<xsl:with-param name="path" select="$path/dnp:analogInputGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.6 put out the Analogue Output database -->
		<xsl:if test="$path/dnp:analogOutputGroup">
			<xsl:call-template name="analogOutputGroup">
				<xsl:with-param name="path" select="$path/dnp:analogOutputGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.7 put out the Sequential File Transfer database -->
		<xsl:if test="$path/dnp:sequentialFileTransfer">
			<xsl:call-template name="sequentialFileTransfer">
				<xsl:with-param name="path" select="$path/dnp:sequentialFileTransfer"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.8 put out the Octet Strings database -->
		<xsl:if test="$path/dnp:octetStringGroup">
			<xsl:call-template name="octetStringGroup">
				<xsl:with-param name="path" select="$path/dnp:octetStringGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.9 put out the Virtual Terminals database -->
		<xsl:if test="$path/dnp:virtualTerminalGroup">
			<xsl:call-template name="virtualTerminalGroup">
				<xsl:with-param name="path" select="$path/dnp:virtualTerminalGroup"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.10 put out the Data Set Prototype database -->
		<xsl:if test="$path/dnp:datasetPrototype">
			<xsl:call-template name="dsPrototype">
				<xsl:with-param name="path" select="$path/dnp:datasetPrototype"/>
			</xsl:call-template>
		</xsl:if>
		<!--  3.11 put out the Data Set Descriptors Contents database -->
		<xsl:if test="$path/dnp:datasetDescriptor">
			<xsl:call-template name="dsDescriptor">
				<xsl:with-param name="path" select="$path/dnp:datasetDescriptor"/>
			</xsl:call-template>
		</xsl:if>
		<!-- 3.12 User Data -->
		<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection3">
			<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
		</xsl:if>
	</xsl:template>
	
	<!-- templates used for section 1 -->
	<!-- 1.1  Device Identification Parameters -->
	<xsl:template name="deviceConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="5">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.1. Device Identification'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.1.1 Device Function -->
				<xsl:if test="$path/dnp:deviceFunction">
					<tr>
						<td width="35%" valign="top">1.1.1. Device Function:<p/>
							<i>Masters send DNP requests, while Outstations send DNP responses. 
						If a single physical device can perform both functions a separate Device Profile Document must be provided for each function.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:if test="$path/dnp:deviceFunction/dnp:capabilities/dnp:master">- Master</xsl:if>
							<xsl:if test="$path/dnp:deviceFunction/dnp:capabilities/dnp:outstation">- Outstation</xsl:if>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:deviceFunction"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:deviceFunction/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:deviceFunction/dnp:currentValue/dnp:master">- Master</xsl:if>
									<xsl:if test="$path/dnp:deviceFunction/dnp:currentValue/dnp:outstation">- Outstation</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:deviceFunction"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:deviceFunction/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.2 Vendor Name -->
				<xsl:if test="$path/dnp:vendorName">
					<tr>
						<td width="35%" valign="top">1.1.2. Vendor Name:<p/>
							<i>The name of the organization producing the device.
							<p/>Note: The current value of this outstation parameter is available remotely using protocol object Group 0 Variation 252.</i>
						</td>
						<xsl:call-template name="greyBox35"/>
						<td width="20%" valign="top">
							<xsl:value-of select="$path/dnp:vendorName/dnp:currentValue/dnp:value"/>&#160;
							<xsl:call-template name="showNotesCV">
								<xsl:with-param name="path" select="$path/dnp:vendorName"/>
							</xsl:call-template>
						</td>
						<td width="10%" valign="top" bgcolor="white">
							<font color="white">&#160;</font>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.3 Device Name -->
				<xsl:if test="$path/dnp:deviceName">
					<tr>
						<td width="35%" valign="top">1.1.3. Device Name:<p/>
							<i>The model and name of the device, sufficient to distinguish it from any other device from the same organization.
							<p/>Note: The current value of this outstation parameter is available remotely using protocol object Group 0 Variation 250.</i>
						</td>
						<xsl:call-template name="greyBox35"/>
						<td width="20%" valign="top">
							<xsl:value-of select="$path/dnp:deviceName/dnp:currentValue/dnp:value"/>&#160;
							<xsl:call-template name="showNotesCV">
								<xsl:with-param name="path" select="$path/dnp:deviceName"/>
							</xsl:call-template>
						</td>
						<td width="10%" valign="top" bgcolor="white">
							<font color="white">&#160;</font>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.4 Device manufacturer's hardware version string -->
				<xsl:if test="$path/dnp:hardwareVersion">
					<tr>
						<td width="35%" valign="top">1.1.4. Device manufacturer's hardware version string:<p/>
							<i>Note: The current value of this outstation parameter is available remotely using protocol object Group 0 Variation 243.</i>
						</td>
						<xsl:call-template name="greyBox35"/>
						<td width="20%" valign="top">
							<xsl:value-of select="$path/dnp:hardwareVersion/dnp:currentValue/dnp:value"/>&#160;
							<xsl:call-template name="showNotesCV">
								<xsl:with-param name="path" select="$path/dnp:hardwareVersion"/>
							</xsl:call-template>
						</td>
						<td width="10%" valign="top" bgcolor="white">
							<font color="white">&#160;</font>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.5 Device manufacturer's software version string -->
				<xsl:if test="$path/dnp:softwareVersion">
					<tr>
						<td width="35%" valign="top">1.1.5. Device manufacturer's software version string:<p/>
							<i>Note: The current value of this outstation parameter is available remotely using protocol object Group 0 Variation 242.</i>
						</td>
						<xsl:call-template name="greyBox35"/>
						<td width="20%" valign="top">
							<xsl:value-of select="$path/dnp:softwareVersion/dnp:currentValue/dnp:value"/>&#160;
							<xsl:call-template name="showNotesCV">
								<xsl:with-param name="path" select="$path/dnp:softwareVersion"/>
							</xsl:call-template>
						</td>
						<td width="10%" valign="top" bgcolor="white">
							<font color="white">&#160;</font>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.6 Device Profile Document Version Number -->
				<xsl:if test="$path/dnp:documentVersionNumber">
					<tr>
						<td width="35%" valign="top">1.1.6. Device Profile Document Version Number:<p/>
							<i>Version of the Device Profile Document is indicated by a whole number incremented with each new release. 
						This should match the latest version shown in the Revision History at the start of this document.</i>
						</td>
						<xsl:call-template name="greyBox35"/>
						<td width="20%" valign="top">
							<xsl:value-of select="$path/dnp:documentVersionNumber/dnp:currentValue/dnp:value"/>&#160;
							<xsl:call-template name="showNotesCV">
								<xsl:with-param name="path" select="$path/dnp:documentVersionNumber"/>
							</xsl:call-template>
						</td>
						<td width="10%" valign="top" bgcolor="white">
							<font color="white">&#160;</font>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.7 Highest DNP Level Supported -->
				<xsl:if test="$path/dnp:dnpLevelSupported">
					<tr>
						<td width="35%" valign="top">1.1.7. DNP Levels Supported for:<p/>
							<i>Indicate each DNP3 Level to which the device conforms fully. For Masters, requests and responses
						can be indicated independently.</i>
						</td>
						<xsl:if test="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master">
							<td width="35%" valign="top">
								<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
									<tr>
										<td width="40%" valign="top" colspan="2">Masters Only</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>Requests</center>
										</td>
										<td width="20%" valign="top">
											<center>Responses</center>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:requests/dnp:none"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:responses/dnp:none"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="60%" valign="middle">None</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:requests/dnp:level1"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:responses/dnp:level1"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="60%" valign="middle">Level 1</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:requests/dnp:level2"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:responses/dnp:level2"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="60%" valign="middle">Level 2</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:requests/dnp:level3"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:master/dnp:responses/dnp:level3"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="60%" valign="middle">Level 3</td>
									</tr>
								</table>
								<xsl:call-template name="showNotesCap">
									<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported"/>
								</xsl:call-template>
							</td>
						</xsl:if>
						<!-- repeat for outstation -->
						<xsl:if test="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:outStation">
							<td width="35%" valign="top">
								<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
									<tr>
										<td width="20%" valign="top" colspan="2">Outstations Only</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2">Requests and Responses</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:outStation/dnp:none"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="80%" valign="middle">None</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:outStation/dnp:level1"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="80%" valign="middle">Level 1</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:outStation/dnp:level2"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="80%" valign="middle">Level 2</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:outStation/dnp:level3"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="80%" valign="middle">Level 3</td>
									</tr>
									<tr>
										<td width="20%" valign="top">
											<center>
												<xsl:call-template name="tickbox">
													<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:capabilities/dnp:outStation/dnp:level4"/>
												</xsl:call-template>
											</center>
										</td>
										<td width="80%" valign="middle">Level 4</td>
									</tr>
								</table>
								<xsl:call-template name="showNotesCap">
									<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported"/>
								</xsl:call-template>
							</td>
						</xsl:if>
						<xsl:choose>
							<xsl:when test="$path/dnp:dnpLevelSupported/dnp:capabilities">
						</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox35"/>
							</xsl:otherwise>
						</xsl:choose>
						<xsl:choose>
							<xsl:when test="$path/dnp:dnpLevelSupported/dnp:currentValue">
								<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master">
									<td width="20%" valign="top">
										For requests:<br/>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:requests/dnp:none">None<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:requests/dnp:level1">Level 1<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:requests/dnp:level2">Level 2<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:requests/dnp:level3">Level 3<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:requests/dnp:level4">Level 4<br/>
										</xsl:if>
										<br/>For responses:<br/>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:responses/dnp:none">None<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:responses/dnp:level1">Level 1<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:responses/dnp:level2">Level 2<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:responses/dnp:level3">Level 3<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:master/dnp:responses/dnp:level4">Level 4<br/>
										</xsl:if>
									</td>
								</xsl:if>
								<!-- repeat for otstation -->
								<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:outStation">
									<td width="20%" valign="top">
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:outStation/dnp:none">None<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:outStation/dnp:level1">Level 1<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:outStation/dnp:level2">Level 2<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:outStation/dnp:level3">Level 3<br/>
										</xsl:if>
										<xsl:if test="$path/dnp:dnpLevelSupported/dnp:currentValue/dnp:outStation/dnp:level4">Level 4<br/>
										</xsl:if>
										&#160;
									</td>
								</xsl:if>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:dnpLevelSupported/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.8 Supported Function Blocks -->
				<xsl:if test="$path/dnp:supportedFunctionBlocks">
					<tr>
						<td width="35%" valign="top">1.1.8. Supported Function Blocks:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:capabilities/dnp:selfAddressReservation"/>
								<xsl:with-param name="prompt" select="'Self Address Reservation'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:capabilities/dnp:dataSets"/>
								<xsl:with-param name="prompt" select="'Data Sets'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:capabilities/dnp:fileTransfer"/>
								<xsl:with-param name="prompt" select="'File Transfer'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:capabilities/dnp:virtualTerminal"/>
								<xsl:with-param name="prompt" select="'Virtual Terminal'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:capabilities/dnp:mappingToIEC61850ObjectModels"/>
								<xsl:with-param name="prompt" select="'Mapping to IEC 61850 Object Models defined in a DNP3 XML file'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:capabilities/dnp:FC31ActivateConfiguration"/>
								<xsl:with-param name="prompt" select="'Function code 31, activate configuration'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:capabilities/dnp:secureAuthentication"/>
								<xsl:with-param name="prompt" select="'Secure Authentication (if checked then see 1.12)'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:supportedFunctionBlocks/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:supportedFunctionBlocks/dnp:currentValue/dnp:selfAddressReservation">Self Address<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:supportedFunctionBlocks/dnp:currentValue/dnp:dataSets">Data Sets<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:supportedFunctionBlocks/dnp:currentValue/dnp:fileTransfer">File Transfer<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:supportedFunctionBlocks/dnp:currentValue/dnp:virtualTerminal">Virtual Terminal<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:supportedFunctionBlocks/dnp:currentValue/dnp:mappingToIEC61850ObjectModels">IEC 61850 Mapping<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:supportedFunctionBlocks/dnp:currentValue/dnp:FC31ActivateConfiguration">FC31<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:supportedFunctionBlocks/dnp:currentValue/dnp:secureAuthentication">Secure Authentication<br/>
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:supportedFunctionBlocks/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.9 Notable Additions -->
				<xsl:if test="$path/dnp:notableAdditions">
					<tr>
						<td width="35%" valign="top" align="left">1.1.9. Notable Additions:<p/>
							<i>A brief description intended to quickly identify (for the reader)  
							the most obvious features the device supports in addition to the Highest DNP Level Supported. 
							The complete list of features is described in the Implementation Table.</i>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:notableAdditions/dnp:capabilities">
								<td width="35%" valign="top" align="left">
									<xsl:for-each select="$path/dnp:notableAdditions/dnp:capabilities/dnp:notableAddition">
										<xsl:value-of select="."/>
										<br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCap">
										<xsl:with-param name="path" select="$path/dnp:notableAdditions"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox35"/>
							</xsl:otherwise>
						</xsl:choose>
						<xsl:choose>
							<xsl:when test="$path/dnp:notableAdditions/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:for-each select="$path/dnp:notableAdditions/dnp:currentValue/dnp:notableAddition">
										<xsl:value-of select="."/>
										<br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:notableAdditions"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:notableAdditions/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.10 Methods to set Configurable Parameters -->
				<xsl:if test="$path/dnp:configurationMethods">
					<tr>
						<td width="35%" valign="top">1.1.10. Methods to set Configurable Parameters:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:xmlViaFileTransfer"/>
								<xsl:with-param name="prompt" select="'XML - Loaded via DNP3 File Transfer'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:xmlViaOtherTransportMechanism"/>
								<xsl:with-param name="prompt" select="'XML - Loaded via other transport mechanism'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:terminal"/>
								<xsl:with-param name="prompt" select="'Terminal - ASCII Terminal Command Line'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:software"/>
								<xsl:with-param name="prompt" select="'Software - Vendor software named '"/>
								<xsl:with-param name="units">
									<xsl:value-of select="$path/dnp:configurationMethods/dnp:capabilities/dnp:software"/>
								</xsl:with-param>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:proprietaryFileViaFileTransfer"/>
								<xsl:with-param name="prompt" select="'Proprietary file loaded via DNP3 File Transfer'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:proprietaryFileViaOtherTransportMechanism"/>
								<xsl:with-param name="prompt" select="'Proprietary file loaded via other transport mechanism'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:direct"/>
								<xsl:with-param name="prompt" select="'Direct - Keypad on device front panel'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:factory"/>
								<xsl:with-param name="prompt" select="'Factory - Specified when device is ordered'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:protocol"/>
								<xsl:with-param name="prompt" select="'Protocol - Set via DNP3 (e.g. assign class)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:capabilities/dnp:other"/>
								<xsl:with-param name="prompt" select="'Other - explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:configurationMethods/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:xmlViaFileTransfer">XML via DNP3<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:xmlViaOtherTransportMechanism">XML via other<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:terminal">Terminal<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:software">Software<br/>
										Vendor software named <b><xsl:value-of select="$path/dnp:configurationMethods/dnp:currentValue/dnp:software/dnp:name"/></b><br/>
										<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:software/dnp:version">
											Version <b><xsl:value-of select="$path/dnp:configurationMethods/dnp:currentValue/dnp:software/dnp:version"/><br/></b>
										</xsl:if>
									</xsl:if>
									<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:direct">Direct<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:factory">Factory<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:configurationMethods/dnp:currentValue/dnp:protocol">Protocol<br/>
									</xsl:if>
									<xsl:for-each select="$path/dnp:configurationMethods/dnp:currentValue/dnp:other">Other, <b>
										<xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:configurationMethods"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:configurationMethods/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.11 DNP3 XML files available On-line -->
				<xsl:if test="$path/dnp:onlineXmlFileNames">
					<tr>
						<td width="35%" valign="top">1.1.11. DNP3 XML files available On-line:<p/>
							<i>XML configuration file names that can be read or written through DNP3 File Transfer to a device.<p/>
						A device's currently running configuration is returned by DNP3 on-line XML file read from the device.<p/>
						DNP3 on-line XML file write to a device will update the device's configuration when the Activate Configuration 
						(function code 31) is received.</i>
						</td>
						<td width="35%" valign="top">
							<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
								<tr>
									<td width="8%" valign="top">
										<u>
											<small>Rd</small>
										</u>
									</td>
									<td width="8%" valign="top">
										<u>
											<small>Wr</small>
										</u>
									</td>
									<td width="30%" valign="top">
										<u>
											<small>Filename</small>
										</u>
									</td>
									<td width="54%" valign="top">
										<u>
											<small>Description of Contents</small>
										</u>
										<br/>
									</td>
								</tr>
								<tr>
									<td width="16%" valign="middle" colspan="2">
										<xsl:choose>
											<xsl:when test="$path/dnp:onlineXmlFileNames/dnp:capabilities/dnp:dnpDPReadSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="30%" valign="middle">dnpDP.xml</td>
									<td width="54%" valign="middle">Complete Device Profile</td>
								</tr>
								<tr>
									<td width="16%" valign="middle" colspan="2">
										<xsl:choose>
											<xsl:when test="$path/dnp:onlineXmlFileNames/dnp:capabilities/dnp:dnpDPCapReadSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="30%" valign="middle">dnpDPCap.xml</td>
									<td width="54%" valign="middle">Device Profile Capabilities</td>
								</tr>
								<tr>
									<td width="16%" valign="middle" colspan="2">
										<xsl:choose>
											<xsl:when test="$path/dnp:onlineXmlFileNames/dnp:capabilities/dnp:dnpDPCfgReadSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="30%" valign="middle">dnpDPCfg.xml</td>
									<td width="54%" valign="middle">Device Profile config values</td>
								</tr>
								<xsl:for-each select="$path/dnp:onlineXmlFileNames/dnp:capabilities/dnp:xmlFile">
									<tr>
										<td width="8%" valign="middle">
											<xsl:choose>
												<xsl:when test="./dnp:readAccess = 'true'">
													<input type="checkbox" name="box" checked="checked"/>
												</xsl:when>
												<xsl:otherwise>
													<input type="checkbox" name="box"/>
												</xsl:otherwise>
											</xsl:choose>
											<label for="box"/>
										</td>
										<td width="8%" valign="middle">
											<xsl:choose>
												<xsl:when test="./dnp:writeAccess = 'true'">
													<input type="checkbox" name="box" checked="checked"/>
												</xsl:when>
												<xsl:otherwise>
													<input type="checkbox" name="box"/>
												</xsl:otherwise>
											</xsl:choose>
											<label for="box"/>
										</td>
										<td width="30%" valign="middle">
											<xsl:value-of select="./dnp:filename"/>
										</td>
										<td width="54%" valign="middle">
											<xsl:value-of select="./dnp:description"/>
										</td>
									</tr>
									<tr>
										<td colspan="4">
											<xsl:call-template name="showNotesGeneral">
												<xsl:with-param name="path" select="."/>
											</xsl:call-template>
										</td>
									</tr>
								</xsl:for-each>
							</table>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:onlineXmlFileNames/dnp:currentValue">
								<td width="20%" valign="top">
									<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
										<tr>
											<td width="15%" valign="top">
												<u>
													<small>Rd</small>
												</u>
											</td>
											<td width="15%" valign="top">
												<u>
													<small>Wr</small>
												</u>
											</td>
											<td width="70%" valign="top">
												<u>
													<small>Filename</small>
												</u>
											</td>
										</tr>
										<tr>
											<td width="30%" valign="top" colspan="2">
												<xsl:choose>
													<xsl:when test="$path/dnp:onlineXmlFileNames/dnp:currentValue/dnp:dnpDPReadSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="70%" valign="middle">dnpDP.xml</td>
										</tr>
										<tr>
											<td width="30%" valign="top" colspan="2">
												<xsl:choose>
													<xsl:when test="$path/dnp:onlineXmlFileNames/dnp:currentValue/dnp:dnpDPCapReadSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="70%" valign="middle">dnpDPCap.xml</td>
										</tr>
										<tr>
											<td width="30%" valign="top" colspan="2">
												<xsl:choose>
													<xsl:when test="$path/dnp:onlineXmlFileNames/dnp:currentValue/dnp:dnpDPCfgReadSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="70%" valign="middle">dnpDPCfg.xml</td>
										</tr>
										<xsl:for-each select="$path/dnp:onlineXmlFileNames/dnp:currentValue/dnp:xmlFile">
											<tr>
												<td width="15%" valign="top">
													<xsl:choose>
														<xsl:when test="./dnp:readAccess = 'true'">
															<input type="checkbox" name="box" checked="checked"/>
														</xsl:when>
														<xsl:otherwise>
															<input type="checkbox" name="box"/>
														</xsl:otherwise>
													</xsl:choose>
													<label for="box"/>
												</td>
												<td width="15%" valign="top">
													<xsl:choose>
														<xsl:when test="./dnp:writeAccess = 'true'">
															<input type="checkbox" name="box" checked="checked"/>
														</xsl:when>
														<xsl:otherwise>
															<input type="checkbox" name="box"/>
														</xsl:otherwise>
													</xsl:choose>
													<label for="box"/>
												</td>
												<td width="70%" valign="top">
													<xsl:value-of select="./dnp:filename"/>
												</td>
											</tr>
											<tr>
												<td colspan="4">
													<xsl:call-template name="showNotesGeneral">
														<xsl:with-param name="path" select="."/>
													</xsl:call-template>
												</td>
											</tr>
										</xsl:for-each>
									</table>
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:onlineXmlFileNames/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.12 DNP3 XML files available Off-line -->
				<xsl:if test="$path/dnp:offlineXmlFileNames">
					<tr>
						<td width="35%" valign="top">1.1.12. External DNP3 XML files available Off-line:<p/>
							<i>XML configuration file names that can be read or written from an external system, typically from a system
						that maintains the outstation configuration.<p/>
						External off-line XML file read permits an XML definition of a new configuration to be supplied from off-line 
						configuration tools.<p/>
						External off-line XML file write permits an XML definition of a new configuration to be supplied to off-line 
						configuration tools.</i>
						</td>
						<td width="35%" valign="top">
							<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
								<tr>
									<td width="8%" valign="top">
										<u>
											<small>Rd</small>
										</u>
									</td>
									<td width="8%" valign="top">
										<u>
											<small>Wr</small>
										</u>
									</td>
									<td width="30%" valign="top">
										<u>
											<small>Filename</small>
										</u>
									</td>
									<td width="54%" valign="top">
										<u>
											<small>Description of Contents</small>
										</u>
										<br/>
									</td>
								</tr>
								<tr>
									<td width="8%" valign="middle">
										<xsl:choose>
											<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:capabilities/dnp:dnpDPReadSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="8%" valign="middle">
										<xsl:choose>
											<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:capabilities/dnp:dnpDPWriteSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="30%" valign="middle">dnpDP.xml</td>
									<td width="54%" valign="middle">Complete Device Profile</td>
								</tr>
								<tr>
									<td width="8%" valign="middle">
										<xsl:choose>
											<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:capabilities/dnp:dnpDPCapReadSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="8%" valign="middle">
										<xsl:choose>
											<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:capabilities/dnp:dnpDPCapWriteSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="30%" valign="middle">dnpDPCap.xml</td>
									<td width="54%" valign="middle">Device Profile Capabilities</td>
								</tr>
								<tr>
									<td width="8%" valign="middle">
										<xsl:choose>
											<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:capabilities/dnp:dnpDPCfgReadSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="8%" valign="middle">
										<xsl:choose>
											<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:capabilities/dnp:dnpDPCfgWriteSupported">
												<input type="checkbox" name="box" checked="checked"/>
											</xsl:when>
											<xsl:otherwise>
												<input type="checkbox" name="box"/>
											</xsl:otherwise>
										</xsl:choose>
										<label for="box"/>
									</td>
									<td width="30%" valign="middle">dnpDPCfg.xml</td>
									<td width="54%" valign="middle">Device Profile config values</td>
								</tr>
								<xsl:for-each select="$path/dnp:offlineXmlFileNames/dnp:capabilities/dnp:xmlFile">
									<tr>
										<td width="8%" valign="middle">
											<xsl:choose>
												<xsl:when test="./dnp:readAccess = 'true'">
													<input type="checkbox" name="box" checked="checked"/>
												</xsl:when>
												<xsl:otherwise>
													<input type="checkbox" name="box"/>
												</xsl:otherwise>
											</xsl:choose>
											<label for="box"/>
										</td>
										<td width="8%" valign="middle">
											<xsl:choose>
												<xsl:when test="./dnp:writeAccess = 'true'">
													<input type="checkbox" name="box" checked="checked"/>
												</xsl:when>
												<xsl:otherwise>
													<input type="checkbox" name="box"/>
												</xsl:otherwise>
											</xsl:choose>
											<label for="box"/>
										</td>
										<td width="30%" valign="middle">
											<xsl:value-of select="./dnp:filename"/>
										</td>
										<td width="54%" valign="middle">
											<xsl:value-of select="./dnp:description"/>
										</td>
									</tr>
									<tr>
										<td colspan="4">
											<xsl:call-template name="showNotesGeneral">
												<xsl:with-param name="path" select="."/>
											</xsl:call-template>
										</td>
									</tr>
								</xsl:for-each>
							</table>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:offlineXmlFileNames"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:currentValue">
								<td width="20%" valign="top">
									<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
										<tr>
											<td width="15%" valign="top">
												<u>
													<small>Rd</small>
												</u>
											</td>
											<td width="15%" valign="top">
												<u>
													<small>Wr</small>
												</u>
											</td>
											<td width="70%" valign="top">
												<u>
													<small>Filename</small>
												</u>
											</td>
										</tr>
										<tr>
											<td width="15%" valign="top">
												<xsl:choose>
													<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:currentValue/dnp:dnpDPReadSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="15%" valign="top">
												<xsl:choose>
													<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:currentValue/dnp:dnpDPWriteSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="70%" valign="middle">dnpDP.xml</td>
										</tr>
										<tr>
											<td width="15%" valign="top">
												<xsl:choose>
													<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:currentValue/dnp:dnpDPCapReadSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="15%" valign="top">
												<xsl:choose>
													<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:currentValue/dnp:dnpDPCapWriteSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="70%" valign="middle">dnpDPCap.xml</td>
										</tr>
										<tr>
											<td width="15%" valign="top">
												<xsl:choose>
													<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:currentValue/dnp:dnpDPCfgReadSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="15%" valign="top">
												<xsl:choose>
													<xsl:when test="$path/dnp:offlineXmlFileNames/dnp:currentValue/dnp:dnpDPCfgWriteSupported">
														<input type="checkbox" name="box" checked="checked"/>
													</xsl:when>
													<xsl:otherwise>
														<input type="checkbox" name="box"/>
													</xsl:otherwise>
												</xsl:choose>
												<label for="box"/>
											</td>
											<td width="70%" valign="middle">dnpDPCfg.xml</td>
										</tr>
										<xsl:for-each select="$path/dnp:offlineXmlFileNames/dnp:currentValue/dnp:xmlFile">
											<tr>
												<td width="15%" valign="top">
													<xsl:choose>
														<xsl:when test="./dnp:readAccess = 'true'">
															<input type="checkbox" name="box" checked="checked"/>
														</xsl:when>
														<xsl:otherwise>
															<input type="checkbox" name="box"/>
														</xsl:otherwise>
													</xsl:choose>
													<label for="box"/>
												</td>
												<td width="15%" valign="top">
													<xsl:choose>
														<xsl:when test="./dnp:writeAccess = 'true'">
															<input type="checkbox" name="box" checked="checked"/>
														</xsl:when>
														<xsl:otherwise>
															<input type="checkbox" name="box"/>
														</xsl:otherwise>
													</xsl:choose>
													<label for="box"/>
												</td>
												<td width="70%" valign="top">
													<xsl:value-of select="./dnp:filename"/>
												</td>
											</tr>
											<tr>
												<td colspan="4">
													<xsl:call-template name="showNotesGeneral">
														<xsl:with-param name="path" select="."/>
													</xsl:call-template>
												</td>
											</tr>
										</xsl:for-each>
									</table>
								</td>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:offlineXmlFileNames"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:offlineXmlFileNames/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.1.13 Connections Supported -->
				<xsl:if test="$path/dnp:connectionsSupported">
					<tr>
						<td width="35%" valign="top">1.1.13. Connections Supported:<p/>
<!--							<i>If IP Networking is supported, both TCP and UDP are required to meet the requirements of DNP3 Specification Volume 7,
						IP Networking Specification.</i> -->
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:connectionsSupported/dnp:capabilities/dnp:serial"/>
								<xsl:with-param name="prompt" select="'Serial (complete section 1.2)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:connectionsSupported/dnp:capabilities/dnp:network"/>
								<xsl:with-param name="prompt" select="'IP Networking (complete section 1.3)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:connectionsSupported/dnp:capabilities/dnp:other"/>
								<xsl:with-param name="prompt" select="'Other, explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:connectionsSupported"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:connectionsSupported/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:connectionsSupported/dnp:currentValue/dnp:serial">Serial<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:connectionsSupported/dnp:currentValue/dnp:network">IP Networking<br/>
									</xsl:if>
									<xsl:for-each select="$path/dnp:connectionsSupported/dnp:currentValue/dnp:other">Other, <b>
										<xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:connectionsSupported"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:connectionsSupported/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.1.14 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-1">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- templates to put out a notes box of the indicated table column  -->
	<xsl:template name="showNotesCap">
		<xsl:param name="path"/>
		<xsl:if test="$notes = 'yes'">
			<xsl:for-each select="$path/dnp:capabilities/dnp:note">
				<br/>
				<table border="0" width="100%" bgcolor="PaleGreen">
					<tbody>
						<tr>
							<td>Note: <xsl:value-of select="."/>
							</td>
						</tr>
					</tbody>
				</table>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>
	<xsl:template name="showNotesCV">
		<xsl:param name="path"/>
		<xsl:if test="$notes = 'yes'">
			<xsl:for-each select="$path/dnp:currentValue/dnp:note">
				<br/>
				<table border="0" width="100%" bgcolor="PaleGreen">
					<tbody>
						<tr>
							<td>Note: <xsl:value-of select="."/>
							</td>
						</tr>
					</tbody>
				</table>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>
	<xsl:template name="showNotesGeneral">
		<xsl:param name="path"/>
		<xsl:if test="$notes = 'yes'">
			<xsl:for-each select="$path/dnp:note">
				<br/>
				<table border="0" width="100%" bgcolor="PaleGreen">
					<tbody>
						<tr>
							<td>Note: <xsl:value-of select="."/>
							</td>
						</tr>
					</tbody>
				</table>
			</xsl:for-each>
		</xsl:if>
	</xsl:template>
	<!-- templates to put out a grey box of the indicated table column width -->
	<xsl:template name="greyBox10">
		<td width="20%" valign="top" bgcolor="lightgrey">
			<font color="lightgrey">&#160;</font>
		</td>
	</xsl:template>
	<xsl:template name="greyBox20">
		<td width="20%" valign="top" bgcolor="lightgrey">
			<font color="lightgrey">&#160;</font>
		</td>
	</xsl:template>
	<xsl:template name="greyBox35">
		<td width="35%" valign="top" bgcolor="lightgrey">
			<font color="lightgrey">&#160;</font>
		</td>
	</xsl:template>
	<!-- templates to put out a white box of the indicated table column width -->
	<xsl:template name="whiteBox10">
		<td width="20%" valign="top" bgcolor="white">
			<font color="white">&#160;</font>
		</td>
	</xsl:template>
	<xsl:template name="whiteBox20">
		<td width="20%" valign="top" bgcolor="white">
			<font color="white">&#160;</font>
		</td>
	</xsl:template>
	<xsl:template name="whiteBox35">
		<td width="35%" valign="top" bgcolor="white">
			<font color="white">&#160;</font>
		</td>
	</xsl:template>
	<xsl:template name="whiteBox45">
		<td width="45%" valign="top" bgcolor="white">
			<font color="white">&#160;</font>
		</td>
	</xsl:template>
	<!-- 1.2  Serial Connection Parameters -->
	<xsl:template name="serialConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.2. Serial Connections'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.2.1 Port Name -->
				<xsl:if test="$path/dnp:portName">
					<tr>
						<td width="35%" valign="top">1.2.1. Port Name:<p/>
							<i>Name used to reference the communications port defined in this section.</i>
						</td>
						<xsl:call-template name="greyBox35"/>
						<td width="20%" valign="top">
							<xsl:value-of select="$path/dnp:portName/dnp:currentValue/dnp:value"/>
							<xsl:call-template name="showNotesCV">
								<xsl:with-param name="path" select="$path/dnp:portName"/>
							</xsl:call-template>
							&#160;
						</td>
						<td width="10%" valign="top" bgcolor="white">
							<font color="white">&#160;</font>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.2.2 Serial Connection Parameters -->
				<xsl:if test="$path/dnp:serialParameters">
					<tr>
						<td width="35%" valign="top">1.2.2. Serial Connection Parameters:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:serialParameters/dnp:capabilities/dnp:asynchronous"/>
								<xsl:with-param name="prompt" select="'Asynchronous - 8 Data Bits, 1 Start Bit, 1 Stop Bit, No Parity'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:serialParameters/dnp:capabilities/dnp:other"/>
								<xsl:with-param name="prompt" select="'Other, explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:serialParameters"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:serialParameters/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:serialParameters/dnp:currentValue/dnp:asynchronous">Asynchronous</xsl:if>
									<xsl:for-each select="$path/dnp:serialParameters/dnp:currentValue/dnp:other">Other, <b><xsl:value-of select="./dnp:explanation"/></b></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:serialParameters"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:serialParameters/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.2.3 Baud Rate -->
				<xsl:if test="$path/dnp:baudRate">
					<tr>
						<td width="35%" valign="top">1.2.3. Baud Rate:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:baudRate"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:baudRate"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:baudRate/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:baudRate/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:baudRate"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:baudRate/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.2.4 Flow Control -->
				<xsl:if test="$path/dnp:flowControl">
					<tr>
						<td width="35%" valign="top">1.2.4. Hardware Flow Control (Handshaking):<p/>
							<i>Describe hardware signaling requirements of the interface.<p/>
							Where a transmitter or receiver is inhibited until a given control signal is asserted, it is considered to require that signal prior to 
							sending or receiving characters.<p/>
							Where a signal is asserted prior to transmitting, that signal will be maintained active until after the end of transmission.<p/>
							Where a signal is asserted to enable reception, any data sent to the device when the signal is not active could be discarded.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:none"/>
								<xsl:with-param name="prompt" select="'None'"/>
							</xsl:call-template>
							<xsl:if test="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options">
								<b>RS-232 / V.24 / V.28 Options:</b>
								<br/>
								<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
									<tr>
										<td width="10%" valign="top"></td>
										<td width="90%" valign="top" colspan="4"><u>Asserts:</u>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:assertsRTSBeforeTx"/>
												<xsl:with-param name="prompt" select="'RTS Before Tx'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:assertsDTRBeforeTx"/>
												<xsl:with-param name="prompt" select="'DTR Before Tx'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:assertsRTSBeforeRx"/>
												<xsl:with-param name="prompt" select="'RTS Before Rx'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:assertsDTRBeforeRx"/>
												<xsl:with-param name="prompt" select="'DTR Before Rx'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:alwaysAssertsRTS"/>
												<xsl:with-param name="prompt" select="'Always RTS'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:alwaysAssertsDTR"/>
												<xsl:with-param name="prompt" select="'Always DTR'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="10%" valign="top"/>
										<td width="90%" valign="top" colspan="4"><u>Requires Before Tx:</u></td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">CTS</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresCTSBeforeTx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresCTSBeforeTx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">DCD</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDCDBeforeTx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDCDBeforeTx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">DSR</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDSRBeforeTx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDSRBeforeTx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">RI</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresRIBeforeTx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresRIBeforeTx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:rxInactive"/>
												<xsl:with-param name="prompt" select="'Requires Rx Inactive before Tx'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="10%" valign="top"/>
										<td width="90%" valign="top" colspan="4"><u>Requires Before Rx:</u></td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">CTS</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresCTSBeforeRx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresCTSBeforeRx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">DCD</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDCDBeforeRx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDCDBeforeRx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">DSR</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDSRBeforeRx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresDSRBeforeRx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="10%" valign="baseline">RI</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresRIBeforeRx/dnp:asserted"/>
												<xsl:with-param name="prompt" select="'Asserted'"/>
											</xsl:call-template>
										</td>
										<td width="35%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:requiresRIBeforeRx/dnp:deasserted"/>
												<xsl:with-param name="prompt" select="'Deasserted'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="10%" valign="top"/>
										<td width="90%" valign="top" colspan="4"><u>Always Ignores:</u></td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:alwaysIgnoresCTS"/>
												<xsl:with-param name="prompt" select="'CTS'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:alwaysIgnoresDCD"/>
												<xsl:with-param name="prompt" select="'DCD'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:alwaysIgnoresDSR"/>
												<xsl:with-param name="prompt" select="'DSR'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="20%" valign="top" colspan="2"/>
										<td width="80%" valign="top" colspan="3">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:alwaysIgnoresRI"/>
												<xsl:with-param name="prompt" select="'RI'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="10%" valign="top"/>
										<td width="90%" valign="top" colspan="4">
											<xsl:call-template name="tickbox-explain">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs232Options/dnp:other"/>
												<xsl:with-param name="prompt" select="'Other, explain:'"/>
											</xsl:call-template>
										</td>
									</tr>
								</table>
							</xsl:if>
							<xsl:if test="$path/dnp:flowControl/dnp:capabilities/dnp:rs422Options">
								<p/><b>RS-422 / V.11 Options:</b>
								<br/>
								<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
									<tr>
										<td width="10%" valign="top"/>
										<td width="75%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs422Options/dnp:requiresIndicationBeforeRx"/>
												<xsl:with-param name="prompt" select="'Requires Indication before Rx'"/>
											</xsl:call-template>
											<xsl:call-template name="showNotesGeneral">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs422Options/dnp:requiresIndicationBeforeRx"/>
											</xsl:call-template>
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs422Options/dnp:assertsControlBeforeTx"/>
												<xsl:with-param name="prompt" select="'Asserts Control before Tx'"/>
											</xsl:call-template>
											<xsl:call-template name="showNotesGeneral">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs422Options/dnp:assertsControlBeforeTx"/>
											</xsl:call-template>
											<xsl:call-template name="tickbox-explain">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs422Options/dnp:other"/>
												<xsl:with-param name="prompt" select="'Other, explain:'"/>
											</xsl:call-template>&#160;
										</td>
									</tr>
								</table>
							</xsl:if>
							<xsl:if test="$path/dnp:flowControl/dnp:capabilities/dnp:rs485Options">
								<p/><b>RS-485 Options:</b>
								<br/>
								<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
									<tr>
										<td width="10%" valign="top"/>
										<td width="75%" valign="top">
											<xsl:call-template name="tickbox">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs485Options/dnp:requiresRxInactiveBeforeTx"/>
												<xsl:with-param name="prompt" select="'Requires Rx inactive before Tx'"/>
											</xsl:call-template>
											<xsl:call-template name="showNotesGeneral">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs485Options/dnp:requiresRxInactiveBeforeTx"/>
											</xsl:call-template>
											<xsl:call-template name="tickbox-explain">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:rs485Options/dnp:other"/>
												<xsl:with-param name="prompt" select="'Other, explain:'"/>
											</xsl:call-template>&#160;
										</td>
									</tr>
								</table>
							</xsl:if>
							<xsl:if test="$path/dnp:flowControl/dnp:capabilities/dnp:other">
								<p/>
								<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
									<tr>
										<td valign="top">
											<xsl:call-template name="tickbox-explain">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:capabilities/dnp:other"/>
												<xsl:with-param name="prompt" select="'Other, explain:'"/>
											</xsl:call-template>
										</td>
									</tr>
								</table>
							</xsl:if>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:flowControl"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:flowControl/dnp:currentValue">
								<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:none">None
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:flowControl"/>
									</xsl:call-template>
								</xsl:if>
								<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options">
									<b>RS-232 / V.24 / V.28 Options:</b>
									<br/>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsRTSBeforeTx">
										Asserts RTS before Tx
										<xsl:call-template name="showNotesGeneral">
												<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsRTSBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsDTRBeforeTx">
										Asserts DTR before Tx
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsDTRBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsRTSBeforeRx">
										Asserts RTS before Rx
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsRTSBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsDTRBeforeRx">
										Asserts DTR before Rx
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:assertsDTRBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysAssertsRTS">
										Always asserts RTS
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysAssertsRTS"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysAssertsDTR">
										Always asserts DTR
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysAssertsDTR"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeTx/dnp:asserted">
										Before Tx, Requires CTS asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeTx/dnp:deasserted">
										Before Tx, Requires CTS deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeTx/dnp:asserted">
										Before Tx, Requires DCD asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeTx/dnp:deasserted">
										Before Tx, Requires DCD deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeTx/dnp:asserted">
										Before Tx, Requires DSR asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeTx/dnp:deasserted">
										Before Tx, Requires DSR deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeTx/dnp:asserted">
										Before Tx, Requires RI asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeTx/dnp:deasserted">
										Before Tx, Requires RI deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:rxInactive">
										Before Tx, Requires Rx Inactive
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:rxInactive"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeRx/dnp:asserted">
										Before Rx, Requires CTS asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeRx/dnp:deasserted">
										Before Rx, Requires CTS deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresCTSBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeRx/dnp:asserted">
										Before Rx, Requires DCD asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeRx/dnp:deasserted">
										Before Rx, Requires DCD deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDCDBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeRx/dnp:asserted">
										Before Rx, Requires DSR asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeRx/dnp:deasserted">
										Before Rx, Requires DSR deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresDSRBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeRx/dnp:asserted">
										Before Rx, Requires RI asserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeRx/dnp:deasserted">
										Before Rx, Requires RI deasserted
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:requiresRIBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresCTS">
										Always ignores CTS
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresCTS"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresDCD">
										Always ignores DCD
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresDCD"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresDSR">
										Always ignores DSR
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresDSR"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresRI">
										Always ignores RI
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:alwaysIgnoresRI"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:for-each select="$path/dnp:flowControl/dnp:currentValue/dnp:rs232Options/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:flowControl"/>
									</xsl:call-template>
									<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs422Options">
									<b>RS-422 / V.11 Options:</b>
									<br/>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs422Options/dnp:requiresIndicationBeforeRx">
										Requires indication before Rx
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs422Options/dnp:requiresIndicationBeforeRx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs422Options/dnp:assertsControlBeforeTx">
										Asserts control before Tx
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs422Options/dnp:assertsControlBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:for-each select="$path/dnp:flowControl/dnp:currentValue/dnp:rs422Options/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:flowControl"/>
									</xsl:call-template>
									<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs485Options">
									<b>RS-485Options:</b>
									<xsl:if test="$path/dnp:flowControl/dnp:currentValue/dnp:rs485Options/dnp:requiresRxInactiveBeforeTx">
										Requires Rx inactive before Tx<br/>
										<xsl:call-template name="showNotesGeneral">
											<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:currentValue/dnp:rs485Options/dnp:requiresRxInactiveBeforeTx"/>
										</xsl:call-template>
										<br/>
									</xsl:if>
									<xsl:for-each select="$path/dnp:flowControl/dnp:currentValue/dnp:rs485Options/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:flowControl"/>
									</xsl:call-template>
									&#160;
								</xsl:if>
								&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<td width="20%" valign="baseline">
									&#160;
								</td>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:flowControl/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.2.5 Interval to Request Link Status -->
				<xsl:if test="$path/dnp:linkStatusInterval">
					<tr>
						<td width="35%" valign="top">1.2.5. Interval to Request Link Status:<p/>
							<i>Indicates how often to send Data Link Layer status requests on a serial connection. This parameter is separate from the TCP Keep-alive timer.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:linkStatusInterval/dnp:capabilities/dnp:notSupported"/>
								<xsl:with-param name="prompt" select="'Not Supported'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:linkStatusInterval"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
								<xsl:with-param name="units" select="'seconds'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:linkStatusInterval"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:linkStatusInterval/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:linkStatusInterval/dnp:currentValue/dnp:notSupported">
										Not Supported
									</xsl:if>
									<xsl:if test="$path/dnp:linkStatusInterval/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:linkStatusInterval/dnp:currentValue/dnp:value"/>&#x20;seconds
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:linkStatusInterval"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:linkStatusInterval/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.2.6 Supports DNP3 Collision Avoidance -->
				<xsl:if test="$path/dnp:supportsCollisionAvoidance">
					<tr>
						<td width="35%" valign="top">1.2.6. Supports DNP3 Collision Avoidance:<p/>
							<i>Indicates whether an Outstation uses a collision avoidance algorithm. <p/>
							Collision avoidance may be implemented by a back-off timer with two parameters that define 
							the back-off time range or by some other vendor-specific mechanism. <p/>
							The recommended back-off time is specified as being a fixed minimum delay plus a random delay,
							where the random delay has a maximum value specified. This defines a range of delay times 
							that are randomly distributed between the minimum value and the minimum plus the maximum of 
							the random value. <p/>
							If a back-off timer is implemented with only a fixed or only a random value, select the 
							Back-off time method and set the parameter that is not supported to “Fixed at 0 ms”.
							</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance/dnp:capabilities/dnp:no"/>
								<xsl:with-param name="prompt" select="'No'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance/dnp:capabilities/dnp:yes"/>
								<xsl:with-param name="prompt" select="'Yes, using Back-off time = (Min + Random) method'"/>
							</xsl:call-template>
							<xsl:if test="$path/dnp:supportsCollisionAvoidance/dnp:capabilities/dnp:yes">
								<table border="0" align="center" width="100%" cellspacing="0" cellpadding="0">
									<tr>
										<td width="10%" valign="top"></td>
										<td width="90%" valign="top" colspan="4"><br/>Minimum Back-off time: <br/><br/>
										</td>
									</tr>
									<tr>
										<td width="10%" valign="top"/>
										<td width="90%" valign="top">
											<xsl:call-template name="tickboxBlockGeneral">
												<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance/dnp:capabilities/dnp:yes/dnp:minimumBackoffTime"/>
												<xsl:with-param name="block" select="'-F-RSO-'"/>
												<xsl:with-param name="units" select="'ms'"/>
											</xsl:call-template>
										</td>
									</tr>
									<tr>
										<td width="10%" valign="top"></td>
										<td width="90%" valign="top" colspan="4">Maximum Random Back-off time component:<br/><br/>
										</td>
									</tr>
									<tr>
										<td width="10%" valign="top"/>
										<td width="90%" valign="top">
											<xsl:call-template name="tickboxBlockGeneral">
												<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance/dnp:capabilities/dnp:yes/dnp:maximumRandomBackoffTime"/>
												<xsl:with-param name="block" select="'-F-RSO-'"/>
												<xsl:with-param name="units" select="'ms'"/>
											</xsl:call-template>
										</td>
									</tr>
								</table>
							</xsl:if>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance/dnp:capabilities/dnp:other"/>
								<xsl:with-param name="prompt" select="'Other, explain:'"/>
							</xsl:call-template>
							
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:supportsCollisionAvoidance/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:supportsCollisionAvoidance/dnp:currentValue/dnp:no">No</xsl:if>
									<xsl:if test="$path/dnp:supportsCollisionAvoidance/dnp:currentValue/dnp:yes">Yes<br/>
										Min back-off time
										<xsl:value-of select="$path/dnp:supportsCollisionAvoidance/dnp:currentValue/dnp:yes/dnp:minimumBackoffTime"/>&#x20;ms
										<br/>
										Max random time
										<xsl:value-of select="$path/dnp:supportsCollisionAvoidance/dnp:currentValue/dnp:yes/dnp:maximumRandomBackoffTime"/>&#x20;ms
									</xsl:if>
									<xsl:if test="$path/dnp:supportsCollisionAvoidance/dnp:currentValue/dnp:other">Other</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:supportsCollisionAvoidance/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.2.7 Receiver Inter-character Timeout -->
				<xsl:if test="$path/dnp:interCharacterTimeout">
					<tr>
						<td width="35%" valign="top">1.2.7. Receiver Inter-character Timeout:<p/>
							<i>When serial interfaces with asynchronous character framing are used, this parameter indicates if the receiver makes a check for gaps
						between characters. (i.e. extensions of the stop bit time of one character prior to the start bit of the following character within a message).
						If the receiver performs this check and the timeout is exceeded then the receiver discards the current data link frame.
						A receiver that does not discard data link frames on the basis of inter-character gaps is considered not to perform this check.
						<p/>Where no asynchronous serial interface is fitted this parameter is not applicable. 
						In this case none of the options shall be selected.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:notChecked"/>
								<xsl:with-param name="prompt" select="'Not Checked'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:noGapPermitted"/>
								<xsl:with-param name="prompt" select="'No gap permitted'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxFixed">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:fixedBitTimes"/>
								<xsl:with-param name="prompt" select="'Fixed at'"/>
								<xsl:with-param name="units" select="'bit times'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxFixed">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:fixedMilliseconds"/>
								<xsl:with-param name="prompt" select="'Fixed at'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxRange">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:rangeBitTimes"/>
								<xsl:with-param name="units" select="'bit times'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxRange">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:rangeMilliseconds"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxSelectable">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:selectableBitTimes"/>
								<xsl:with-param name="units" select="'bit times'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxSelectable">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:selectableMilliseconds"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:configurable"/>
								<xsl:with-param name="prompt" select="'Configurable, other, describe:'"/>
							</xsl:call-template>
							<br/>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:capabilities/dnp:variable"/>
								<xsl:with-param name="prompt" select="'Variable, explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:interCharacterTimeout/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:notChecked">Not Checked</xsl:if>
									<xsl:if test="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:noGapPermitted">No gap permitted</xsl:if>
									<xsl:if test="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:valueMilliseconds">
										<xsl:value-of select="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:valueMilliseconds"/>&#x20;ms</xsl:if>
									<xsl:if test="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:valueBitTimes">
										<xsl:value-of select="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:valueBitTimes"/>&#x20;bit times</xsl:if>
									<xsl:if test="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:variable">Variable</xsl:if>
									<xsl:if test="$path/dnp:interCharacterTimeout/dnp:currentValue/dnp:configurable">Configurable</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:interCharacterTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.2.8 Inter-character gaps in transmission -->
				<xsl:if test="$path/dnp:interCharacterGap">
					<tr>
						<td width="35%" valign="top">1.2.8. Inter-character gaps in transmission:<p/>
							<i>When serial interfaces with asynchronous character framing are used, this parameter indicates whether extra delay is ever
						introduced between characters in the message, and if so, the maximum width of the gap.
						<p/>Where no asynchronous serial interface is fitted this parameter is not applicable. 
						In this case none of the options shall be selected. </i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:interCharacterGap/dnp:capabilities/dnp:none"/>
								<xsl:with-param name="prompt" select="'None (always transmits with no inter-character gap)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxValue">
								<xsl:with-param name="path" select="$path/dnp:interCharacterGap/dnp:capabilities/dnp:maximumBitTimes"/>
								<xsl:with-param name="prompt" select="'Maximum'"/>
								<xsl:with-param name="units" select="'bit times'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxValue">
								<xsl:with-param name="path" select="$path/dnp:interCharacterGap/dnp:capabilities/dnp:maximumMilliseconds"/>
								<xsl:with-param name="prompt" select="'Maximum'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:interCharacterGap"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:interCharacterGap/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:interCharacterGap/dnp:currentValue/dnp:none">None</xsl:if>
									<xsl:if test="$path/dnp:interCharacterGap/dnp:currentValue/dnp:maximumBitTimes">
										<xsl:value-of select="$path/dnp:interCharacterGap/dnp:currentValue/dnp:maximumBitTimes"/>&#x20;bit times
								</xsl:if>
									<xsl:if test="$path/dnp:interCharacterGap/dnp:currentValue/dnp:maximumMilliseconds">
										<xsl:value-of select="$path/dnp:interCharacterGap/dnp:currentValue/dnp:maximumMilliseconds"/>&#x20;ms
								</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:interCharacterGap"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:interCharacterGap/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.2.9 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-2">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.3  IP Networking Parameters -->
	<xsl:template name="networkConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.3. IP Networking'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.3.1 Port Name -->
				<xsl:if test="$path/dnp:portName">
					<tr>
						<td width="35%" valign="top">1.3.1. Port Name:<p/>
							<i>Name used to reference the communications port defined in this section.</i>
						</td>
						<xsl:call-template name="greyBox35"/>
						<td width="20%" valign="top">
							<xsl:value-of select="$path/dnp:portName/dnp:currentValue/dnp:value"/>
							<xsl:call-template name="showNotesCV">
								<xsl:with-param name="path" select="$path/dnp:portName"/>
							</xsl:call-template>
						</td>
						<td width="10%" valign="top" bgcolor="white">
							<font color="white">&#160;</font>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.2 Type of End Point -->
				<xsl:if test="$path/dnp:typeOfEndPoint">
					<tr>
						<td width="35%" valign="top">1.3.2. Type of End Point:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:typeOfEndPoint/dnp:capabilities/dnp:tcpInitiating"/>
								<xsl:with-param name="prompt" select="'TCP Initiating (Master Only)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:typeOfEndPoint/dnp:capabilities/dnp:tcpListening"/>
								<xsl:with-param name="prompt" select="'TCP Listening (Outstation Only)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:typeOfEndPoint/dnp:capabilities/dnp:tcpDual"/>
								<xsl:with-param name="prompt" select="'TCP Dual (required for Masters)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:typeOfEndPoint/dnp:capabilities/dnp:udpDatagram"/>
								<xsl:with-param name="prompt" select="'UDP Datagram (required)'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:typeOfEndPoint"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:typeOfEndPoint/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:typeOfEndPoint/dnp:currentValue/dnp:tcpInitiating">TCP Initiating<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:typeOfEndPoint/dnp:currentValue/dnp:tcpListening">TCP Listening<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:typeOfEndPoint/dnp:currentValue/dnp:tcpDual">TCP Dual<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:typeOfEndPoint/dnp:currentValue/dnp:udpDatagram">UDP Datagram</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:typeOfEndPoint"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:typeOfEndPoint/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.3 IP Address of this Device -->
				<xsl:if test="$path/dnp:ipAddress">
					<tr>
						<td width="35%" valign="top">1.3.3. IP Address of this Device:<p/>
							<i/>
						</td>
						<xsl:call-template name="greyBox35"/>
						<xsl:choose>
							<xsl:when test="$path/dnp:ipAddress/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:ipAddress/dnp:currentValue/dnp:address"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:ipAddress"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:ipAddress/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.4 Subnet Mask -->
				<xsl:if test="$path/dnp:subnetMask">
					<tr>
						<td width="35%" valign="top">1.3.4. Subnet Mask:<p/>
							<i/>
						</td>
						<xsl:call-template name="greyBox35"/>
						<xsl:choose>
							<xsl:when test="$path/dnp:subnetMask/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:subnetMask/dnp:currentValue/dnp:mask"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:subnetMask"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:subnetMask/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.5 Gateway IP Address -->
				<xsl:if test="$path/dnp:gatewayIPAddress">
					<tr>
						<td width="35%" valign="top">1.3.5. Gateway IP Address:<p/>
							<i/>
						</td>
						<xsl:call-template name="greyBox35"/>
						<xsl:choose>
							<xsl:when test="$path/dnp:gatewayIPAddress/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:gatewayIPAddress/dnp:currentValue/dnp:address"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:gatewayIPAddress"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:gatewayIPAddress/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.6 TCP Connection Establishment -->
				<xsl:if test="$path/dnp:tcpConnectionEstablishment">
					<tr>
						<td width="35%" valign="top">1.3.6. Accepts TCP Connections or UDP Datagrams from:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment/dnp:capabilities/dnp:allowsAll"/>
								<xsl:with-param name="prompt" select="'Allows all (show as *.*.*.* in 1.3.7)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment/dnp:capabilities/dnp:basedOnIPAddress"/>
								<xsl:with-param name="prompt" select="'Limits based on IP address'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment/dnp:capabilities/dnp:basedOnListOfIPAddresses"/>
								<xsl:with-param name="prompt" select="'Limits based on list of IP addresses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment/dnp:capabilities/dnp:basedOnWildcardIPAddress"/>
								<xsl:with-param name="prompt" select="'Limits based on a wildcard IP address'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment/dnp:capabilities/dnp:basedOnListOfWildcardIPAddresses"/>
								<xsl:with-param name="prompt" select="'Limits based on list of wildcard IP addresses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment/dnp:capabilities/dnp:other"/>
								<xsl:with-param name="prompt" select="'Other validation, explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:tcpConnectionEstablishment/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:tcpConnectionEstablishment/dnp:currentValue/dnp:allowsAll">Allows all<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:tcpConnectionEstablishment/dnp:currentValue/dnp:basedOnIPAddress">IP address<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:tcpConnectionEstablishment/dnp:currentValue/dnp:basedOnListOfIPAddresses">List of IP addresses<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:tcpConnectionEstablishment/dnp:currentValue/dnp:basedOnWildcardIPAddress">Wildcard IP address<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:tcpConnectionEstablishment/dnp:currentValue/dnp:basedOnListOfWildcardIPAddresses">List of wildcard IP addresses<br/>
									</xsl:if>
									<xsl:for-each select="$path/dnp:tcpConnectionEstablishment/dnp:currentValue/dnp:other">
										Other<b><xsl:value-of select="./dnp:explanation"/></b><br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:tcpConnectionEstablishment/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.7 IP Address of Remote Device -->
				<xsl:if test="$path/dnp:ipAddressOfRemoteDevice">
					<tr>
						<td width="35%" valign="top">1.3.7. IP Address(es) from which TCP Connections or UDP Datagrams are accepted:
					</td>
						<xsl:call-template name="greyBox35"/>
						<xsl:choose>
							<xsl:when test="$path/dnp:ipAddressOfRemoteDevice/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:for-each select="$path/dnp:ipAddressOfRemoteDevice/dnp:currentValue/dnp:address">
										<xsl:value-of select="."/>
										<br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:ipAddressOfRemoteDevice"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:ipAddressOfRemoteDevice/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.8 TCP Listen Port Number -->
				<xsl:if test="$path/dnp:tcpListenPort">
					<tr>
						<td width="35%" valign="top">1.3.8. TCP Listen Port Number:<p/>
							<i>If Outstation or dual end point Master, port number on which to listen for incoming TCP connect requests.
						Required to be configureable for Masters and recommended to be configurable for Outstations.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:tcpListenPort/dnp:capabilities/dnp:notApplicable"/>
								<xsl:with-param name="prompt" select="'Not Applicable (Master w/o dual end point)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:tcpListenPort"/>
								<xsl:with-param name="block" select="'--2RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:tcpListenPort"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:tcpListenPort/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:tcpListenPort/dnp:currentValue/dnp:notApplicable">Not Applicable</xsl:if>
									<xsl:if test="$path/dnp:tcpListenPort/dnp:currentValue/dnp:fixedAt20000">Fixed at 20,000</xsl:if>
									<xsl:value-of select="$path/dnp:tcpListenPort/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:tcpListenPort"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:tcpListenPort/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.9 TCP Listen Port Number of remote device -->
				<xsl:if test="$path/dnp:tcpPortOfRemoteDevice">
					<tr>
						<td width="35%" valign="top">1.3.9. TCP Listen Port Number of remote device:<p/>
							<i>If Master or dual end point Outstation, port number on remote device with which to initiate connection.
						Required to be configurable for Masters and recommended to be configurable for Outstations.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:tcpPortOfRemoteDevice/dnp:capabilities/dnp:notApplicable"/>
								<xsl:with-param name="prompt" select="'Not Applicable (Outstation w/o dual end point)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:tcpPortOfRemoteDevice"/>
								<xsl:with-param name="block" select="'--2RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:tcpPortOfRemoteDevice"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:tcpPortOfRemoteDevice/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:tcpPortOfRemoteDevice/dnp:currentValue/dnp:notApplicable">Not Applicable</xsl:if>
									<xsl:if test="$path/dnp:tcpPortOfRemoteDevice/dnp:currentValue/dnp:fixedAt20000">Fixed at 20,000</xsl:if>
									<xsl:value-of select="$path/dnp:tcpPortOfRemoteDevice/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:tcpPortOfRemoteDevice"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:tcpPortOfRemoteDevice/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.10 TCP Keep-alive timer -->
				<xsl:if test="$path/dnp:tcpKeepAliveTimer">
					<tr>
						<td width="35%" valign="top">1.3.10. TCP Keep-alive timer:<p/>
							<i>The time period for the keep-alive timer on active TCP connections.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:tcpKeepAliveTimer"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:tcpKeepAliveTimer"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:tcpKeepAliveTimer/dnp:currentValue/dnp:value">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:tcpKeepAliveTimer/dnp:currentValue/dnp:value"/>&#x20;ms
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:tcpKeepAliveTimer"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:tcpKeepAliveTimer/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.11 Local UDP port -->
				<xsl:if test="$path/dnp:localUDPPort">
					<tr>
						<td width="35%" valign="top">1.3.11. Local UDP port:<p/>
							<i>Local UDP port for sending and/or receiving UDP datagrams. Masters may let system choose an available port.
						Outstations must use one that is known by the Master.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:localUDPPort"/>
								<xsl:with-param name="block" select="'--2RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:localUDPPort/dnp:capabilities/dnp:letSystemChoose"/>
								<xsl:with-param name="prompt" select="'Let system choose (Master only)'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:localUDPPort"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:localUDPPort/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:localUDPPort/dnp:currentValue/dnp:fixedAt20000">Fixed at 20,000</xsl:if>
									<xsl:value-of select="$path/dnp:localUDPPort/dnp:currentValue/dnp:value"/>
									<xsl:if test="$path/dnp:localUDPPort/dnp:currentValue/dnp:letSystemChoose">Let system choose</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:localUDPPort"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:localUDPPort/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.12 Destination UDP port for DNP3 Requests (Master Only) -->
				<xsl:if test="$path/dnp:destinationUDPPort">
					<tr>
						<td width="35%" valign="top">1.3.12. Destination UDP port for DNP3 Requests (Master Only):<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:destinationUDPPort"/>
								<xsl:with-param name="block" select="'--2RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:destinationUDPPort"/>
							</xsl:call-template>
							
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:destinationUDPPort/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:destinationUDPPort/dnp:currentValue/dnp:fixedAt20000">Fixed at 20,000</xsl:if>
									<xsl:value-of select="$path/dnp:destinationUDPPort/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:destinationUDPPort"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:destinationUDPPort/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.13 Destination UDP port -->
				<xsl:if test="$path/dnp:udpPortForUnsolicitedNullResponses">
					<tr>
						<td width="35%" valign="top">1.3.13. Destination UDP port for initial unsolicited null responses (UDP only Outstations):<p/>
							<i>For a UDP only Outstation, the destination UDP port for sending initial unsolicited Null response.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:udpPortForUnsolicitedNullResponses"/>
								<xsl:with-param name="block" select="'N-2RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:udpPortForUnsolicitedNullResponses"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:udpPortForUnsolicitedNullResponses/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:udpPortForUnsolicitedNullResponses/dnp:currentValue/dnp:none">None</xsl:if>
									<xsl:if test="$path/dnp:udpPortForUnsolicitedNullResponses/dnp:currentValue/dnp:fixedAt20000">Fixed at 20,000</xsl:if>
									<xsl:value-of select="$path/dnp:udpPortForUnsolicitedNullResponses/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:udpPortForUnsolicitedNullResponses"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:udpPortForUnsolicitedNullResponses/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.14 Destination UDP port for responses -->
				<xsl:if test="$path/dnp:udpPortForResponses">
					<tr>
						<td width="35%" valign="top">1.3.14. Destination UDP port for responses:<p/>
							<i>For a UDP only Outstation, the destination UDP port for sending all responses other than the initial unsolicited Null response.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:udpPortForResponses"/>
								<xsl:with-param name="block" select="'N-2RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:udpPortForResponses/dnp:capabilities/dnp:useSourcePortNumber"/>
								<xsl:with-param name="prompt" select="'Use source port number'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:udpPortForResponses"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:udpPortForResponses/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:udpPortForResponses/dnp:currentValue/dnp:none">None</xsl:if>
									<xsl:if test="$path/dnp:udpPortForResponses/dnp:currentValue/dnp:fixedAt20000">Fixed at 20,000</xsl:if>
									<xsl:value-of select="$path/dnp:udpPortForResponses/dnp:currentValue/dnp:value"/>
									<xsl:if test="$path/dnp:udpPortForResponses/dnp:currentValue/dnp:useSourcePortNumber">Source port number</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:udpPortForResponses"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:udpPortForResponses/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.15 Multiple outstation connections (Masters only) -->
				<xsl:if test="$path/dnp:multipleOutstationConnections">
					<tr>
						<td width="35%" valign="top">1.3.15. Multiple outstation connections (Masters only):<p/>
							<i>Master only. Indicates whether multiple outstation connections are supported.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:choose>
								<xsl:when test="$path/dnp:multipleOutstationConnections/dnp:capabilities/dnp:supportsMultipleOutstations = 'true'">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Supports multiple outstations (Masters only)
						<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:multipleOutstationConnections"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:multipleOutstationConnections/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:multipleOutstationConnections/dnp:currentValue/dnp:supportsMultipleOutstations = 'true'">True</xsl:if>
									<xsl:if test="$path/dnp:multipleOutstationConnections/dnp:currentValue/dnp:supportsMultipleOutstations = 'false'">False</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:multipleOutstationConnections"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:multipleOutstationConnections/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.16 Multiple master connections (Outstations only) -->
				<xsl:if test="$path/dnp:multipleMasterConnections">
					<tr>
						<td width="35%" valign="top">1.3.16. Multiple master connections (Outstations only):<p/>
							<i>Outstations only. Indicates whether multiple master connections are supported and the method that can be used to establish connections.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:choose>
								<xsl:when test="($path/dnp:multipleMasterConnections/dnp:capabilities/dnp:notSupported)
								or not(($path/dnp:multipleMasterConnections/dnp:capabilities/dnp:basedOnIPAddress) 
								or($path/dnp:multipleMasterConnections/dnp:capabilities/dnp:basedOnIPPortNumber)
								or($path/dnp:multipleMasterConnections/dnp:capabilities/dnp:browsingForStaticData))">
									<input type="checkbox" name="box"/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box" checked=""/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Supports multiple masters (Outstations only)<p/>
						If supported, the following methods may be used:<br/>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:multipleMasterConnections/dnp:capabilities/dnp:basedOnIPAddress"/>
								<xsl:with-param name="prompt" select="'Method 1 (based on IP address) - required'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:multipleMasterConnections/dnp:capabilities/dnp:basedOnIPPortNumber"/>
								<xsl:with-param name="prompt" select="'Method 2 (based on IP port number) - recommended'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:multipleMasterConnections/dnp:capabilities/dnp:browsingForStaticData"/>
								<xsl:with-param name="prompt" select="'Method 3 (browsing for static data) - optional'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:multipleMasterConnections"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:multipleMasterConnections/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:multipleMasterConnections/dnp:currentValue/dnp:notSupported">Not supported</xsl:if>
									<xsl:if test="$path/dnp:multipleMasterConnections/dnp:currentValue/dnp:basedOnIPAddress">IP address<br/></xsl:if>
									<xsl:if test="$path/dnp:multipleMasterConnections/dnp:currentValue/dnp:basedOnIPPortNumber">IP port number<br/></xsl:if>
									<xsl:if test="$path/dnp:multipleMasterConnections/dnp:currentValue/dnp:browsingForStaticData">Browsing for static data</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:multipleMasterConnections"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:multipleMasterConnections/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.3.17 Time synchronization support -->
				<xsl:if test="$path/dnp:timeSynchronization">
					<tr>
						<td width="35%" valign="top">1.3.17. Time synchronization support:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:timeSynchronization/dnp:capabilities/dnp:dnpLANProcedure"/>
								<xsl:with-param name="prompt" select="'DNP3 LAN procedure (function code 24)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:timeSynchronization/dnp:capabilities/dnp:dnpWriteTimeProcedure"/>
								<xsl:with-param name="prompt" select="'DNP3 Write Time (not recommended over LAN)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:timeSynchronization/dnp:capabilities/dnp:other"/>
								<xsl:with-param name="prompt" select="'Other, explain:'"/>
							</xsl:call-template>
							<br/>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:timeSynchronization/dnp:capabilities/dnp:notSupported"/>
								<xsl:with-param name="prompt" select="'Not Supported'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:timeSynchronization"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:timeSynchronization/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:timeSynchronization/dnp:currentValue/dnp:notSupported">Not supported</xsl:if>
									<xsl:for-each select="$path/dnp:timeSynchronization/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:if test="$path/dnp:timeSynchronization/dnp:currentValue/dnp:dnpLANProcedure">LAN procedure<br/></xsl:if>
									<xsl:if test="$path/dnp:timeSynchronization/dnp:currentValue/dnp:dnpWriteTimeProcedure">Write Time</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:timeSynchronization"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:timeSynchronization/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.3.18 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-3">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.4  Link Layer Parameters -->
	<xsl:template name="linkConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.4. Link Layer'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.4.1 Data Link Address -->
				<xsl:if test="$path/dnp:dataLinkAddress">
					<tr>
						<td width="35%" valign="top">1.4.1. Data Link Address:<p/>
							<i>Indicates if the link address is configurable over the entire valid range of 0 to 65,519.
						Data link addresses 0xFFF0 through 0xFFFF are reserved for broadcast or other special purposes.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:dataLinkAddress"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:dataLinkAddress"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:dataLinkAddress/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:dataLinkAddress/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:dataLinkAddress"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:dataLinkAddress/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.2 DNP3 Source Address Validation -->
				<xsl:if test="$path/dnp:sourceAddressValidation">
					<tr>
						<td width="35%" valign="top">1.4.2. DNP3 Source Address Validation:<p/>
							<i>Indicates whether the Outstation will filter out requests not from a specific source address.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:sourceAddressValidation/dnp:capabilities/dnp:never"/>
								<xsl:with-param name="prompt" select="'Never'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:sourceAddressValidation/dnp:capabilities/dnp:alwaysSingleAddress"/>
								<xsl:with-param name="prompt" select="'Always, one address allowed (shown in 1.4.3)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:sourceAddressValidation/dnp:capabilities/dnp:alwaysMultipleAddresses"/>
								<xsl:with-param name="prompt" select="'Always, any one of multiple addresses allowed'"/>
							</xsl:call-template>(each selectable as shown in 1.4.3)<br/>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:sourceAddressValidation/dnp:capabilities/dnp:sometimes"/>
								<xsl:with-param name="prompt" select="'Sometimes, explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:sourceAddressValidation"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:sourceAddressValidation/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:sourceAddressValidation/dnp:currentValue/dnp:never">Never</xsl:if>
									<xsl:if test="$path/dnp:sourceAddressValidation/dnp:currentValue/dnp:alwaysSingleAddress">Always - single address</xsl:if>
									<xsl:if test="$path/dnp:sourceAddressValidation/dnp:currentValue/dnp:alwaysMultipleAddresses">Always - multiple addresses</xsl:if>
									<xsl:if test="$path/dnp:sourceAddressValidation/dnp:currentValue/dnp:sometimes">Sometimes</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:sourceAddressValidation"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:sourceAddressValidation/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.3 DNP3 Source Address expected when Validation is Enabled -->
				<xsl:if test="$path/dnp:expectedSourceAddress">
					<tr>
						<td width="35%" valign="top">1.4.3. DNP3 Source Address(es) expected when Validation is Enabled:<p/>
							<i>Selects the allowed source address(es)</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:expectedSourceAddress/dnp:capabilities/dnp:anyDataLinkAddress"/>
								<xsl:with-param name="prompt" select="'Configurable to any 16 bit DNP Data Link Address value'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:expectedSourceAddress"/>
								<xsl:with-param name="block" select="'---RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:expectedSourceAddress"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:expectedSourceAddress/dnp:currentValue/dnp:value">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:expectedSourceAddress/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:expectedSourceAddress"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:when test="$path/dnp:expectedSourceAddress/dnp:currentValue/dnp:anyDataLinkAddress">
								<td width="20%" valign="baseline">
									Any Data Link Address
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:expectedSourceAddress"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:expectedSourceAddress/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.4 Self Address Support using address 0xFFFC -->
				<xsl:if test="$path/dnp:selfAddressSupport">
					<tr>
						<td width="35%" valign="top">1.4.4. Self Address Support using address 0xFFFC:<p/>
							<i>If an Outstation receives a message with a destination address of 0xFFFC it shall respond normally with its own source address.
						It must be possible to diasble this feature if supported.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:choose>
								<xsl:when test="$path/dnp:selfAddressSupport/dnp:capabilities/dnp:yes">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Yes (only allowed if configurable)<br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:selfAddressSupport/dnp:capabilities/dnp:no">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;No
						<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:selfAddressSupport"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:selfAddressSupport/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:selfAddressSupport/dnp:currentValue/dnp:yes">Yes</xsl:if>
									<xsl:if test="$path/dnp:selfAddressSupport/dnp:currentValue/dnp:no">No</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:selfAddressSupport"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:selfAddressSupport/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.5 Sends Confirmed User Data Frames -->
				<xsl:if test="$path/dnp:sendsConfirmedUserDataFrames">
					<tr>
						<td width="35%" valign="top">1.4.5. Sends Confirmed User Data Frames:<p/>
							<i>A list of conditions under which the device transmits confirmed link layer services 
						(TEST_LINK_STATES, RESET_LINK_STATES, CONFIRMED_USER_DATA).</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:sendsConfirmedUserDataFrames/dnp:capabilities/dnp:never"/>
								<xsl:with-param name="prompt" select="'Never'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:sendsConfirmedUserDataFrames/dnp:capabilities/dnp:always"/>
								<xsl:with-param name="prompt" select="'Always'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:sendsConfirmedUserDataFrames/dnp:capabilities/dnp:sometimes"/>
								<xsl:with-param name="prompt" select="'Sometimes, explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:sendsConfirmedUserDataFrames"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:sendsConfirmedUserDataFrames/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:sendsConfirmedUserDataFrames/dnp:currentValue/dnp:never">Never</xsl:if>
									<xsl:if test="$path/dnp:sendsConfirmedUserDataFrames/dnp:currentValue/dnp:always">Always</xsl:if>
									<xsl:if test="$path/dnp:sendsConfirmedUserDataFrames/dnp:currentValue/dnp:sometimes">Sometimes</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:sendsConfirmedUserDataFrames"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:sendsConfirmedUserDataFrames/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.6 Data Link Layer Confirmation Timeout -->
				<xsl:if test="$path/dnp:linkLayerConfirmTimeout">
					<tr>
						<td width="35%" valign="top">1.4.6. Data Link Layer Confirmation Timeout:<p/>
							<i>This timeout applies to any secondary data link message that requires a confirm or response (link reset, link status, user data, etc).</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:linkLayerConfirmTimeout"/>
								<xsl:with-param name="block" select="'NF-RSOV'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:linkLayerConfirmTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:linkLayerConfirmTimeout/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:linkLayerConfirmTimeout"/>
										<xsl:with-param name="units" select="'ms'"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:linkLayerConfirmTimeout"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:linkLayerConfirmTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.7 Maximum Data Link Retries -->
				<xsl:if test="$path/dnp:maxDataLinkRetries">
					<tr>
						<td width="35%" valign="top">1.4.7. Maximum Data Link Retries:<p/>
							<i>The number of times the device will retransmit a frame that requests Link Layer confirmation.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxDataLinkRetries"/>
								<xsl:with-param name="block" select="'NF-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxDataLinkRetries"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxDataLinkRetries/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:maxDataLinkRetries/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:maxDataLinkRetries/dnp:currentValue/dnp:value"/>
									</xsl:if>
									<xsl:if test="$path/dnp:maxDataLinkRetries/dnp:currentValue/dnp:none">None</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxDataLinkRetries"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxDataLinkRetries/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.8 Maximum number of octets Transmitted in a Data Link Frame -->
				<xsl:if test="$path/dnp:maxTransmittedFrameSize">
					<tr>
						<td width="35%" valign="top">1.4.8. Maximum number of octets Transmitted in a Data Link Frame:<p/>
							<i>This number includes the CRCs. With a length field of 255, the maximum size would be 292.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxTransmittedFrameSize"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxTransmittedFrameSize"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxTransmittedFrameSize/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxTransmittedFrameSize/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxTransmittedFrameSize"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxTransmittedFrameSize/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.4.9 Maximum number of octets that can be Received in a Data Link Frame -->
				<xsl:if test="$path/dnp:maxReceivedFrameSize">
					<tr>
						<td width="35%" valign="top">1.4.9. Maximum number of octets that can be Received in a Data Link Frame:<p/>
							<i>This number includes the CRCs. With a field length of 255, the maximum size would be 292.
						The device must be able to receive 292 octets to be compliant.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxReceivedFrameSize"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxReceivedFrameSize"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxReceivedFrameSize/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxReceivedFrameSize/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxReceivedFrameSize"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxReceivedFrameSize/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.4.10 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-4">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.5  Application Layer Parameters -->
	<xsl:template name="applConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.5. Application Layer'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.5.1 Maximum number of octets Transmitted in an Application Layer Fragment other than File Transfer -->
				<xsl:if test="$path/dnp:maxTransmittedFragmentSize">
					<tr>
						<td width="35%" valign="top">1.5.1. Maximum number of octets Transmitted in an Application Layer Fragment other than File Transfer:<p/>
							<i>This size does not include any transport or frame octets.<br/>
						- Masters must provide a setting less than or equal to 249.<br/>
						- Outstations must provide a setting less than or equal to 2048.
						<p/>Note: The current value of this outstation parameter is available remotely using protocol object Group 0 Variation 240.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxTransmittedFragmentSize"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxTransmittedFragmentSize"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxTransmittedFragmentSize/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxTransmittedFragmentSize/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxTransmittedFragmentSize"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxTransmittedFragmentSize/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.5.2 Maximum number of octets Transmitted in an Application Layer Fragment containing File Transfer -->
				<xsl:if test="$path/dnp:maxFileTransferTransmittedFragmentSize">
					<tr>
						<td width="35%" valign="top">1.5.2. Maximum number of octets Transmitted in an Application Layer Fragment containing File Transfer:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxFileTransferTransmittedFragmentSize"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxFileTransferTransmittedFragmentSize"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxFileTransferTransmittedFragmentSize/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxFileTransferTransmittedFragmentSize/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxFileTransferTransmittedFragmentSize"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxFileTransferTransmittedFragmentSize/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.5.3 Maximum number of octets that can be received in an Application Layer Fragment -->
				<xsl:if test="$path/dnp:maxReceivedFragmentSize">
					<tr>
						<td width="35%" valign="top">1.5.3. Maximum number of octets that can be received in an Application Layer Fragment:<p/>
							<i>This size does not include any transport or frame octets.<br/>
						- Masters must provide a setting greater than or equal to 2048.<br/>
						- Outstations must provide a setting greater than or equal to 249.
						<p/>Note: The current value of this outstation parameter is available remotely using protocol object Group 0 Variation 241.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxReceivedFragmentSize"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxReceivedFragmentSize"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxReceivedFragmentSize/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxReceivedFragmentSize/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxReceivedFragmentSize"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxReceivedFragmentSize/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.5.4 Timeout waiting for Complete Application Layer Fragment -->
				<xsl:if test="$path/dnp:fragmentTimeout">
					<tr>
						<td width="35%" valign="top">1.5.4. Timeout waiting for Complete Application Layer Fragment:<p/>
							<i>Timeout if all frames of a message fragment are not received in the specified time.
						Measured from time first frame of a fragment is received until the last frame is received.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:fragmentTimeout"/>
								<xsl:with-param name="block" select="'NF-RSOV'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:fragmentTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:fragmentTimeout/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:fragmentTimeout"/>
										<xsl:with-param name="units" select="'ms'"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:fragmentTimeout"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:fragmentTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.5.5 Maximum number of objects allowed in a single control request for CROB (Group 12) -->
				<xsl:if test="$path/dnp:maxObjectsInCROBControlRequest">
					<tr>
						<td width="35%" valign="top">1.5.5. Maximum number of objects allowed in a single control request for CROB (Group 12):<p/>
							<i>Note: The current value of this outstation parameter is available remotely using protocol object Group 0 Variation 216.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxFixed">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInCROBControlRequest/dnp:capabilities/dnp:fixed"/>
								<xsl:with-param name="prompt" select="'Fixed at'"/>
								<xsl:with-param name="units" select="'(enter 0 if controls are not supported)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInCROBControlRequest"/>
								<xsl:with-param name="block" select="'---RSOV'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInCROBControlRequest"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxObjectsInCROBControlRequest/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxObjectsInCROBControlRequest/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxObjectsInCROBControlRequest"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInCROBControlRequest/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.5.6 Maximum number of objects allowed in a single control request for Analog Outputs (Group 41) -->
				<xsl:if test="$path/dnp:maxObjectsInAnalogOutputControlRequest">
					<tr>
						<td width="35%" valign="top">1.5.6. Maximum number of objects allowed in a single control request for Analog Outputs (Group 41):<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxFixed">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInAnalogOutputControlRequest/dnp:capabilities/dnp:fixed"/>
								<xsl:with-param name="prompt" select="'Fixed at'"/>
								<xsl:with-param name="units" select="'(enter 0 if controls are not supported)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInAnalogOutputControlRequest"/>
								<xsl:with-param name="block" select="'---RSOV'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInAnalogOutputControlRequest"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxObjectsInAnalogOutputControlRequest/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxObjectsInAnalogOutputControlRequest/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxObjectsInAnalogOutputControlRequest"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInAnalogOutputControlRequest/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.5.7 Maximum number of objects allowed in a single control request for Data Sets (Groups 85, 86, 87) -->
				<xsl:if test="$path/dnp:maxObjectsInDataSetsControlRequest">
					<tr>
						<td width="35%" valign="top">1.5.7. Maximum number of objects allowed in a single control request for Data Sets (Groups 85, 86, 87):<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxFixed">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInDataSetsControlRequest/dnp:capabilities/dnp:fixed"/>
								<xsl:with-param name="prompt" select="'Fixed at'"/>
								<xsl:with-param name="units" select="'(enter 0 if controls are not supported)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInDataSetsControlRequest"/>
								<xsl:with-param name="block" select="'---RSOV'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInDataSetsControlRequest"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxObjectsInDataSetsControlRequest/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:maxObjectsInDataSetsControlRequest/dnp:currentValue/dnp:value"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxObjectsInDataSetsControlRequest"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxObjectsInDataSetsControlRequest/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.5.8 Supports mixed object groups (AOBs, CROBs and Data Sets) in the same control request -->
				<xsl:if test="$path/dnp:supportsMixedObjectGroupsInControlRequest">
					<tr>
						<td width="35%" valign="top">1.5.8. Supports mixed object groups (AOBs, CROBs and Data Sets) in the same control request:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:capabilities/dnp:notApplicable"/>
								<xsl:with-param name="prompt" select="'Not applicable - controls are not supported'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:capabilities/dnp:yes"/>
								<xsl:with-param name="prompt" select="'Yes'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:capabilities/dnp:no"/>
								<xsl:with-param name="prompt" select="'No'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:supportsMixedObjectGroupsInControlRequest"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:currentValue/dnp:notApplicable">N/A</xsl:if>
									<xsl:if test="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:currentValue/dnp:yes">Yes</xsl:if>
									<xsl:if test="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:currentValue/dnp:no">No</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:supportsMixedObjectGroupsInControlRequest"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:supportsMixedObjectGroupsInControlRequest/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.5.9 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-5">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.6  Masters Configuration -->
	<xsl:template name="masterConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.6. Fill Out The Following Items For Masters Only'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.6.1 Timeout waiting for Complete Application Layer Responses (ms) -->
				<xsl:if test="$path/dnp:responseTimeout">
					<tr>
						<td width="35%" valign="top">1.6.1. Timeout waiting for Complete Application Layer Responses (ms):<p/>
							<i>Timeout on Master if all fragments of a response message are not received in the specified time.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:responseTimeout"/>
								<xsl:with-param name="block" select="'NF-RSOV'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:responseTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:responseTimeout/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:responseTimeout"/>
										<xsl:with-param name="units" select="'ms'"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:responseTimeout"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:responseTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.2 Maximum Application Layer Retries for Request Messages -->
				<xsl:if test="$path/dnp:applicationLayerRetries">
					<tr>
						<td width="35%" valign="top">1.6.2. Maximum Application Layer Retries for Request Messages:<p/>
							<i>The number of times a Master will retransmit an application layer request message if a response is not received.
						This parameter must never cause a Master to retransmit time sync messages.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:applicationLayerRetries"/>
								<xsl:with-param name="block" select="'NF-RSOV'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:applicationLayerRetries"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:applicationLayerRetries/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:applicationLayerRetries"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:applicationLayerRetries"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:applicationLayerRetries/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.3 Incremental Timeout waiting for First or Next Fragment of an Application Layer Response -->
				<xsl:if test="$path/dnp:responseIncrementalTimeout">
					<tr>
						<td width="35%" valign="top">1.6.3. Incremental Timeout waiting for First or Next Fragment of an Application Layer Response:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:responseIncrementalTimeout"/>
								<xsl:with-param name="block" select="'NF-RSOV'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:responseIncrementalTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:responseIncrementalTimeout/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:responseIncrementalTimeout"/>
										<xsl:with-param name="units" select="'ms'"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:responseIncrementalTimeout"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:responseIncrementalTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.4 Issuing controls to off-line devices -->
				<xsl:if test="$path/dnp:issuesControlsToOfflineDevices">
					<tr>
						<td width="35%" valign="top">1.6.4 Issuing controls to off-line devices:<p/>
							<i>Indicates if the Master issues control requests to devices that are thought to be off-line 
							     (i.e. the Master has not seen responses to previous Master requests). </i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOfflineDevices/dnp:capabilities/dnp:notApplicable"/>
								<xsl:with-param name="prompt" select="'Not applicable - controls are not supported'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOfflineDevices/dnp:capabilities/dnp:yes"/>
								<xsl:with-param name="prompt" select="'Yes'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOfflineDevices/dnp:capabilities/dnp:no"/>
								<xsl:with-param name="prompt" select="'No'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOfflineDevices"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:issuesControlsToOfflineDevices/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:issuesControlsToOfflineDevices/dnp:currentValue/dnp:notApplicable">N/A</xsl:if>
									<xsl:if test="$path/dnp:issuesControlsToOfflineDevices/dnp:currentValue/dnp:yes">Yes</xsl:if>
									<xsl:if test="$path/dnp:issuesControlsToOfflineDevices/dnp:currentValue/dnp:no">No</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:issuesControlsToOfflineDevices"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOfflineDevices/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.5 Issuing controls to off-scan devices -->
				<xsl:if test="$path/dnp:issuesControlsToOffscanDevices">
					<tr>
						<td width="35%" valign="top">1.6.5 Issuing controls to off-scan devices:<p/>
							<i>Indicates if the Master issues control requests to devices that are currently off-scan 
							    (i.e. the Master has been configured not to issue poll requests to the device).</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOffscanDevices/dnp:capabilities/dnp:notApplicable"/>
								<xsl:with-param name="prompt" select="'Not applicable - controls are not supported'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOffscanDevices/dnp:capabilities/dnp:yes"/>
								<xsl:with-param name="prompt" select="'Yes'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOffscanDevices/dnp:capabilities/dnp:no"/>
								<xsl:with-param name="prompt" select="'No'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOffscanDevices"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:issuesControlsToOffscanDevices/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:issuesControlsToOffscanDevices/dnp:currentValue/dnp:notApplicable">N/A</xsl:if>
									<xsl:if test="$path/dnp:issuesControlsToOffscanDevices/dnp:currentValue/dnp:yes">Yes</xsl:if>
									<xsl:if test="$path/dnp:issuesControlsToOffscanDevices/dnp:currentValue/dnp:no">No</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:issuesControlsToOffscanDevices"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:issuesControlsToOffscanDevices/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.6 Maximum Application Layer Retries for Control Select Messages (same sequence number): -->
				<xsl:if test="$path/dnp:maxControlRetriesSameSN">
					<tr>
						<td width="35%" valign="top">1.6.6 Maximum Application Layer Retries for Control Select Messages (same sequence number):<p/>
							<i>Indicates the number of times a Master will retransmit an application layer control select request message if a response 
							is not received - using the same message sequence number. </i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesSameSN/dnp:capabilities/dnp:none"/>
								<xsl:with-param name="prompt" select="'None (required)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesSameSN"/>
								<xsl:with-param name="block" select="'-F-RSOV'"/>
								<xsl:with-param name="units" select="' '"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesSameSN"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxControlRetriesSameSN/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:maxControlRetriesSameSN"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxControlRetriesSameSN"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesSameSN/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.7 Maximum Application Layer Retries for Control Select Messages (new sequence number): -->
				<xsl:if test="$path/dnp:maxControlRetriesNewSN">
					<tr>
						<td width="35%" valign="top">1.6.7 Maximum Application Layer Retries for Control Select Messages (new sequence number):<p/>
							<i>Indicates the number of times a Master will retransmit an application layer control select request message if a response 
							is not received - using a new message sequence number. </i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesNewSN/dnp:capabilities/dnp:none"/>
								<xsl:with-param name="prompt" select="'None (required)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesNewSN"/>
								<xsl:with-param name="block" select="'-F-RSOV'"/>
								<xsl:with-param name="units" select="' '"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesNewSN"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxControlRetriesNewSN/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:maxControlRetriesNewSN"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxControlRetriesNewSN"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxControlRetriesNewSN/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.8 Maximum error in the time that the Master issues freeze requests: -->
				<xsl:if test="$path/dnp:maxTimeErrorScheduledFreezes">
					<tr>
						<td width="35%" valign="top">1.6.8 Maximum error in the time that the Master issues freeze requests:<p/>
							<i>If the Master is scheduled to issue freeze requests at a specific time, what is the maximum error
							 in the time that the Master may actually issue a request? </i>
					</td>
						<xsl:call-template name="greyBox35"/>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxTimeErrorScheduledFreezes/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:for-each select="$path/dnp:maxTimeErrorScheduledFreezes/dnp:currentValue/dnp:time">
										<xsl:value-of select="."/>&#x20;ms
										<br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxTimeErrorScheduledFreezes"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxTimeErrorScheduledFreezes/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.9 Maximum error in the time that the Master schedules repetitive freeze requests: -->
				<xsl:if test="$path/dnp:maxTimeErrorRepetitiveFreezes">
					<tr>
						<td width="35%" valign="top">1.6.9 Maximum error in the time that the Master schedules repetitive freeze requests:<p/>
							<i>If the Master is scheduled to issue freeze requests at a regular interval, what is the maximum error in the time
							 interval that the Master may actually issue a request? (i.e. how early / late could the request actually be issued)? </i>
					</td>
						<xsl:call-template name="greyBox35"/>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxTimeErrorRepetitiveFreezes/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:for-each select="$path/dnp:maxTimeErrorRepetitiveFreezes/dnp:currentValue/dnp:time">
										<xsl:value-of select="."/>&#x20;ms
										<br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxTimeErrorRepetitiveFreezes"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxTimeErrorRepetitiveFreezes/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.10 Scheduled actions that may affect the accuracy of freeze requests: -->
				<xsl:if test="$path/dnp:actionsAffectingFreezeRequests">
					<tr>
						<td width="35%" valign="top">1.6.10 Scheduled actions that may affect the accuracy of freeze requests:<p/>
							<i>Indicates if the Master's accuracy of issuing freeze requests may be affected by other 
							scheduled operations such as poll requests or control requests. </i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:actionsAffectingFreezeRequests/dnp:capabilities/dnp:pollRequests"/>
								<xsl:with-param name="prompt" select="'Freeze time may be affected by Poll requests'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:actionsAffectingFreezeRequests/dnp:capabilities/dnp:controlRequests"/>
								<xsl:with-param name="prompt" select="'Freeze time may be affected by Control requests'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:actionsAffectingFreezeRequests"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:actionsAffectingFreezeRequests/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:actionsAffectingFreezeRequests/dnp:currentValue/dnp:pollRequests">Poll Requests<br/></xsl:if>
									<xsl:if test="$path/dnp:actionsAffectingFreezeRequests/dnp:currentValue/dnp:controlRequests">Control Requests<br/></xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:actionsAffectingFreezeRequests"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:actionsAffectingFreezeRequests/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.6.11	Master’s algorithm for scheduling request operations: -->
				<xsl:if test="$path/dnp:schedulingAlgorithm">
					<tr>
						<td width="35%" valign="top">1.6.11	Master's algorithm for scheduling request operations:<p/>
							<i>Describe the Master's algorithm for determination of which activity is performed when more
							 than one is due at the same moment. Discuss precedence and priorities for activities such as
							  time synchronization, poll requests, control requests and freeze requests.</i>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:schedulingAlgorithm/dnp:capabilities">
								<td width="35%" valign="top">
									<xsl:for-each select="$path/dnp:schedulingAlgorithm/dnp:capabilities/dnp:schedulingAlgorithm">
										<xsl:value-of select="."/>
										<br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCap">
										<xsl:with-param name="path" select="$path/dnp:schedulingAlgorithm"/>
									</xsl:call-template>&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox35"/>
							</xsl:otherwise>
						</xsl:choose>
						<xsl:choose>
							<xsl:when test="$path/dnp:schedulingAlgorithm/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:for-each select="$path/dnp:schedulingAlgorithm/dnp:currentValue/dnp:schedulingAlgorithm">
										<xsl:value-of select="."/>
										<br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:schedulingAlgorithm"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:schedulingAlgorithm/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.6.12 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-6">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.7  Outstations Configuration -->
	<xsl:template name="outstationConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.7. Fill Out The Following Items For Outstations Only'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.7.1 Timeout waiting for Application Confirm of solicited response message -->
				<xsl:if test="$path/dnp:applicationLayerConfirmTimeout">
					<tr>
						<td width="35%" valign="top">1.7.1. Timeout waiting for Application Confirm of solicited response message:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:applicationLayerConfirmTimeout"/>
								<xsl:with-param name="block" select="'NF-RSOV'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:applicationLayerConfirmTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:applicationLayerConfirmTimeout/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:call-template name="currentValueNVOV">
										<xsl:with-param name="path" select="$path/dnp:applicationLayerConfirmTimeout"/>
										<xsl:with-param name="units" select="'ms'"/>
									</xsl:call-template>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:applicationLayerConfirmTimeout"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:applicationLayerConfirmTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.2 How often is time synchronization required from the master -->
				<xsl:if test="$path/dnp:timeSyncRequired">
					<tr>
						<td width="35%" valign="top">1.7.2. How often is time synchronization required from the master:<p/>
							<i>Details of when the master needs to perform a time synchronization to ensure that the outstation clock does not drift outside 
							of an acceptable tolerance. If the option to relate this to IIN1.4 is used then details of when IIN1.4 is asserted are in section 1.10.2.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:never"/>
								<xsl:with-param name="prompt" select="'Never needs time'"/>
							</xsl:call-template>
							<xsl:choose>
								<xsl:when test="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:withinSecondsOfIIN14">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Within&#x20;<b>
								<xsl:value-of select="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:withinSecondsOfIIN14"/>
							</b>
							&#x20;seconds after IIN1.4 is set<br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:periodicallyFixed">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Periodically, fixed at&#x20;<b>
								<xsl:value-of select="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:periodicallyFixed"/>
							</b>
							&#x20;seconds<br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:periodicallyRange">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Periodically, between&#x20;<b>
								<xsl:value-of select="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:periodicallyRange/dnp:minimum"/>
							</b>&#x20;and&#x20;<b>
								<xsl:value-of select="$path/dnp:timeSyncRequired/dnp:capabilities/dnp:periodicallyRange/dnp:maximum"/>
							</b>
							&#x20;seconds<br/>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:timeSyncRequired"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:timeSyncRequired/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:timeSyncRequired/dnp:currentValue/dnp:never">Never</xsl:if>
									<xsl:if test="$path/dnp:timeSyncRequired/dnp:currentValue/dnp:withinSecondsOfIIN14">Within &#x20;
									<xsl:value-of select="$path/dnp:timeSyncRequired/dnp:currentValue/dnp:withinSecondsOfIIN14"/> seconds of IIN1.4</xsl:if>
									<xsl:if test="$path/dnp:timeSyncRequired/dnp:currentValue/dnp:periodically">
										Periodically, every <xsl:value-of select="$path/dnp:timeSyncRequired/dnp:currentValue/dnp:periodically"/> seconds.
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:timeSyncRequired"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:timeSyncRequired/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.3 Device Trouble Bit IIN1.6 -->
				<xsl:if test="$path/dnp:deviceTroubleBit">
					<tr>
						<td width="35%" valign="top">1.7.3. Device Trouble Bit IIN1.6:<p/>
							<i>If IIN1.6 device trouble bit is set under certain conditions, explain the possible causes.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:deviceTroubleBit/dnp:capabilities/dnp:neverUsed"/>
								<xsl:with-param name="prompt" select="'Never used'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:deviceTroubleBit/dnp:capabilities/dnp:reasonForSetting"/>
								<xsl:with-param name="prompt" select="'Reason for setting:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:deviceTroubleBit"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:deviceTroubleBit/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:deviceTroubleBit/dnp:currentValue/dnp:neverUsed">Never used</xsl:if>
									<xsl:if test="$path/dnp:deviceTroubleBit/dnp:currentValue/dnp:reasonForSetting">Used as described</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:deviceTroubleBit"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:deviceTroubleBit/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.4 File Handle Timeout -->
				<xsl:if test="$path/dnp:fileHandleTimeout">
					<tr>
						<td width="35%" valign="top">1.7.4. File Handle Timeout:<p/>
							<i>If there is no activity referencing a file handle for a configurable length of time, the outstation must do an automatic close on the file.
						The timeout value must be configurable up to 1 hour.
						When this condition occurs the outstation will send a File Transport Status Object 
						(obj grp 70 var 6) using a status code value of handle expired (0x02).</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:fileHandleTimeout/dnp:capabilities/dnp:notApplicable"/>
								<xsl:with-param name="prompt" select="'Not applicable, files not supported'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:fileHandleTimeout"/>
								<xsl:with-param name="block" select="'-F-RSOV'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:fileHandleTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:fileHandleTimeout/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:fileHandleTimeout/dnp:currentValue/dnp:notApplicable">Not applicable</xsl:if>
									<xsl:if test="$path/dnp:fileHandleTimeout/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:fileHandleTimeout/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:if test="$path/dnp:fileHandleTimeout/dnp:currentValue/dnp:variable">Variable</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:fileHandleTimeout"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:fileHandleTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.5 Event Buffer Overflow Behavior -->
				<xsl:if test="$path/dnp:eventBufferOverflowBehavior">
					<tr>
						<td width="35%" valign="top">1.7.5. Event Buffer Overflow Behavior:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:eventBufferOverflowBehavior/dnp:capabilities/dnp:discardOldest"/>
								<xsl:with-param name="prompt" select="'Discard the oldest event'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:eventBufferOverflowBehavior/dnp:capabilities/dnp:discardNewest"/>
								<xsl:with-param name="prompt" select="'Discard the newest event'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:eventBufferOverflowBehavior/dnp:capabilities/dnp:other"/>
								<xsl:with-param name="prompt" select="'Other, explain:'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:eventBufferOverflowBehavior"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:eventBufferOverflowBehavior/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:eventBufferOverflowBehavior/dnp:currentValue/dnp:discardOldest">Discard oldest</xsl:if>
									<xsl:if test="$path/dnp:eventBufferOverflowBehavior/dnp:currentValue/dnp:discardNewest">Discard newest</xsl:if>
									<xsl:for-each select="$path/dnp:eventBufferOverflowBehavior/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:eventBufferOverflowBehavior"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:eventBufferOverflowBehavior/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.6 Event Buffer Organization -->
				<xsl:if test="$path/dnp:eventBufferOrganization">
					<tr>
						<td width="35%" valign="top">1.7.6. Event Buffer Organization:<p/>
							<i>Explain how event buffers are arranged (per Object Group, per Class, single buffer etc) and provide their sizes.</i>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:eventBufferOrganization/dnp:capabilities">
								<td width="35%" valign="top">
									<xsl:value-of select="$path/dnp:eventBufferOrganization/dnp:capabilities"/>
									<xsl:call-template name="showNotesCap">
										<xsl:with-param name="path" select="$path/dnp:eventBufferOrganization"/>
									</xsl:call-template>
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox35"/>
							</xsl:otherwise>
						</xsl:choose>
						<xsl:choose>
							<xsl:when test="$path/dnp:eventBufferOrganization/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:eventBufferOrganization/dnp:currentValue"/>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:eventBufferOrganization"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:eventBufferOrganization/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.7 Sends Multi-Fragment Responses -->
				<xsl:if test="$path/dnp:sendsMultiFragmentResponses">
					<tr>
						<td width="35%" valign="top">1.7.7. Sends Multi-Fragment Responses:<p/>
							<i>Indicates whether an Outstation sends multi-fragment responses (Masters do not send multi-fragment requests).</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:sendsMultiFragmentResponses/dnp:capabilities/dnp:yes"/>
								<xsl:with-param name="prompt" select="'Yes'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:sendsMultiFragmentResponses/dnp:capabilities/dnp:no"/>
								<xsl:with-param name="prompt" select="'No'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:sendsMultiFragmentResponses"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:sendsMultiFragmentResponses/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:sendsMultiFragmentResponses/dnp:currentValue/dnp:yes">Yes</xsl:if>
									<xsl:if test="$path/dnp:sendsMultiFragmentResponses/dnp:currentValue/dnp:no">No</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:sendsMultiFragmentResponses"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:sendsMultiFragmentResponses/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.8 Last Fragment Confirmation -->
				<xsl:if test="$path/dnp:requestsLastFragmentConfirmation">
					<tr>
						<td width="35%" valign="top">1.7.8. Last Fragment Confirmation:<p/>
							<i>Indicates whether the Outstation requests confirmation of the last fragment of a multi-fragment response.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:requestsLastFragmentConfirmation/dnp:capabilities/dnp:always"/>
								<xsl:with-param name="prompt" select="'Always'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:requestsLastFragmentConfirmation/dnp:capabilities/dnp:sometimes"/>
								<xsl:with-param name="prompt" select="'Sometimes, explain:'"/>
							</xsl:call-template><br/>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:requestsLastFragmentConfirmation/dnp:capabilities/dnp:never"/>
								<xsl:with-param name="prompt" select="'Never'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:requestsLastFragmentConfirmation"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:requestsLastFragmentConfirmation/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:requestsLastFragmentConfirmation/dnp:currentValue/dnp:always">Always</xsl:if>
									<xsl:if test="$path/dnp:requestsLastFragmentConfirmation/dnp:currentValue/dnp:sometimes">Sometimes</xsl:if>
									<xsl:if test="$path/dnp:requestsLastFragmentConfirmation/dnp:currentValue/dnp:never">Never</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:requestsLastFragmentConfirmation"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:requestsLastFragmentConfirmation/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.7.9 DNP Command Settings preserved through a device reset -->
				<xsl:if test="$path/dnp:settingsPreservedThroughDeviceReset">
					<tr>
						<td width="35%" valign="top">1.7.9. DNP Command Settings preserved through a device reset:<p/>
							<i>If any of these settings are written through the DNP protocol and they are not preserved through a restart of the Outstation,
						the Master will have to write them again anytime the Restart IIN bit is set.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset/dnp:capabilities/dnp:assignClass"/>
								<xsl:with-param name="prompt" select="'Assign Class'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset/dnp:capabilities/dnp:analogDeadbands"/>
								<xsl:with-param name="prompt" select="'Analog Deadbands'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset/dnp:capabilities/dnp:dataSetPrototypes"/>
								<xsl:with-param name="prompt" select="'Data Set Prototypes'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset/dnp:capabilities/dnp:dataSetDescriptors"/>
								<xsl:with-param name="prompt" select="'Data Set Descriptors'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset/dnp:capabilities/dnp:FC31ActivateConfiguration"/>
								<xsl:with-param name="prompt" select="'Function Code 31 Activate Configuration'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:settingsPreservedThroughDeviceReset/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:settingsPreservedThroughDeviceReset/dnp:currentValue/dnp:assignClass">Assign Class<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:settingsPreservedThroughDeviceReset/dnp:currentValue/dnp:analogDeadbands">Analog Deadbands<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:settingsPreservedThroughDeviceReset/dnp:currentValue/dnp:dataSetPrototypes">Data Set Prototypes<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:settingsPreservedThroughDeviceReset/dnp:currentValue/dnp:dataSetDescriptors">Data Set Descriptores<br/>
									</xsl:if>
									<xsl:if test="$path/dnp:settingsPreservedThroughDeviceReset/dnp:currentValue/dnp:FC31ActivateConfiguration">FC31<br/>
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:settingsPreservedThroughDeviceReset/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.7.10 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-7">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.8  Outstations Unsolicited Response Support -->
	<xsl:template name="unsolicitedConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.8. Outstation Unsolicited Response Support'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.8.1 Supports Unsolicited Reporting -->
				<xsl:if test="$path/dnp:supportsUnsolicitedReporting">
					<tr>
						<td width="35%" valign="top">1.8.1. Supports Unsolicited Reporting:<p/>
							<i>When the unsolicited response mode is configured "off", the device is to behave exactly like an equivalent device that has no
						support for unsolicited responses. If set to "on", the Outstation will send a null Unsolicited Response after it restarts,
						then wait for an Enable Unsolicited Response command from the master before sending additional Unsolicited Responses containing event data.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:supportsUnsolicitedReporting/dnp:capabilities/dnp:notSupported"/>
								<xsl:with-param name="prompt" select="'Not Supported'"/>
							</xsl:call-template>
							<xsl:choose>
								<xsl:when test="$path/dnp:supportsUnsolicitedReporting/dnp:capabilities/dnp:configurable">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Configurable, selectable from On and Off<br/>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:supportsUnsolicitedReporting"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:supportsUnsolicitedReporting/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:supportsUnsolicitedReporting/dnp:currentValue/dnp:on">On</xsl:if>
									<xsl:if test="$path/dnp:supportsUnsolicitedReporting/dnp:currentValue/dnp:off">Off</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:supportsUnsolicitedReporting"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:supportsUnsolicitedReporting/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.8.2 Master Data Link Address -->
				<xsl:if test="$path/dnp:masterDataLinkAddress">
					<tr>
						<td width="35%" valign="top">1.8.2. Master Data Link Address:<p/>
							<i>The destination address of the master device where the unsolicited responses will be sent.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:masterDataLinkAddress"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:masterDataLinkAddress"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:masterDataLinkAddress/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:masterDataLinkAddress/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:masterDataLinkAddress/dnp:currentValue/dnp:value"/>
									</xsl:if>
									<xsl:for-each select="$path/dnp:masterDataLinkAddress/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:masterDataLinkAddress"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:masterDataLinkAddress/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.8.3 Unsolicited Response Confirmation Timeout -->
				<xsl:if test="$path/dnp:unsolicitedResponseConfirmationTimeout">
					<tr>
						<td width="35%" valign="top">1.8.3. Unsolicited Response Confirmation Timeout:<p/>
							<i>This is the amount of time that the outstation will wait for an Application Layer confirmation back from the master indicating that
						the master received the unsolicited response message. As a minimum, the range of configurable values must include times from one second
						to one minute. This parameter may be the same one that is used for normal, solicited, application confirmation timeouts, 
						or it may be a separate parameter.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:unsolicitedResponseConfirmationTimeout"/>
								<xsl:with-param name="block" select="'-F-RSOV'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:unsolicitedResponseConfirmationTimeout"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:unsolicitedResponseConfirmationTimeout/dnp:currentValue/dnp:value">
								<td width="20%" valign="baseline">
									<xsl:value-of select="$path/dnp:unsolicitedResponseConfirmationTimeout/dnp:currentValue/dnp:value"/>&#x20;ms
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:unsolicitedResponseConfirmationTimeout"/>
									</xsl:call-template>
									&#160;
							</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:unsolicitedResponseConfirmationTimeout/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.8.4 Number of Unsolicited Retries -->
				<xsl:if test="$path/dnp:maxUnsolicitedRetries">
					<tr>
						<td width="35%" valign="top">1.8.4. Number of Unsolicited Retries:<p/>
							<i>This is the number of retries that an outstation transmits in each unsolicited response series if it does not receive confirmation
						back from the master. The configured value includes identical and regenerated retry messages. One of the choices must provide
						for an indefinite (and potentially infinite) number of transmissions.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:maxUnsolicitedRetries"/>
								<xsl:with-param name="block" select="'NF-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:maxUnsolicitedRetries/dnp:capabilities/dnp:infinite"/>
								<xsl:with-param name="prompt" select="'Always infinite, never gives up'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxUnsolicitedRetries"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxUnsolicitedRetries/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:maxUnsolicitedRetries/dnp:currentValue/dnp:none">None</xsl:if>
									<xsl:if test="$path/dnp:maxUnsolicitedRetries/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:maxUnsolicitedRetries/dnp:currentValue/dnp:value"/>
									</xsl:if>
									<xsl:if test="$path/dnp:maxUnsolicitedRetries/dnp:currentValue/dnp:infinite">Infinite</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxUnsolicitedRetries"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxUnsolicitedRetries/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.8.5 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-8">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.9  Outstations Unsolicited Response Trigger Conditions -->
	<xsl:template name="unsolicitedResponseTriggerConditions">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.9. Outstation Unsolicited Response Trigger Conditions'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.9.1 Number of class 1 events -->
				<xsl:if test="$path/dnp:numberOfClassOneEvents">
					<tr>
						<td width="35%" valign="top">1.9.1. Number of class 1 events:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassOneEvents/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Class 1 not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassOneEvents"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassOneEvents"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:numberOfClassOneEvents/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:numberOfClassOneEvents/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:numberOfClassOneEvents/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:numberOfClassOneEvents/dnp:currentValue/dnp:value"/>
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:numberOfClassOneEvents"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassOneEvents/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.2 Number of class 2 events -->
				<xsl:if test="$path/dnp:numberOfClassTwoEvents">
					<tr>
						<td width="35%" valign="top">1.9.2. Number of class 2 events:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassTwoEvents/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Class 2 not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassTwoEvents"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassTwoEvents"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:numberOfClassTwoEvents/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:numberOfClassTwoEvents/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:numberOfClassTwoEvents/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:numberOfClassTwoEvents/dnp:currentValue/dnp:value"/>
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:numberOfClassTwoEvents"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassTwoEvents/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.3 Number of class 3 events -->
				<xsl:if test="$path/dnp:numberOfClassThreeEvents">
					<tr>
						<td width="35%" valign="top">1.9.3. Number of class 3 events:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassThreeEvents/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Class 3 not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassThreeEvents"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassThreeEvents"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:numberOfClassThreeEvents/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:numberOfClassThreeEvents/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:numberOfClassThreeEvents/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:numberOfClassThreeEvents/dnp:currentValue/dnp:value"/>
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:numberOfClassThreeEvents"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:numberOfClassThreeEvents/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.4 Total number of events from any class -->
				<xsl:if test="$path/dnp:totalNumberOfClassEvents">
					<tr>
						<td width="35%" valign="top">1.9.4. Total number of events from any class:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:totalNumberOfClassEvents/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Total Number of Events not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:totalNumberOfClassEvents"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:totalNumberOfClassEvents"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:totalNumberOfClassEvents/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:totalNumberOfClassEvents/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:totalNumberOfClassEvents/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:totalNumberOfClassEvents/dnp:currentValue/dnp:value"/>
									</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:totalNumberOfClassEvents"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:totalNumberOfClassEvents/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.5 Hold time after class 1 event -->
				<xsl:if test="$path/dnp:holdTimeAfterClassOneEvent">
					<tr>
						<td width="35%" valign="top">1.9.5. Hold time after class 1 event:<p/>
							<i>
								<xsl:value-of select="$holdTimeText"/>
							</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassOneEvent/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Class 1 not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassOneEvent"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassOneEvent"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:holdTimeAfterClassOneEvent/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:holdTimeAfterClassOneEvent/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:holdTimeAfterClassOneEvent/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:holdTimeAfterClassOneEvent/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassOneEvent"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassOneEvent/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.6 Hold time after class 2 event -->
				<xsl:if test="$path/dnp:holdTimeAfterClassTwoEvent">
					<tr>
						<td width="35%" valign="top">1.9.6. Hold time after class 2 event:<p/>
							<i>
								<xsl:value-of select="$holdTimeText"/>
							</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassTwoEvent/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Class 2 not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassTwoEvent"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassTwoEvent"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:holdTimeAfterClassTwoEvent/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:holdTimeAfterClassTwoEvent/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:holdTimeAfterClassTwoEvent/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:holdTimeAfterClassTwoEvent/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassTwoEvent"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassTwoEvent/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.7 Hold time after class 3 event -->
				<xsl:if test="$path/dnp:holdTimeAfterClassThreeEvent">
					<tr>
						<td width="35%" valign="top">1.9.7. Hold time after class 3 event:<p/>
							<i>
								<xsl:value-of select="$holdTimeText"/>
							</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassThreeEvent/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Class 3 not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassThreeEvent"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassThreeEvent"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:holdTimeAfterClassThreeEvent/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:holdTimeAfterClassThreeEvent/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:holdTimeAfterClassThreeEvent/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:holdTimeAfterClassThreeEvent/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassThreeEvent"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterClassThreeEvent/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.8 Hold time after event assigned to any class -->
				<xsl:if test="$path/dnp:holdTimeAfterAnyEvent">
					<tr>
						<td width="35%" valign="top">1.9.8. Hold time after event assigned to any class:<p/>
							<i>
								<xsl:value-of select="$holdTimeText"/>
							</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterAnyEvent/dnp:capabilities/dnp:notUsedToTriggerEvents"/>
								<xsl:with-param name="prompt" select="'Class events not used to trigger Unsolicited Responses'"/>
							</xsl:call-template>
							<xsl:call-template name="tickboxBlock">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterAnyEvent"/>
								<xsl:with-param name="block" select="'-F-RSO-'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterAnyEvent"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:holdTimeAfterAnyEvent/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:holdTimeAfterAnyEvent/dnp:currentValue/dnp:classNotUsedToTriggerEvents">Not Used</xsl:if>
									<xsl:if test="$path/dnp:holdTimeAfterAnyEvent/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:holdTimeAfterAnyEvent/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:holdTimeAfterAnyEvent"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:holdTimeAfterAnyEvent/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.9 Retrigger Hold Time -->
				<xsl:if test="$path/dnp:retriggerHoldTimer">
					<tr>
						<td width="35%" valign="top">1.9.9. Retrigger Hold Time:<p/>
							<i>The hold-time timer may be retriggered for each new event detected (increased possibility of capturing all the changes in a single response)
						or not retriggered (giving the master a guaranteed update time).</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:retriggerHoldTimer/dnp:capabilities/dnp:retriggeredForEachNewEvent"/>
								<xsl:with-param name="prompt" select="'Hold-time timer will be retriggered for each new event detected (may get more changes in next response)'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:retriggerHoldTimer/dnp:capabilities/dnp:notRetriggeredForEachNewEvent"/>
								<xsl:with-param name="prompt" select="'Hold-time timer will not be retriggered for each new event detected (guaranteed update time)'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:retriggerHoldTimer"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:retriggerHoldTimer/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:retriggerHoldTimer/dnp:currentValue/dnp:retriggeredForEachNewEvent">Retriggered</xsl:if>
									<xsl:if test="$path/dnp:retriggerHoldTimer/dnp:currentValue/dnp:notRetriggeredForEachNewEvent">Not retriggered</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:retriggerHoldTimer"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:retriggerHoldTimer/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.9.10 Other Unsolicited Response Trigger Conditions -->
				<xsl:if test="$path/dnp:otherTriggerConditions">
					<tr>
						<td width="35%" valign="top">1.9.10. Other Unsolicited Response Trigger Conditions:<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox-explain">
								<xsl:with-param name="path" select="$path/dnp:otherTriggerConditions/dnp:capabilities/dnp:other"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:otherTriggerConditions"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:otherTriggerConditions/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:for-each select="$path/dnp:otherTriggerConditions/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/>
									</xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:otherTriggerConditions"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:otherTriggerConditions/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.9.11 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-9">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.10  Outstation Performance -->
	<xsl:template name="outstationPerformance">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4">
				<xsl:with-param name="title" select="'1.10. Outstation Performance'"/>
			</xsl:call-template>
			<tbody>
				<!-- Section 1.10.1 Maximum Time Base Drift (milliseconds per minute) -->
				<xsl:if test="$path/dnp:maxTimeBaseDrift">
					<tr>
						<td width="35%" valign="top">1.10.1. Maximum Time Base Drift (milliseconds per minute):<p/>
							<i>If the device is synchronized by DNP, what is the clock drift rate over the full operating temperature range.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlockNotConfigurable">
								<xsl:with-param name="path" select="$path/dnp:maxTimeBaseDrift"/>
								<xsl:with-param name="block" select="'FRSO'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:maxTimeBaseDrift"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:maxTimeBaseDrift/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:maxTimeBaseDrift/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:maxTimeBaseDrift/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:for-each select="$path/dnp:maxTimeBaseDrift/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:maxTimeBaseDrift"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:maxTimeBaseDrift/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.10.2 When does outstation set IIN1.4? -->
				<xsl:if test="$path/dnp:outstationSetsIIN14">
					<tr>
						<td width="35%" valign="top">1.10.2. When does outstation set IIN1.4?<p/>
							<i>When does the outstation set the internal indication NEED_TIME</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:never"/>
								<xsl:with-param name="prompt" select="'Never'"/>
							</xsl:call-template>
							<xsl:call-template name="tickbox">
								<xsl:with-param name="path" select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:atStartup"/>
								<xsl:with-param name="prompt" select="'Asserted at startup until first Time Synchronization request received'"/>
							</xsl:call-template>
							<xsl:choose>
								<xsl:when test="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:periodicallyRange">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Periodically, range 
							<b>
								<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:periodicallyRange/dnp:minimum"/>
							</b> to 
							<b>
								<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:periodicallyRange/dnp:maximum"/>
							</b> &#x20;seconds<br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:periodicallySelectable">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Periodically, selectable from 
							<xsl:for-each select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:periodicallySelectable">
								<b>
									<xsl:value-of select="."/>
								</b>
								<xsl:if test="not(position() = last())">, </xsl:if>
							</xsl:for-each>&#x20;seconds<br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:afterLastTimeSyncRange">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Range &#x20;
							<b>
								<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:afterLastTimeSyncRange/dnp:minimum"/>
							</b> to 
							<b>
								<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:afterLastTimeSyncRange/dnp:maximum"/>
							</b> &#x20;
							seconds after last time sync<br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:afterLastTimeSyncSelectable">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;Selectable from  
							<xsl:for-each select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:afterLastTimeSyncSelectable">
								<b>
									<xsl:value-of select="."/>
								</b>
								<xsl:if test="not(position() = last())">, </xsl:if>
							</xsl:for-each>&#x20;seconds after last time sync <br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:whenTimeErrorExceedsRange">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;When time error may have drifted by range 
							<b>
								<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:whenTimeErrorExceedsRange/dnp:minimum"/>
							</b> to 
							<b>
								<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:whenTimeErrorExceedsRange/dnp:maximum"/>
							</b> &#x20;ms<br/>
							<xsl:choose>
								<xsl:when test="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:whenTimeErrorExceedsSelectable">
									<input type="checkbox" name="box" checked=""/>
								</xsl:when>
								<xsl:otherwise>
									<input type="checkbox" name="box"/>
								</xsl:otherwise>
							</xsl:choose>
							<label for="box"/>&#x20;When time error may have drifted by selectable from 
							<xsl:for-each select="$path/dnp:outstationSetsIIN14/dnp:capabilities/dnp:whenTimeErrorExceedsSelectable">
								<b>
									<xsl:value-of select="."/>
								</b>
								<xsl:if test="not(position() = last())">, </xsl:if>
							</xsl:for-each>&#x20;ms
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:outstationSetsIIN14"/>
							</xsl:call-template>
					</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:outstationSetsIIN14/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:never">Never</xsl:if>
									<xsl:if test="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:atStartup">At startup<br/></xsl:if>
									<xsl:if test="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:periodically">Periodically every 
										<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:periodically"/>&#x20;seconds</xsl:if>
									<xsl:if test="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:afterLastTimeSync">
										<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:afterLastTimeSync"/>&#x20;seconds after last sync</xsl:if>
									<xsl:if test="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:whenTimeErrorExceeds">Time error exceeds 
									<xsl:value-of select="$path/dnp:outstationSetsIIN14/dnp:currentValue/dnp:whenTimeErrorExceeds"/>&#x20;ms</xsl:if>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:outstationSetsIIN14"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:outstationSetsIIN14/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.10.3 Maximum Internal Time Reference Error when set via DNP (ms) -->
				<xsl:if test="$path/dnp:referenceErrorViaDNP">
					<tr>
						<td width="35%" valign="top">1.10.3. Maximum Internal Time Reference Error when set via DNP (ms):<p/>
							<i>The difference between the time set in DNP Write Time message, and the time actually set in the outstation.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlockNotConfigurable">
								<xsl:with-param name="path" select="$path/dnp:referenceErrorViaDNP"/>
								<xsl:with-param name="block" select="'FRSO'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:referenceErrorViaDNP"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:referenceErrorViaDNP/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:referenceErrorViaDNP/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:referenceErrorViaDNP/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:for-each select="$path/dnp:referenceErrorViaDNP/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:referenceErrorViaDNP"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:referenceErrorViaDNP/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.10.4 Maximum Delay Measurement Error (ms) -->
				<xsl:if test="$path/dnp:delayMeasurementError">
					<tr>
						<td width="35%" valign="top">1.10.4. Maximum Delay Measurement Error (ms):<p/>
							<i>The difference between the time reported in the delay measurement response and the actual time between receipt of the 
						delay measurement request and issuing the delay measurement reply.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlockNotConfigurable">
								<xsl:with-param name="path" select="$path/dnp:delayMeasurementError"/>
								<xsl:with-param name="block" select="'FRSO'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:delayMeasurementError"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:delayMeasurementError/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:delayMeasurementError/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:delayMeasurementError/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:for-each select="$path/dnp:delayMeasurementError/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:delayMeasurementError"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:delayMeasurementError/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.10.5 Maximum Response Time (ms) -->
				<xsl:if test="$path/dnp:responseTime">
					<tr>
						<td width="35%" valign="top">1.10.5. Maximum Response Time (ms):<p/>
							<i>The amount of time an outstation will take to respond upon receipt of a valid request.
						This does not include the message transmission time.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlockNotConfigurable">
								<xsl:with-param name="path" select="$path/dnp:responseTime"/>
								<xsl:with-param name="block" select="'FRSO'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:responseTime"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:responseTime/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:responseTime/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:responseTime/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:for-each select="$path/dnp:responseTime/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:responseTime"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:responseTime/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.10.6 Maximum time from start-up to IIN 1.4 assertion (ms) -->
				<xsl:if test="$path/dnp:startupToIIN14">
					<tr>
						<td width="35%" valign="top">1.10.6. Maximum time from start-up to IIN 1.4 assertion (ms):<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlockNotConfigurable">
								<xsl:with-param name="path" select="$path/dnp:startupToIIN14"/>
								<xsl:with-param name="block" select="'FRSO'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:startupToIIN14"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:startupToIIN14/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:startupToIIN14/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:startupToIIN14/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:for-each select="$path/dnp:startupToIIN14/dnp:currentValue/dnp:other">
									Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:startupToIIN14"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:startupToIIN14/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section  1.10.7 Maximum Event Time-tag error for local Binary and Double Bit I/O (ms) -->
				<xsl:if test="$path/dnp:binaryOrDoubleBitEventError">
					<tr>
						<td width="35%" valign="top">1.10.7. Maximum Event Time-tag error for local Binary and Double Bit I/O (ms):<p/>
							<i>The error between the time-tag reported and the absolute time of the physical event.
							This error includes the Internal Time Reference Error.
							<br/>Note: The current value of this parameter is available remotely using protocol object Group 0 Variation 217.</i>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlockNotConfigurable">
								<xsl:with-param name="path" select="$path/dnp:binaryOrDoubleBitEventError"/>
								<xsl:with-param name="block" select="'FRSO'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:binaryOrDoubleBitEventError"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:binaryOrDoubleBitEventError/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:binaryOrDoubleBitEventError/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:binaryOrDoubleBitEventError/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:for-each select="$path/dnp:binaryOrDoubleBitEventError/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:binaryOrDoubleBitEventError"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:binaryOrDoubleBitEventError/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- Section 1.10.8 Maximum Event Time-tag error for local I/O other than Binary and Double Bit data types (ms) -->
				<xsl:if test="$path/dnp:nonBinaryOrDoubleBitEventError">
					<tr>
						<td width="35%" valign="top">1.10.8. Maximum Event Time-tag error for local I/O other than Binary and Double Bit data types (ms):<p/>
							<i/>
						</td>
						<td width="35%" valign="top">
							<xsl:call-template name="tickboxBlockNotConfigurable">
								<xsl:with-param name="path" select="$path/dnp:nonBinaryOrDoubleBitEventError"/>
								<xsl:with-param name="block" select="'FRSO'"/>
								<xsl:with-param name="units" select="'ms'"/>
							</xsl:call-template>
							<xsl:call-template name="showNotesCap">
								<xsl:with-param name="path" select="$path/dnp:nonBinaryOrDoubleBitEventError"/>
							</xsl:call-template>
						</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:nonBinaryOrDoubleBitEventError/dnp:currentValue">
								<td width="20%" valign="baseline">
									<xsl:if test="$path/dnp:nonBinaryOrDoubleBitEventError/dnp:currentValue/dnp:value">
										<xsl:value-of select="$path/dnp:nonBinaryOrDoubleBitEventError/dnp:currentValue/dnp:value"/>&#x20;ms</xsl:if>
									<xsl:for-each select="$path/dnp:nonBinaryOrDoubleBitEventError/dnp:currentValue/dnp:other">
										Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
									<xsl:call-template name="showNotesCV">
										<xsl:with-param name="path" select="$path/dnp:nonBinaryOrDoubleBitEventError"/>
									</xsl:call-template>
									&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox20"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="baseline">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:nonBinaryOrDoubleBitEventError/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.10.9 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-10">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>
	<!-- 1.11  Individual Field Outstation Parameters -->
	<xsl:template name="fieldConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox3">
				<xsl:with-param name="title" select="'1.11. Individual Field Outstation Parameters'"/>
			</xsl:call-template>
			<tbody>
				<!-- 1.11.1 User-assigned location name or code string (same as g0v245) -->
				<xsl:if test="$path/dnp:outstationLocation">
					<tr>
						<td width="45%" valign="top">1.11.1. User-assigned location name or code string (same as g0v245):</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:outstationLocation/dnp:currentValue">
								<td width="45%" valign="top">
									<xsl:value-of select="$path/dnp:outstationLocation/dnp:currentValue"/>&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox45"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:outstationLocation/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.11.2 User-assigned ID code/number string (same as g0v246) -->
				<xsl:if test="$path/dnp:outstationId">
					<tr>
						<td width="45%" valign="top">1.11.2. User-assigned ID code/number string (same as g0v246):</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:outstationId/dnp:currentValue">
								<td width="45%" valign="top">
									<xsl:value-of select="$path/dnp:outstationId/dnp:currentValue"/>&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox45"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:outstationId/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.11.3 User-assigned name string for the outstation (same as g0v247) -->
				<xsl:if test="$path/dnp:outstationName">
					<tr>
						<td width="45%" valign="top">1.11.3 User-assigned name string for the outstation (same as g0v247):</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:outstationName/dnp:currentValue">
								<td width="45%" valign="top">
									<xsl:value-of select="$path/dnp:outstationName/dnp:currentValue"/>&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox45"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:outstationName/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.11.4 Device Serial Number string (same as g0v248) -->
				<xsl:if test="$path/dnp:deviceSerialNumber">
					<tr>
						<td width="45%" valign="top">1.11.4 Device Serial Number string (same as g0v248):</td>
						<xsl:choose>
							<xsl:when test="$path/dnp:deviceSerialNumber/dnp:currentValue">
								<td width="45%" valign="top">
									<xsl:value-of select="$path/dnp:deviceSerialNumber/dnp:currentValue"/>&#160;
								</td>
							</xsl:when>
							<xsl:otherwise>
								<xsl:call-template name="whiteBox45"/>
							</xsl:otherwise>
						</xsl:choose>
						<td width="10%" valign="top">
							<xsl:call-template name="methodsBox">
								<xsl:with-param name="path" select="$path/dnp:deviceSerialNumber/dnp:methods"/>
							</xsl:call-template>
						</td>
					</tr>
				</xsl:if>
				<!-- 1.11.5 User Data -->
				<xsl:if test="$path/dnp:userData"><xsl:call-template name="userDataSection1-11">
					<xsl:with-param name="path" select="$path/dnp:userData"/></xsl:call-template>
				</xsl:if>
			</tbody>
		</table>
	</xsl:template>

	<!-- 1.12  Security Parameters -->
	<xsl:template name="securityConfig">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<xsl:call-template name="titleBox4"><xsl:with-param name="title" select="'1.12. Security Parameters'"/></xsl:call-template>
			<tbody>
			<!-- Section 1.12.1 DNP3 device support for secure authentication -->
				<xsl:if test="$path/dnp:secureAuthenticationSupported"><tr>
					<td width="35%" valign="top">1.12.1 DNP3 device support for secure authentication:<p/>
						<i>The support for secure authentication is optional in DNP3 devices. Indicate here if the device supports
						secure authentication.<p/>
						If the device does not support secure authentication then ignore the rest of this section.<p/>
						If the device does support secure authentication then specify the version(s) that are supported in the device. 
						The version number is an integer value defined in the protocol document "DNP3Spec-V2-Sup1-SecureAuthentication". 
						The volume 2 supplement shows version numbers of all associated documents that comprise that version of Secure Authentication.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:secureAuthenticationSupported/dnp:capabilities/dnp:notSupported"/>
							<xsl:with-param name="prompt" select="'Secure Authentication not supported'"/>
						</xsl:call-template>
						<p/>If Secure Authentication is supported, what Version(s) are supported:<br/>
						<xsl:choose>
							<xsl:when test="$path/dnp:secureAuthenticationSupported/dnp:capabilities/dnp:version/dnp:fixed">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Fixed at version 
						<xsl:value-of select="$path/dnp:secureAuthenticationSupported/dnp:capabilities/dnp:version/dnp:fixed"/><br/>  
						<xsl:choose>
							<xsl:when test="$path/dnp:secureAuthenticationSupported/dnp:capabilities/dnp:version/dnp:selectable">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Configurable, selectable from versions   
						<xsl:for-each select="$path/dnp:secureAuthenticationSupported/dnp:capabilities/dnp:version/dnp:selectable">
							<b>
								<xsl:value-of select="."/>
							</b>
							<xsl:if test="not(position() = last())">, </xsl:if>
						</xsl:for-each>&#x20;
						<br/>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:secureAuthenticationSupported"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:secureAuthenticationSupported/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:secureAuthenticationSupported/dnp:currentValue/dnp:notSupported">Not Supported</xsl:if>
								<xsl:if test="$path/dnp:secureAuthenticationSupported/dnp:currentValue/dnp:version">Supports security<br/>
								Version: <xsl:value-of select="$path/dnp:secureAuthenticationSupported/dnp:currentValue/dnp:version"/><br/></xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:secureAuthenticationSupported"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:secureAuthenticationSupported/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.2 Maximum number of users -->
				<xsl:if test="$path/dnp:maxNumberUsers"><tr>
					<td width="35%" valign="top">1.12.2 Maximum number of users:<p/>
						<i>The secure authentication algorithm provides support for multiple users. The device must support details for
						each user (update keys, session keys, etc). A user is identified by a 16-bit user number, allowing a maximum of
						65535 users. Devices are not mandated to support this number of potential users. Indicate here the actual limit to
						 the number of simultaneous users that can be supported.</i>
					</td>
					<td width="35%" valign="top">
						Maximum nunber of users supported: <xsl:value-of select="$path/dnp:maxNumberUsers/dnp:capabilities"/>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:maxNumberUsers"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:maxNumberUsers/dnp:currentValue">
							<td width="20%" valign="baseline">
								Maximum number of users supported: <xsl:value-of select="$path/dnp:maxNumberUsers/dnp:currentValue"/>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:maxNumberUsers"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:maxNumberUsers/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.3 Security message response timeout -->
				<xsl:if test="$path/dnp:securityResponseTimeout"><tr>
					<td width="35%" valign="top">1.12.3 Security message response timeout:<p/>
						<i>Authentication of critical messages may involve additional message exchanges (challenges and responses) 
						which can require an extension to the normal DNP3 message response timeout. 
						This timeout specifies an additional time to be used when the extra security transactions are involved. 
						The maximum allowable timeout extension should not exceed 120 seconds.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:securityResponseTimeout"/><xsl:with-param name="block" select="'-F-RSO-'"/>
							<xsl:with-param name="units" select="'ms'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:securityResponseTimeout"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:securityResponseTimeout/dnp:currentValue/dnp:value">
							<td width="20%" valign="baseline">
								<xsl:value-of select="$path/dnp:securityResponseTimeout/dnp:currentValue/dnp:value"/> ms
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:securityResponseTimeout"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:securityResponseTimeout/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.4 Aggressive mode of operation (receive) -->
				<xsl:if test="$path/dnp:acceptsAggressiveMode"><tr>
					<td width="35%" valign="top">1.12.4 Aggressive mode of operation (receive):<p/>
						<i>DNP3 devices may (optionally) accept "aggressive" mode requests, where challenge data used for 
						authentication is appended to a critical message rather than needing to be solicited via a separate message exchange. </i>
					</td>
					<xsl:call-template name="greyBox35"/>
					<td width="20%" valign="baseline">
						<xsl:choose>
							<xsl:when test="$path/dnp:acceptsAggressiveMode/dnp:currentValue/dnp:yes">
								<input type="checkbox" name="box" checked=""/>
								<label for="box"/>&#x20;Yes, accepts aggressive mode requests<br/>
							</xsl:when>
						</xsl:choose>
						<xsl:choose>
							<xsl:when test="$path/dnp:acceptsAggressiveMode/dnp:currentValue/dnp:no">
								<input type="checkbox" name="box" checked=""/>
								<label for="box"/>&#x20;No, does not accept aggressive mode requests<br/>
							</xsl:when>
						</xsl:choose>
						<xsl:call-template name="showNotesCV">
							<xsl:with-param name="path" select="$path/dnp:acceptsAggressiveMode"/>
						</xsl:call-template>
						&#160;
					</td>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:acceptsAggressiveMode/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.5 Aggressive mode of operation (issuing) -->
				<xsl:if test="$path/dnp:issuesAggressiveMode"><tr>
					<td width="35%" valign="top">1.12.5 Aggressive mode of operation (issuing):<p/>
						<i>DNP3 devices must support the issuing of "aggressive" mode of operation, where challenge data used for 
						authentication is appended to a critical message rather than needing to be solicited via a separate message exchange. 
						Specific instances of devices may have the use of aggressive mode switched off. </i>
					</td>
					<xsl:call-template name="greyBox35"/>
					<td width="20%" valign="baseline">
						<xsl:choose>
							<xsl:when test="$path/dnp:issuesAggressiveMode/dnp:currentValue/dnp:yes">
								<input type="checkbox" name="box" checked=""/>
								<label for="box"/>&#x20;Yes, issues aggressive mode requests<br/>
							</xsl:when>
						</xsl:choose>
						<xsl:choose>
							<xsl:when test="$path/dnp:issuesAggressiveMode/dnp:currentValue/dnp:no">
								<input type="checkbox" name="box" checked=""/>
								<label for="box"/>&#x20;No, does not issue aggressive mode requests<br/>
							</xsl:when>
						</xsl:choose>
						<xsl:call-template name="showNotesCV">
							<xsl:with-param name="path" select="$path/dnp:issuesAggressiveMode"/>
						</xsl:call-template>
						&#160;
					</td>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:issuesAggressiveMode/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.6 Session key change interval -->
				<xsl:if test="$path/dnp:sessionKeyChangeInterval"><tr>
					<td width="35%" valign="top">1.12.6 Session key change interval:<p/>
						<i>To counter an attack that compromises the session key, the session key is changed at regular intervals. 
						The maximum interval is 2 hours. Outstation devices invalidate the current set of session keys if they have not been changed 
						by the master station after a period of twice this configured value. <p/>
						To accommodate systems with infrequent communications, this change interval can be disabled and just the session 
						key change message count used (see 1.12.7) </i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeInterval/dnp:capabilities/dnp:canBeDisabled"/>
							<xsl:with-param name="prompt" select="'Can be disabled'"/>
						</xsl:call-template>
						<p/>When enabled<br/>
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeInterval"/><xsl:with-param name="block" select="'---R---'"/>
							<xsl:with-param name="units" select="'seconds'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeInterval"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:sessionKeyChangeInterval/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:sessionKeyChangeInterval/dnp:currentValue/dnp:isDisabled">Disabled<br/></xsl:if>
								<xsl:if test="$path/dnp:sessionKeyChangeInterval/dnp:currentValue/dnp:value">Enabled<br/>
								<xsl:value-of select="$path/dnp:sessionKeyChangeInterval/dnp:currentValue/dnp:value"/> seconds</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeInterval"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeInterval/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.7 Session key change message count -->
				<xsl:if test="$path/dnp:sessionKeyChangeMessageCount"><tr>
					<td width="35%" valign="top">1.12.7 Session key change message count:<p/>
						<i>In addition to changing the session key at regular intervals, the key shall also be changed after a specified number of messages
						 have been exchanged. The maximum allowable value for this message count is 10,000</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeMessageCount"/><xsl:with-param name="block" select="'---R---'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeMessageCount"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:sessionKeyChangeMessageCount/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:value-of select="$path/dnp:sessionKeyChangeMessageCount/dnp:currentValue/dnp:value"/>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeMessageCount"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:sessionKeyChangeMessageCount/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.8 Maximum error count -->
				<xsl:if test="$path/dnp:maxErrorCount"><tr>
					<td width="35%" valign="top">1.12.8 Maximum error count:<p/>
						<i>To assist in countering denial of service attacks, a DNP3 device shall stop replying with error codes after a number of 
						successive authentication failures. This error count has a maximum value of 10. Setting the error count to zero inhibits all 
						error messages.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:maxErrorCount"/><xsl:with-param name="block" select="'---R---'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:maxErrorCount"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:maxErrorCount/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:value-of select="$path/dnp:maxErrorCount/dnp:currentValue/dnp:value"/><br/>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:maxErrorCount"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:maxErrorCount/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.9 HMAC algorithm requested in a challenge exchange -->
				<xsl:if test="$path/dnp:hmacAlgorithmRequested"><tr>
					<td width="35%" valign="top">1.12.9 HMAC algorithm requested in a challenge exchange:<p/>
						<i>Part of the authentication message is hashed using an HMAC algorithm. DNP3 devices must support SHA-1 and 
						may optionally support SHA-256 for this hashing process. The output of the HMAC algorithm is truncated 
						(the resulting length dependant on the media being used).</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested/dnp:capabilities/dnp:sha1Truncated4"/>
							<xsl:with-param name="prompt" select="'SHA-1 (truncated to 4 octets)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested/dnp:capabilities/dnp:sha1Truncated10"/>
							<xsl:with-param name="prompt" select="'SHA-1 (truncated to 10 octets)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested/dnp:capabilities/dnp:sha256Truncated8"/>
							<xsl:with-param name="prompt" select="'SHA-256 (truncated to 8 octets)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested/dnp:capabilities/dnp:sha256Truncated16"/>
							<xsl:with-param name="prompt" select="'SHA-256 (truncated to 16 octets)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>					
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:hmacAlgorithmRequested/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:hmacAlgorithmRequested/dnp:currentValue/dnp:sha1Truncated4">SHA-1 (4)<br/></xsl:if>
								<xsl:if test="$path/dnp:hmacAlgorithmRequested/dnp:currentValue/dnp:sha1Truncated10">SHA-1 (10)<br/></xsl:if>
								<xsl:if test="$path/dnp:hmacAlgorithmRequested/dnp:currentValue/dnp:sha256Truncated8">SHA-256 (8)<br/></xsl:if>
								<xsl:if test="$path/dnp:hmacAlgorithmRequested/dnp:currentValue/dnp:sha256Truncated16">SHA-256 (16)<br/></xsl:if>
								<xsl:for-each select="$path/dnp:hmacAlgorithmRequested/dnp:currentValue/dnp:other">
									Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:hmacAlgorithmRequested/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.10 Key wrap algorithm -->
				<xsl:if test="$path/dnp:keyWrapAlgorithm"><tr>
					<td width="35%" valign="top">1.12.10 Key-wrap algorithm to encrypt session keys:<p/>
						<i>During the update of a session key, the key is encrypted using AES-128 or optionally using other algorithms.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:keyWrapAlgorithm/dnp:capabilities/dnp:aes128"/>
							<xsl:with-param name="prompt" select="'AES-128'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:keyWrapAlgorithm/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>					
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:keyWrapAlgorithm"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:keyWrapAlgorithm/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:keyWrapAlgorithm/dnp:currentValue/dnp:aes128">AES-128<br/></xsl:if>
								<xsl:for-each select="$path/dnp:keyWrapAlgorithm/dnp:currentValue/dnp:other">
									Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:keyWrapAlgorithm"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:keyWrapAlgorithm/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.11 Cipher Suites used with DNP implementations using TLS -->
				<xsl:if test="$path/dnp:TLSCipherSuites"><tr>
					<td width="35%" valign="top">1.12.11 Cipher Suites used with DNP implementations using TLS:<p/>
						<i>Indicate the supported Cipher Suites for implementations using TLS..</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:notApplicable"/>
							<xsl:with-param name="prompt" select="'Not relevant - TLS is not used'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSRSAEncryptedRC4"/>
							<xsl:with-param name="prompt" select="'TLS_RSA encrypted with RC4_128'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSRSAEncrypted3DES"/>
							<xsl:with-param name="prompt" select="'TLS_RSA encrypted with 3DES_EDE_CBC'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHSignedDSSEncrypted3DES"/>
							<xsl:with-param name="prompt" select="'TLS_DH, signed with DSS, encrypted with 3DES_EDE_CBC'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHSignedRSAEncrypted3DES"/>
							<xsl:with-param name="prompt" select="'TLS_DH, signed with RSA, encrypted with 3DES_EDE_CBC'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHESignedDSSEncrypted3DES"/>
							<xsl:with-param name="prompt" select="'TLS_DHE, signed with DSS, encrypted with 3DES_EDE_CBC'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHESignedRSAEncrypted3DES"/>
							<xsl:with-param name="prompt" select="'TLS_DHE, signed with RSA, encrypted with 3DES_EDE_CBC'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHSignedDSSEncryptedAES128"/>
							<xsl:with-param name="prompt" select="'TLS_DH, signed with DSS, encrypted with AES128'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHSignedDSSEncryptedAES256"/>
							<xsl:with-param name="prompt" select="'TLS_DH, signed with DSS, encrypted with AES256'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHEncryptedAES128"/>
							<xsl:with-param name="prompt" select="'TLS_DH encrypted with AES128'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:TLSDHEncryptedAES256"/>
							<xsl:with-param name="prompt" select="'TLS_DH encrypted with AES256'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>					
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:TLSCipherSuites/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:notApplicable">Not relevant<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSRSAEncryptedRC4">TLS_RSA Encrypted RC4<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSRSAEncrypted3DES">TLS_RSA Encrypted 3DES<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHSignedDSSEncrypted3DES">TLS_DH Signed DSS Encrypted 3DES<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHSignedRSAEncrypted3DES">TLS_DH Signed RSA Encrypted 3DES<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHESignedDSSEncrypted3DES">TLS_DHE Signed DSS Encrypted 3DES<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHESignedRSAEncrypted3DES">TLS_DHE Signed RSA Encrypted 3DES<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHSignedDSSEncryptedAES128">TLS_DH Signed DSS Encrypted AES128<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHSignedDSSEncryptedAES256">TLS_DH Signed DSS Encrypted AES256<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHEncryptedAES128">TLS_DH Encrypted AES128<br/></xsl:if>
								<xsl:if test="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:TLSDHEncryptedAES256">TLS_DH Encrypted AES256<br/></xsl:if>
								<xsl:for-each select="$path/dnp:TLSCipherSuites/dnp:currentValue/dnp:other">
									Other, <b><xsl:value-of select="./dnp:explanation"/></b><br/></xsl:for-each>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:TLSCipherSuites/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.12 Change cipher request timeout -->
				<xsl:if test="$path/dnp:changeCipherRequestTimeout"><tr>
					<td width="35%" valign="top">1.12.12 Change cipher request timeout:<p/>
						<i>Implementations using TLS shall terminate the connection if a response to a change cipher request is not seen 
						within this timeout period.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:changeCipherRequestTimeout/dnp:capabilities/dnp:notApplicable"/>
							<xsl:with-param name="prompt" select="'Not relevant - TLS is not used'"/>
						</xsl:call-template>
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:changeCipherRequestTimeout"/><xsl:with-param name="block" select="'-F-RSO-'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:changeCipherRequestTimeout"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:changeCipherRequestTimeout/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:changeCipherRequestTimeout/dnp:currentValue/dnp:notApplicable">Not relevant<br/></xsl:if>
								<xsl:if test="$path/dnp:changeCipherRequestTimeout/dnp:currentValue/dnp:value">
									<xsl:value-of select="$path/dnp:changeCipherRequestTimeout/dnp:currentValue/dnp:value"/><br/></xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:changeCipherRequestTimeout"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:changeCipherRequestTimeout/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.13 Number of Certificate Authorities supported -->
				<xsl:if test="$path/dnp:numberCASupported"><tr>
					<td width="35%" valign="top">1.12.13 Number of Certificate Authorities supported:<p/>
						<i>Implementations using TLS shall support at least 4 Certificate Authorities. Indicate the number supported. </i>
					</td>
					<xsl:call-template name="greyBox35"/>
					<xsl:choose>
						<xsl:when test="$path/dnp:numberCASupported/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:numberCASupported/dnp:currentValue/dnp:value">
									<xsl:value-of select="$path/dnp:numberCASupported/dnp:currentValue/dnp:value"/><br/></xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:numberCASupported"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:numberCASupported/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.14 Certificate revocation check time -->
				<xsl:if test="$path/dnp:certificateRevocationCheckTime"><tr>
					<td width="35%" valign="top">1.12.14 Certificate Revocation check time:<p/>
						<i>Implementations using TLS shall evaluate Certificate Revocation Lists on a periodic basis, 
						terminating a connection if a certificate is revoked.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:certificateRevocationCheckTime/dnp:capabilities/dnp:notApplicable"/>
							<xsl:with-param name="prompt" select="'Not relevant - TLS is not used'"/>
						</xsl:call-template>
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:certificateRevocationCheckTime"/><xsl:with-param name="block" select="'-F-RSO-'"/>
							<xsl:with-param name="units" select="'hours'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:certificateRevocationCheckTime"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:certificateRevocationCheckTime/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:certificateRevocationCheckTime/dnp:currentValue/dnp:notApplicable">Not relevant<br/></xsl:if>
								<xsl:if test="$path/dnp:certificateRevocationCheckTime/dnp:currentValue/dnp:value">
									<xsl:value-of select="$path/dnp:certificateRevocationCheckTime/dnp:currentValue/dnp:value"/><br/></xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:certificateRevocationCheckTime"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:certificateRevocationCheckTime/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.15 Additional critical function codes -->
				<xsl:if test="$path/dnp:additionalCriticalFCs"><tr>
					<td width="35%" valign="top">1.12.15 Additional critical function codes:<p/>
						<i>The DNP3 security supplement defines those messages with specific function codes that are critical and must be used 
						as part of a secure authentication message exchange. Messages with other function codes are optional and changes to this 
						list should be noted here.</i>
					</td>
					<td width="35%" valign="top">
						Additional function codes that are to be considered as "critical":<br/>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC0"/>
							<xsl:with-param name="prompt" select="'0 (Confirm)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC1"/>
							<xsl:with-param name="prompt" select="'1 (Read)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC7"/>
							<xsl:with-param name="prompt" select="'7 (Immediate freeze)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC8"/>
							<xsl:with-param name="prompt" select="'8 (Immediate freeze - no ack)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC9"/>
							<xsl:with-param name="prompt" select="'9 (Freeze-and-clear)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC10"/>
							<xsl:with-param name="prompt" select="'10 (Freeze-and-clear - no ack)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC11"/>
							<xsl:with-param name="prompt" select="'11 (Freeze-at-time)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC12"/>
							<xsl:with-param name="prompt" select="'12 (Freeze-at-time - no ack)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC22"/>
							<xsl:with-param name="prompt" select="'22 (Assign Class)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC23"/>
							<xsl:with-param name="prompt" select="'23 (Delay Measurement)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC25"/>
							<xsl:with-param name="prompt" select="'25 (Open File)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC26"/>
							<xsl:with-param name="prompt" select="'26 (Close File)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC27"/>
							<xsl:with-param name="prompt" select="'27 (Delete File)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC28"/>
							<xsl:with-param name="prompt" select="'28 (Get File Info)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC30"/>
							<xsl:with-param name="prompt" select="'30 (Abort File)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC129"/>
							<xsl:with-param name="prompt" select="'129 (Response)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:capabilities/dnp:FC130"/>
							<xsl:with-param name="prompt" select="'130 (Unsolicited Response)'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:additionalCriticalFCs/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC0">FC0<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC1">FC1<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC7">FC7<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC8">FC8<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC9">FC9<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC10">FC10<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC11">FC11<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC12">FC12<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC22">FC22<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC23">FC23<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC25">FC25<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC26">FC26<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC27">FC27<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC28">FC28<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC30">FC30<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC129">FC129<br/></xsl:if>
								<xsl:if test="$path/dnp:additionalCriticalFCs/dnp:currentValue/dnp:FC130">FC130<br/></xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:additionalCriticalFCs/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			<!-- Section 1.12.16 Other critical fragments -->
				<xsl:if test="$path/dnp:otherCriticalFragments"><tr>
					<td width="35%" valign="top">1.12.16 Other critical fragments:<p/>
						<i>Other critical transactions can be defined and should be detailed here. Examples could be based on time (for example: 
						the first transaction after a communications session is established). Other examples could be based on specific data objects 
						(for example: the reading of specific data points).</i>
					</td>
					<td width="35%" valign="top">
						<xsl:for-each select="$path/dnp:otherCriticalFragments/dnp:capabilities/dnp:criticalFragments">
							<xsl:value-of select="."/><br/>
						</xsl:for-each>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:otherCriticalFragments"/>
						</xsl:call-template>&#160;
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:otherCriticalFragments/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:for-each select="$path/dnp:otherCriticalFragments/dnp:currentValue/dnp:criticalFragments">
									<xsl:value-of select="."/><br/>
								</xsl:for-each>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:otherCriticalFragments"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise><xsl:call-template name="whiteBox20"></xsl:call-template></xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:otherCriticalFragments/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr></xsl:if>
			</tbody>
		</table>
	</xsl:template>

	<!-- templates used for section 3 -->
	<!-- BINARY INPUTS -->
	<xsl:template name="binaryInputGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.1. Binary Input Points</big>
						</font>
						<br/>Static (Steady-State) Object Number: <b>1</b>
						<br/>Event Object Number: <b>2</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<!-- 3.1.1 Static Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.1.1. Static Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - Single-bit packed format<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - Single-bit with flag<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.1.2 Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.1.2. Event Variation reported when variation 0 requested:
						<p/><i>Note: The support for binary input events can be determined remotely using protocol object Group 0 Variation 237.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - with absolute time<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - with relative time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.1.3 Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:eventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.1.3. Event reporting mode:
				<p/>
						<i>
							<xsl:value-of select="$evtext"/>
							<br/>All events are typically reported for Binary Inputs </i>
					</td>
					<xsl:call-template name="evmode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:eventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.1.4 Binary Inputs included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.1.4. Binary Inputs included in Class 0 response:<p/>
						<i>If Binary Inputs are not included in the Class 0 response, Binary Input Events (group 2) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:class0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!-- DOUBLE-BIT BINARY INPUTS -->
	<xsl:template name="doubleBitInputGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.2. Double-bit Input Points</big>
						</font>
						<br/>Static (Steady-State) Object Number: <b>3</b>
						<br/>Event Object Number: <b>4</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<!-- 3.2.1 Static Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.2.1. Static Variation reported when variation 0 requested
						<p/><i>Note: The support for double-bit inputs can be determined remotely using protocol object Group 0 Variation 234.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - Double-bit packed format<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - Double-bit with flag<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.2.2 Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.2.2. Event Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - with absolute time<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - with relative time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.2.3 Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:eventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.2.3. Event reporting mode:
				<p/>
						<i>
							<xsl:value-of select="$evtext"/>
							<br/>All events are typically reported for Double Bit Inputs </i>
					</td>
					<xsl:call-template name="evmode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:eventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.2.4 Double Bit Inputs included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.2.4. Double Bit Inputs included in Class 0 response:<p/>
						<i>If Double-bit Inputs are not included in the Class 0 response, Double-bit Input Events (group 4) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:class0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!-- BINARY OUTPUTS -->
	<xsl:template name="binaryOutputGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.3. Binary Output Status and Control Relay Output Block</big>
						</font>
						<br/>Binary Output Status Object Number: <b>10</b>
						<br/>Binary Output Event Object Number: <b>11</b>
						<br/>CROB Object Number: <b>12</b>
						<br/>Binary Output Command Event Object Number: <b>13</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<!-- 3.3.1 Minimum pulse time allowed with Trip, Close and Pulse On commands -->
			<xsl:if test="$path/dnp:configuration/dnp:minimumPulseTime">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.1. Minimum pulse time allowed with Trip, Close and Pulse On commands.</td>
					<td width="35%" valign="top">
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:minimumPulseTime/dnp:capabilities/dnp:fixed">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Fixed at <b>
							<xsl:value-of select="$path/dnp:configuration/dnp:minimumPulseTime/dnp:capabilities/dnp:fixed"/>
						</b> ms 
						(hardware may limit this further)<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:minimumPulseTime"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:minimumPulseTime"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:minimumPulseTime/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:minimumPulseTime/dnp:currentValue/dnp:fixed">Fixed at 
								<xsl:value-of select="$path/dnp:configuration/dnp:minimumPulseTime/dnp:currentValue/dnp:fixed"/> ms</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:minimumPulseTime/dnp:currentValue/dnp:basedOnPointIndex">Based on point index (see tables in part 5)</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:minimumPulseTime"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:minimumPulseTime/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.3.2 Maximum pulse time allowed with Trip, Close and Pulse On commands -->
			<xsl:if test="$path/dnp:configuration/dnp:maximumPulseTime">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.2. Maximum pulse time allowed with Trip, Close and Pulse On commands.</td>
					<td width="35%" valign="top">
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:maximumPulseTime/dnp:capabilities/dnp:fixed">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Fixed at <b>
							<xsl:value-of select="$path/dnp:configuration/dnp:maximumPulseTime/dnp:capabilities/dnp:fixed"/>
						</b> ms 
						(hardware may limit this further<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maximumPulseTime"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maximumPulseTime"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:maximumPulseTime/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:maximumPulseTime/dnp:currentValue/dnp:fixed">Fixed at 
								<xsl:value-of select="$path/dnp:configuration/dnp:maximumPulseTime/dnp:currentValue/dnp:fixed"/> ms</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maximumPulseTime/dnp:currentValue/dnp:basedOnPointIndex">Based on point index (see tables in part 5)</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maximumPulseTime"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maximumPulseTime/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.3.3 Binary Output Status included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.3.3. Binary Output Status included in Class 0 response:<p/>
						<i>If Binary Output Status points are not included in the Class 0 response, Binary Output Status Events (group 11) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:class0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.3.4 Reports Output Command Event Objects -->
			<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.4. Reports Output Command Event Objects:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:capabilities/dnp:never"/>
							<xsl:with-param name="prompt" select="'Never'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:capabilities/dnp:onSuccess"/>
							<xsl:with-param name="prompt" select="'Only upon a successful Control'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:capabilities/dnp:allControlAttempts"/>
							<xsl:with-param name="prompt" select="'Upon all control attempts'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue/dnp:never">Never</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue/dnp:onSuccess">On success</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue/dnp:allControlAttempts">On all attempts</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.3.5 Static Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.5. Static Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - Continuous control<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - Continuous control, binary output status<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.3.6 Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.6. Event Variation reported when variation 0 requested
						<p/><i>Note: The support for binary output events can be determined remotely using protocol object Group 0 Variation 222.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - with absolute time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.3.7 Command Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultCommandEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.7. Command Event Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> - without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template>
						 - with absolute time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.3.8 Change Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:changeEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.8. Change Event reporting mode:
					<p/>
						<i>
							<xsl:value-of select="$evtext"/>
							<br/>
						</i>
					</td>
					<xsl:call-template name="changeevmode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:changeEventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.3.9 Command Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:commandEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.3.9. Command Event reporting mode:
					<p/>
						<i>
							<xsl:value-of select="$evtext"/>
							<br/>
						</i>
					</td>
					<xsl:call-template name="commandevmode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:commandEventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.3.10. Maximum Time between Select and Operate -->
			<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate">
				<tr>
					<td width="35%" valign="top">3.3.10. Maximum Time between Select and Operate:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:capabilities/dnp:notApplicable"/>
							<xsl:with-param name="prompt" select="'Not Applicable'"/>
						</xsl:call-template>
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
							<xsl:with-param name="block" select="'-F-RSOV'"/>
							<xsl:with-param name="units" select="'seconds'"/>
						</xsl:call-template>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:notApplicable">Not Applicable</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:value">
									<xsl:value-of select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:value"/>&#x20;seconds</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:other">Other</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:variable">Variable</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!-- COUNTERS -->
	<xsl:template name="counterGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.4. Counters / Frozen Counters</big>
						</font>
						<br/>Static Counter Object Number: <b>20</b>
						<br/>Static Frozen Counter Object Number: <b>21</b>
						<br/>Counter Event Object Number: <b>22</b>
						<br/>Frozen Counter Event Object Number: <b>23</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<!-- 3.4.1 Static Counter Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultCounterStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.4.1. Static Counter Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
						</xsl:call-template> - 32-bit with flag<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
						</xsl:call-template> - 16-bit with flag<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
						</xsl:call-template> - 32-bit without flag
						<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
						</xsl:call-template> - 16-bit without flag
						<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultCounterStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!--  Counter Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultCounterEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.4.2. Counter Event Variation reported when variation 0 requested
						<p/><i>Note: The support for counter events can be determined remotely using protocol object Group 0 Variation 227.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
						</xsl:call-template> - 32-bit with flag<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
						</xsl:call-template> - 16-bit with flag<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
						</xsl:call-template>
						 - 32-bit with flag and time<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
						</xsl:call-template>
						 - 16-bit with flag and time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultCounterEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCounterEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.4.3 Counters included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:counterClass0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.4.3. Counters included in Class 0 response:<p/>
						<i>If counters are not included in the Class 0 response, Counter Events (group 22) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterClass0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.4.4 Counter Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:counterEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.4.4. Counter Event reporting mode:
						<p/><i>
							<br/>When responding with event data and more than one event has occurred for a data point, an Outstation may include all events
							 or only the most recent event. Only the most recent event is typically reported for Counters. When reporting only the most 
							 recent event the counter value returned in the response may be
							 either the value at the time that the event is queued or it may be the value at the time of the response.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:capabilities/dnp:mostRecentEventTimeValue"/>
							<xsl:with-param name="prompt" select="'A: Only most recent (value at time of event)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:capabilities/dnp:mostRecentResponseTimeValue"/>
							<xsl:with-param name="prompt" select="'B: Only most recent (value at time of response)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:capabilities/dnp:allEvents"/>
							<xsl:with-param name="prompt" select="'C: All events'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:capabilities/dnp:basedOnPointIndex"/>
							<xsl:with-param name="prompt" select="'Based on point index (see tables in part 5)'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterEventReportingMode"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:currentValue/dnp:mostRecentEventTimeValue">
									Most recent - event time</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:currentValue/dnp:mostRecentResponseTimeValue">
									Most recent - response time</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:currentValue/dnp:allEvents">
									All events</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:counterEventReportingMode/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterEventReportingMode"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.4.5 Static Frozen Counter Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.4.5. Static Frozen Counter Variation reported when variation 0 requested:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
						 - 32-bit with flag<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
						 - 16-bit with flag<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
						 - 32-bit with flag and time<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
						 - 16-bit with flag and time<br/>
						<xsl:call-template name="variation9">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
						 - 32-bit without flag<br/>
						<xsl:call-template name="variation10">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
						 - 16-bit without flag<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
						</xsl:call-template>
				</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.4.6 Frozen Counter Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.4.6. Frozen Counter Event Variation reported when variation 0 requested:
						<p/><i>Note: The support for frozen counter events can be determined remotely using protocol object Group 0 Variation 225.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
						</xsl:call-template>
						 - 32-bit with flag<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
						</xsl:call-template>
						 - 16-bit with flag<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
						</xsl:call-template>
						 - 32-bit without flag<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
						</xsl:call-template>
						 - 16-bit without flag<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.4.7 Frozen Counters included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:frozenCounterClass0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.4.7. Frozen Counters included in Class 0 response:<p/>
						<i>If Frozen Counters are not included in the Class 0 response, Frozen Counter Events (group 23) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:frozenCounterClass0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.4.8 Frozen Counter Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:frozenCounterEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.4.8. Frozen Counter Event reporting mode:
					<p/><i>
						<xsl:value-of select="$evtext"/> All events are typically reported for Frozen Counters </i>
					</td>
					<xsl:call-template name="evmode">
						<xsl:with-param name="frozen" select="'yes'"/>
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:frozenCounterEventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.4.9. Counters Roll Over at -->
			<xsl:if test="$path/dnp:configuration/dnp:counterRollOver">
				<tr>
					<td width="35%" valign="top">3.4.9. Counters Roll Over at:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterRollOver/dnp:capabilities/dnp:sixteenBits"/>
							<xsl:with-param name="prompt" select="'16 Bits (65,535)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterRollOver/dnp:capabilities/dnp:thirtyTwoBits"/>
							<xsl:with-param name="prompt" select="'32 Bits (4,294,967,295)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterRollOver"/>
							<xsl:with-param name="block" select="'-F-RSO-'"/>
						</xsl:call-template>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterRollOver"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterRollOver"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:sixteenBits">65,535</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:thirtyTwoBits">4,294,967,295</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:value">
									<xsl:value-of select="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:value"/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:other">Other</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterRollOver"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:counterRollOver/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.4.10. Counters frozen by means of -->
			<xsl:if test="$path/dnp:configuration/dnp:countersFrozen">
				<tr>
					<td width="35%" valign="top">3.4.10. Counters frozen by means of:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:countersFrozen/dnp:capabilities/dnp:masterRequest"/>
							<xsl:with-param name="prompt" select="'Master Request'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:countersFrozen/dnp:capabilities/dnp:localFreezeWithoutTimeOfDay"/>
							<xsl:with-param name="prompt" select="'Freezes itself without concern for time of day'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:countersFrozen/dnp:capabilities/dnp:localFreezeRequiredTimeOfDay"/>
							<xsl:with-param name="prompt" select="'Freezes itself and requires time of day'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:countersFrozen/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:countersFrozen"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:countersFrozen/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:countersFrozen/dnp:currentValue/dnp:masterRequest">Master Request</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:countersFrozen/dnp:currentValue/dnp:localFreezeWithoutTimeOfDay">Local w/o time</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:countersFrozen/dnp:currentValue/dnp:localFreezeRequiredTimeOfDay">Local with time</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:countersFrozen/dnp:currentValue/dnp:other">Other</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:countersFrozen"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:countersFrozen/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!-- ANALOG INPUTS -->
	<xsl:template name="analogInputGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.5. Analog Input Points</big>
						</font>
						<br/>Static (Steady-State) Object Number: <b>30</b>
						<br/>Event Object Number: <b>32</b>
						<br/>Deadband Object Number: <b>34</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<!-- 3.5.1 Static Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.5.1. Static Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 32-bit with flag<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 16-bit with flag<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 32-bit without flag<br/>
						<xsl:call-template name="variation4">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 16-bit without flag<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 
						single-precision floating point with flag<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 
						double-precision floating point with flag<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.5.2 Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.5.2. Event Variation reported when variation 0 requested:
						<p/><i>Note: The support for analog input events can be determined remotely using protocol object Group 0 Variation 231.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						32-bit without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						16-bit without time<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						32-bit with time<br/>
						<xsl:call-template name="variation4">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						16-bit with time<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						single-precision floating point w/o time<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						double-precision floating point w/o time<br/>
						<xsl:call-template name="variation7">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						single-precision floating point with time<br/>
						<xsl:call-template name="variation8">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template> - 
						double-precision floating point with time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.5.3 Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:analogEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.5.3. Event reporting mode:
						<p/>
						<i>
							<br/>When responding with event data and more than one event has occurred for a data point, an Outstation may include all events
							 or only the most recent event. Only the most recent event is typically reported for Analog Inputs.
							 When reporting only the most recent event the analog value returned in the response may be
							 either the value at the time that the event is queued or it may be the value at the time of the response.
						</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:capabilities/dnp:mostRecentEventTimeValue"/>
							<xsl:with-param name="prompt" select="'A: Only most recent (value at time of event)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:capabilities/dnp:mostRecentResponseTimeValue"/>
							<xsl:with-param name="prompt" select="'B: Only most recent (value at time of response)'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:capabilities/dnp:allEvents"/>
							<xsl:with-param name="prompt" select="'C: All events'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:capabilities/dnp:basedOnPointIndex"/>
							<xsl:with-param name="prompt" select="'Based on point index (see tables in part 5)'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:currentValue/dnp:mostRecentEventTimeValue">
									Most recent - event time</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:currentValue/dnp:mostRecentResponseTimeValue">
									Most recent - response time</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:currentValue/dnp:allEvents">
									All events</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.5.4 Analog Inputs included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:analogInputClass0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.5.4. Analog Inputs included in Class 0 response:<p/>
						<i>If Analog Inputs are not included in the Class 0 response, Analog Input Events (group 32) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogInputClass0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.5.5 Analog Deadband Assignments -->
			<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments">
				<tr>
					<td width="35%" valign="top">3.5.5. How Deadbands are set:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:capabilities/dnp:fixed"/>
							<xsl:with-param name="prompt" select="'A. Global Fixed'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:capabilities/dnp:configurableViaDNP3"/>
							<xsl:with-param name="prompt" select="'B. Configurable through DNP'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:capabilities/dnp:configurableViaOtherMeans"/>
							<xsl:with-param name="prompt" select="'C. Configurable via other means'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'D. Other, explain:'"/>
						</xsl:call-template>
						<br/>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:capabilities/dnp:basedOnPointIndex"/>
							<xsl:with-param name="prompt" select="'Based on point index (see tables in part 5)'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue/dnp:fixed">A<br/></xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue/dnp:configurableViaDNP3">B<br/></xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue/dnp:configurableViaOtherMeans">C<br/></xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue/dnp:other">D<br/></xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.5.6. Analog Deadband Algorithm -->
			<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm">
				<tr>
					<td width="35%" valign="top">3.5.6. Analog Deadband Algorithm:
					<p/>
						<table border="0" align="center" width="100%" cellspacing="0" cellpadding="3">
							<i>
								<tr>
									<td width="20%" valign="top">simple-</td>
									<td width="80%" valign="top">just compares the difference from the previous reported value</td>
								</tr>
								<tr>
									<td width="20%" valign="top">integrating-</td>
									<td width="80%" valign="top">keeps track of the accumulated change</td>
								</tr>
								<tr>
									<td width="20%" valign="top">other-</td>
									<td width="80%" valign="top">indicating another algorithm</td>
								</tr>
							</i>
						</table>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:capabilities/dnp:simple"/>
							<xsl:with-param name="prompt" select="'Simple'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:capabilities/dnp:integrating"/>
							<xsl:with-param name="prompt" select="'Integrating'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:capabilities/dnp:basedOnPointIndex"/>
							<xsl:with-param name="prompt" select="'Based on point index (see tables in part 5)'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAlgorithm"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:currentValue/dnp:simple">Simple</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:currentValue/dnp:integrating">Integrating</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:currentValue/dnp:other">Other</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:currentValue/dnp:basedOnPointIndex">
									Based on point index
								</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAlgorithm"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.5.7 Static Frozen Analog Input Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.5.7. Static Frozen Analog Input Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 32-bit with flag<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 16-bit with flag<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 32-bit with time-of-freeze<br/>
						<xsl:call-template name="variation4">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 16-bit with time-of-freeze<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 32-bit without flag<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 16-bit without flag<br/>
						<xsl:call-template name="variation7">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 
						single-precision floating point with flag<br/>
						<xsl:call-template name="variation8">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template> - 
						double-precision floating point with flag<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.5.8 Frozen Analog Input Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.5.8. Frozen Analog Input Event Variation reported when variation 0 requested:
						<p/><i>Note: The support for frozen analog input events can be determined remotely using protocol object Group 0 Variation 230.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						32-bit without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						16-bit without time<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						32-bit with time<br/>
						<xsl:call-template name="variation4">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						16-bit with time<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						single-precision floating point w/o time<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						double-precision floating point w/o time<br/>
						<xsl:call-template name="variation7">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						single-precision floating point with time<br/>
						<xsl:call-template name="variation8">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template> - 
						double-precision floating point with time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultFrozenAnalogInputEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.5.9 Frozen Analog Inputs included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:frozenAnalogInputClass0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.5.9. Frozen Analog Inputs included in Class 0 response:<p/>
						<i>If Frozen Analog Inputs are not included in the Class 0 response, Frozen Analog Input Events (group 33) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:frozenAnalogInputClass0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.5.10 Frozen Analog Input Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:frozenAnalogEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.5.10. Frozen Analog Input Event reporting mode:
					<p/><i>
						<xsl:value-of select="$evtext"/> All events are typically reported for Frozen Analog Inputs. </i>
					</td>
					<xsl:call-template name="evmode">
						<xsl:with-param name="frozen" select="'yes'"/>
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:frozenAnalogEventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!-- ANALOG OUTPUTS -->
	<xsl:template name="analogOutputGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.6. Analog Output Status and Analog Output Control Block</big>
						</font>
						<br/>Analog Output Status Object Number: <b>40</b>
						<br/>Analog Output Control Block Object Number: <b>41</b>
						<br/>Analog Output Event Object Number: <b>42</b>
						<br/>Analog Output Command Event Object Number: <b>43</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<!-- 3.6.1 Static Analog Output Status Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.6.1. Static Analog Output Status Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 32-bit with flag<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 16-bit with flag<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 
						single-precision floating point with flag<br/>
						<xsl:call-template name="variation4">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template> - 
						double-precision floating point with flag<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.6.2 Analog Output Status included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.6.2. Analog Output Status included in Class 0 response:<p/>
						<i>If Analog Output Status points are not included in the Class 0 response, Analog Output Events (group 42) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:class0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.6.3 Reports Output Command Event Objects -->
			<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.6.3. Reports Output Command Event Objects:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:capabilities/dnp:never"/>
							<xsl:with-param name="prompt" select="'Never'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:capabilities/dnp:onSuccess"/>
							<xsl:with-param name="prompt" select="'Only upon a successful Control'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:capabilities/dnp:allControlAttempts"/>
							<xsl:with-param name="prompt" select="'Upon all control attempts'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue/dnp:never">Never</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue/dnp:onSuccess">On success</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:currentValue/dnp:allControlAttempts">On all attempts</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:outputCommandEventObjects/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.6.4 Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultChangeEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.6.4. Event Variation reported when variation 0 requested
						<p/><i>Note: The support for analog output events can be determined remotely using protocol object Group 0 Variation 219.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						32-bit without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						16-bit without time<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						32-bit with time<br/>
						<xsl:call-template name="variation4">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						16-bit with time<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						single-precision floating point w/o time<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						double-precision floating point w/o time<br/>
						<xsl:call-template name="variation7">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						single-precision floating point with time<br/>
						<xsl:call-template name="variation8">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template> - 
						double-precision floating point with time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultChangeEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultChangeEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.6.5 Command Event Variation reported when variation 0 requested -->
			<xsl:if test="$path/dnp:configuration/dnp:defaultCommandEventVariation">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.6.5. Command Event Variation reported when variation 0 requested</td>
					<td width="35%" valign="top">
						<xsl:call-template name="variation1">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 32-bit without time<br/>
						<xsl:call-template name="variation2">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 16-bit without time<br/>
						<xsl:call-template name="variation3">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 32-bit with time<br/>
						<xsl:call-template name="variation4">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 16-bit with time<br/>
						<xsl:call-template name="variation5">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 single-precision floating point w/o time<br/>
						<xsl:call-template name="variation6">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 double-precision floating point w/o time<br/>
						<xsl:call-template name="variation7">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 single-precision floating point with time<br/>
						<xsl:call-template name="variation8">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template> -
						 double-precision floating point with time<br/>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:call-template name="variationCurrentValue">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
								</xsl:call-template>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.6.6 Change Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:changeEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.6.6. Change Event reporting mode:
						<p/>
						<i>
							<xsl:value-of select="$evtext"/>
							<br/>
						</i>
					</td>
					<xsl:call-template name="changeevmode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:changeEventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.6.7 Command Event reporting mode -->
			<xsl:if test="$path/dnp:configuration/dnp:commandEventReportingMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">3.6.7. Command Event reporting mode:
						<p/>
						<i>
							<xsl:value-of select="$evtext"/>
							<br/>
						</i>
					</td>
					<xsl:call-template name="commandevmode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:commandEventReportingMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
			<!-- 3.6.8 Maximum Time between Select and Operate -->
			<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate">
				<tr>
					<td width="35%" valign="top">3.6.8. Maximum Time between Select and Operate:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:capabilities/dnp:notApplicable"/>
							<xsl:with-param name="prompt" select="'Not Applicable'"/>
						</xsl:call-template>
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
							<xsl:with-param name="block" select="'-F-RSOV'"/>
							<xsl:with-param name="units" select="'seconds'"/>
						</xsl:call-template>
						<xsl:call-template name="variationPointIndex">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:notApplicable">Not Applicable</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:value">
									<xsl:value-of select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:value"/>&#x20;seconds</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:other">Other</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:variable">Variable</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!-- SEQUENTIAL FILE TRANSFER -->
	<xsl:template name="sequentialFileTransfer">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.7. Sequential File Transfer</big>
						</font>
						<br/>Object Number: <b>70</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<!-- 3.7.1. File Transfer Supported -->
			<xsl:if test="$path/dnp:configuration/dnp:fileTransferSupported">
				<tr>
					<td width="35%" valign="top">3.7.1. File Transfer Supported:</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileTransferSupported/dnp:capabilities/dnp:yes"/>
							<xsl:with-param name="prompt" select="'Yes'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileTransferSupported/dnp:capabilities/dnp:no"/>
							<xsl:with-param name="prompt" select="'No (do not complete any further entries in section 3.7)'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileTransferSupported"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:fileTransferSupported/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:fileTransferSupported/dnp:currentValue/dnp:yes">Yes</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:fileTransferSupported/dnp:currentValue/dnp:no">No</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileTransferSupported"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileTransferSupported/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.7.2. File Authentication -->
			<xsl:if test="$path/dnp:configuration/dnp:fileAuthentication">
				<tr>
					<td width="35%" valign="top">3.7.2. File Authentication:<p/>
						<i>Indicates whether a valid authentication key must be obtained prior to open and delete requests.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAuthentication/dnp:capabilities/dnp:always"/>
							<xsl:with-param name="prompt" select="'Always'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAuthentication/dnp:capabilities/dnp:sometimes"/>
							<xsl:with-param name="prompt" select="'Sometimes, explain'"/>
						</xsl:call-template>
						<br/>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAuthentication/dnp:capabilities/dnp:never"/>
							<xsl:with-param name="prompt" select="'Never'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAuthentication"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:fileAuthentication/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:fileAuthentication/dnp:currentValue/dnp:always">Always</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:fileAuthentication/dnp:currentValue/dnp:sometimes">Sometimes</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:fileAuthentication/dnp:currentValue/dnp:never">Never</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAuthentication"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAuthentication/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.7.3. File Append Mode -->
			<xsl:if test="$path/dnp:configuration/dnp:fileAppendMode">
				<tr>
					<td width="35%" valign="top">3.7.3. File Append Mode:<p/>
						<i>Indicates if a file can be opened and appended to versus just overwritten.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAppendMode/dnp:capabilities/dnp:always"/>
							<xsl:with-param name="prompt" select="'Always'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAppendMode/dnp:capabilities/dnp:sometimes"/>
							<xsl:with-param name="prompt" select="'Sometimes, explain'"/>
						</xsl:call-template>
						<br/>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAppendMode/dnp:capabilities/dnp:never"/>
							<xsl:with-param name="prompt" select="'Never'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAppendMode"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:fileAppendMode/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:fileAppendMode/dnp:currentValue/dnp:always">Always</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:fileAppendMode/dnp:currentValue/dnp:sometimes">Sometimes</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:fileAppendMode/dnp:currentValue/dnp:never">Never</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAppendMode"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileAppendMode/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.7.4. Permissions Support -->
			<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport">
				<tr>
					<td width="35%" valign="top">3.7.4. Permissions Support:<p/>
						<i>Indicates the device is capable of using the indicated permissions.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:ownerReadAllowed"/>
							<xsl:with-param name="prompt" select="'Owner Read Allowed: 0x0100'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:ownerWriteAllowed"/>
							<xsl:with-param name="prompt" select="'Owner Write Allowed: 0x0080'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:ownerExecuteAllowed"/>
							<xsl:with-param name="prompt" select="'Owner Execute Allowed: 0x0040'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:groupReadAllowed"/>
							<xsl:with-param name="prompt" select="'Group Read Allowed: 0x0020'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:groupWriteAllowed"/>
							<xsl:with-param name="prompt" select="'Group Write Allowed: 0x0010'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:groupExecuteAllowed"/>
							<xsl:with-param name="prompt" select="'Group Execute Allowed: 0x0008'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:worldReadAllowed"/>
							<xsl:with-param name="prompt" select="'World Read Allowed: 0x0004'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:worldWriteAllowed"/>
							<xsl:with-param name="prompt" select="'World Write Allowed: 0x0002'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:capabilities/dnp:worldExecuteAllowed"/>
							<xsl:with-param name="prompt" select="'World Execute Allowed: 0x0001'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:ownerReadAllowed">Owner Read<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:ownerWriteAllowed">Owner Write<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:ownerExecuteAllowed">Owner Execute<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:groupReadAllowed">Group Read<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:groupWriteAllowed">Group Write<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:groupExecuteAllowed">Group Execute<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:worldReadAllowed">World Read<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:worldWriteAllowed">World Write<br/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:permissionsSupport/dnp:currentValue/dnp:worldExecuteAllowed">World Execute</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:permissionsSupport/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.7.5. Multiple Blocks in a Fragment -->
			<xsl:if test="$path/dnp:configuration/dnp:multipleBlocksInFragment">
				<tr>
					<td width="35%" valign="top">3.7.5. Multiple Blocks in a Fragment:<p/>
						<i>File data is transferred in a series of blocks of a maximum specified size. This indicates whether only a single block or multiple blocks
				will be sent in fragment.</i>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:multipleBlocksInFragment/dnp:capabilities/dnp:yes"/>
							<xsl:with-param name="prompt" select="'Yes'"/>
						</xsl:call-template>
						<xsl:call-template name="tickbox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:multipleBlocksInFragment/dnp:capabilities/dnp:no"/>
							<xsl:with-param name="prompt" select="'No'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:multipleBlocksInFragment"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:multipleBlocksInFragment/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:multipleBlocksInFragment/dnp:currentValue/dnp:yes">Yes</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:multipleBlocksInFragment/dnp:currentValue/dnp:no">No</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:multipleBlocksInFragment"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:multipleBlocksInFragment/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
			<!-- 3.7.6. Max number of Files Open at one time -->
			<xsl:if test="$path/dnp:configuration/dnp:maxOpenFiles">
				<tr>
					<td width="35%" valign="top">3.7.6. Max number of Files Open at one time:<p/>
						<i/>
					</td>
					<td width="35%" valign="top">
						<xsl:call-template name="tickboxBlock">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxOpenFiles"/>
							<xsl:with-param name="block" select="'-F-RSO-'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxOpenFiles"/>
						</xsl:call-template>
					</td>
					<xsl:choose>
						<xsl:when test="$path/dnp:configuration/dnp:maxOpenFiles/dnp:currentValue">
							<td width="20%" valign="baseline">
								<xsl:if test="$path/dnp:configuration/dnp:maxOpenFiles/dnp:currentValue/dnp:value">
									<xsl:value-of select="$path/dnp:configuration/dnp:maxOpenFiles/dnp:currentValue/dnp:value"/>
								</xsl:if>
								<xsl:if test="$path/dnp:configuration/dnp:maxOpenFiles/dnp:currentValue/dnp:other">Other</xsl:if>
								<xsl:call-template name="showNotesCV">
									<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxOpenFiles"/>
								</xsl:call-template>
								&#160;
							</td>
						</xsl:when>
						<xsl:otherwise>
							<xsl:call-template name="whiteBox20"/>
						</xsl:otherwise>
					</xsl:choose>
					<td width="10%" valign="baseline">
						<xsl:call-template name="methodsBox">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:maxOpenFiles/dnp:methods"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!--  OCTET STRINGS -->
	<xsl:template name="octetStringGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.8. Octet String Points</big>
						</font>
						<br/>Static (Steady-State) Object Number: <b>110</b>
						<br/>Event Object Number: <b>111</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
			<tr bgcolor="white">
				<!-- 3.8.1 Event reporting mode -->
				<xsl:if test="$path/dnp:configuration/dnp:eventReportingMode">
					<tr bgcolor="white">
						<td width="35%" valign="top">3.8.1. Event reporting mode:
							<p/>
							<i>
								<xsl:value-of select="$evtext"/>
								<br/>
							</i>
						</td>
						<xsl:call-template name="evmode">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:eventReportingMode"/>
						</xsl:call-template>
					</tr>
				</xsl:if>
			</tr>
			<!-- 3.8.2 Octet Strings included in Class 0 response -->
			<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode">
				<tr bgcolor="white">
					<td width="35%" valign="top">
						3.8.2. Octet Strings included in Class 0 response:<p/>
						<i>If Octet Strings are not included in the Class 0 response, Octet String Events (group 111) may not be reported.</i>
					</td>
					<xsl:call-template name="class0Mode">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:class0ResponseMode"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
	</xsl:template>
	<!-- VIRTUAL TERMINALS -->
	<xsl:template name="virtualTerminalGroup">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.9. Virtual Terminal Port Numbers (Points)</big>
						</font>
						<br/>Static (Steady-State) Object Number: <b>112</b>
						<br/>Event Object Number: <b>113</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
		</table>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
		This version of the Device Profile has no requirement for describing Virtual Terminal point capabilities
		and current settings. This page is intentionally left blank, existing as placeholder for future use.
		</table>
	</xsl:template>
	
	<!-- Data Set Prototypes -->
	<xsl:template name="dsPrototype">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left" colspan="4">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.10. Data Set Prototype</big>
						</font>
						<br/>Object Number: <b>85</b>
						<br/>Variation Number: <b>1</b>
					</th>
				</b>
			</tr>
			<xsl:call-template name="titleBoxCapabilities"/>
		</table>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
		This version of the Device Profile has no requirement for describing Data Set Prototype capabilities
		and current settings. This page is intentionally left blank, existing as placeholder for future use.
		</table>
	</xsl:template>
	
	<!--  DATA SET DESCRIPTORS -->
	<xsl:template name="dsDescriptor">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="yellow">
				<b>
					<th align="left">
						<font face="Times New Roman" style="font-variant: small-caps;">
							<big>3.11. Data Set Descriptor Contents and Characteristics</big>
						</font>
						<br/>Object Number: <b>86</b>
						<br/>Variation Numbers: <b>1 and 2</b>
					</th>
				</b>
			</tr>
		</table>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
		This version of the Device Profile has no requirement for describing Data Set Descriptor capabilities
		and current settings. This page is intentionally left blank, existing as placeholder for future use.
		</table>
	</xsl:template>
	
	
	<!-- template to display a tickbox according to a specific path and then to display a text prompt -->
	<xsl:template name="tickbox">
		<xsl:param name="path"/>
		<xsl:param name="prompt"/>
		<xsl:param name="units"/>
		<xsl:choose>
			<xsl:when test="$path">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;<xsl:value-of select="$prompt"/>
		<xsl:value-of select="$units"/>
		<br/>
	</xsl:template>
	<!-- template to display a tickbox with value according to a specific path and then to display a text prompt -->
	<xsl:template name="tickboxValue">
		<xsl:param name="path"/>
		<xsl:param name="prompt"/>
		<xsl:param name="units"/>
		<xsl:choose>
			<xsl:when test="$path">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;<xsl:value-of select="$prompt"/>&#x20;<b>
			<xsl:value-of select="$path"/>
		</b>&#x20;
		<xsl:value-of select="$units"/>
		<br/>
	</xsl:template>
	<!-- template to display a tickbox with fixed according to a specific path and then to display a text prompt -->
	<xsl:template name="tickboxFixed">
		<xsl:param name="path"/>
		<xsl:param name="prompt"/>
		<xsl:param name="units"/>
		<xsl:choose>
			<xsl:when test="$path">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Fixed at <b>
			<xsl:value-of select="$path"/>
		</b>&#x20;<xsl:value-of select="$units"/>
		<br/>
	</xsl:template>
	<!-- template to display a tickbox with range according to a specific path and then to display a text prompt -->
	<xsl:template name="tickboxRange">
		<xsl:param name="path"/>
		<xsl:param name="units"/>
		<xsl:choose>
			<xsl:when test="$path">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Configurable, range 
		<b>
			<xsl:value-of select="$path/dnp:minimum"/>
		</b> to 
		<b>
			<xsl:value-of select="$path/dnp:maximum"/>
		</b> &#x20;
		<xsl:value-of select="$units"/>
		<br/>
	</xsl:template>
	<!-- template to display a tickbox with selectable according to a specific path and then to display a text prompt -->
	<xsl:template name="tickboxSelectable">
		<xsl:param name="path"/>
		<xsl:param name="units"/>
		<xsl:choose>
			<xsl:when test="$path">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Configurable, selectable from  
		<xsl:for-each select="$path">
			<b>
				<xsl:value-of select="."/>
			</b>
			<xsl:if test="not(position() = last())">, </xsl:if>
		</xsl:for-each>&#x20;
		<xsl:value-of select="$units"/>
		<br/>
	</xsl:template>
	<!-- template to display a tickbox and an explanation (or more) -->
	<xsl:template name="tickbox-explain">
		<xsl:param name="path"/>
		<xsl:param name="prompt"/>
		<xsl:choose>
			<xsl:when test="$path">
				<xsl:for-each select="$path">
					<xsl:call-template name="tickbox-explain-once">
						<xsl:with-param name="path" select="."></xsl:with-param>
						<xsl:with-param name="prompt" select="$prompt"></xsl:with-param>
					</xsl:call-template><br/>
				</xsl:for-each>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="tickbox-explain-once">
					<xsl:with-param name="path" select="$path"></xsl:with-param>
					<xsl:with-param name="prompt" select="$prompt"></xsl:with-param>
				</xsl:call-template><br/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="tickbox-explain-once">
		<xsl:param name="path"/>
		<xsl:param name="prompt"/>
		<xsl:choose>
			<xsl:when test="$path">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;<xsl:value-of select="$prompt"/>&#x20;
		<xsl:choose>
			<xsl:when test="$path">
				<xsl:for-each select="$path/dnp:explanation">
					<b>
						<xsl:value-of select="."/>
					</b>
					<xsl:if test="not(position() = last())">
						<br/>
					</xsl:if>
				</xsl:for-each>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<!-- template to display a block of tickboxes -->
	<xsl:template name="tickboxBlock">
		<xsl:param name="path"/>
		<xsl:param name="block"/>
		<xsl:param name="units"/>
		<!-- block tells us which type of box and prompt according to a specific character in a character position:
			None
			Fixed
			Fixed at 20000
 			Range
			Selectable
			Other
			Variable -->
		<xsl:if test="substring($block, 1, 1) = 'N'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:none">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;None<br/>
		</xsl:if>
		<xsl:if test="substring($block, 2, 1) = 'F'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:fixed">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Fixed at <b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:fixed"/>
			</b>&#x20;<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 3, 1) = '2'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:fixedAt20000">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Fixed at 20,000<br/>
		</xsl:if>
		<xsl:if test="substring($block, 4, 1) = 'R'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:range">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Configurable, range 
			<b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:range/dnp:minimum"/>
			</b> to 
			<b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:range/dnp:maximum"/>
			</b> &#x20;
			<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 5, 1) = 'S'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:selectable">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Configurable, selectable from  
			<xsl:for-each select="$path/dnp:capabilities/dnp:selectable">
				<b>
					<xsl:value-of select="."/>
				</b>
				<xsl:if test="not(position() = last())">, </xsl:if>
			</xsl:for-each>&#x20;
			<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 6, 1) = 'O'">
			<xsl:call-template name="tickbox-explain">
				<xsl:with-param name="path" select="$path/dnp:capabilities/dnp:other"/>
				<xsl:with-param name="prompt" select="'Other, explain:'"/>
			</xsl:call-template>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 7, 1) = 'V'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:variable">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Variable, explain:  &#x20;
			<b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:variable/dnp:explanation"/>
			</b>
			<br/>
		</xsl:if>
	</xsl:template>
	<xsl:template name="tickboxBlockGeneral">
		<xsl:param name="path"/>
		<xsl:param name="block"/>
		<xsl:param name="units"/>
		<!-- block tells us which type of box and prompt according to a specific character in a character position:
			None
			Fixed
			Fixed at 20000
 			Range
			Selectable
			Other
			Variable -->
		<xsl:if test="substring($block, 2, 1) = 'F'">
			<xsl:choose>
				<xsl:when test="$path/dnp:fixed">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Fixed at <b>
				<xsl:value-of select="$path/dnp:fixed"/>
			</b>&#x20;<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 4, 1) = 'R'">
			<xsl:choose>
				<xsl:when test="$path/dnp:range">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Configurable, range 
			<b>
				<xsl:value-of select="$path/dnp:range/dnp:minimum"/>
			</b> to 
			<b>
				<xsl:value-of select="$path/dnp:range/dnp:maximum"/>
			</b> &#x20;
			<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 5, 1) = 'S'">
			<xsl:choose>
				<xsl:when test="$path/dnp:selectable">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Configurable, selectable from  
			<xsl:for-each select="$path/dnp:selectable">
				<b>
					<xsl:value-of select="."/>
				</b>
				<xsl:if test="not(position() = last())">, </xsl:if>
			</xsl:for-each>&#x20;
			<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 6, 1) = 'O'">
			<xsl:call-template name="tickbox-explain">
				<xsl:with-param name="path" select="$path/dnp:other"/>
				<xsl:with-param name="prompt" select="'Configurable, other, describe:'"/>
			</xsl:call-template>
			<br/>
		</xsl:if>
	</xsl:template>
	<!-- template to display a block of tickboxes that are not configurable -->
	<xsl:template name="tickboxBlockNotConfigurable">
		<xsl:param name="path"/>
		<xsl:param name="block"/>
		<xsl:param name="units"/>
		<!-- block tells us which type of box and prompt according to a specific character in a character position:
			Fixed
			Range
			Selectable
			Other -->
		<xsl:if test="substring($block, 1, 1) = 'F'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:fixed">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Fixed at <b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:fixed"/>
			</b>&#x20;<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 2, 1) = 'R'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:range">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Range 
			<b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:range/dnp:minimum"/>
			</b> to 
			<b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:range/dnp:maximum"/>
			</b> &#x20;
			<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 3, 1) = 'S'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:selectable">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Selectable from  
			<xsl:for-each select="$path/dnp:capabilities/dnp:selectable">
				<b>
					<xsl:value-of select="."/>
				</b>
				<xsl:if test="not(position() = last())">, </xsl:if>
			</xsl:for-each>&#x20;
			<xsl:value-of select="$units"/>
			<br/>
		</xsl:if>
		<xsl:if test="substring($block, 4, 1) = 'O'">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:other">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Other, describe:  
			<b>
				<xsl:value-of select="$path/dnp:capabilities/dnp:other/dnp:explanation"/>
			</b>
			<br/>
		</xsl:if>
	</xsl:template>
	<!-- template to display methods -->
	<xsl:template name="methodsBox">
		<xsl:param name="path"/>
		<xsl:if test="$path/dnp:xmlViaFileTransfer">XML via File Transfer<br/>----------------<br/>
		</xsl:if>
		<xsl:if test="$path/dnp:xmlViaOtherTransportMechanism">XML via Other Mechanism<br/>----------------<br/>
		</xsl:if>
		<xsl:if test="$path/dnp:proprietaryFileViaFileTransfer">Proprietary File via File Transfer<br/>----------------<br/>
		</xsl:if>
		<xsl:if test="$path/dnp:proprietaryFileViaOtherTransportMechanism">Proprietary File via Other Mechanism<br/>----------------<br/>
		</xsl:if>
		<xsl:if test="$path/dnp:terminal">terminal<br/>----------------<br/></xsl:if>
		<xsl:if test="$path/dnp:software">
			software <b>
				<xsl:value-of select="$path/dnp:software/dnp:name"/>
			</b>
			<xsl:if test="$path/dnp:software/dnp:version"> Vers </xsl:if><b>
				<xsl:value-of select="$path/dnp:software/dnp:version"/>
			</b>
			<br/>----------------<br/>
		</xsl:if>
		<xsl:if test="$path/dnp:direct">direct<br/>----------------<br/></xsl:if>
		<xsl:if test="$path/dnp:factory">factory<br/>----------------<br/></xsl:if>
		<xsl:if test="$path/dnp:protocol">protocol<br/>----------------<br/></xsl:if>
		<xsl:if test="$path/dnp:other">other<br/>(<xsl:value-of select="$path/dnp:other/dnp:explanation"/>)</xsl:if>
		<!-- in case there are no methods, we need a space to format the display correctly -->
		<font color="white">&#160;</font>
		<xsl:call-template name="showNotesGeneral">
			<xsl:with-param name="path" select="$path"/>
		</xsl:call-template>
	</xsl:template>
	<!-- template to display the initial title box for a 4 column configuration section -->
	<xsl:template name="titleBox4">
		<xsl:param name="title"/>
		<tr bgcolor="yellow">
			<b>
				<th align="left" width="35%">
					<font face="Times New Roman" style="font-variant: small-caps;">
						<xsl:value-of select="$title"/>
					</font>
				</th>
				<th align="left" width="35%">Capabilities</th>
				<th align="left" width="20%">Current Value</th>
				<th align="left" width="10%">If configurable list methods</th>
			</b>
		</tr>
	</xsl:template>
	<!-- template to display the initial title box for a 3 column configuration section -->
	<xsl:template name="titleBox3">
		<xsl:param name="title"/>
		<tr bgcolor="yellow">
			<b>
				<th align="left" width="45%">
					<font face="Times New Roman" style="font-variant: small-caps;">
						<xsl:value-of select="$title"/>
					</font>
				</th>
				<th align="left" width="45%">Value of Current Setting</th>
				<th align="left" width="10%">If configurable list methods</th>
			</b>
		</tr>
	</xsl:template>
	<!-- template to display the initial title box for a single column configuration section -->
	<xsl:template name="titleBox1">
		<xsl:param name="title"/>
		<tr bgcolor="yellow">
			<b>
				<th align="left" width="100%" colspan="4">
					<font face="Times New Roman" style="font-variant: small-caps;">
						<xsl:value-of select="$title"/>
					</font>
				</th>
			</b>
		</tr>
	</xsl:template>
	<!-- template to display the Capabilities title box  -->
	<xsl:template name="titleBoxCapabilities">
		<tr bgcolor="yellow">
			<b>
				<th align="left" width="35%">
					<font color="yellow">.</font>
				</th>
				<th align="left" width="35%">Capabilities</th>
				<th align="left" width="20%">Current Value</th>
				<th align="left" width="10%">If configurable list methods</th>
			</b>
		</tr>
	</xsl:template>
	<!-- template to put out a tickbox for variation 1 -->
	<xsl:template name="variation1">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:one">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 1
	</xsl:template>
	<!-- template to put out a tickbox for variation 2 -->
	<xsl:template name="variation2">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:two">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 2
	</xsl:template>
	<!-- template to put out a tickbox for variation 3 -->
	<xsl:template name="variation3">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:three">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 3
	</xsl:template>
	<!-- template to put out a tickbox for variation 4 -->
	<xsl:template name="variation4">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:four">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 4
	</xsl:template>
	<!-- template to put out a tickbox for variation 5 -->
	<xsl:template name="variation5">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:five">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 5
	</xsl:template>
	<!-- template to put out a tickbox for variation 6 -->
	<xsl:template name="variation6">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:six">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 6
	</xsl:template>
	<!-- template to put out a tickbox for variation 7 -->
	<xsl:template name="variation7">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:seven">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 7
	</xsl:template>
	<!-- template to put out a tickbox for variation 8 -->
	<xsl:template name="variation8">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:eight">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 8
	</xsl:template>
	<!-- template to put out a tickbox for variation 9 -->
	<xsl:template name="variation9">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:nine">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 9
	</xsl:template>
	<!-- template to put out a tickbox for variation 10 -->
	<xsl:template name="variation10">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:ten">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Variation 10
	</xsl:template>
	<!-- template to put out a current value of None, Value, Other or variable -->
	<xsl:template name="currentValueNVOV">
		<xsl:param name="path"/>
		<xsl:param name="units"/>
			<xsl:if test="$path/dnp:currentValue/dnp:none">None</xsl:if>
			<xsl:if test="$path/dnp:currentValue/dnp:value">
				<xsl:value-of select="$path/dnp:currentValue/dnp:value"/>&#x20;
				<xsl:value-of select="$units"/></xsl:if>
			<xsl:if test="$path/dnp:currentValue/dnp:other">Other</xsl:if>
			<xsl:if test="$path/dnp:currentValue/dnp:variable">Variable</xsl:if>
	</xsl:template>
	<!-- template to put out a tickbox for variation based on point index -->
	<xsl:template name="variationPointIndex">
		<xsl:param name="path"/>
		<xsl:choose>
			<xsl:when test="$path/dnp:capabilities/dnp:basedOnPointIndex">
				<input type="checkbox" name="box" checked=""/>
			</xsl:when>
			<xsl:otherwise>
				<input type="checkbox" name="box"/>
			</xsl:otherwise>
		</xsl:choose>
		<label for="box"/>&#x20;Based on point index (see tables in part 5)
	</xsl:template>
	<!-- template to put out a current value for variation  -->
	<xsl:template name="variationCurrentValue">
		<xsl:param name="path"/>
		<xsl:if test="$path/dnp:currentValue/dnp:one">One</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:two">Two</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:three">Three</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:four">Four</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:five">Five</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:six">Six</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:seven">Seven</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:eight">Eight</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:nine">Nine</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:ten">Ten</xsl:if>
		<xsl:if test="$path/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
	</xsl:template>
	<!-- template to put out an entry for the event reporting mode -->
	<xsl:template name="evmode">
		<xsl:param name="path"/>
		<xsl:param name="frozen"/>
		<td width="35%" valign="top">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:mostRecent">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Only most recent
			<xsl:if test="$frozen = 'yes'"> frozen value</xsl:if>
			<br/>
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:allEvents">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;All
			<xsl:choose>
				<xsl:when test="$frozen = 'yes'"> frozen values</xsl:when>
				<xsl:otherwise> events</xsl:otherwise>
			</xsl:choose>
			<xsl:if test="$path/dnp:capabilities/dnp:basedOnPointIndex">
				<br/>
				<xsl:choose>
					<xsl:when test="$path/dnp:capabilities/dnp:basedOnPointIndex">
						<input type="checkbox" name="box" checked=""/>
					</xsl:when>
					<xsl:otherwise>
						<input type="checkbox" name="box"/>
				</xsl:otherwise>
				</xsl:choose>
				<label for="box"/>&#x20;Based on point index (see tables in part 5)
			</xsl:if>
			<xsl:call-template name="showNotesCap">
				<xsl:with-param name="path" select="$path"/>
			</xsl:call-template>
		</td>
		<xsl:choose>
			<xsl:when test="$path/dnp:currentValue">
				<td width="20%" valign="baseline">
					<xsl:if test="$path/dnp:currentValue/dnp:mostRecent">Most recent</xsl:if>
					<xsl:if test="$path/dnp:currentValue/dnp:allEvents">All events</xsl:if>
					<xsl:if test="$path/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
					<xsl:call-template name="showNotesCV">
						<xsl:with-param name="path" select="$path"/>
					</xsl:call-template>
					&#160;
				</td>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="whiteBox20"/>
			</xsl:otherwise>
		</xsl:choose>
		<td width="10%" valign="baseline">
			<xsl:call-template name="methodsBox">
				<xsl:with-param name="path" select="$path/dnp:methods"/>
			</xsl:call-template>
		</td>
	</xsl:template>
	<!-- template to put out an entry for the change event reporting mode -->
	<xsl:template name="changeevmode">
		<xsl:param name="path"/>
		<td width="35%" valign="top">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:mostRecent">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Only most recent<br/>
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:allEvents">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;All events
			<xsl:call-template name="showNotesCap">
				<xsl:with-param name="path" select="$path"/>
			</xsl:call-template>
		</td>
		<xsl:choose>
			<xsl:when test="$path/dnp:currentValue">
				<td width="20%" valign="baseline">
					<xsl:if test="$path/dnp:currentValue/dnp:mostRecent">Most recent</xsl:if>
					<xsl:if test="$path/dnp:currentValue/dnp:allEvents">All events</xsl:if>
					<xsl:call-template name="showNotesCV">
						<xsl:with-param name="path" select="$path"/>
					</xsl:call-template>
					&#160;
				</td>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="whiteBox20"/>
			</xsl:otherwise>
		</xsl:choose>
		<td width="10%" valign="baseline">
			<xsl:call-template name="methodsBox">
				<xsl:with-param name="path" select="$path/dnp:methods"/>
			</xsl:call-template>
		</td>
	</xsl:template>
	<!-- template to put out an entry for the command event reporting mode -->
	<xsl:template name="commandevmode">
		<xsl:param name="path"/>
		<td width="35%" valign="top">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:mostRecent">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Only most recent<br/>
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:allEvents">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;All events
			<xsl:call-template name="showNotesCap">
				<xsl:with-param name="path" select="$path"/>
			</xsl:call-template>
		</td>
		<xsl:choose>
			<xsl:when test="$path/dnp:currentValue">
				<td width="20%" valign="baseline">
					<xsl:if test="$path/dnp:currentValue/dnp:mostRecent">Most recent</xsl:if>
					<xsl:if test="$path/dnp:currentValue/dnp:allEvents">All events</xsl:if>
					<xsl:call-template name="showNotesCV">
						<xsl:with-param name="path" select="$path"/>
					</xsl:call-template>
					&#160;
				</td>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="whiteBox20"/>
			</xsl:otherwise>
		</xsl:choose>
		<td width="10%" valign="baseline">
			<xsl:call-template name="methodsBox">
				<xsl:with-param name="path" select="$path/dnp:methods"/>
			</xsl:call-template>
		</td>
	</xsl:template>
	<!-- template to put out an entry for the class 0 response mode -->
	<xsl:template name="class0Mode">
		<xsl:param name="path"/>
		<td width="35%" valign="top">
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:always">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Always<br/>
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:never">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Never<br/>
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:onlyWhenAssignedToClass123">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Only if point is assigned to Class 1, 2, or 3<br/>
			<xsl:choose>
				<xsl:when test="$path/dnp:capabilities/dnp:basedOnPointIndex">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Based on point index (see tables in part 5)
			<xsl:call-template name="showNotesCap">
				<xsl:with-param name="path" select="$path"/>
			</xsl:call-template>
		</td>
		<xsl:choose>
			<xsl:when test="$path/dnp:currentValue">
				<td width="20%" valign="baseline">
					<xsl:if test="$path/dnp:currentValue/dnp:always">Always</xsl:if>
					<xsl:if test="$path/dnp:currentValue/dnp:never">Never</xsl:if>
					<xsl:if test="$path/dnp:currentValue/dnp:onlyWhenAssignedToClass123">If assigned</xsl:if>
					<xsl:if test="$path/dnp:currentValue/dnp:basedOnPointIndex">Based on point index</xsl:if>
					<xsl:call-template name="showNotesCV">
						<xsl:with-param name="path" select="$path"/>
					</xsl:call-template>
					&#160;
				</td>
			</xsl:when>
			<xsl:otherwise>
				<xsl:call-template name="whiteBox20"/>
			</xsl:otherwise>
		</xsl:choose>
		<td width="10%" valign="baseline">
			<xsl:call-template name="methodsBox">
				<xsl:with-param name="path" select="$path/dnp:methods"/>
			</xsl:call-template>
		</td>
	</xsl:template>
	
	<!-- interpret a function code and display the relevant text -->
	<xsl:template name="dispFcode">
		<xsl:param name="fcode"/>
		<xsl:if test="$fcode = 1"><i>(read)</i></xsl:if>
		<xsl:if test="$fcode = 2"><i>(write)</i></xsl:if>
		<xsl:if test="$fcode = 3"><i>(select)</i></xsl:if>
		<xsl:if test="$fcode = 4"><i>(operate)</i></xsl:if>
		<xsl:if test="$fcode = 5"><i>(direct op.)</i></xsl:if>
		<xsl:if test="$fcode = 6"><i>(direct op, no ack)</i></xsl:if>
		<xsl:if test="$fcode = 7"><i>(freeze)</i></xsl:if>
		<xsl:if test="$fcode = 8"><i>(freeze, no ack)</i></xsl:if>
		<xsl:if test="$fcode = 9"><i>(freeze &#38; clear )</i></xsl:if>
		<xsl:if test="$fcode = 10"><i>(frz &#38; clr, no ack)</i></xsl:if>
		<xsl:if test="$fcode = 11"><i>(frz at time)</i></xsl:if>
		<xsl:if test="$fcode = 12"><i>(frz at time, no ack)</i></xsl:if>
		<xsl:if test="$fcode = 13"><i>(cold restart)</i></xsl:if>
		<xsl:if test="$fcode = 14"><i>(warm restart)</i></xsl:if>
		<xsl:if test="$fcode = 15"><i>(init. data)</i></xsl:if>
		<xsl:if test="$fcode = 16"><i>(init appl.)</i></xsl:if>
		<xsl:if test="$fcode = 17"><i>(start appl.)</i></xsl:if>
		<xsl:if test="$fcode = 18"><i>(stop appl.)</i></xsl:if>
		<xsl:if test="$fcode = 20"><i>(enable unsol.)</i></xsl:if>
		<xsl:if test="$fcode = 21"><i>(disable unsol.)</i></xsl:if>
		<xsl:if test="$fcode = 22"><i>(assign class)</i></xsl:if>
		<xsl:if test="$fcode = 23"><i>(delay meas.)</i></xsl:if>
		<xsl:if test="$fcode = 24"><i>(record current time)</i></xsl:if>
		<xsl:if test="$fcode = 25"><i>(open file)</i></xsl:if>
		<xsl:if test="$fcode = 26"><i>(close file)</i></xsl:if>
		<xsl:if test="$fcode = 27"><i>(delete file)</i></xsl:if>
		<xsl:if test="$fcode = 28"><i>(get file info)</i></xsl:if>
		<xsl:if test="$fcode = 29"><i>(authenticate file)</i></xsl:if>
		<xsl:if test="$fcode = 30"><i>(abort file)</i></xsl:if>
		<xsl:if test="$fcode = 31"><i>(activate config)</i></xsl:if>
		<xsl:if test="$fcode = 32"><i>(auth req)</i></xsl:if>
		<xsl:if test="$fcode = 33"><i>(auth req, no ack)</i></xsl:if>
	</xsl:template>
	<!-- convert a qualifier code to a hex display with the relevant text
        note that the first hex digit (assumed to be 0 to 9) is already displayed -->
	<xsl:template name="dispQualcode">
		<xsl:param name="text"/>
		<xsl:param name="digit2"/>
		<xsl:param name="qualcode"/>
		<xsl:if test="$digit2 &lt; 10">
			<xsl:value-of select="$digit2"/>
		</xsl:if>
		<xsl:if test="$digit2 = 10">A</xsl:if>
		<xsl:if test="$digit2 = 11">B</xsl:if>
		<xsl:if test="$digit2 = 12">C</xsl:if>
		<xsl:if test="$digit2 = 13">D</xsl:if>
		<xsl:if test="$digit2 = 14">E</xsl:if>
		<xsl:if test="$digit2 = 15">F</xsl:if>
		<xsl:choose>
			<xsl:when test="$text = 'yes'">
				<xsl:if test="$qualcode = 01"><i><small> (start-stop)</small></i></xsl:if>
				<xsl:if test="$qualcode = 06"><i><small> (no range, or all)</small></i></xsl:if>
				<xsl:if test="$qualcode = 08"><i><small> (limited qty)</small></i></xsl:if>
				<xsl:if test="$qualcode = 40"><i><small> (index)</small></i></xsl:if>
				<xsl:if test="$qualcode = 27"><i><small> (free format)</small></i></xsl:if>
				<xsl:if test="$qualcode = 91"><i><small> (free format)</small></i></xsl:if>
			</xsl:when>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="implementationTable">
		<xsl:param name="path"/>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<big>
					<b>4. Implementation Table</b>
				</big>
				<p/>The following implementation table identifies which object groups and variations, function codes and qualifiers the device supports
			in both requests and responses. The <i>Request</i> columns identify all requests that may be sent by a Master, or all requests that must be parsed by
			an Outstation. The <i>Response</i> columns identify all responses that must be parsed by a Master, or all responses that may be sent by an Outstation.
			</caption>
		</table>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- first line of table heading -->
			<tr>
				<th colspan="3" align="center" width="48%">DNP OBJECT GROUP &#38; VARIATION</th>
				<th colspan="2" align="center" width="26%">REQUEST<br/>Master may issue<br/>Outstation must parse</th>
				<th colspan="2" align="center" width="26%">RESPONSE<br/>Master must parse<br/>Outstation may issue</th>
			</tr>
			<!-- second line of table heading -->
			<tr>
				<th width="6%" align="center">Object<br/>Group<br/>Number</th>
				<th width="6%" align="center">Variation<br/>Number</th>
				<th width="36%" align="left">Description</th>
				<th width="13%" align="left">Function Codes (dec)</th>
				<th width="13%" align="left">Qualifier Codes (hex)</th>
				<th width="13%" align="left">Function Codes (dec)</th>
				<th width="13%" align="left">Qualifier Codes (hex)</th>
			</tr>
			<tbody>
				<!-- a row for every entry in the implementation table -->
				<xsl:for-each select="$path/dnp:table/dnp:supportedVariation">
					<xsl:choose>
						<!-- allow for the case where the object group number is not present - would this make any sense ? -->
						<xsl:when test="dnp:objectGroup">
							<tr>
								<!-- display the first three columns of group, variation and description -->
								<td valign="top" width="6%" align="center">
									<xsl:value-of select="dnp:objectGroup"/>
								</td>
								<xsl:choose>
									<xsl:when test="(dnp:objectGroup = 110 or dnp:objectGroup = 111 or dnp:objectGroup = 112 or dnp:objectGroup = 113) and (dnp:variation = 255)">
										<td valign="top" width="6%" align="center">string length</td>
									</xsl:when>
									<xsl:otherwise>
										<td valign="top" width="6%" align="center">
											<xsl:value-of select="dnp:variation"/>
										</td>
									</xsl:otherwise>
								</xsl:choose>
								<td valign="top" width="36%" align="left">
									<xsl:value-of select="dnp:description"/>
								</td>
								<xsl:choose>
									<xsl:when test="dnp:request">
										<td valign="top" width="13%" align="left">
											<xsl:choose>
												<xsl:when test="(dnp:objectGroup = 120) and (dnp:variation = 3 or dnp:variation = 9)">any</xsl:when>
												<xsl:otherwise>
													<xsl:variable name="fcode">
														<xsl:value-of select="dnp:request/dnp:functionCode"/>
													</xsl:variable>
													<xsl:value-of select="$fcode"/>
													&#x20;
													<small>
														<xsl:call-template name="dispFcode">
															<xsl:with-param name="fcode" select="$fcode"/>
														</xsl:call-template>
													</small>
												</xsl:otherwise>
											</xsl:choose>
										</td>
										<xsl:choose>
											<!-- deal with object group 0 as a special -->
											<xsl:when test="dnp:objectGroup = 0">
												<td valign="top" width="13%" align="left">
													<xsl:for-each select="dnp:request/dnp:qualifierCode">
														<xsl:variable name="qualcode">
															<xsl:value-of select="."/>
														</xsl:variable>
														<xsl:variable name="digit1" select="floor ($qualcode div 16)"/>
														<xsl:value-of select="$digit1"/>
														<xsl:variable name="digit2" select="$qualcode - ($digit1 * 16)"/>
														<xsl:call-template name="dispQualcode">
															<xsl:with-param name="text" select="'yes'"/>
															<xsl:with-param name="digit2" select="$digit2"/>
															<xsl:with-param name="qualcode" select="$qualcode"/>
														</xsl:call-template>
														<xsl:if test="($qualcode = 00)">
															<i><small> (start-stop)</small></i>
														</xsl:if>
														<xsl:if test="not(position() = last())">,&#160;</xsl:if>
                                                                                                                <br/>
													</xsl:for-each>
												</td>
											</xsl:when>
											<!-- deal with object group 70 and 82 as a special -->
											<xsl:when test="(dnp:objectGroup = 70 and dnp:variation &lt; 8) or (dnp:objectGroup = 82)">
												<td valign="top" width="13%" align="left">
													<xsl:for-each select="dnp:request/dnp:qualifierCode">
														<xsl:variable name="qualcode">
															<xsl:value-of select="."/>
														</xsl:variable>
														<xsl:variable name="digit1" select="floor ($qualcode div 16)"/>
														<xsl:value-of select="$digit1"/>
														<xsl:variable name="digit2" select="$qualcode - ($digit1 * 16)"/>
														<xsl:call-template name="dispQualcode">
															<xsl:with-param name="text" select="'no'"/>
															<xsl:with-param name="digit2" select="$digit2"/>
															<xsl:with-param name="qualcode" select="$qualcode"/>
														</xsl:call-template>
														<xsl:if test="($qualcode = 91)">
															<i><small> (Cnt = 1)</small></i>
														</xsl:if>
														<xsl:if test="not(position() = last())">,&#160;</xsl:if>
														<br/>
													</xsl:for-each>
												</td>
											</xsl:when>
											<xsl:otherwise>
												<td valign="top" width="13%" align="left">
													<xsl:for-each select="dnp:request/dnp:qualifierCode">
														<xsl:variable name="qualcode">
															<xsl:value-of select="."/>
														</xsl:variable>
														<xsl:if test="($qualcode = 07 and position() = last())"><br/></xsl:if>
														<xsl:if test="($qualcode = 23 and position() = last())"><br/></xsl:if>
														<xsl:variable name="digit1" select="floor ($qualcode div 16)"/>
														<xsl:value-of select="$digit1"/>
														<xsl:variable name="digit2" select="$qualcode - ($digit1 * 16)"/>
														<xsl:call-template name="dispQualcode">
															<xsl:with-param name="text" select="'yes'"/>
															<xsl:with-param name="digit2" select="$digit2"/>
															<xsl:with-param name="qualcode" select="$qualcode"/>
														</xsl:call-template>
														<xsl:if test="($qualcode = 00 and position() = last())">
															<i><small> (start-stop)</small></i>
														</xsl:if>
														<xsl:if test="($qualcode = 07 and position() = last())">
															<i><small> (limited qty = 1)</small></i>
														</xsl:if>
														<xsl:if test="($qualcode = 23 and position() = last())">
															<i><small> (index)</small></i>
														</xsl:if>
														<xsl:if test="($qualcode = 91 and dnp:objectGroup = 70)">
															<i><small> (Cnt = 1)</small></i>
														</xsl:if>
														<xsl:if test="not(position() = last())">,&#160;</xsl:if>
														<xsl:if test="not($qualcode = 00 or $qualcode = 07 or $qualcode = 23)">
															<br/>
														</xsl:if>
													</xsl:for-each>
												</td>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<!-- no request data - so display empty columns
											  whitespace is removed, some do not like no-break space, so put out a white dash -->
										<td width="13%">
											<font color="white">&#160;</font>
										</td>
										<td width="13%">
											<font color="white">&#160;</font>
										</td>
									</xsl:otherwise>
								</xsl:choose>
								<xsl:choose>
									<xsl:when test="dnp:response">
										<!-- if the response columns are present then display them
										      first the function code -->
										<td valign="top" width="13%" align="left">
											<xsl:for-each select="dnp:response/dnp:functionCode">
												<xsl:variable name="fcode">
													<xsl:value-of select="."/>
												</xsl:variable>
												<xsl:value-of select="$fcode"/>
												&#x20;
											   <xsl:if test="$fcode = 129">
													<i><small> (Response)</small></i>
												</xsl:if>
												<xsl:if test="$fcode = 130">
													<i><small> (Unsol. Resp.)</small></i>
												</xsl:if>
												<xsl:if test="$fcode = 131">
													<i><small> (Auth. Resp.)</small></i>
												</xsl:if>
												<xsl:if test="not(position() = last())">
													<br/>
												</xsl:if>
											</xsl:for-each>
										</td>
										<!-- followed by the qualifier code - which may not be present for example
											  for group 12 where we have "echo of request" -->
										<xsl:choose>
											<!-- deal with object group 0 as a special -->
											<xsl:when test="dnp:objectGroup = 0">
												<td valign="top" width="17%" align="left">
													<xsl:for-each select="dnp:response/dnp:qualifierCode">
														<xsl:variable name="qualcode">
															<xsl:value-of select="."/>
														</xsl:variable>
														<xsl:variable name="digit1" select="floor ($qualcode div 16)"/>
														<xsl:value-of select="$digit1"/>
														<xsl:variable name="digit2" select="$qualcode - ($digit1 * 16)"/>
														<xsl:call-template name="dispQualcode">
															<xsl:with-param name="text" select="'yes'"/>
															<xsl:with-param name="digit2" select="$digit2"/>
															<xsl:with-param name="qualcode" select="$qualcode"/>
														</xsl:call-template>
														<xsl:if test="($qualcode = 00)">
															<i><small> (start-stop)</small></i>
														</xsl:if>
														<xsl:if test="($qualcode = 23)">
															<i><small> (index)</small></i>
														</xsl:if>
														<xsl:if test="not(position() = last())">,&#160;</xsl:if>
                                                                                                                <br/>
													</xsl:for-each>
												</td>
											</xsl:when>
											<!-- deal with object groups 12 and 41 as a special -->
											<xsl:when test="(dnp:objectGroup = 12) or (dnp:objectGroup = 41)">
												<td valign="top" width="17%" align="left">echo of request</td>
											</xsl:when>
											<!-- deal with object group 70 and 82 as a special -->
											<xsl:when test="(dnp:objectGroup = 70 and dnp:variation &lt; 8) or (dnp:objectGroup = 82)">
												<td valign="top" width="13%" align="left">
													<xsl:for-each select="dnp:response/dnp:qualifierCode">
														<xsl:variable name="qualcode">
															<xsl:value-of select="."/>
														</xsl:variable>
														<xsl:variable name="digit1" select="floor ($qualcode div 16)"/>
														<xsl:value-of select="$digit1"/>
														<xsl:variable name="digit2" select="$qualcode - ($digit1 * 16)"/>
														<xsl:call-template name="dispQualcode">
															<xsl:with-param name="text" select="'no'"/>
															<xsl:with-param name="digit2" select="$digit2"/>
															<xsl:with-param name="qualcode" select="$qualcode"/>
														</xsl:call-template>
														<xsl:if test="($qualcode = 91)">
															<i><small> (Cnt = 1)</small></i>
														</xsl:if>
														<xsl:if test="not(position() = last())">,&#160;</xsl:if>
														<br/>
													</xsl:for-each>
												</td>
											</xsl:when>
											<xsl:otherwise>
												<!-- Not group 0, 70, 82, and not a control, so handle in a generic way -->
												<td valign="top" width="17%" align="left">
													<xsl:for-each select="dnp:response/dnp:qualifierCode">
														<xsl:variable name="qualcode">
															<xsl:value-of select="."/>
														</xsl:variable>
														<xsl:variable name="digit1" select="floor ($qualcode div 16)"/>
														<xsl:value-of select="$digit1"/>
														<xsl:variable name="digit2" select="$qualcode - ($digit1 * 16)"/>
														<xsl:call-template name="dispQualcode">
															<xsl:with-param name="digit2" select="$digit2"/>
															<xsl:with-param name="text" select="'yes'"/>
															<xsl:with-param name="qualcode" select="$qualcode"/>
														</xsl:call-template>
														<xsl:if test="($qualcode = 00 and position() = last())">
															<i><small> (start-stop)</small></i>
														</xsl:if>
														<xsl:if test="($qualcode = 07 and position() = last())">
															<i><small> (limited qty = 1)</small></i>
														</xsl:if>
														<xsl:if test="($qualcode = 23 and position() = last())">
															<i><small> (index)</small></i>
														</xsl:if>
														<xsl:if test="not(position() = last())">,&#160;</xsl:if>
														<xsl:if test="not($qualcode = 00 or $qualcode = 07 or $qualcode = 23)">
															<br/>
														</xsl:if>
													</xsl:for-each>
												</td>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<!-- no response column data - so display empty columns
											  whitespace is removed, some do not like no-break space, so put out a white dash -->
										<td width="13%">
											<font color="white">&#160;</font>
										</td>
										<td width="13%">
											<font color="white">&#160;</font>
										</td>
									</xsl:otherwise>
								</xsl:choose>
							</tr>
						</xsl:when>
						<xsl:otherwise>
							<!-- No group number specified - not expected -->
						</xsl:otherwise>
					</xsl:choose>
				</xsl:for-each>
				<!-- a row for every additional function code in the implementation table -->
				<xsl:for-each select="$path/dnp:table/dnp:supportedFunctionCode">
					<tr>
						<!-- display the first three columns of group, variation and description -->
						<td valign="top" colspan="3" width="48%" align="center">No object (function code only)</td>
						<td valign="top" width="13%" align="left">
						<xsl:variable name="fcode">
							<xsl:value-of select="dnp:request/dnp:functionCode"/>
						</xsl:variable>
							<xsl:value-of select="$fcode"/>
							&#x20;
							<small>
								<xsl:call-template name="dispFcode">
									<xsl:with-param name="fcode" select="$fcode"/>
								</xsl:call-template>
							</small>
						</td>
						<td valign="top" width="13%" align="left">&#160;</td>
						<td valign="top" width="13%" align="left">&#160;</td>
						<td valign="top" width="17%" align="left">&#160;</td>
					</tr>
				</xsl:for-each>
				<tr><td colspan="7">
				<xsl:call-template name="showNotesGeneral">
					<xsl:with-param name="path" select="$path"/>
				</xsl:call-template>
				</td></tr>
			</tbody>
		</table>
	</xsl:template>
	
	<xsl:template name="dataPointsList">
		<xsl:param name="path"/>
		<p/>
		<table border="0" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<big>
					<b>5. Data Points List (outstation only)</b>
				</big>
				<p/>This part of the Device Profile shows, for each data type, a table defining the data points available
				 in the device or a description of how this information can be obtained if the database is configurable.
			</caption>
		</table>
		<!--  5.1 put out the Binary Input Point List -->
		<xsl:if test="$path/dnp:binaryInputPoints">
			<xsl:call-template name="binaryInputPointList">
				<xsl:with-param name="path" select="$path/dnp:binaryInputPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.2 put out the Double-bit Input Point List -->
		<xsl:if test="$path/dnp:doubleBitInputPoints">
			<xsl:call-template name="doubleBitInputPointList">
				<xsl:with-param name="path" select="$path/dnp:doubleBitInputPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.3 put out the Binary Output Point List -->
		<xsl:if test="$path/dnp:binaryOutputPoints">
			<xsl:call-template name="binaryOutputPointList">
				<xsl:with-param name="path" select="$path/dnp:binaryOutputPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.4 put out the Counter Point List -->
		<xsl:if test="$path/dnp:counterPoints">
			<xsl:call-template name="counterPointList">
				<xsl:with-param name="path" select="$path/dnp:counterPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.5 put out the Analog Input Point List -->
		<xsl:if test="$path/dnp:analogInputPoints">
			<xsl:call-template name="analogInputPointList">
				<xsl:with-param name="path" select="$path/dnp:analogInputPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.6 put out the Analog Output Point List -->
		<xsl:if test="$path/dnp:analogOutputPoints">
			<xsl:call-template name="analogOutputPointList">
				<xsl:with-param name="path" select="$path/dnp:analogOutputPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.7 put out the Sequential File List -->
		<xsl:if test="$path/dnp:sequentialFiles">
			<xsl:call-template name="fileList">
				<xsl:with-param name="path" select="$path/dnp:sequentialFiles"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.8 put out the Octet String Point List -->
		<xsl:if test="$path/dnp:octetStringPoints">
			<xsl:call-template name="octetStringPointList">
				<xsl:with-param name="path" select="$path/dnp:octetStringPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.9 put out the Virtual Terminal Point List -->
		<xsl:if test="$path/dnp:virtualTerminalPoints">
			<xsl:call-template name="virtualTerminalPointList">
				<xsl:with-param name="path" select="$path/dnp:virtualTerminalPoints"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.10 put out the Dataset Prototype List -->
		<xsl:if test="$path/dnp:datasetPrototype">
			<xsl:call-template name="datasetPrototypeList">
				<xsl:with-param name="path" select="$path/dnp:datasetPrototype"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.11 put out the Dataset Descriptor List -->
		<xsl:if test="$path/dnp:datasetDescriptor">
			<xsl:call-template name="datasetDescriptorList">
				<xsl:with-param name="path" select="$path/dnp:datasetDescriptor"/>
			</xsl:call-template>
		</xsl:if>
		<!--  5.12 put out the Dataset Point Index Attributes -->
		<xsl:if test="$path/dnp:datasetDescriptor">
			<xsl:call-template name="dsdPointIndexAttributes">
				<xsl:with-param name="path" select="$path/dnp:datasetDescriptor"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<!-- templates used for section 5 -->
	<!-- BINARY INPUTS -->
	<xsl:template name="binaryInputPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.1 Definition of Binary Input Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.1. Definition of Binary Input Point List:</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
							<p/>Note: the number of binary inputs present in the device, and the maximum binary input index, 
							are available remotely using object Group 0 Variations 239 and 238.
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<!-- now put out the Binary Input Point List -->
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
			<br/>Binary Input points list:</caption>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Point Index</td>
				<td width="10%" align="center">Name</td>
				<td width="6%" align="center" style="writing-mode:tb-rl">Event Class Assigned (1, 2, 3 or none)</td>
				<td width="10%" align="center">Name for State when value is 0</td>
				<td width="10%" align="center">Name for State when value is 1</td>
				<td width="45%" align="center">Description</td>
				<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="5%" align="center" style="writing-mode:tb-rl">Static variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="5%" align="center" style="writing-mode:tb-rl">Event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="5%" align="center" style="writing-mode:tb-rl">Included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:binaryInput">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="6%" align="left">
						<xsl:value-of select="./dnp:changeEventClass"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:nameState0"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:nameState1"/>&#160;
					</td>
					<td width="45%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
					<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="5%" align="center">
							<xsl:if test="./dnp:defaultStaticVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:two">2</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="5%" align="center">
							<xsl:if test="./dnp:defaultEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:three">3</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="5%" align="center">
							<xsl:if test="./dnp:includedInClass0Response = 'always'">A</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'never'">N</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- DOUBLE-BIT INPUTS -->
	<xsl:template name="doubleBitInputPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.2 Definition of Double Bit Input Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.2. Definition of Double Bit Input Point List:</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
							<p/>Note: the number of double-bit inputs present in the device, and the maximum double-bit input index, 
							are available remotely using object Group 0 Variations 236 and 235.
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<!-- now put out the Double Bit Input Point List -->
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<br/>Double-bit Input points list:</caption>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Point Index</td>
				<td width="10%" align="center">Name</td>
				<td width="6%" align="center" style="writing-mode:tb-rl">Event Class Assigned (1, 2, 3 or none)</td>
				<td width="10%" align="center">Name for State when value is 0 (intermediate)</td>
				<td width="10%" align="center">Name for State when value is 1 (off)</td>
				<td width="10%" align="center">Name for State when value is 2 (on)</td>
				<td width="10%" align="center">Name for State when value is 3 (indeterminate)</td>
				<td width="25%" align="center">Description</td>
				<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="5%" align="center" style="writing-mode:tb-rl">Static variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="5%" align="center" style="writing-mode:tb-rl">Event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="5%" align="center" style="writing-mode:tb-rl">Included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:doubleBitInput">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="6%" align="left">
						<xsl:value-of select="./dnp:changeEventClass"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:nameState0"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:nameState1"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:nameState2"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:nameState3"/>&#160;
					</td>
					<td width="25%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
					<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="5%" align="center">
							<xsl:if test="./dnp:defaultStaticVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:two">2</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="5%" align="center">
							<xsl:if test="./dnp:defaultEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:three">3</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="5%" align="center">
							<xsl:if test="./dnp:includedInClass0Response = 'always'">A</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'never'">N</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- BINARY OUTPUTS -->
	<xsl:template name="binaryOutputPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.3 Definition of Binary Output Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- 5.3 Definition of Binary Output Status / Control Relay Output Block Point List -->
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.3. Definition of Binary Output Status / Control Relay Output Block Points List:
					</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
							<p/>Note: the number of binary outputs present in the device, and the maximum binary output index, 
							are available remotely using object Group 0 Variations 224 and 223.
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<!-- now put out the CROB Point List -->
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left"><br/>Binary Output Status and CROB points list:</caption>
			<tr bgcolor="lightyellow">
				<td width="14%" align="left" colspan="2">
					<font color="lightyellow">&#160;</font>
				</td>
				<td width="22%" align="center" colspan="11">Supported Control Operations</td>
				<td width="14%" align="left" colspan="2">
					<font color="lightyellow">&#160;</font>
				</td>
				<td width="8%" align="center" style="writing-mode:tb-rl" colspan="2">Event Class Assigned <br/>(1,2,3 or none)</td>
				<td width="34%" align="left" colspan="8">
					<font color="lightyellow">&#160;</font>
				</td>
			</tr>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Point Index</td>
				<td width="8%" align="center">Name</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Select/Operate</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Direct Operate</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Direct Operate - No Ack</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Pulse On</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Pulse Off</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Latch On</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Latch Off</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Trip</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Close</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Count > 1</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Cancel Currently Running Operation</td>
				<td width="7%" align="center">Name for State when value is 0</td>
				<td width="7%" align="center">Name for State when value is 1</td>
				<td width="4%" align="center" style="writing-mode:tb-rl">Change</td>
				<td width="4%" align="center" style="writing-mode:tb-rl">Command</td>
				<td width="20%" align="center">Description</td>
				<xsl:if test="$path/dnp:configuration/dnp:minimumPulseTime/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="4%" align="center" style="writing-mode:tb-rl">Minimum pulse time</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:maximumPulseTime/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="4%" align="center" style="writing-mode:tb-rl">Minimum pulse time</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Static variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Command event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="4%" align="center" style="writing-mode:tb-rl">Max time between select and operate</td>
				</xsl:if>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:binaryOutput">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="8%" align="left">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportSelectOperate">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportDirectOperate">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportDirectOperateNoAck">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportPulseOn">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportPulseOff">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportLatchOn">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportLatchOff">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportTrip">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportClose">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:countGreaterThanOne">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:cancelCurrentOperation">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="7%" align="left">
						<xsl:value-of select="./dnp:nameState0"/>&#160;
					</td>
					<td width="7%" align="left">
						<xsl:value-of select="./dnp:nameState1"/>&#160;
					</td>
					<td width="4%" align="left">
						<xsl:value-of select="./dnp:changeEventClass"/>&#160;
					</td>
					<td width="4%" align="left">
						<xsl:value-of select="./dnp:commandEventClass"/>&#160;
					</td>
					<td width="20%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
					<xsl:if test="$path/dnp:configuration/dnp:minimumPulseTime/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="4%" align="center">
							<xsl:value-of select="./dnp:minimumPulseWidth"/>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:maximumPulseTime/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="4%" align="center">
							<xsl:value-of select="./dnp:maximumPulseWidth"/>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultStaticVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:two">2</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:two">2</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:two">2</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:includedInClass0Response = 'always'">A</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'never'">N</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="4%" align="center">
							<xsl:value-of select="./dnp:maxTimeSelectOperate"/>&#160;
						</td>
					</xsl:if>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- COUNTERS -->
	<xsl:template name="counterPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.4 Definition of Counter Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- 5.4 Definition of Counter / Frozen Counter Point List -->
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.4. Definition of Counter / Frozen Counter Point List:
				</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
							<p/>Note: the number of counters present in the device, and the maximum counter index, 
							are available remotely using object Group 0 Variations 229 and 228.
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<!-- now put out the Counters Point List -->
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<br/>Counter / Frozen Counter points list:</caption>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Point Index</td>
				<td width="10%" align="center">Name</td>
				<td width="8%" align="center" style="writing-mode:tb-rl">Event Class Assigned to Counter Events (1, 2, 3 or none)</td>
				<td width="8%" align="center" style="writing-mode:tb-rl">Frozen Counter Exists (Yes or No)</td>
				<td width="8%" align="center" style="writing-mode:tb-rl">Event Class Assigned  to Frozen Counter Events (1, 2, 3 or none)</td>
				<td width="39%" align="center">Description</td>
				<xsl:if test="$path/dnp:configuration/dnp:defaultCounterStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Static counter variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultCounterEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Counter event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:counterClass0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Conter included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Static frozen counter variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Frozen counter event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:frozenCounterClass0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Frozen counter included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="5%" align="center" style="writing-mode:tb-rl">Counter rollover at</td>
				</xsl:if>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:counter">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="8%" align="left">
						<xsl:value-of select="./dnp:counterEventClass"/>&#160;
					</td>
					<td width="8%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:frozenCounterExists = 'true'">Y</xsl:when>
							<xsl:otherwise>N</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="8%" align="left">
						<xsl:value-of select="./dnp:frozenCounterEventClass"/>&#160;
					</td>
					<td width="39%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
					<xsl:if test="$path/dnp:configuration/dnp:defaultCounterStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultCounterStaticVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultCounterStaticVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultCounterStaticVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultCounterStaticVariation/dnp:six">6</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultCounterEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultCounterEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultCounterEventVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultCounterEventVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultCounterEventVariation/dnp:six">6</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:counterClass0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:countersIncludedInClass0 = 'always'">A</xsl:if>
							<xsl:if test="./dnp:countersIncludedInClass0 = 'never'">N</xsl:if>
							<xsl:if test="./dnp:countersIncludedInClass0 = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenCounterStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultFrozenCounterStaticVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultFrozenCounterStaticVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultFrozenCounterStaticVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultFrozenCounterStaticVariation/dnp:six">6</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultFrozenCounterEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultFrozenCounterEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultFrozenCounterEventVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultFrozenCounterEventVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultFrozenCounterEventVariation/dnp:six">6</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:frozenCounterClass0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:frozenCountersIncludedInClass0 = 'always'">A</xsl:if>
							<xsl:if test="./dnp:frozenCountersIncludedInClass0 = 'never'">N</xsl:if>
							<xsl:if test="./dnp:frozenCountersIncludedInClass0 = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:counterRollOver/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="5%" align="center">
							<xsl:value-of select="./dnp:counterRollOver"/>&#160;
						</td>
					</xsl:if>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>
	
	<!-- ANALOG INPUTS -->
	<xsl:template name="analogInputPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.5 Definition of Analog Input Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- 5.5 Definition of Analog Input Point List -->
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.5. Definition of Analog Input Point List:</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
							<p/>Note: the number of analog inputs present in the device, and the maximum analog input index, 
							are available remotely using object Group 0 Variations 233 and 232.
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<br/>Analog Input points list:</caption>
			<tr bgcolor="lightyellow">
				<td width="21%" align="left" colspan="3">
					<font color="lightyellow">.</font>
				</td>
				<td width="16%" align="center" colspan="2">Transmitted Value</td>
				<td width="16%" align="center" colspan="2">Scaling</td>
				<td width="38%" align="left" colspan="9">
					<font color="lightyellow">.</font>
				</td>
			</tr>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Point Index</td>
				<td width="10%" align="center">Name</td>
				<td width="6%" align="center" style="writing-mode:tb-rl">Event Class Assigned (1, 2, 3 or none)</td>
				<td width="7%" align="center">Min<br/>int / flt</td>
				<td width="6%" align="center">Max<br/>int / flt</td>
				<td width="6%" align="center">Multiplier</td>
				<td width="6%" align="center">Offset</td>
				<td width="6%" align="center">Units</td>
				<td width="6%" align="center">Resolution</td>
				<td width="22%" align="center">Description</td>
				<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Static variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="4%" align="center" style="writing-mode:tb-rl">Event Reporting Mode<br/>(A, B or C : see 3.5.3)</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="4%" align="center" style="writing-mode:tb-rl">Deadband Assignment<br/>(B, C or D : see 3.5.5.)</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="4%" align="center" style="writing-mode:tb-rl">Deadband Algorithm<br/>(Simple, Integrating, Other)</td>
				</xsl:if>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:analogInput">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="10%" align="left">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="6%" align="left">
						<xsl:value-of select="./dnp:changeEventClass"/>&#160;
					</td>
					<td width="7%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:minIntegerTransmittedValue">
								<xsl:value-of select="./dnp:minIntegerTransmittedValue"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
						&#160;/&#160;
						<xsl:choose>
							<xsl:when test="./dnp:minFloatTransmittedValue">
								<xsl:value-of select="./dnp:minFloatTransmittedValue"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:maxIntegerTransmittedValue">
								<xsl:value-of select="./dnp:maxIntegerTransmittedValue"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
						&#160;/&#160;
						<xsl:choose>
							<xsl:when test="./dnp:maxFloatTransmittedValue">
								<xsl:value-of select="./dnp:maxFloatTransmittedValue"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:choose>
							<xsl:when test="./dnp:scaleFactor">
								<xsl:value-of select="./dnp:scaleFactor"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:choose>
							<xsl:when test="./dnp:scaleOffset">
								<xsl:value-of select="./dnp:scaleOffset"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:value-of select="./dnp:units"/>&#160;
					</td>
					<td width="6%" align="left">
						<xsl:value-of select="./dnp:resolution"/>&#160;
					</td>
					<td width="22%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
					<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultStaticVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:three">3</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:four">4</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:six">6</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:three">3</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:four">4</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:six">6</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:seven">7</xsl:if>
							<xsl:if test="./dnp:defaultEventVariation/dnp:eight">8</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:includedInClass0Response = 'always'">A</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'never'">N</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:analogEventReportingMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="4%" align="center">
							<xsl:if test="./dnp:analogEventReportingMode/dnp:mostRecentEventTimeValue">A</xsl:if>
							<xsl:if test="./dnp:analogEventReportingMode/dnp:mostRecentResponseTimeValue">B</xsl:if>
							<xsl:if test="./dnp:analogEventReportingMode/dnp:allEvents">C</xsl:if>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAssignments/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="4%" align="center">
							<xsl:if test="./dnp:analogDeadbandAssignments/dnp:configurableViaDNP3">B</xsl:if>
							<xsl:if test="./dnp:analogDeadbandAssignments/dnp:configurableViaOtherMeans">C</xsl:if>
							<xsl:if test="./dnp:analogDeadbandAssignments/dnp:other">D</xsl:if>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:analogDeadbandAlgorithm/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="4%" align="center">
							<xsl:if test="./dnp:analogDeadbandAlgorithm/dnp:simple">S</xsl:if>
							<xsl:if test="./dnp:analogDeadbandAlgorithm/dnp:integrating">I</xsl:if>
							<xsl:if test="./dnp:analogDeadbandAlgorithm/dnp:other">Other</xsl:if>&#160;
						</td>
					</xsl:if>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- ANALOG OUTPUTS -->
	<xsl:template name="analogOutputPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.6 Definition of Analog IOutput Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- 5.6 Definition of Analog Output Status / Analog Output Block Point List -->
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.6. Definition of Analog Output Status / Analog Output Block Point List:
						</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
							<p/>Note: the number of analog outputs present in the device, and the maximum analog output index, 
							are available remotely using object Group 0 Variations 221 and 220.
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<br/>Analog Output points list:</caption>
			<tr bgcolor="lightyellow">
				<td width="14%" align="left" colspan="2">
					<font color="lightyellow">.</font>
				</td>
				<td width="6%" align="center" colspan="3">Supported Control Operations</td>
				<td width="16%" align="center" colspan="2">Transmitted Value</td>
				<td width="16%" align="center" colspan="2">Scaling</td>
				<td width="16%" align="center" colspan="2">
					<font color="lightyellow">.</font>
				</td>
				<td width="10%" align="center" colspan="2">Event Class Assigned (1, 2, 3 or none)</td>
				<td width="22%" align="left" colspan="6">
					<font color="lightyellow">.</font>
				</td>
			</tr>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Point Index</td>
				<td width="8%" align="center">Name</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Select/Operate</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Direct Operate</td>
				<td width="2%" align="center" style="writing-mode:tb-rl">Direct Operate - No Ack</td>
				<td width="6%" align="center">Min</td>
				<td width="6%" align="center">Max</td>
				<td width="6%" align="center">Min</td>
				<td width="6%" align="center">Max</td>
				<td width="6%" align="center">Units</td>
				<td width="6%" align="center" style="writing-mode:tb-rl">Resolution</td>
				<td width="5%" align="center" style="writing-mode:tb-rl">Change</td>
				<td width="5%" align="center" style="writing-mode:tb-rl">Command</td>
				<td width="20%" align="center">Description</td>
				<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Static variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultChangeEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Command event variation when v0 requested</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
				<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="4%" align="center" style="writing-mode:tb-rl">Max time between select and operate</td>
				</xsl:if>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:analogOutput">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="8%" align="left">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportSelectOperate">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportDirectOperate">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="2%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:supportedControlOperations/dnp:supportDirectOperateNoAck">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:choose>
							<xsl:when test="./dnp:minTransmittedValue">
								<xsl:value-of select="./dnp:minTransmittedValue"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:choose>
							<xsl:when test="./dnp:maxTransmittedValue">
								<xsl:value-of select="./dnp:maxTransmittedValue"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:choose>
							<xsl:when test="./dnp:scaleFactor">
								<xsl:value-of select="./dnp:scaleFactor"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:choose>
							<xsl:when test="./dnp:scaleOffset">
								<xsl:value-of select="./dnp:scaleOffset"/>
							</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="6%" align="left">
						<xsl:value-of select="./dnp:units"/>&#160;
					</td>
					<td width="6%" align="left">
						<xsl:value-of select="./dnp:resolution"/>&#160;
					</td>
					<td width="5%" align="left">
						<xsl:value-of select="./dnp:changeEventClass"/>&#160;
					</td>
					<td width="5%" align="left">
						<xsl:value-of select="./dnp:commandEventClass"/>&#160;
					</td>
					<td width="20%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
					<xsl:if test="$path/dnp:configuration/dnp:defaultStaticVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultStaticVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:three">3</xsl:if>
							<xsl:if test="./dnp:defaultStaticVariation/dnp:four">4</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultChangeEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:three">3</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:four">4</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:six">6</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:seven">7</xsl:if>
							<xsl:if test="./dnp:defaultChangeEventVariation/dnp:eight">8</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:defaultCommandEventVariation/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:one">1</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:two">2</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:three">3</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:four">4</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:five">5</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:six">6</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:seven">7</xsl:if>
							<xsl:if test="./dnp:defaultCommandEventVariation/dnp:eight">8</xsl:if>
							&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:includedInClass0Response = 'always'">A</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'never'">N</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
					<xsl:if test="$path/dnp:configuration/dnp:maxTimeBetweenSelectAndOperate/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="4%" align="center">
							<xsl:value-of select="./dnp:maxTimeSelectOperate"/>&#160;
						</td>
					</xsl:if>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- SEQUENTIAL FILES -->
	<xsl:template name="fileList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.7 Definition of File Names that may be read or written -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<xsl:if test="$path/dnp:configuration/dnp:fileListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.7. Definition of File Names that may be read or written:</b></td>
					<td width="50%" valign="top">
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:fileListDefinition/dnp:fixed">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Fixed, list shown in table below<br/>
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:fileListDefinition/dnp:configurable">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Configurable (current list may be shown in table below)<br/>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileListDefinition/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>
						<xsl:call-template name="showNotesCap">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:fileListDefinition"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
		</table>
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left"><br/>Sequential Files list:</caption>
			<tr bgcolor="lightyellow">
				<td width="45%" align="left" colspan="2">
					<font color="lightyellow">.</font>
				</td>
				<td width="15%" align="center" colspan="3">Authentication Required for:</td>
				<td width="40%" align="left">
					<font color="lightyellow">.</font>
				</td>
			</tr>
			<tr bgcolor="lightyellow">
				<td width="35%" align="center">File Name</td>
				<td width="10%" align="center">Event Class Assigned (1, 2, 3 or none)</td>
				<td width="5%" align="center">Read</td>
				<td width="5%" align="center">Write</td>
				<td width="5%" align="center">Delete</td>
				<td width="40%" align="center">Description</td>
			</tr>
			<xsl:for-each select="$path/dnp:files/dnp:sequentialFile">
				<tr bgcolor="white">
					<td width="35%" align="center">
						<xsl:value-of select="./dnp:fileName"/>&#160;
					</td>
					<td width="10%" align="center">
						<xsl:value-of select="./dnp:eventClass"/>&#160;
					</td>
					<td width="5%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:readAuthenticateRequired = 'true'">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="5%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:writeAuthenticateRequired = 'true'">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="5%" align="center">
						<xsl:choose>
							<xsl:when test="./dnp:deleteAuthenticateRequired = 'true'">Y</xsl:when>
							<xsl:otherwise>&#160;</xsl:otherwise>
						</xsl:choose>
					</td>
					<td width="40%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>
	
	<!-- Octet Strings -->
	<xsl:template name="octetStringPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.8 Definition of Octet String Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- 5.8 Definition of Octet String Point List -->
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.8. Definition of Octet String Point List:</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<table border="1" bordercolor="Lightgrey" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
				<br/>Octet String points list:</caption>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Point Index</td>
				<td width="10%" align="center">Name</td>
				<td width="10%" align="center">Event Class Assigned (1, 2, 3 or none)</td>
				<td width="73%" align="center">Description</td>
				<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
					<td width="3%" align="center" style="writing-mode:tb-rl">Included in Class 0 response
					<br/>(Always, Never, Class 1/2/3)</td>
				</xsl:if>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:octetString">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="10%" align="center">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="10%" align="center">
						<xsl:value-of select="./dnp:changeEventClass"/>&#160;
					</td>
					<td width="73%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
					<xsl:if test="$path/dnp:configuration/dnp:class0ResponseMode/dnp:currentValue/dnp:basedOnPointIndex">
						<td width="3%" align="center">
							<xsl:if test="./dnp:includedInClass0Response = 'always'">A</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'never'">N</xsl:if>
							<xsl:if test="./dnp:includedInClass0Response = 'onlyWhenAssignedToClass123'">C</xsl:if>&#160;
						</td>
					</xsl:if>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!-- Virtual Terminals -->
	<xsl:template name="virtualTerminalPointList">
		<xsl:param name="path"/>
		<p/>
		<!-- 5.9 Definition of Virtual terminal Point List -->
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<!-- 5.9 Definition of Virtual Terminal Port Numbers -->
			<xsl:if test="$path/dnp:configuration/dnp:pointListDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.9. Definition of Virtual Terminal Port Numbers:</b><p/>
						<i>
							<xsl:value-of select="$pointListText"/>
						</i>
					</td>
					<xsl:call-template name="pointListDefinition">
						<xsl:with-param name="path" select="$path/dnp:configuration/dnp:pointListDefinition"/>
					</xsl:call-template>
				</tr>
			</xsl:if>
		</table>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
			<br/>Ports list:</caption>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Virtual Port Number (Point Index)</td>
				<td width="10%" align="center">Name</td>
				<td width="10%" align="center">Event Class Assigned (1, 2, 3 or none)</td>
				<td width="76%" align="center">Description</td>
			</tr>
			<xsl:for-each select="$path/dnp:dataPoints/dnp:virtualTerminal">
				<tr bgcolor="white">
					<td width="4%" align="center">
						<xsl:value-of select="./dnp:index"/>&#160;
					</td>
					<td width="10%" align="center">
						<xsl:value-of select="./dnp:name"/>&#160;
					</td>
					<td width="10%" align="center">
						<xsl:value-of select="./dnp:changeEventClass"/>&#160;
					</td>
					<td width="76%" align="left">
						<xsl:value-of select="./dnp:description"/>&#160;
					</td>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>

	<!--  DATA SET PROTOTYPES -->
	<xsl:template name="datasetPrototypeList">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<xsl:if test="$path/dnp:configuration/dnp:prototypeDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.10. Definition of Data Set Prototypes:</b><p/>
						<i>
							List of all data set prototypes. The following table is repeated for each Data Set Prototype defined.
							<p/>Note: the number of data set prototypes known to the device	are available remotely 
							using object Group 0 Variations 212 and 213.
						</i>
					</td>
					<td width="50%" valign="top">
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:prototypeDefinition/dnp:capabilities/dnp:fixed">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Fixed, list shown in table below<br/>
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:prototypeDefinition/dnp:capabilities/dnp:configurable">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Configurable (current list may be shown in table below)<br/>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:prototypeDefinition/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
		</table>
		<xsl:for-each select="$path/dnp:prototype">
			<p/>
			<xsl:call-template name="dsp">
				<xsl:with-param name="path" select="."/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>
	
	<xsl:template name="dsp">
		<xsl:param name="path"/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
			Prototype Description: <xsl:value-of select="$path/dnp:description"/></caption>
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Element Number</td>
				<td width="10%" align="left">Descriptor Code</td>
				<td width="30%" align="left">Element Description</td>
				<td width="10%" align="left">Data Type Code</td>
				<td width="5%" align="left">Max Data Length</td>
				<td align="left">Ancillary Value</td>
			</tr>
			<tr>
				<td width="4%" align="center">0</td>
				<td width="10%" align="left">ID (identifier)</td>
				<td width="30%" align="left">Mandatory DS identifier</td>
				<td width="10%" align="left">None</td>
				<td width="5%" align="left">0</td>
				<td align="left">
					<xsl:value-of select="$path/dnp:id"/>
				</td>
			</tr>
			<tr>
				<td width="4%" align="center">1</td>
				<td width="10%" align="left">UUID</td>
				<td width="30%" align="left">UUID assigned to prototype</td>
				<td width="10%" align="left">None</td>
				<td width="5%" align="left">0</td>
				<td align="left">
					<xsl:value-of select="$path/dnp:uuid"/>
				</td>
			</tr>
			<xsl:choose>
				<xsl:when test="$path/dnp:namespace">
					<tr>
						<td width="4%" align="center">2</td>
						<td width="10%" align="left">NSPC</td>
						<td width="30%" align="left">Prototype namespace</td>
						<td width="10%" align="left">None</td>
						<td width="5%" align="left">0</td>
						<td align="left">
							<xsl:value-of select="$path/dnp:namespace"/>
						</td>
					</tr>
					<tr>
						<td width="4%" align="center">3</td>
						<td width="10%" align="left">Name</td>
						<td width="30%" align="left">Prototype name</td>
						<td width="10%" align="left">None</td>
						<td width="5%" align="left">0</td>
						<td align="left">
							<xsl:value-of select="$path/dnp:name"/>
						</td>
					</tr>
					<xsl:for-each select="$path/dnp:dataElement">
						<xsl:call-template name="dspe">
							<xsl:with-param name="elementNumber" select="position() + 3"/>
						</xsl:call-template>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<xsl:for-each select="$path/dnp:dataElement">
						<xsl:call-template name="dspe">
							<xsl:with-param name="elementNumber" select="position() + 1"/>
						</xsl:call-template>
					</xsl:for-each>
				</xsl:otherwise>
			</xsl:choose>
		</table>
	</xsl:template>

	<!--  DATA SET DESCRIPTORS -->
	<xsl:template name="datasetDescriptorList">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<xsl:if test="$path/dnp:configuration/dnp:descriptorDefinition">
				<tr bgcolor="white">
					<td width="50%" valign="top" bgcolor="yellow"><b>5.11. Definition of Data Set Descriptors:</b><p/>
						<i>
							List of all data set descriptors. The following table is repeated for each Data Set Descriptor defined.
							<p/>Note: the number of data sets known to the device are available remotely using object Group 0 
							Variations 214 and 215.
						</i>
					</td>
					<td width="50%" valign="top">
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:descriptorDefinition/dnp:capabilities/dnp:fixed">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Fixed, list shown in table below<br/>
						<xsl:choose>
							<xsl:when test="$path/dnp:configuration/dnp:descriptorDefinition/dnp:capabilities/dnp:configurable">
								<input type="checkbox" name="box" checked=""/>
							</xsl:when>
							<xsl:otherwise>
								<input type="checkbox" name="box"/>
							</xsl:otherwise>
						</xsl:choose>
						<label for="box"/>&#x20;Configurable (current list may be shown in table below)<br/>
						<xsl:call-template name="tickbox-explain">
							<xsl:with-param name="path" select="$path/dnp:configuration/dnp:descriptorDefinition/dnp:capabilities/dnp:other"/>
							<xsl:with-param name="prompt" select="'Other, explain:'"/>
						</xsl:call-template>
					</td>
				</tr>
			</xsl:if>
		</table>
		<xsl:for-each select="$path/dnp:descriptor">
			<xsl:call-template name="dsdList">
				<xsl:with-param name="posn" select="position()"/>
				<xsl:with-param name="path" select="."/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>
	
	<xsl:template name="dsdList">
		<xsl:param name="posn"/>
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<caption align="left">
			Data Set Description: <xsl:value-of select="$path/dnp:description"/></caption>
			<tr bgcolor="Khaki">
				<td width="50%" valign="top">5.11.<xsl:value-of select="($posn -1)*3 + 1"/>. Data Set Properties:</td>
				<td width="50%" valign="top">
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:properties/dnp:readable"/>
						<xsl:with-param name="prompt" select="'Readable'"/>
					</xsl:call-template>
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:properties/dnp:writeable"/>
						<xsl:with-param name="prompt" select="'Writable'"/>
					</xsl:call-template>
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:properties/dnp:outstationMaintainsStaticData"/>
						<xsl:with-param name="prompt" select="'Outstation maintains a static data set'"/>
					</xsl:call-template>
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:properties/dnp:outstationGeneratesEventData"/>
						<xsl:with-param name="prompt" select="'Outstation generates a data set event'"/>
					</xsl:call-template>
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:properties/dnp:datasetDefinedByMaster"/>
						<xsl:with-param name="prompt" select="'Data set defined by master'"/>
					</xsl:call-template>
					<xsl:call-template name="showNotesGeneral">
						<xsl:with-param name="path" select="$path/dnp:properties"/>
					</xsl:call-template>
				</td>
			</tr>
			<tr bgcolor="Khaki">
				<td width="50%" valign="top">5.11.<xsl:value-of select="($posn -1)*3 + 2"/>. Event Class Assigned :</td>
				<td width="50%" valign="top">
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:changeEventClass/dnp:one"/>
						<xsl:with-param name="prompt" select="'One'"/>
					</xsl:call-template>
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:changeEventClass/dnp:two"/>
						<xsl:with-param name="prompt" select="'Two'"/>
					</xsl:call-template>
					<xsl:call-template name="tickbox">
						<xsl:with-param name="path" select="$path/dnp:changeEventClass/dnp:three"/>
						<xsl:with-param name="prompt" select="'Three'"/>
					</xsl:call-template>
					<xsl:call-template name="showNotesGeneral">
						<xsl:with-param name="path" select="$path/dnp:changeEventClass"/>
					</xsl:call-template>
				</td>
			</tr>
			<tr bgcolor="Khaki">
				<td width="50%" valign="top">5.11.<xsl:value-of select="($posn -1)*3 + 3"/>. Static Data Set included in Class 0 response:</td>
				<td width="50%" valign="top">
					<xsl:choose>
						<xsl:when test="$path/dnp:class0ResponseMode/dnp:always">
							<input type="checkbox" name="box" checked=""/>
						</xsl:when>
						<xsl:otherwise>
							<input type="checkbox" name="box"/>
						</xsl:otherwise>
					</xsl:choose>
					<label for="box"/>&#x20;Always<br/>
					<xsl:choose>
						<xsl:when test="$path/dnp:class0ResponseMode/dnp:never">
							<input type="checkbox" name="box" checked=""/>
						</xsl:when>
						<xsl:otherwise>
							<input type="checkbox" name="box"/>
						</xsl:otherwise>
					</xsl:choose>
					<label for="box"/>&#x20;Never<br/>
					<xsl:choose>
						<xsl:when test="$path/dnp:class0ResponseMode/dnp:onlyWhenAssignedToClass123">
							<input type="checkbox" name="box" checked=""/>
						</xsl:when>
						<xsl:otherwise>
							<input type="checkbox" name="box"/>
						</xsl:otherwise>
					</xsl:choose>
					<label for="box"/>&#x20;Only if point is assigned to Class 1, 2, or 3<br/>
					<xsl:call-template name="showNotesGeneral">
						<xsl:with-param name="path" select="$path/dnp:class0ResponseMode"/>
					</xsl:call-template>
				</td>
			</tr>
		</table>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<tr bgcolor="lightyellow">
				<td width="4%" align="center" style="writing-mode:tb-rl">Element Number</td>
				<td width="10%" align="left">Descriptor Code</td>
				<td width="30%" align="left">Element Description</td>
				<td width="10%" align="left">Data Type Code</td>
				<td width="5%" align="left">Max Data Length</td>
				<td align="left">Ancillary Value</td>
			</tr>
			<tr>
				<td width="4%" align="center">0</td>
				<td width="10%" align="left">ID (identifier)</td>
				<td width="30%" align="left">Mandatory DS identifier</td>
				<td width="10%" align="left">None</td>
				<td width="5%" align="left">0</td>
				<td align="left">
					<xsl:value-of select="$path/dnp:id"/>
				</td>
			</tr>
			<xsl:choose>
				<xsl:when test="dnp:datasetElements/dnp:name">
					<tr>
						<td width="4%" align="center">1</td>
						<td width="10%" align="left">Name</td>
						<td width="30%" align="left">DS name</td>
						<td width="10%" align="left">None</td>
						<td width="5%" align="left">0</td>
						<td align="left">
							<xsl:value-of select="$path/dnp:datasetElements/dnp:name"/>
						</td>
					</tr>
					<xsl:for-each select="$path/dnp:datasetElements/dnp:dataSetElement">
						<xsl:call-template name="dse">
							<xsl:with-param name="elementNumber" select="position() + 1"/>
						</xsl:call-template>
					</xsl:for-each>
				</xsl:when>
				<xsl:otherwise>
					<xsl:for-each select="$path/dnp:datasetElements/dnp:dataSetElement">
						<xsl:call-template name="dse">
							<xsl:with-param name="elementNumber" select="position()"/>
						</xsl:call-template>
					</xsl:for-each>
				</xsl:otherwise>
			</xsl:choose>
		</table>
	</xsl:template>
	
	<xsl:template name="dsdPointIndexAttributes">
		<xsl:param name="path"/>
		<p/>
		<table border="1" align="center" width="90%" cellspacing="0" cellpadding="3">
			<xsl:if test="$path/dnp:configuration/dnp:descriptorDefinition">
				<tr bgcolor="white">
					<td width="100%" valign="top" bgcolor="yellow"><b>5.12. Data Set Descriptors - Point Index Attributes</b><p/>
						<p/><i> The following table is optional and correlates data set elements to point indexes of
						standard DNP3 Data Objects. The element number below refers to the position in the present value
						object (object 87) or event (object 88) data set and will not match the element number in the 
						data set descriptor or data set prototype tables above.							
						</i>
					</td>
				</tr>
			</xsl:if>
		</table>
		<xsl:for-each select="$path/dnp:descriptor">
			<xsl:call-template name="dsdPointIndexList">
				<xsl:with-param name="posn" select="position()"/>
				<xsl:with-param name="path" select="."/>
			</xsl:call-template>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="dsdPointIndexList">
		<xsl:param name="path"/>
		<xsl:param name="posn"/>
		<xsl:if test="$path/dnp:pointIndexAttribute">
			<p/><table border="1" align="center" width="50%" cellspacing="0" cellpadding="3">
				<caption align="left">Data Set Description: <xsl:value-of select="$path/dnp:description"/></caption>
				<tr bgcolor="lightyellow">
					<td width="40%" align="center">Element Number</td>
					<td width="30%" align="center">DNP3 Group Number</td>
					<td width="30%" colspan="4" align="left">Point Index</td>
				</tr>
				<xsl:for-each select="$path/dnp:pointIndexAttribute">
					<tr>
						<td width="40%" align="center">
							<xsl:value-of select="position()-1"/>&#160;
						</td>
						<td width="30%" align="center">
							<xsl:value-of select="./dnp:groupNumber"/>&#160;
						</td>
						<td width="30%" align="center">
							<xsl:value-of select="./dnp:pointIndex"/>&#160;
						</td>
					</tr>
				</xsl:for-each>
			</table>
		</xsl:if>
	</xsl:template>
	
	<!-- template to display a Dataset element, with a particular element number -->
	<xsl:template name="dse">
		<xsl:param name="elementNumber"/>
		<tr>
			<td width="4%" align="center">
				<xsl:value-of select="$elementNumber"/>
			</td>
			<xsl:if test="./dnp:dataElement">
				<td width="10%" align="left">DAEL</td>
				<td width="30%" align="left">Data Element</td>
				<td width="10%" align="left">
					<xsl:value-of select="./dnp:dataElement/dnp:dataType"/>&#160;
				</td>
				<td width="5%" align="left">
					<xsl:value-of select="./dnp:dataElement/dnp:maxDataLength"/>&#160;
				</td>
				<td align="left">
					<xsl:value-of select="./dnp:dataElement/dnp:elementName"/>&#160;
				</td>
			</xsl:if>
			<xsl:if test="./dnp:prototypeElement">
				<td width="10%" align="left">PTYP</td>
				<td width="30%" align="left">Prototype Element</td>
				<td width="10%" align="left">None</td>
				<td width="5%" align="left">0</td>
				<td align="left">
					<xsl:value-of select="./dnp:prototypeElement/dnp:uuid"/>&#160;
				</td>
			</xsl:if>
		</tr>
	</xsl:template>


	
	<!-- template to put out a Data Set Prototype data element with a specified element number -->
	<xsl:template name="dspe">
		<xsl:param name="elementNumber"/>
		<tr>
			<td width="4%" align="center">
				<xsl:value-of select="$elementNumber"/>
			</td>
			<td width="10%" align="left">
				<xsl:if test="./dnp:elemTypeCode = 'dael'">DAEL</xsl:if>
				<xsl:if test="./dnp:elemTypeCode = 'ctls'">CTLS</xsl:if>
				<xsl:if test="./dnp:elemTypeCode = 'ctlv'">CTLV</xsl:if>
			</td>
			<td width="30%" align="left">Data Element</td>
			<td width="10%" align="left">
				<xsl:value-of select="./dnp:dataType"/>
			</td>
			<td width="5%" align="left">
				<xsl:value-of select="./dnp:maxDataLength"/>&#160;
			</td>
			<td align="left">
				<xsl:value-of select="./dnp:elementName"/>&#160;
			</td>
		</tr>
	</xsl:template>

	
	<!-- template to put out an entry for the points list definition -->
	<xsl:template name="pointListDefinition">
		<xsl:param name="path"/>
		<td width="50%" valign="top">
			<xsl:choose>
				<xsl:when test="$path/dnp:fixed">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Fixed, list shown in table below<br/>
			<xsl:choose>
				<xsl:when test="$path/dnp:configurable">
					<input type="checkbox" name="box" checked=""/>
				</xsl:when>
				<xsl:otherwise>
					<input type="checkbox" name="box"/>
				</xsl:otherwise>
			</xsl:choose>
			<label for="box"/>&#x20;Configurable (current list may be shown in table below)<br/>
			<xsl:call-template name="tickbox-explain">
				<xsl:with-param name="path" select="$path/dnp:other"/>
				<xsl:with-param name="prompt" select="'Other, explain:'"/>
			</xsl:call-template>
			<xsl:call-template name="showNotesGeneral">
				<xsl:with-param name="path" select="$path"/>
			</xsl:call-template>
		</td>
	</xsl:template>

	
	
</xsl:stylesheet>
