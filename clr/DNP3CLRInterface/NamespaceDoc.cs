using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    ///   <para>
    ///     Public API of the opendnp3 CLR bindings.
    ///     
    ///     The DNP3.Adapter namespace contains 1 exposed factory class for obtaining a concrete implementation of IDNP3Manager.
    ///     
    ///     <code>
    /// using DNP3.Adapter;
    /// using DNP3.Interface;
    /// 
    /// class Program
    /// {
    ///    static void Main(string[] args)
    ///    {
    ///        //obtain an IDNP3Manager by using the factory class from the DNP3.Adapter namespace
    ///        IDNP3Manager mgr = DNP3ManagerFactory.CreateManager();
    ///        
    ///        // proceed to add channels and stacks .... checkout the wiki for details
    ///    }
    /// }
    ///     </code>
    ///   </para>
    /// </summary>        
    internal class NamespaceDoc
    {
    }
}
