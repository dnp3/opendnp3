package com.automatak.render.dnp3.enums

import com.automatak.render._


object FilterResult {

  private val comments = List(
    "Error codes for DNP3 filtering"
  )

  def apply(): EnumModel = EnumModel("FilterResult", comments, EnumModel.UInt8, codes, None, Hex)

  private def codes = names.zipWithIndex.map(x => EnumValue(x._1._1, x._2, x._1._2))

  private def names = List(
    "PASS" -> "No protocol or white-list violations",
    "FAIL_LINK_BAD_START_BYTES" -> "The stream did not contain the expected start bytes",
    "FAIL_LINK_BAD_HEADER_CRC" -> "The header had an incorrect CRC value",
    "FAIL_LINK_BAD_BODY_CRC" -> "The payload had an incorrect CRC value",
    "FAIL_LINK_BAD_LEN_FIELD" -> "The length field had a disallowed value",
    "FAIL_LINK_BAD_DIR_BIT" -> "The DIR bit in the header did not match the configuration",
    "FAIL_LINK_UNKNOWN_PRI_TO_SEC" -> "The header had an unknown primary to secondary function code",
    "FAIL_LINK_UNKNOWN_SEC_TO_PRI" -> "The header had an unknown secondary to primary function code",
    "FAIL_LINK_BAD_FCV_FOR_FUNCTION" -> "The FCV bit in the header did not match the function code",
    "FAIL_LINK_UNEXPECTED_FCB_BIT" -> "The FCB bit in the header did not match the function code",
    "FAIL_LINK_BAD_PAYLOAD_SIZE_FOR_FUNC" -> "There was a mismatch between the function code and the data payload",
    "FAIL_LINK_BROADCAST_DISABLED" -> "Broadcast addresses disallowed by configuration",
    "FAIL_LINK_SELF_ADDRESS_DISABLED" -> "Self addresses disallowed by configuration",
    "FAIL_LINK_BAD_DEST_ADDRESS" -> "The destination address did not match the configuration",
    "FAIL_LINK_BAD_SRC_ADDRESS" -> "The source address did not match the configuration",

    "FAIL_TRANSPORT_NO_HEADER" -> "The transport segment did not contain a header",
    "FAIL_TRANSPORT_BUFFER_OVERRUN" -> "The maximum reassembled fragment size has been exceeded",

    "FAIL_APP_INSUFFICIENT_DATA_FOR_HEADER" -> "The fragment did not contain enough data for a header",
    "FAIL_APP_INSUFFICIENT_DATA_FOR_OBJECT_HEADER" -> "The fragment does not contain enough data for an object header",
    "FAIL_APP_BAD_OBJECT_QUALIFIER" -> "The combination of object and qualifier is not allowed",
    "FAIL_APP_BAD_START_STOP" -> "The stop address was < the start address",
    "FAIL_APP_COUNT_OF_ZERO" -> "The count field had a value of zero",
    "FAIL_APP_INSUFFICIENT_DATA_FOR_OBJECTS" -> "The fragment does not contain enough data for the specified objects",
    "FAIL_APP_UNKNOWN_FUNCTION_CODE" -> "The fragment has an unknown function code",
    "FAIL_APP_UNKNOWN_OBJECT" -> "The fragment has an unknown function object",
    "FAIL_APP_UNKNOWN_QUALIFIER" -> "The fragment has an unknown function qualifier code",
    "FAIL_APP_NOT_ON_WHITELIST" -> "The fragment contains objects/qualifiers not on the whitelist"
  )

}



