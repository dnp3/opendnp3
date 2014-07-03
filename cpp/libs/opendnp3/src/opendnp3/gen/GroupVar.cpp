//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "GroupVar.h"

namespace opendnp3 {

uint16_t GroupVarToType(GroupVar arg)
{
  return static_cast<uint16_t>(arg);
}
GroupVar GroupVarFromType(uint16_t arg)
{
  switch(arg)
  {
    case(0x100):
      return GroupVar::Group1Var0;
    case(0x101):
      return GroupVar::Group1Var1;
    case(0x102):
      return GroupVar::Group1Var2;
    case(0x200):
      return GroupVar::Group2Var0;
    case(0x201):
      return GroupVar::Group2Var1;
    case(0x202):
      return GroupVar::Group2Var2;
    case(0x203):
      return GroupVar::Group2Var3;
    case(0x300):
      return GroupVar::Group3Var0;
    case(0x301):
      return GroupVar::Group3Var1;
    case(0x302):
      return GroupVar::Group3Var2;
    case(0x400):
      return GroupVar::Group4Var0;
    case(0x401):
      return GroupVar::Group4Var1;
    case(0x402):
      return GroupVar::Group4Var2;
    case(0x403):
      return GroupVar::Group4Var3;
    case(0xA00):
      return GroupVar::Group10Var0;
    case(0xA02):
      return GroupVar::Group10Var2;
    case(0xB00):
      return GroupVar::Group11Var0;
    case(0xB01):
      return GroupVar::Group11Var1;
    case(0xB02):
      return GroupVar::Group11Var2;
    case(0xC00):
      return GroupVar::Group12Var0;
    case(0xC01):
      return GroupVar::Group12Var1;
    case(0x1400):
      return GroupVar::Group20Var0;
    case(0x1401):
      return GroupVar::Group20Var1;
    case(0x1402):
      return GroupVar::Group20Var2;
    case(0x1405):
      return GroupVar::Group20Var5;
    case(0x1406):
      return GroupVar::Group20Var6;
    case(0x1500):
      return GroupVar::Group21Var0;
    case(0x1501):
      return GroupVar::Group21Var1;
    case(0x1502):
      return GroupVar::Group21Var2;
    case(0x1505):
      return GroupVar::Group21Var5;
    case(0x1506):
      return GroupVar::Group21Var6;
    case(0x1509):
      return GroupVar::Group21Var9;
    case(0x150A):
      return GroupVar::Group21Var10;
    case(0x1600):
      return GroupVar::Group22Var0;
    case(0x1601):
      return GroupVar::Group22Var1;
    case(0x1602):
      return GroupVar::Group22Var2;
    case(0x1700):
      return GroupVar::Group23Var0;
    case(0x1701):
      return GroupVar::Group23Var1;
    case(0x1702):
      return GroupVar::Group23Var2;
    case(0x1705):
      return GroupVar::Group23Var5;
    case(0x1706):
      return GroupVar::Group23Var6;
    case(0x1E00):
      return GroupVar::Group30Var0;
    case(0x1E01):
      return GroupVar::Group30Var1;
    case(0x1E02):
      return GroupVar::Group30Var2;
    case(0x1E03):
      return GroupVar::Group30Var3;
    case(0x1E04):
      return GroupVar::Group30Var4;
    case(0x1E05):
      return GroupVar::Group30Var5;
    case(0x1E06):
      return GroupVar::Group30Var6;
    case(0x2000):
      return GroupVar::Group32Var0;
    case(0x2001):
      return GroupVar::Group32Var1;
    case(0x2002):
      return GroupVar::Group32Var2;
    case(0x2003):
      return GroupVar::Group32Var3;
    case(0x2004):
      return GroupVar::Group32Var4;
    case(0x2005):
      return GroupVar::Group32Var5;
    case(0x2006):
      return GroupVar::Group32Var6;
    case(0x2007):
      return GroupVar::Group32Var7;
    case(0x2008):
      return GroupVar::Group32Var8;
    case(0x2800):
      return GroupVar::Group40Var0;
    case(0x2801):
      return GroupVar::Group40Var1;
    case(0x2802):
      return GroupVar::Group40Var2;
    case(0x2803):
      return GroupVar::Group40Var3;
    case(0x2804):
      return GroupVar::Group40Var4;
    case(0x2900):
      return GroupVar::Group41Var0;
    case(0x2901):
      return GroupVar::Group41Var1;
    case(0x2902):
      return GroupVar::Group41Var2;
    case(0x2903):
      return GroupVar::Group41Var3;
    case(0x2904):
      return GroupVar::Group41Var4;
    case(0x2A00):
      return GroupVar::Group42Var0;
    case(0x2A01):
      return GroupVar::Group42Var1;
    case(0x2A02):
      return GroupVar::Group42Var2;
    case(0x2A03):
      return GroupVar::Group42Var3;
    case(0x2A04):
      return GroupVar::Group42Var4;
    case(0x2A05):
      return GroupVar::Group42Var5;
    case(0x2A06):
      return GroupVar::Group42Var6;
    case(0x2A07):
      return GroupVar::Group42Var7;
    case(0x2A08):
      return GroupVar::Group42Var8;
    case(0x3201):
      return GroupVar::Group50Var1;
    case(0x3301):
      return GroupVar::Group51Var1;
    case(0x3302):
      return GroupVar::Group51Var2;
    case(0x3401):
      return GroupVar::Group52Var1;
    case(0x3402):
      return GroupVar::Group52Var2;
    case(0x3C00):
      return GroupVar::Group60Var0;
    case(0x3C01):
      return GroupVar::Group60Var1;
    case(0x3C02):
      return GroupVar::Group60Var2;
    case(0x3C03):
      return GroupVar::Group60Var3;
    case(0x5001):
      return GroupVar::Group80Var1;
    case(0xFFFF):
      return GroupVar::Undefined;
  }
  return GroupVar::Undefined;
}
char const* GroupVarToString(GroupVar arg)
{
  switch(arg)
  {
    case(GroupVar::Group1Var0):
      return "Binary Input - Any Variation";
    case(GroupVar::Group1Var1):
      return "Binary Input - Packed Format";
    case(GroupVar::Group1Var2):
      return "Binary Input - With Flags";
    case(GroupVar::Group2Var0):
      return "Binary Input Event - Any Variation";
    case(GroupVar::Group2Var1):
      return "Binary Input Event - Without Time";
    case(GroupVar::Group2Var2):
      return "Binary Input Event - With Absolute Time";
    case(GroupVar::Group2Var3):
      return "Binary Input Event - With Relative Time";
    case(GroupVar::Group3Var0):
      return "Double-bit Binary Input - Any Variation";
    case(GroupVar::Group3Var1):
      return "Double-bit Binary Input - Packed Format";
    case(GroupVar::Group3Var2):
      return "Double-bit Binary Input - With Flags";
    case(GroupVar::Group4Var0):
      return "Double-bit Binary Input Event - Any Variation";
    case(GroupVar::Group4Var1):
      return "Double-bit Binary Input Event - Without Time";
    case(GroupVar::Group4Var2):
      return "Double-bit Binary Input Event - With Absolute Time";
    case(GroupVar::Group4Var3):
      return "Double-bit Binary Input Event - With Relative Time";
    case(GroupVar::Group10Var0):
      return "Binary Output - Any Variation";
    case(GroupVar::Group10Var2):
      return "Binary Output - Output Status With Flags";
    case(GroupVar::Group11Var0):
      return "Binary Output Event - Any Variation";
    case(GroupVar::Group11Var1):
      return "Binary Output Event - Output Status Without Time";
    case(GroupVar::Group11Var2):
      return "Binary Output Event - Output Status With Time";
    case(GroupVar::Group12Var0):
      return "Binary Command - Any Variation";
    case(GroupVar::Group12Var1):
      return "Binary Command - CROB";
    case(GroupVar::Group20Var0):
      return "Counter - Any Variation";
    case(GroupVar::Group20Var1):
      return "Counter - 32-bit With Flag";
    case(GroupVar::Group20Var2):
      return "Counter - 16-bit With Flag";
    case(GroupVar::Group20Var5):
      return "Counter - 32-bit Without Flag";
    case(GroupVar::Group20Var6):
      return "Counter - 16-bit Without Flag";
    case(GroupVar::Group21Var0):
      return "Frozen Counter - Any Variation";
    case(GroupVar::Group21Var1):
      return "Frozen Counter - 32-bit With Flag";
    case(GroupVar::Group21Var2):
      return "Frozen Counter - 16-bit With Flag";
    case(GroupVar::Group21Var5):
      return "Frozen Counter - 32-bit With Flag and Time";
    case(GroupVar::Group21Var6):
      return "Frozen Counter - 16-bit With Flag and Time";
    case(GroupVar::Group21Var9):
      return "Frozen Counter - 32-bit Without Flag";
    case(GroupVar::Group21Var10):
      return "Frozen Counter - 16-bit Without Flag";
    case(GroupVar::Group22Var0):
      return "Counter Event - Any Variation";
    case(GroupVar::Group22Var1):
      return "Counter Event - 32-bit With Flag";
    case(GroupVar::Group22Var2):
      return "Counter Event - 16-bit With Flag";
    case(GroupVar::Group23Var0):
      return "Frozen Counter Event - Any Variation";
    case(GroupVar::Group23Var1):
      return "Frozen Counter Event - 32-bit With Flag";
    case(GroupVar::Group23Var2):
      return "Frozen Counter Event - 16-bit With Flag";
    case(GroupVar::Group23Var5):
      return "Frozen Counter Event - 32-bit With Flag and Time";
    case(GroupVar::Group23Var6):
      return "Frozen Counter Event - 16-bit With Flag and Time";
    case(GroupVar::Group30Var0):
      return "Analog Input - Any Variation";
    case(GroupVar::Group30Var1):
      return "Analog Input - 32-bit With Flag";
    case(GroupVar::Group30Var2):
      return "Analog Input - 16-bit With Flag";
    case(GroupVar::Group30Var3):
      return "Analog Input - 32-bit Without Flag";
    case(GroupVar::Group30Var4):
      return "Analog Input - 16-bit Without Flag";
    case(GroupVar::Group30Var5):
      return "Analog Input - Single-precision With Flag";
    case(GroupVar::Group30Var6):
      return "Analog Input - Double-precision With Flag";
    case(GroupVar::Group32Var0):
      return "Analog Input Event - Any Variation";
    case(GroupVar::Group32Var1):
      return "Analog Input Event - 32-bit With Flag";
    case(GroupVar::Group32Var2):
      return "Analog Input Event - 16-bit With Flag";
    case(GroupVar::Group32Var3):
      return "Analog Input Event - 32-bit With Flag and Time";
    case(GroupVar::Group32Var4):
      return "Analog Input Event - 16-bit With Flag and Time";
    case(GroupVar::Group32Var5):
      return "Analog Input Event - Single-precision With Flag";
    case(GroupVar::Group32Var6):
      return "Analog Input Event - Double-precision With Flag";
    case(GroupVar::Group32Var7):
      return "Analog Input Event - Single-precision With Flag and Time";
    case(GroupVar::Group32Var8):
      return "Analog Input Event - Double-precision With Flag abd Time";
    case(GroupVar::Group40Var0):
      return "Analog Output Status - Any Variation";
    case(GroupVar::Group40Var1):
      return "Analog Output Status - 32-bit With Flag";
    case(GroupVar::Group40Var2):
      return "Analog Output Status - 16-bit With Flag";
    case(GroupVar::Group40Var3):
      return "Analog Output Status - Single-precision With Flag";
    case(GroupVar::Group40Var4):
      return "Analog Output Status - Double-precision With Flag";
    case(GroupVar::Group41Var0):
      return "Analog Output - Any Variation";
    case(GroupVar::Group41Var1):
      return "Analog Output - 32-bit With Flag";
    case(GroupVar::Group41Var2):
      return "Analog Output - 16-bit With Flag";
    case(GroupVar::Group41Var3):
      return "Analog Output - Single-precision";
    case(GroupVar::Group41Var4):
      return "Analog Output - Double-precision";
    case(GroupVar::Group42Var0):
      return "Analog Output Event - Any Variation";
    case(GroupVar::Group42Var1):
      return "Analog Output Event - 32-bit With Flag";
    case(GroupVar::Group42Var2):
      return "Analog Output Event - 16-bit With Flag";
    case(GroupVar::Group42Var3):
      return "Analog Output Event - 32-bit With Flag and Time";
    case(GroupVar::Group42Var4):
      return "Analog Output Event - 16-bit With Flag and Time";
    case(GroupVar::Group42Var5):
      return "Analog Output Event - Single-precision With Flag";
    case(GroupVar::Group42Var6):
      return "Analog Output Event - Double-precision With Flag";
    case(GroupVar::Group42Var7):
      return "Analog Output Event - Single-precision With Flag and Time";
    case(GroupVar::Group42Var8):
      return "Analog Output Event - Double-precision With Flag abd Time";
    case(GroupVar::Group50Var1):
      return "Time and Date - Absolute Time";
    case(GroupVar::Group51Var1):
      return "Time and Date CTO - Absolute time, synchronized";
    case(GroupVar::Group51Var2):
      return "Time and Date CTO - Absolute time, synchronized";
    case(GroupVar::Group52Var1):
      return "Time Delay - Coarse";
    case(GroupVar::Group52Var2):
      return "Time Delay - Fine";
    case(GroupVar::Group60Var0):
      return "Class Data - Class 0 data";
    case(GroupVar::Group60Var1):
      return "Class Data - Class 1 data";
    case(GroupVar::Group60Var2):
      return "Class Data - Class 2 data";
    case(GroupVar::Group60Var3):
      return "Class Data - Class 3 data";
    case(GroupVar::Group80Var1):
      return "Internal Indications - Packed Format";
    case(GroupVar::Undefined):
      return "Undefined";
  }
  return "Undefined";
}

}
