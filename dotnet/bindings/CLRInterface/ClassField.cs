using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DNP3.Interface
{
    /// <summary>
    /// Structure that records which events are scanned / evented
    /// </summary>
    public struct ClassField
    {      
        public ClassField(byte mask)
        {
            this.bitfield =  mask;
        }

        public static ClassField From(params PointClass[] classes)
        {
            byte mask = 0;
            foreach(var pc in classes)
            {
                mask |= (byte)pc;
            }
            return new ClassField(mask);
        }

        public static ClassField AllClasses
        {
            get
            {
                return ClassField.From(PointClass.Class0, PointClass.Class1, PointClass.Class2, PointClass.Class3);
            }
        }

        public static ClassField AllEventClasses
        {
            get
            {
                return ClassField.From(PointClass.Class1, PointClass.Class2, PointClass.Class3);
            }
        }

        public byte ClassMask
        {
            get
            {
                return bitfield;
            }
        }

        public  bool HasClass0
        {
            get
            {
                return (((byte)PointClass.Class0) & bitfield) != 0;
            }
        }

        public bool HasClass1
        {
            get
            {
                return (((byte)PointClass.Class1) & bitfield) != 0;
            }
        }

        public bool HasClass2
        {
            get
            {
                return (((byte)PointClass.Class2) & bitfield) != 0;
            }
        }

        public bool HasClass3
        {
            get
            {
                return (((byte)PointClass.Class3) & bitfield) != 0;
            }
        }

        private byte bitfield;
    }   
}
