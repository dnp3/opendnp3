//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "GroupVariation.h"

namespace opendnp3 {

uint16_t GroupVariationToType(GroupVariation arg)
{
  return static_cast<uint16_t>(arg);
}
GroupVariation GroupVariationFromType(uint16_t arg)
{
  switch(arg)
  {
    case(0x100):
      return GroupVariation::Group1Var0;
    case(0x101):
      return GroupVariation::Group1Var1;
    case(0x102):
      return GroupVariation::Group1Var2;
    case(0x200):
      return GroupVariation::Group2Var0;
    case(0x201):
      return GroupVariation::Group2Var1;
    case(0x202):
      return GroupVariation::Group2Var2;
    case(0x203):
      return GroupVariation::Group2Var3;
    case(0x300):
      return GroupVariation::Group3Var0;
    case(0x301):
      return GroupVariation::Group3Var1;
    case(0x302):
      return GroupVariation::Group3Var2;
    case(0x400):
      return GroupVariation::Group4Var0;
    case(0x401):
      return GroupVariation::Group4Var1;
    case(0x402):
      return GroupVariation::Group4Var2;
    case(0x403):
      return GroupVariation::Group4Var3;
    case(0xA00):
      return GroupVariation::Group10Var0;
    case(0xA01):
      return GroupVariation::Group10Var1;
    case(0xA02):
      return GroupVariation::Group10Var2;
    case(0xB00):
      return GroupVariation::Group11Var0;
    case(0xB01):
      return GroupVariation::Group11Var1;
    case(0xB02):
      return GroupVariation::Group11Var2;
    case(0xC00):
      return GroupVariation::Group12Var0;
    case(0xC01):
      return GroupVariation::Group12Var1;
	case(0xD01):
	  return GroupVariation::Group13Var1;
	case(0xD02):
	  return GroupVariation::Group13Var2;
    case(0x1400):
      return GroupVariation::Group20Var0;
    case(0x1401):
      return GroupVariation::Group20Var1;
    case(0x1402):
      return GroupVariation::Group20Var2;
    case(0x1405):
      return GroupVariation::Group20Var5;
    case(0x1406):
      return GroupVariation::Group20Var6;
    case(0x1500):
      return GroupVariation::Group21Var0;
    case(0x1501):
      return GroupVariation::Group21Var1;
    case(0x1502):
      return GroupVariation::Group21Var2;
    case(0x1505):
      return GroupVariation::Group21Var5;
    case(0x1506):
      return GroupVariation::Group21Var6;
    case(0x1509):
      return GroupVariation::Group21Var9;
    case(0x150A):
      return GroupVariation::Group21Var10;
    case(0x1600):
      return GroupVariation::Group22Var0;
    case(0x1601):
      return GroupVariation::Group22Var1;
    case(0x1602):
      return GroupVariation::Group22Var2;
    case(0x1605):
      return GroupVariation::Group22Var5;
    case(0x1606):
      return GroupVariation::Group22Var6;
    case(0x1700):
      return GroupVariation::Group23Var0;
    case(0x1701):
      return GroupVariation::Group23Var1;
    case(0x1702):
      return GroupVariation::Group23Var2;
    case(0x1705):
      return GroupVariation::Group23Var5;
    case(0x1706):
      return GroupVariation::Group23Var6;
    case(0x1E00):
      return GroupVariation::Group30Var0;
    case(0x1E01):
      return GroupVariation::Group30Var1;
    case(0x1E02):
      return GroupVariation::Group30Var2;
    case(0x1E03):
      return GroupVariation::Group30Var3;
    case(0x1E04):
      return GroupVariation::Group30Var4;
    case(0x1E05):
      return GroupVariation::Group30Var5;
    case(0x1E06):
      return GroupVariation::Group30Var6;
    case(0x2000):
      return GroupVariation::Group32Var0;
    case(0x2001):
      return GroupVariation::Group32Var1;
    case(0x2002):
      return GroupVariation::Group32Var2;
    case(0x2003):
      return GroupVariation::Group32Var3;
    case(0x2004):
      return GroupVariation::Group32Var4;
    case(0x2005):
      return GroupVariation::Group32Var5;
    case(0x2006):
      return GroupVariation::Group32Var6;
    case(0x2007):
      return GroupVariation::Group32Var7;
    case(0x2008):
      return GroupVariation::Group32Var8;
    case(0x2800):
      return GroupVariation::Group40Var0;
    case(0x2801):
      return GroupVariation::Group40Var1;
    case(0x2802):
      return GroupVariation::Group40Var2;
    case(0x2803):
      return GroupVariation::Group40Var3;
    case(0x2804):
      return GroupVariation::Group40Var4;
    case(0x2900):
      return GroupVariation::Group41Var0;
    case(0x2901):
      return GroupVariation::Group41Var1;
    case(0x2902):
      return GroupVariation::Group41Var2;
    case(0x2903):
      return GroupVariation::Group41Var3;
    case(0x2904):
      return GroupVariation::Group41Var4;
    case(0x2A00):
      return GroupVariation::Group42Var0;
    case(0x2A01):
      return GroupVariation::Group42Var1;
    case(0x2A02):
      return GroupVariation::Group42Var2;
    case(0x2A03):
      return GroupVariation::Group42Var3;
    case(0x2A04):
      return GroupVariation::Group42Var4;
    case(0x2A05):
      return GroupVariation::Group42Var5;
    case(0x2A06):
      return GroupVariation::Group42Var6;
    case(0x2A07):
      return GroupVariation::Group42Var7;
    case(0x2A08):
      return GroupVariation::Group42Var8;
    case(0x3201):
      return GroupVariation::Group50Var1;
    case(0x3204):
      return GroupVariation::Group50Var4;
    case(0x3301):
      return GroupVariation::Group51Var1;
    case(0x3302):
      return GroupVariation::Group51Var2;
    case(0x3401):
      return GroupVariation::Group52Var1;
    case(0x3402):
      return GroupVariation::Group52Var2;
    case(0x3C01):
      return GroupVariation::Group60Var1;
    case(0x3C02):
      return GroupVariation::Group60Var2;
    case(0x3C03):
      return GroupVariation::Group60Var3;
    case(0x3C04):
      return GroupVariation::Group60Var4;
    case(0x5001):
      return GroupVariation::Group80Var1;
    case(0x6E00):
      return GroupVariation::Group110AnyVar;
    case(0x6F00):
      return GroupVariation::Group111AnyVar;
    case(0xFFFF):
      return GroupVariation::UNKNOWN;
  }
  return GroupVariation::UNKNOWN;
}
char const* GroupVariationToString(GroupVariation arg)
{
  switch(arg)
  {
    case(GroupVariation::Group1Var0):
      return "Binary Input - Any Variation";
    case(GroupVariation::Group1Var1):
      return "Binary Input - Packed Format";
    case(GroupVariation::Group1Var2):
      return "Binary Input - With Flags";
    case(GroupVariation::Group2Var0):
      return "Binary Input Event - Any Variation";
    case(GroupVariation::Group2Var1):
      return "Binary Input Event - Without Time";
    case(GroupVariation::Group2Var2):
      return "Binary Input Event - With Absolute Time";
    case(GroupVariation::Group2Var3):
      return "Binary Input Event - With Relative Time";
    case(GroupVariation::Group3Var0):
      return "Double-bit Binary Input - Any Variation";
    case(GroupVariation::Group3Var1):
      return "Double-bit Binary Input - Packed Format";
    case(GroupVariation::Group3Var2):
      return "Double-bit Binary Input - With Flags";
    case(GroupVariation::Group4Var0):
      return "Double-bit Binary Input Event - Any Variation";
    case(GroupVariation::Group4Var1):
      return "Double-bit Binary Input Event - Without Time";
    case(GroupVariation::Group4Var2):
      return "Double-bit Binary Input Event - With Absolute Time";
    case(GroupVariation::Group4Var3):
      return "Double-bit Binary Input Event - With Relative Time";
    case(GroupVariation::Group10Var0):
      return "Binary Output - Any Variation";
    case(GroupVariation::Group10Var1):
      return "Binary Output - Packed Format";
    case(GroupVariation::Group10Var2):
      return "Binary Output - Output Status With Flags";
    case(GroupVariation::Group11Var0):
      return "Binary Output Event - Any Variation";
    case(GroupVariation::Group11Var1):
      return "Binary Output Event - Output Status Without Time";
    case(GroupVariation::Group11Var2):
      return "Binary Output Event - Output Status With Time";
    case(GroupVariation::Group12Var0):
      return "Binary Command - Any Variation";
    case(GroupVariation::Group12Var1):
      return "Binary Command - CROB";
    case(GroupVariation::Group20Var0):
      return "Counter - Any Variation";
    case(GroupVariation::Group20Var1):
      return "Counter - 32-bit With Flag";
    case(GroupVariation::Group20Var2):
      return "Counter - 16-bit With Flag";
    case(GroupVariation::Group20Var5):
      return "Counter - 32-bit Without Flag";
    case(GroupVariation::Group20Var6):
      return "Counter - 16-bit Without Flag";
    case(GroupVariation::Group21Var0):
      return "Frozen Counter - Any Variation";
    case(GroupVariation::Group21Var1):
      return "Frozen Counter - 32-bit With Flag";
    case(GroupVariation::Group21Var2):
      return "Frozen Counter - 16-bit With Flag";
    case(GroupVariation::Group21Var5):
      return "Frozen Counter - 32-bit With Flag and Time";
    case(GroupVariation::Group21Var6):
      return "Frozen Counter - 16-bit With Flag and Time";
    case(GroupVariation::Group21Var9):
      return "Frozen Counter - 32-bit Without Flag";
    case(GroupVariation::Group21Var10):
      return "Frozen Counter - 16-bit Without Flag";
    case(GroupVariation::Group22Var0):
      return "Counter Event - Any Variation";
    case(GroupVariation::Group22Var1):
      return "Counter Event - 32-bit With Flag";
    case(GroupVariation::Group22Var2):
      return "Counter Event - 16-bit With Flag";
    case(GroupVariation::Group22Var5):
      return "Counter Event - 32-bit With Flag and Time";
    case(GroupVariation::Group22Var6):
      return "Counter Event - 16-bit With Flag and Time";
    case(GroupVariation::Group23Var0):
      return "Frozen Counter Event - Any Variation";
    case(GroupVariation::Group23Var1):
      return "Frozen Counter Event - 32-bit With Flag";
    case(GroupVariation::Group23Var2):
      return "Frozen Counter Event - 16-bit With Flag";
    case(GroupVariation::Group23Var5):
      return "Frozen Counter Event - 32-bit With Flag and Time";
    case(GroupVariation::Group23Var6):
      return "Frozen Counter Event - 16-bit With Flag and Time";
    case(GroupVariation::Group30Var0):
      return "Analog Input - Any Variation";
    case(GroupVariation::Group30Var1):
      return "Analog Input - 32-bit With Flag";
    case(GroupVariation::Group30Var2):
      return "Analog Input - 16-bit With Flag";
    case(GroupVariation::Group30Var3):
      return "Analog Input - 32-bit Without Flag";
    case(GroupVariation::Group30Var4):
      return "Analog Input - 16-bit Without Flag";
    case(GroupVariation::Group30Var5):
      return "Analog Input - Single-precision With Flag";
    case(GroupVariation::Group30Var6):
      return "Analog Input - Double-precision With Flag";
    case(GroupVariation::Group32Var0):
      return "Analog Input Event - Any Variation";
    case(GroupVariation::Group32Var1):
      return "Analog Input Event - 32-bit With Flag";
    case(GroupVariation::Group32Var2):
      return "Analog Input Event - 16-bit With Flag";
    case(GroupVariation::Group32Var3):
      return "Analog Input Event - 32-bit With Flag and Time";
    case(GroupVariation::Group32Var4):
      return "Analog Input Event - 16-bit With Flag and Time";
    case(GroupVariation::Group32Var5):
      return "Analog Input Event - Single-precision With Flag";
    case(GroupVariation::Group32Var6):
      return "Analog Input Event - Double-precision With Flag";
    case(GroupVariation::Group32Var7):
      return "Analog Input Event - Single-precision With Flag and Time";
    case(GroupVariation::Group32Var8):
      return "Analog Input Event - Double-precision With Flag abd Time";
    case(GroupVariation::Group40Var0):
      return "Analog Output Status - Any Variation";
    case(GroupVariation::Group40Var1):
      return "Analog Output Status - 32-bit With Flag";
    case(GroupVariation::Group40Var2):
      return "Analog Output Status - 16-bit With Flag";
    case(GroupVariation::Group40Var3):
      return "Analog Output Status - Single-precision With Flag";
    case(GroupVariation::Group40Var4):
      return "Analog Output Status - Double-precision With Flag";
    case(GroupVariation::Group41Var0):
      return "Analog Output - Any Variation";
    case(GroupVariation::Group41Var1):
      return "Analog Output - 32-bit With Flag";
    case(GroupVariation::Group41Var2):
      return "Analog Output - 16-bit With Flag";
    case(GroupVariation::Group41Var3):
      return "Analog Output - Single-precision";
    case(GroupVariation::Group41Var4):
      return "Analog Output - Double-precision";
    case(GroupVariation::Group42Var0):
      return "Analog Output Event - Any Variation";
    case(GroupVariation::Group42Var1):
      return "Analog Output Event - 32-bit With Flag";
    case(GroupVariation::Group42Var2):
      return "Analog Output Event - 16-bit With Flag";
    case(GroupVariation::Group42Var3):
      return "Analog Output Event - 32-bit With Flag and Time";
    case(GroupVariation::Group42Var4):
      return "Analog Output Event - 16-bit With Flag and Time";
    case(GroupVariation::Group42Var5):
      return "Analog Output Event - Single-precision With Flag";
    case(GroupVariation::Group42Var6):
      return "Analog Output Event - Double-precision With Flag";
    case(GroupVariation::Group42Var7):
      return "Analog Output Event - Single-precision With Flag and Time";
    case(GroupVariation::Group42Var8):
      return "Analog Output Event - Double-precision With Flag and Time";
    case(GroupVariation::Group50Var1):
      return "Time and Date - Absolute Time";
    case(GroupVariation::Group50Var4):
      return "Time and Date - Indexed absolute time and long interval";
    case(GroupVariation::Group51Var1):
      return "Time and Date CTO - Absolute time, synchronized";
    case(GroupVariation::Group51Var2):
      return "Time and Date CTO - Absolute time, synchronized";
    case(GroupVariation::Group52Var1):
      return "Time Delay - Coarse";
    case(GroupVariation::Group52Var2):
      return "Time Delay - Fine";
    case(GroupVariation::Group60Var1):
      return "Class Data - Class 0 data";
    case(GroupVariation::Group60Var2):
      return "Class Data - Class 1 data";
    case(GroupVariation::Group60Var3):
      return "Class Data - Class 2 data";
    case(GroupVariation::Group60Var4):
      return "Class Data - Class 3 data";
    case(GroupVariation::Group80Var1):
      return "Internal Indications - Packed Format";
    case(GroupVariation::Group110AnyVar):
      return "Octet String";
    case(GroupVariation::Group111AnyVar):
      return "Octet String Event";
    case(GroupVariation::UNKNOWN):
      return "UNKNOWN";
  }
  return "UNKNOWN";
}

}
