%TOOLS_HOME%\XSDObjectGenerator\xsdobjectgen.exe "%CD%\SlaveTestSet.xsd" /d "/y:APLXML.STS|APLXML.DNP|APLXML.Base" /n:ignored "/z:%CD%\APLXML.STS.cs|%CD%\APLXML.DNP.cs|%CD%\APLXML.Base.cs"
%TOOLS_HOME%\XSDObjectGenerator\xsdobjectgen.exe "%CD%\MasterTestSet.xsd" /d "/y:APLXML.DNP|APLXML.Base|APLXML.MTS" /n:ignored "/z:%CD%\APLXML.DNP.cs|%CD%\APLXML.Base.cs|%CD%\APLXML.MTS.cs"
%TOOLS_HOME%\XSDObjectGenerator\xsdobjectgen.exe "%CD%\DNP.xsd" /d /y:"APLXML.DNP|APLXML.Base" /n:ignored "/z:%CD%\APLXML.DNP.cs|%CD%\APLXML.Base.cs"
%TOOLS_HOME%\XSDObjectGenerator\xsdobjectgen.exe "%CD%\APL.xsd" /d /n:"APLXML.Base"

call "%VS80COMNTOOLS%vsvars32.bat"
csc /t:module /out:Bindings.netmodule  APLXML.DNP.cs APLXML.Base.cs APLXML.MTS.cs APLXML.STS.cs
al /t:lib /out:Bindings.assembly Bindings.netmodule
CSToCppXMLBinder.exe APLXML.Base Bindings.assembly ../XMLBindings XMLBindings
CSToCppXMLBinder.exe APLXML.DNP Bindings.assembly ../XMLBindings XMLBindings
CSToCppXMLBinder.exe APLXML.MTS Bindings.assembly ../XMLBindings XMLBindings
CSToCppXMLBinder.exe APLXML.STS Bindings.assembly ../XMLBindings XMLBindings


del Bindings.assembly
del Bindings.netmodule
pause
