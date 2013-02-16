package com.automatak.dnp3;

/**
 * All quality enumeration fields can be converted to a byte bit-mask
 */
public interface QualityField {

    /**
     * @return The value of the quality enumeration as a byte
     */
    byte toByte();

}
