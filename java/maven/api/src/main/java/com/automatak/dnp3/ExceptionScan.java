package com.automatak.dnp3;



class ExceptionScan
{
        public int classMask;
        public long scanRateMs;

        /**
         * Constructor that provides reasonable defaults
         */
        public ExceptionScan()
        {
            this.classMask = PointClass.ALL_EVENTS.toInt();
            this.scanRateMs = 5000;
        }
}
