using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Collections;
using System.Reflection;
using System.Xml.Serialization;


namespace CSToCppXMLBinder
{
  class CppBinder
  {
    private class TypeSnippet
    {
      public TypeSnippet(Type t)
      {
        type = t;
      }
      public string declaration = "";
      public string definition = "";
      public Type type;
      public ArrayList dependencies = new ArrayList();
    }
    private class ClassSnippet : TypeSnippet
    {
      public ClassSnippet(Type t)
        : base(t)
      {
      }
      public string constructor = "";
      public string toXml = "";
      public string fromXml = "";
      public string classDeclaration = "";
      public string needsChildren = "";
      public string declarations = "";
      public string className = "";
    }
    private class EnumSnippet : TypeSnippet
    {
      public EnumSnippet(Type t)
        : base(t)
      {

      }
    }
    private class FileLevelInfo
    {
      public FileLevelInfo(string nameSpace)
      {
        nSpace = nameSpace;
        knownSimpleTypes.Add("int", "int");
        knownSimpleTypes.Add("float", "float");
        knownSimpleTypes.Add("double", "double");
        knownSimpleTypes.Add("bool", "bool");
        knownSimpleTypes.Add("string", "string");
      }
      public string nSpace;
      public Hashtable includes = new Hashtable();
      public Hashtable collections = new Hashtable();
      public Hashtable enumNames = new Hashtable();
      public Hashtable typesToBind = new Hashtable();
      public Hashtable knownSimpleTypes = new Hashtable();
      public Hashtable unKnownSimpleTypes = new Hashtable();
      public Hashtable enumValues = new Hashtable();
    }

    public static void BindAssembly(string fileDir, string nSpace, Assembly assembly, string xml_support_dir)
    {
      StreamWriter headerFile = null;
      StreamWriter cppFile = null;
      string fileName = fileDir + "/" + getCppNamespace(nSpace);
      try
      {
        FileLevelInfo info = BuildCppFileInfo(nSpace, assembly);
        headerFile = new StreamWriter(fileName + ".h");
        cppFile = new StreamWriter(fileName + ".cpp");
        CreateOutputFiles(headerFile, cppFile, info, xml_support_dir);
      } // try
      catch (Exception ex)
      {
        Console.WriteLine(ex.StackTrace);
        string strErr = String.Format("Error '{0}'", ex.Message);
        Console.WriteLine(strErr);
      }
      finally
      {
        if (cppFile != null) cppFile.Close();
        cppFile = null;
        if (headerFile != null) headerFile.Close();
        headerFile = null;
      } // finally
    }
    private static FileLevelInfo BuildCppFileInfo(string nSpace, Assembly assembly)
    {

      FileLevelInfo info = new FileLevelInfo(getCppNamespace(nSpace));

      GetTypesToBind(info, assembly);

      foreach (TypeSnippet ts in info.typesToBind.Values)
      {
        if (ts is EnumSnippet)
        {
          BuildEnumSnippets(info, (EnumSnippet)ts);
        }
        else
        {
          //we don't need bind "Collected" types since they are templated in the c++ code.
          BuildClassSnippets(info, (ClassSnippet)ts);
        }
      }

      return info;

    }

    private static void GetTypesToBind(FileLevelInfo info, Assembly ass)
    {
      Hashtable typesToBind = new Hashtable();
      Type[] ts = ass.GetTypes();
      for (int i = 0; i < ts.Length; i++)
      {
        Type t = ts[i];
        if (!t.IsSerializable || getCppNamespace(t.Namespace) != info.nSpace)
        {
          continue;
        }

        if (t.IsEnum)
        {
          string enumName = t.Name + "Enum";
          info.enumNames.Add(t.Name, enumName);
          info.typesToBind.Add(t, new EnumSnippet(t));
        }
        else if (t.BaseType == typeof(ArrayList))
        {
          info.collections.Add(t.Name.Substring(0, t.Name.Length - "Collection".Length), t.Name);
        }
        else
        {
          info.typesToBind.Add(t, new ClassSnippet(t));
        }

      }
    }

