package com.automatak.dnp3;

import com.automatak.dnp3.enums.PointClass;

public final class PointClassMasks {
    private PointClassMasks() {}

    public static int ALL_EVENTS = PointClass.Class1.toType() | PointClass.Class2.toType() | PointClass.Class3.toType();
    public static int ALL_CLASSES = ALL_EVENTS | PointClass.Class0.toType();
}
