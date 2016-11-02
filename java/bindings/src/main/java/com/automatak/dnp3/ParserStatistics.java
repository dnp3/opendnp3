package com.automatak.dnp3;

/**
 * Statistics for the link layer parser
 */
public class ParserStatistics
{
    public ParserStatistics(
            long numHeaderCrcError,
            long numBodyCrcError,
            long numLinkFrameRx,
            long numBadLength,
            long numBadFunctionCode,
            long numBadFCV,
            long numBadFCB
    )
    {
        this.numHeaderCrcError = numHeaderCrcError;
        this.numBodyCrcError = numBodyCrcError;
        this.numLinkFrameRx = numLinkFrameRx;
        this.numBadLength = numBadLength;
        this.numBadFunctionCode = numBadFunctionCode;
        this.numBadFCV = numBadFCV;
        this.numBadFCB = numBadFCB;
    }

    /**
     * Number of frames discarded due to header CRC errors
     */
    final long numHeaderCrcError;

    /**
     * Number of frames discarded due to body CRC errors
     */
    final long numBodyCrcError;

    /**
     * Number of frames received
     */
    final long numLinkFrameRx;

    /**
     * number of bad LEN fields received (malformed frame)
     */
    final long numBadLength;

    /**
     * number of bad function codes (malformed frame)
     */
    final long numBadFunctionCode;

    /**
     * number of FCV / function code mismatches (malformed frame)
     */
    final long numBadFCV;

    /**
     * number of frames w/ unexpected FCB bit set (malformed frame)
     */
    final long numBadFCB;
}