      private static void CreateOutputFiles(StreamWriter header, StreamWriter cpp, FileLevelInfo info, string xml_support_dir)
    {
      string nSpace = info.nSpace;
      string commentText = "/* ******************************\n\tGENERATED CONTENT DO NOT ALTER!\n*********************************\n*/";
      string nameSpaceText = "using namespace std;\nnamespace " + nSpace + " {";
      cpp.WriteLine(commentText);
      cpp.WriteLine("#include \"" + nSpace + ".h\"");
      cpp.WriteLine(nameSpaceText);

      header.WriteLine(commentText);
      header.WriteLine("#ifndef _" + nSpace.ToUpper().Replace('-', '_') + "_H_");
      header.WriteLine("#define _" + nSpace.ToUpper().Replace('-', '_') + "_H_");

      header.WriteLine("#include <APLXML/tinybinding.h>");

      foreach (string include in info.includes.Keys)
      {
          Console.WriteLine("include: " + include);
          string dir = xml_support_dir;
          Console.WriteLine("include: " + include + " -> " + dir);
          header.WriteLine("#include <"+dir+"/" + include + ".h>");
      }

      header.WriteLine(nameSpaceText);

      ArrayList orderOfParts = GetDeclarationOrder(info.typesToBind);

      foreach (Type t in orderOfParts)
      {
        TypeSnippet ts = (TypeSnippet)info.typesToBind[t];
        header.WriteLine(ts.declaration);
        cpp.WriteLine(ts.definition);
        info.typesToBind.Remove(t);
      }
      foreach (TypeSnippet ts in info.typesToBind.Values)
      {
        Console.Error.WriteLine(ts.type.Name + " has missing dependencies:");
        foreach (string s in ts.dependencies)
        {
          Console.Error.WriteLine("\t" + s);
        }
        Console.Error.WriteLine(ts.declaration);
        //Console.Error.WriteLine(ts.definition);
      }
      foreach (string s in info.unKnownSimpleTypes.Keys)
      {
        Console.Error.WriteLine("UnKnown type: " + s);
      }
      header.WriteLine("}\n#endif\n");
      cpp.WriteLine("}");
    }
    private class AscendingNameSorter : IComparer
    {
      public int Compare(Object x, Object y)
      {
        TypeSnippet p1 = (TypeSnippet)x;
        IComparable ic1 = (IComparable)p1.type.Name;

        TypeSnippet p2 = (TypeSnippet)y;
        IComparable ic2 = (IComparable)p2.type.Name;

        return ic1.CompareTo(ic2);
      }
    }
    private static ArrayList GetDeclarationOrder(Hashtable typesToBind)
    {
      ArrayList orderOfParts = new ArrayList();

      bool keepGoing = true;
      bool oneRemoved;

      //this is a bit of a hack to keep the order of the fields
      //from changing so we can track changes in SVN better.
      //if we use a hashtable it can return the values in a 
      //randomish order!
      ArrayList list = new ArrayList() ;
      foreach (TypeSnippet ts in typesToBind.Values)
      {
        list.Add(ts);
      }
      list.Sort(new AscendingNameSorter());

      while (keepGoing)
      {
        keepGoing = false;
        oneRemoved = false;
        foreach (TypeSnippet ts in list)
        {
          if (ts.dependencies == null) continue;
          if (ts.dependencies.Count == 0)
          {
            removeEntries(list, ts.type.Name);
            orderOfParts.Add(ts.type);
            ts.dependencies = null;
            oneRemoved = true;
          }
          else
          {
            keepGoing = true;
          }
        }
        if (keepGoing && !oneRemoved)
        {
          Console.Error.WriteLine("Couldn't resolve all dependencies!!");

          break;
        }
      }
      return orderOfParts;
    }
    private static string getCppNamespace(string nSpace)
    {
      nSpace = nSpace.Replace(".", "_");
      nSpace = nSpace.Replace("-", "_");
      return nSpace;
    }
    private static void removeEntries(ArrayList typesToBindList, string type)
    {
      foreach (TypeSnippet ts in typesToBindList)
      {
        if (ts.dependencies == null) continue;
        if (ts.dependencies.Count > 0)
        {
          ArrayList deps = ts.dependencies;
          for (int j = deps.Count - 1; j >= 0; j--)
          {
            if (deps[j].Equals(type))
            {
              deps.RemoveAt(j);
            }
          }
        }
      }
    }

