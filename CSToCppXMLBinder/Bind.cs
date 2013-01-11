
using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Xml.Serialization;
using System.Collections.Specialized;

namespace CSToCppXMLBinder
{
  class Bind
  {
    static void Main(string[] args)
    {
      if (args.Length != 4)
      {
        Console.Error.WriteLine("Command line should be:\n\t\tBind.exe [NameSpace] [Assembly] [output]\n\nEx: Bind PSiXML.Common Bindings.netmodule mappings.ini");
        return;
      }
      try
      {
        string xml_support_dir = args[3];
        
        Assembly asm = Assembly.LoadFrom(Environment.CurrentDirectory+"\\"+args[1]);
        CppBinder.BindAssembly(args[2], args[0], asm, xml_support_dir);
        Console.WriteLine("Success!");
      }
      catch (Exception e)
      {
        Console.Error.WriteLine(e.ToString());
      }
    }
  }
}
