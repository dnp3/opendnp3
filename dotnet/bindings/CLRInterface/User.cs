using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Represents a secure authentication user
    /// </summary>
    public class User
    {
        readonly UInt16 number;

        User(UInt16 number)
        {
            this.number = number;
        }

        public UInt16 Number
        {
            get
            {
                return number;
            }
        }

        public static User FromNumber(UInt16 number)
        {
            return new User(number);
        }

        public static User Default
        {
            get {
                return new User(1);
            }
        }


    }
}
