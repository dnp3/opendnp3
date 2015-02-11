package com.automatak.render.dnp3.enums

import com.automatak.render._
import scala.Some

object GroupVariation {

  private def name(g: Int, v: Int) = List("Group",g.toString,"Var", v.toString).mkString

  private def value(g: Int, v: Int) : Int = g*256 + v

  private def gvsub(g: Int, v: Int)(desc: String, subdesc: String): EnumValue = gv(g,v)(desc + " - " + subdesc)

  private def gv(g: Int, v: Int)(desc: String): EnumValue = EnumValue(name(g,v), value(g,v), None, Some(desc))

  private def group(g: Int, desc: String, objects: List[(Int, String)]) : List[EnumValue] = objects.map(pair => gvsub(g,pair._1)(desc, pair._2))

  private val anyVariation = 0 -> "Any Variation"
  private val packedFormat = "Packed Format"
  private val withFlags = "With Flags"
  private val withTime = "With Time"
  private val withoutTime = "Without Time"
  private val withAbsoluteTime = "With Absolute Time"
  private val withRelativeTime = "With Relative Time"
  private val outputStatusWithFlags = "Output Status With Flags"
  private val outputStatusWithTime = "Output Status With Time"
  private val outputStatusWithoutTime = "Output Status Without Time"
  private val crob = "CROB"
  private val bit32 = "32-bit"
  private val bit16 = "16-bit"
  private val bit32WithTime = "32-bit With Time"
  private val bit16WithTime = "16-bit With Time"
  private val bit32WithFlag = "32-bit With Flag"
  private val bit16WithFlag = "16-bit With Flag"
  private val bit32WithoutFlag = "32-bit Without Flag"
  private val bit16WithoutFlag = "16-bit Without Flag"
  private val bit32WithFlagTime = "32-bit With Flag and Time"
  private val bit16WithFlagTime = "16-bit With Flag and Time"
  private val singlePrecision = "Single-precision"
  private val doublePrecision = "Double-precision"
  private val singlePrecisionWithTime = "Single-precision With Time"
  private val doublePrecisionWithTime = "Double-precision With Time"
  private val singlePrecisionWithFlag = "Single-precision With Flag"
  private val doublePrecisionWithFlag = "Double-precision With Flag"
  private val singlePrecisionWithFlagTime = "Single-precision With Flag and Time"
  private val doublePrecisionWithFlagTime = "Double-precision With Flag and Time"

  private val group1 = List(anyVariation, 1 -> packedFormat, 2 -> withFlags)

  private val group2 = List(anyVariation, 1 -> withoutTime, 2 -> withAbsoluteTime, 3 -> withRelativeTime)

  private val group3 = List(anyVariation, 1 -> packedFormat, 2 -> withFlags)

  private val group4 = List(anyVariation, 1 -> withoutTime, 2 -> withAbsoluteTime, 3 -> withRelativeTime)

  private val group10 = List(anyVariation, 1 -> packedFormat, 2 -> outputStatusWithFlags)

  private val group11 = List(anyVariation, 1 -> outputStatusWithoutTime, 2-> outputStatusWithTime)

  private val group12 = List(anyVariation, 1 -> crob)

  private val group13 = List(1 -> withoutTime, 2 -> withTime)

  private val group20 = List(anyVariation, 1 -> bit32WithFlag, 2 -> bit16WithFlag, 5 ->bit32WithoutFlag, 6 -> bit16WithoutFlag)

  private val group21 = List(anyVariation, 1 -> bit32WithFlag, 2-> bit16WithFlag, 5 -> bit32WithFlagTime, 6 -> bit16WithFlagTime, 9->bit32WithoutFlag, 10->bit16WithoutFlag)

  private val group22 = List(anyVariation, 1 -> bit32WithFlag, 2-> bit16WithFlag, 5 -> bit32WithFlagTime, 6 -> bit16WithFlagTime)

  private val group23 = List(anyVariation, 1 -> bit32WithFlag, 2-> bit16WithFlag, 5 -> bit32WithFlagTime, 6 -> bit16WithFlagTime)

