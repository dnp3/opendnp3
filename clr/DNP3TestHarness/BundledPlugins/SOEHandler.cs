using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;

using DNP3.Interface;

namespace Automatak.DNP3.Simulator
{
    

    class SOEHandler: ISOEHandler
    {
        private IDictionary<ushort, MeasurementPoco> binaryMap = new Dictionary<ushort, MeasurementPoco>();
        private BindingList<MeasurementPoco> binaryList = new BindingList<MeasurementPoco>();

        public IBindingList BinarySource
        {
            get
            {
                return binaryList;
            }
        }

        void ISOEHandler.Start()
        {
            System.Threading.Monitor.Enter(this);
        }

        void ISOEHandler.End()
        {
            System.Threading.Monitor.Exit(this);            
        }

        void ISOEHandler.LoadStatic(Binary meas, ushort index)
        {            
            if (binaryMap.ContainsKey(index))
            {
                var item = binaryMap[index];
                item.Update(meas.Value.ToString(), meas);
                binaryList.ResetItem(item.Row);
            }
            else
            {                
                // first determine on what row the measurement will be inserted
                if (binaryList.Count == 0)
                {
                    var poco = new MeasurementPoco(0, meas.Value.ToString(), meas, index);
                    binaryList.Add(poco);
                    binaryMap[index] = poco;
                }
                else
                { 
                    var result = binaryList.FirstOrDefault((poco) => poco.Index > index);
                    if(result != null)
                    {                        
                        var poco = new MeasurementPoco(result.Row, meas.Value.ToString(), meas, index);
                        binaryList.Insert(result.Row, poco);
                        binaryMap[index] = poco;
                        foreach (var item in binaryList)
                        {
                            if (item.Index > index)
                            {
                                item.NextRow();
                            }
                        }
                    }
                    else
                    {
                        var poco = new MeasurementPoco(binaryList.Count, meas.Value.ToString(), meas, index);
                        binaryList.Add(poco);
                        binaryMap[index] = poco;
                    }                   
                }
            }            
        }

        void ISOEHandler.LoadStatic(DoubleBitBinary meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadStatic(Analog meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadStatic(Counter meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadStatic(FrozenCounter meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadStatic(BinaryOutputStatus meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadStatic(AnalogOutputStatus meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadStatic(OctetString meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadEvent(Binary meas, ushort index)
        {
            //binaries[index] = new Measurement { Index = index, Value = meas.Value.ToString(), Flags = meas.Quality.ToString("%02x"), TimeStamp = meas.Timestamp };
        }

        void ISOEHandler.LoadEvent(DoubleBitBinary meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadEvent(Analog meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadEvent(Counter meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadEvent(FrozenCounter meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadEvent(BinaryOutputStatus meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadEvent(AnalogOutputStatus meas, ushort index)
        {
            
        }

        void ISOEHandler.LoadEvent(OctetString meas, ushort index)
        {
            
        }
    }
}
