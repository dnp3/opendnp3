namespace Automatak.DNP3.Interface
{
    using System;

    namespace Automatak.DNP3.Interface 
    {
        public class Header 
        {
            public readonly GroupVariation variation;
            public readonly QualifierCode qualifier;

            protected Header(GroupVariation variation, QualifierCode qualifier) 
            {
                this.variation = variation;
                this.qualifier = qualifier;
            }

            /// <summary>
            /// Header requesting all objects of the given type or class (in the case of Group 60 objects) are returned.
            /// </summary>
            /// <param name="groupVariation">Group & Variation being requested.</param>
            /// <returns>A Header with Qualifier 0x06.</returns>
            public static Header AllObjects(GroupVariation groupVariation) 
            {
                return new Header(groupVariation, QualifierCode.ALL_OBJECTS);
            }

            /// <summary>
            /// Header requesting that the first "x" indexes are returned, where x is the quantity.
            /// </summary>
            /// <param name="groupVariation">Group & Variation being requested.</param>
            /// <param name="quantity">The number of objects to return.</param>
            /// <returns>A Header with Qualifier 0x07.</returns>
            public static Header Count(GroupVariation groupVariation, sbyte quantity) 
            {
                return new CountHeader(groupVariation, QualifierCode.UINT8_CNT, Convert.ToUInt16(quantity));
            }

            /// <summary>
            /// Header requesting that the first "x" indexes are returned, where x is the quantity.
            /// </summary>
            /// <param name="groupVariation">Group & Variation being requested.</param>
            /// <param name="quantity">The number of objects to return.</param>
            /// <returns>A Header with Qualifier 0x08.</returns>
            public static Header Count(GroupVariation groupVariation, ushort quantity) 
            {
                return new CountHeader(groupVariation, QualifierCode.UINT16_CNT, quantity);
            }

            /// <summary>
            /// Similar to a CountHeader except a starting index is provided.
            /// </summary>
            /// <param name="groupVariation">Group & Variation being requested.</param>
            /// <param name="index">The index to start at.</param>
            /// <param name="quantity">The number of objects to return.</param>
            /// <returns>A Header with Qualifier 0x17.</returns>
            public static Header IndexedCount(GroupVariation groupVariation, sbyte index, sbyte quantity) 
            {
                return new IndexedCountHeader(groupVariation, QualifierCode.UINT8_CNT_UINT8_INDEX, Convert.ToUInt16(index), Convert.ToUInt16(quantity));
            }

            /// <summary>
            /// Similar to a CountHeader except a starting index is provided.
            /// </summary>
            /// <param name="groupVariation">Group & Variation being requested.</param>
            /// <param name="index">The index to start at.</param>
            /// <param name="quantity">The number of objects to return.</param>
            /// <returns>A Header with Qualifier 0x28.</returns>
            public static Header IndexedCount(GroupVariation groupVariation, ushort index, ushort quantity) 
            {
                return new IndexedCountHeader(groupVariation, QualifierCode.UINT16_CNT_UINT16_INDEX, Convert.ToUInt16(index), Convert.ToUInt16(quantity));
            }

            /// <summary>
            /// Similar to a Count Header except both the start and stop indexes are provided.
            /// </summary>
            /// <param name="groupVariation">Group & Variation being requested.</param>
            /// <param name="startIndex">The index to start at.</param>
            /// <param name="stopIndex">The index to stop at.</param>
            /// <returns>A Header with Qualifier 0x00</returns>
            public static Header Range(GroupVariation groupVariation, sbyte startIndex, sbyte stopIndex) 
            {
                return new RangeHeader(groupVariation, QualifierCode.UINT8_START_STOP, Convert.ToUInt16(startIndex), Convert.ToUInt16(stopIndex));
            }

            /// <summary>
            /// Similar to a Count Header except both the start and stop indexes are provided.
            /// </summary>
            /// <param name="groupVariation">Group & Variation being requested.</param>
            /// <param name="startIndex">The index to start at.</param>
            /// <param name="stopIndex">The index to stop at.</param>
            /// <returns>A Header with Qualifier 0x01</returns>
            public static Header Range(GroupVariation groupVariation, ushort startIndex, ushort stopIndex) 
            {
                return new RangeHeader(groupVariation, QualifierCode.UINT16_START_STOP, startIndex, stopIndex);
            }
        }

        public class CountHeader : Header 
        {
            public readonly ushort quantity;

            internal CountHeader(GroupVariation groupVariation, QualifierCode qualifierCode, ushort quantity)
                : base(groupVariation, qualifierCode) 
            {
                this.quantity = quantity;
            }
        }

        public class IndexedCountHeader : Header 
        {
            public readonly ushort index;
            public readonly ushort quantity;

            internal IndexedCountHeader(GroupVariation variation, QualifierCode qualifier, ushort index, ushort quantity)
                : base(variation, qualifier) 
            {
                this.index = index;
                this.quantity = quantity;
            }
        }

        public class RangeHeader : Header 
        {
            public readonly ushort startIndex;
            public readonly ushort stopIndex;

            internal RangeHeader(GroupVariation variation, QualifierCode qualifier, ushort startIndex, ushort stopIndex)
                : base(variation, qualifier) 
            {
                this.startIndex = startIndex;
                this.stopIndex = stopIndex;
            }
        }
    }
}