  private val group30 = List(anyVariation, 1 -> bit32WithFlag, 2-> bit16WithFlag, 3 -> bit32WithoutFlag, 4 -> bit16WithoutFlag, 5 -> singlePrecisionWithFlag, 6 -> doublePrecisionWithFlag)

  private val group32 = List(
    anyVariation,
    1 -> bit32WithFlag,
    2 -> bit16WithFlag,
    3 -> bit32WithFlagTime,
    4 -> bit16WithFlagTime,
    5 -> singlePrecisionWithFlag,
    6 -> doublePrecisionWithFlag,
    7 -> singlePrecisionWithFlagTime,
    8 -> doublePrecisionWithFlagTime
  )

  private val group40 = List(anyVariation, 1 -> bit32WithFlag, 2 -> bit16WithFlag, 3 -> singlePrecisionWithFlag, 4 -> doublePrecisionWithFlag)
  private val group41 = List(anyVariation, 1 -> bit32WithFlag, 2 -> bit16WithFlag, 3 -> singlePrecision, 4 -> doublePrecision)

  private val group42 = List(
    anyVariation,
    1 -> bit32WithFlag,
    2 -> bit16WithFlag,
    3 -> bit32WithFlagTime,
    4 -> bit16WithFlagTime,
    5 -> singlePrecisionWithFlag,
    6 -> doublePrecisionWithFlag,
    7 -> singlePrecisionWithFlagTime,
    8 -> doublePrecisionWithFlagTime
  )

  private val group43 = List(
    1 -> bit32,
    2 -> bit16,
    3 -> bit32WithTime,
    4 -> bit16WithTime,
    5 -> singlePrecision,
    6 -> doublePrecision,
    7 -> singlePrecisionWithTime,
    8 -> doublePrecisionWithTime
  )

  private val group60 = List(1 -> "Class 0 data", 2 -> "Class 1 data", 3 -> "Class 2 data", 4 -> "Class 3 data")

  private val group120 = List(
    1 -> "Challenge",
    2 -> "Reply",
    3 -> "Aggressive Mode Request",
    4 -> "Session Key Status Request",
    5 -> "Session Key Status",
    6 -> "Session Key Change",
    7 -> "Error",
    9 -> "HMAC"
  )

  private val defaultValue = EnumValue("UNKNOWN", value(0xFF, 0xFF))


  // defined using syntactic sugar above
  private def values : List[EnumValue] = List(

    group(1, "Binary Input", group1),
    group(2, "Binary Input Event", group2),
    group(3, "Double-bit Binary Input", group3),
    group(4, "Double-bit Binary Input Event", group4),
    group(10, "Binary Output", group10),
    group(11, "Binary Output Event", group11),
    group(12, "Binary Command", group12),
    group(13, "Binary Command Event", group13),
    group(20, "Counter", group20),
    group(21, "Frozen Counter", group21),
    group(22, "Counter Event", group22),
    group(23, "Frozen Counter Event", group23),
    group(30, "Analog Input", group30),
    group(32, "Analog Input Event", group32),
    group(40, "Analog Output Status", group40),
    group(41, "Analog Output", group41),
    group(42, "Analog Output Event", group42),
    group(43, "Analog Command Event", group43),

    group(50, "Time and Date", List(1 -> "Absolute Time", 4 -> "Indexed absolute time and long interval")),
    group(51, "Time and Date CTO", List(1 -> "Absolute time, synchronized", 2 -> "Absolute time, synchronized")),
    group(52, "Time Delay", List(1 -> "Coarse", 2 -> "Fine")),


    group(60, "Class Data", group60),

    group(80, "Internal Indications", List(1 -> packedFormat)),

    List(EnumValue("Group110AnyVar", value(110,0), None, Some("Octet String"))),

    List(EnumValue("Group111AnyVar", value(111,0), None, Some("Octet String Event"))),

    group(120, "Authentication", group120)

  ).flatten


  def apply() : EnumModel = EnumModel(
    "GroupVariation",
    List("Comprehensive list of supported groups and variations"),
    EnumModel.UInt16,
    values,
    Some(defaultValue),
    Hex
  )

}
