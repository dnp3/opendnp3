package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.*;


public abstract class ExpectedCommand {
    enum Type {
        Crob,
        AoInt16,
        AoInt32,
        AoFloat32,
        AoDouble64
    }

    public static final ExpectedCommand.Type[] ALL_TYPES = ExpectedCommand.Type.values();

    public static ExpectedCommand build(ControlRelayOutputBlock crob, int index) {
        return new CrobCommand(crob, index);
    }

    public static ExpectedCommand build(AnalogOutputInt16 ao, int index) {
        return new AnalogOutputInt16Command(ao, index);
    }

    public static ExpectedCommand build(AnalogOutputInt32 ao, int index) {
        return new AnalogOutputInt32Command(ao, index);
    }

    public static ExpectedCommand build(AnalogOutputFloat32 ao, int index) {
        return new AnalogOutputFloat32Command(ao, index);
    }

    public static ExpectedCommand build(AnalogOutputDouble64 ao, int index) {
        return new AnalogOutputDouble64Command(ao, index);
    }

    @Override
    public abstract boolean equals(Object obj);

    static class CrobCommand extends ExpectedCommand {
        private final ControlRelayOutputBlock crob;
        private final int index;

        public CrobCommand(ControlRelayOutputBlock crob, int index) {
            this.crob = crob;
            this.index = index;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            CrobCommand that = (CrobCommand) o;
            return this.crob.opType.equals(that.crob.opType) &&
                    this.crob.tcc.equals(that.crob.tcc) &&
                    this.crob.clear == that.crob.clear &&
                    this.crob.count == that.crob.count &&
                    this.crob.onTimeMs == that.crob.onTimeMs &&
                    this.crob.offTimeMs == that.crob.offTimeMs &&
                    this.crob.status.equals(that.crob.status) &&
                    this.index == that.index;
        }
    }

    static class AnalogOutputInt16Command extends ExpectedCommand {
        private final AnalogOutputInt16 ao;
        private final int index;

        public AnalogOutputInt16Command(AnalogOutputInt16 ao, int index) {
            this.ao = ao;
            this.index = index;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            AnalogOutputInt16Command that = (AnalogOutputInt16Command) o;
            return this.ao.value == that.ao.value &&
                    this.ao.status.equals(that.ao.status) &&
                    this.index == that.index;
        }
    }

    static class AnalogOutputInt32Command extends ExpectedCommand {
        private final AnalogOutputInt32 ao;
        private final int index;

        public AnalogOutputInt32Command(AnalogOutputInt32 ao, int index) {
            this.ao = ao;
            this.index = index;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            AnalogOutputInt32Command that = (AnalogOutputInt32Command) o;
            return this.ao.value == that.ao.value &&
                    this.ao.status.equals(that.ao.status) &&
                    this.index == that.index;
        }
    }

    static class AnalogOutputFloat32Command extends ExpectedCommand {
        private final AnalogOutputFloat32 ao;
        private final int index;

        public AnalogOutputFloat32Command(AnalogOutputFloat32 ao, int index) {
            this.ao = ao;
            this.index = index;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            AnalogOutputFloat32Command that = (AnalogOutputFloat32Command) o;
            return this.ao.value == that.ao.value &&
                    this.ao.status.equals(that.ao.status) &&
                    this.index == that.index;
        }
    }

    static class AnalogOutputDouble64Command extends ExpectedCommand {
        private final AnalogOutputDouble64 ao;
        private final int index;

        public AnalogOutputDouble64Command(AnalogOutputDouble64 ao, int index) {
            this.ao = ao;
            this.index = index;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;
            AnalogOutputDouble64Command that = (AnalogOutputDouble64Command) o;
            return this.ao.value == that.ao.value &&
                    this.ao.status.equals(that.ao.status) &&
                    this.index == that.index;
        }
    }
}