    private static void BuildClassSnippets(FileLevelInfo info, ClassSnippet snip)
    {
      Type t = snip.type;
      FieldInfo[] fi = t.GetFields();
      snip.className = t.Name + "_t";
      snip.classDeclaration = "class " + snip.className + " : ";
      //set the base class
      string baseClass = null;
      if (t.BaseType != typeof(System.Object))
      {
          string nspace = getCppNamespace(t.BaseType.Namespace);
          if (t.BaseType.Namespace != t.Namespace)
          {
              if (!info.includes.ContainsKey(nspace)) info.includes.Add(nspace, t.BaseType.FullName);
          }
          else snip.dependencies.Add(t.BaseType.Name);
          baseClass = nspace + "::" + t.BaseType.Name + "_t";
          snip.classDeclaration += "public " + baseClass;
      }
      else
      {
        snip.classDeclaration += "public IXMLDataBound ";
      }
      snip.classDeclaration += "{\npublic:\n\tvoid toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid);\n\tvoid fromXml(TiXmlNode* pNode);\n";
      snip.constructor = null;
      snip.declarations = "";
      snip.fromXml = "void " + snip.className + " :: fromXml(TiXmlNode* pNode){\n";
      snip.toXml = "";
      snip.needsChildren = null;
      snip.fromXml += "\tif(pNode == NULL)return;\n";
      snip.fromXml += "\tXML_CHECK(\""+t.Name+"\",pNode->Type() == TiXmlNode::ELEMENT);\n";
      snip.fromXml += "\tTiXmlElement* pEm = pNode->ToElement();\n";
      snip.fromXml += "\tXML_CHECK(\"" + t.Name + "\",pEm != 0);\n";
      if (baseClass != null)
      {
        snip.fromXml += "\tthis->" + baseClass + "::fromXml(pNode);\n";
      }
      foreach (FieldInfo f in fi)
      {
        if (f.DeclaringType != t)
        {
          //Console.WriteLine("Skipped " + f.Name + " in " + t.ToString() + " since its allready defined in: " + f.DeclaringType.ToString());
          continue;
        }
        object[] atts = f.GetCustomAttributes(true);

        foreach (Attribute a in atts)
        {

          if (a.GetType() == typeof(XmlAttributeAttribute))
          {
            AddXMLAttributte(info, (XmlAttributeAttribute)a, snip, f);

          }
          if (a.GetType() == typeof(XmlElementAttribute))
          {
            AddXMLElement(info, (XmlElementAttribute)a, snip, f);
          }
        }
      }
      if (snip.constructor == null) snip.constructor = "";
      else
      {
        //when we have had to construct 
        snip.constructor += "{};\n";
        snip.classDeclaration += "\t" + snip.className + "();\n\n";
      }
      snip.fromXml += "\tvalid=true;\n};\n";
      string tempToXml = snip.toXml;
      snip.toXml = "void " + snip.className + " :: toXml(TiXmlNode* pParent, bool aCreateNode, bool aIgnoreValid){\n";
      if (snip.needsChildren != null)
      {
          snip.toXml += "\tif(" + snip.needsChildren + ")return;\n";
      }

      snip.toXml += "\tif(!aIgnoreValid && !valid) return;\n";
      
      snip.toXml += "\tTiXmlElement * pEm;\n\tif(aCreateNode){\n\t\tpEm = new TiXmlElement(\"" + t.Name + "\");\n";
      snip.toXml += "\t\tpParent->LinkEndChild(pEm);\n\t}else{\n\t\tpEm = pParent->ToElement();\n\t}\n";
      if (baseClass != null)
      {
         snip.toXml += "\tthis->" + baseClass + "::toXml(pEm, false, aIgnoreValid);\n";
      }
      snip.toXml += tempToXml;
      snip.toXml += "};\n";
      snip.declaration = snip.classDeclaration + snip.declarations + "};";
      snip.definition = snip.constructor + snip.fromXml + snip.toXml;
    }

