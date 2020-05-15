// Copyright 2013-2020 Automatak, LLC
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements. 
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Automatak.DNP3.Interface
{
    /// <summary>
    /// Structure that records which events are scanned / evented
    /// </summary>
    [Serializable]
    public struct ClassField
    {       
        public ClassField(byte mask)
        {
            this.bitfield =  mask;
        }

        public static ClassField None
        {
            get
            {
                return new ClassField();
            }
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

        [XmlElement("Class0")]
        public  bool Class0
        {
            get
            {
                return (((byte)PointClass.Class0) & bitfield) != 0;
            }
            set
            {
                if (value)
                {
                    bitfield |= (byte) PointClass.Class0;
                }
                else
                {
                    bitfield &= (~(byte)(PointClass.Class0)) & 0xFF;
                }
            }
        }

        [XmlElement("Class1")]
        public bool Class1
        {
            get
            {
                return (((byte)PointClass.Class1) & bitfield) != 0;
            }
            set
            {
                if (value)
                {
                    bitfield |= (byte)PointClass.Class1;
                }
                else
                {
                    bitfield &= (~(byte)(PointClass.Class1)) & 0xFF;
                }
            }
        }

        [XmlElement("Class2")]
        public bool Class2
        {
            get
            {
                return (((byte)PointClass.Class2) & bitfield) != 0;
            }
            set
            {
                if (value)
                {
                    bitfield |= (byte)PointClass.Class2;
                }
                else
                {
                    bitfield &= (~(byte)(PointClass.Class2)) & 0xFF;
                }
            }
        }

        [XmlElement("Class3")]
        public bool Class3
        {
            get
            {
                return (((byte)PointClass.Class3) & bitfield) != 0;
            }
            set
            {
                if (value)
                {
                    bitfield |= (byte)PointClass.Class3;
                }
                else
                {
                    bitfield &= (~(byte)(PointClass.Class3)) & 0xFF;
                }
            }
        }

        [XmlIgnore]
        private byte bitfield;
    }   
}