    //when we add an element it either needs to be 
    private static void AddXMLElement(FileLevelInfo info, XmlElementAttribute xea, ClassSnippet snip, FieldInfo finfo)
    {
      if (xea.Type == null)
      {
        AddSimpleXMLElement(info, xea, snip, finfo);
        return;
      }
      string typeName = xea.Type.ToString();
      string[] ss = typeName.Split(new char[] { '.' });
      typeName = ss[ss.Length - 1];
      string typesuffix = "_t";
      string fromNamespace = getCppNamespace(xea.Namespace);
      string fullName;
      string swigFullName;
      string ename = xea.ElementName;
      if (xea.Type.Namespace == "System")
      {
        if (!info.knownSimpleTypes.ContainsKey(typeName.ToLower()))
        {
          throw new Exception("Unknown system array type : " + typeName);
        }
        fromNamespace = info.nSpace;
        typesuffix = "";
        typeName = (string)info.knownSimpleTypes[typeName.ToLower()];
        fullName = typeName;
        swigFullName = info.nSpace + "::" + typeName;
        if (!info.collections.ContainsKey(typeName)) info.collections.Add(typeName, typeName);
      }
      else if (fromNamespace == info.nSpace)
      {
        if (typeName != snip.type.Name)
        {
          //don't make a class dependent on itself.
          snip.dependencies.Add(typeName);
        }
        fullName = typeName;
        swigFullName = info.nSpace +"::"+typeName;
      }
      else
      {
        fullName = fromNamespace + "::" + typeName;
        swigFullName = fullName;
        if (!info.includes.ContainsKey(fromNamespace)) info.includes.Add(fromNamespace, fullName);
      }
      bool isCollection = false;
      if (info.collections.ContainsKey(typeName))
      {
        snip.declarations += "#ifdef SWIG\n%immutable " + ename + "Vector;\n#endif\n";


        snip.classDeclaration = "#ifdef SWIG\n}\n%template(" + ename + "_c) std::vector<" + swigFullName + typesuffix + "*>;\nnamespace " + info.nSpace + "{\n#endif\n" + snip.classDeclaration;
        snip.declarations += "\tprivate: collectedType < " + fullName + typesuffix+" > " + ename + ";\n";
        snip.declarations += "\tpublic: vector < " + fullName + typesuffix+"* >& "+ename+"Vector;\n";
        if (snip.constructor == null)
        {
          snip.constructor = snip.className + "::" + snip.className + "():\n\t\t";
        }
        else
        {
          snip.constructor += ",\n\t\t";
        }
        snip.constructor += ename + "(\"" + typeName + "\"), "+ename+"Vector(" + ename + ".collection)";

        if (snip.needsChildren != null) snip.needsChildren += " && ";
        snip.needsChildren += ename + ".size() == 0";
        
        isCollection = true;
      }
      else
      {
        snip.declarations += "#ifdef SWIG\n%immutable " + ename + ";\n#endif\n";
        snip.declarations += "\t" + fullName + typesuffix +" " + ename + ";\n";
      }

      snip.toXml += "\t" + ename + ".toXml(pEm, true, aIgnoreValid);\n";
      string nodeName = "pNode->FirstChildElement(\"" + ename + "\")";
      if (ename == snip.type.Name || isCollection) nodeName = "pNode";
      snip.fromXml += "\t" + ename + ".fromXml(" + nodeName + ");\n";
    }

    private static void AddSimpleXMLElement(FileLevelInfo info, XmlElementAttribute xea, ClassSnippet snip, FieldInfo finfo)
    {
      string fieldName = xea.ElementName;
      string dtype = SetupSimpleType(info, snip, xea.DataType, fieldName, finfo.FieldType.Name);

      string nodeGetter = "pNode->FirstChildElement(\"" + fieldName + "\")";

      snip.fromXml += "\t" + fieldName + " = FromString_" + dtype + "(pNode, GetValue(\"" + fieldName + "\", pNode->FirstChildElement(\"" + fieldName + "\"), pNode));\n";
      snip.toXml += "\tTiXmlElement* n" + fieldName + " = new TiXmlElement(\"" + fieldName+ "\");\n";
      snip.toXml += "\tTiXmlText* nt" + fieldName + " = new TiXmlText(ToString_" + dtype + "(" + fieldName + "));\n";
      snip.toXml += "\tn" + fieldName + "->LinkEndChild(nt"+fieldName+");\n";
      snip.toXml += "\tpEm->LinkEndChild(n" + fieldName + ");\n";

      snip.declarations += "\t" + dtype + " " + fieldName + ";\n";

    }
    //attributes have to be simple types, either enum, string, int, double, bool
    private static void AddXMLAttributte(FileLevelInfo info, XmlAttributeAttribute xa, ClassSnippet snip, FieldInfo finfo)
    {
      string fieldName = xa.AttributeName;
      string dtype = SetupSimpleType(info, snip, xa.DataType, fieldName, finfo.FieldType.Name);

      snip.fromXml += "\t"+fieldName+" = FromString_" + dtype +"(pEm, pEm->Attribute(\"" + fieldName + "\"));\n";
      snip.toXml += "\tpEm->SetAttribute(\"" + fieldName + "\", ToString_" + dtype + "(" + fieldName + "));\n";
  
      snip.declarations += "\t" + dtype + " " + fieldName + ";\n";
    }

    private static string SetupSimpleType(FileLevelInfo info, ClassSnippet snip, string dtype, string fieldName, string fieldType)
    {
      if (dtype == null || dtype.Length == 0)
      {
        if (fieldType != null && info.enumNames.ContainsKey(fieldType))
        {
          fieldName = fieldType;
        }
        if (info.enumNames.ContainsKey(fieldName))
        {
          dtype = (string)info.enumNames[fieldName];
          snip.dependencies.Add(fieldName);
          return dtype;
        }
        else
        {
          throw new Exception("Bad simple type: " + fieldName + " : " + dtype);
        }
      }
      if (dtype == "integer") dtype = "int";
      if (dtype == "boolean") dtype = "bool";

      if (!info.knownSimpleTypes.ContainsKey(dtype))
      {
        if(!info.unKnownSimpleTypes.ContainsKey(dtype)) info.unKnownSimpleTypes.Add(dtype,dtype);
        dtype = "string";
      }
      return dtype;
    }

    private static void BuildEnumSnippets(FileLevelInfo info, EnumSnippet snip)
    {
      string[] s = Enum.GetNames(snip.type);
      string enumName = (string)info.enumNames[snip.type.Name];

      string enumPrefix = "";
      bool needsUnderScore = true;
      bool needsPrefix = false;
      for(int i=0; i <s.Length; i++)
      {
          string n = s[i];
        if (info.enumValues.ContainsKey(n))
        {
          needsPrefix = true;
        }
        else
        {
          info.enumValues.Add(n, snip.type);
        }

        if (n.StartsWith("_"))
        {
            int result;
            if (Int32.TryParse(n.Substring(1), out result))
            {
                needsPrefix = true;  
            }
            s[i] = n.Substring(1);
        }
      }
      if (needsPrefix)
      {
        enumPrefix = snip.type.Name.ToUpper();
        if (needsUnderScore) enumPrefix += "_";
      }
      string enumDef = "";
      string enumDeclarataion = "enum " + enumName + " {\n";
      foreach (string n in s) enumDeclarataion += "\t" + enumPrefix + n + ",\n";
      enumDeclarataion += "};\n";


      string definition = enumName + " FromString_" + enumName + "(TiXmlNode* apParent, const char* aValue)";
      enumDeclarataion += "\t" + definition + ";\n";

      enumDef += definition + " {\n";
      foreach (string n in s) enumDef += "\tif(aValue != NULL && strcmp(aValue,\"" + n + "\") == 0) return " + enumPrefix + n + ";\n";
      enumDef += "\tstd::ostringstream oss;\n\toss << \"String \" << (aValue==NULL?\"null\":aValue) << \" isnt valid for " + enumName;
      enumDef += " at \" << apParent->Row() << \":\" << apParent->Column();";
      enumDef += "\n\tif(IXMLDataBound::msExceptOnFailure){throw apl::Exception(LOCATION, oss.str());}";
      enumDef += "\n\telse{std::cout << oss.str() << std::endl; return "+enumPrefix + s[0]+";}\n";
      enumDef += "};\n";

      definition = "string ToString_" + enumName + "(" + enumName + " aValue)";
      enumDeclarataion += "\t" + definition + ";\n";
      enumDef += definition + " {\n";
      enumDef += "\tswitch(aValue){\n";
      foreach (string n in s) enumDef += "\tcase " + enumPrefix + n + ": return \"" + n + "\";\n";
      enumDef += "\t}\n\tstd::ostringstream oss;\n\toss << \"Bad Enum value \" << aValue << \" for type "+enumName+"\";";
      enumDef += "\n\tthrow apl::Exception(LOCATION, oss.str());\n";
      enumDef += "};\n";
      snip.declaration = enumDeclarataion;
      snip.definition = enumDef;
    }

  }


}
