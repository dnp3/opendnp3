package com.automatak.dnp3.impl.mocks;

import com.automatak.dnp3.*;
import com.automatak.dnp3.enums.CommandStatus;
import com.automatak.dnp3.enums.OperateType;

import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class QueuedCommandHandler implements CommandHandler {
    final LinkedBlockingQueue<List<ExpectedCommand>> items = new LinkedBlockingQueue<>();
    List<ExpectedCommand> temp = null;

    public List<ExpectedCommand> waitForCommands(int numCommands, Duration duration)
    {
        List<ExpectedCommand> result = new ArrayList<>();
        long startTime = System.currentTimeMillis();
        try
        {
            while(true) {
                long remainingMillis = duration.toMillis() - (System.currentTimeMillis() - startTime);
                if(remainingMillis < 0) return result;

                List<ExpectedCommand> retrievedItems = items.poll(remainingMillis, TimeUnit.MILLISECONDS);
                if(retrievedItems != null) {
                    result.addAll(retrievedItems);
                    if(result.size() >= numCommands) {
                        return result;
                    }
                }
            }
        }
        catch(InterruptedException ex)
        {
            throw new RuntimeException(ex);
        }
    }

    @Override
    public void begin() {
        this.temp = new ArrayList<>();
    }

    @Override
    public void end() {
        this.items.add(this.temp);
        this.temp = null;
    }

    @Override
    public CommandStatus select(ControlRelayOutputBlock command, int index) {
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus select(AnalogOutputInt32 command, int index) {
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus select(AnalogOutputInt16 command, int index) {
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus select(AnalogOutputFloat32 command, int index) {
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus select(AnalogOutputDouble64 command, int index) {
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus operate(ControlRelayOutputBlock command, int index, Database database, OperateType opType) {
        this.temp.add(ExpectedCommand.build(command, index));
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus operate(AnalogOutputInt32 command, int index, Database database, OperateType opType) {
        this.temp.add(ExpectedCommand.build(command, index));
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus operate(AnalogOutputInt16 command, int index, Database database, OperateType opType) {
        this.temp.add(ExpectedCommand.build(command, index));
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus operate(AnalogOutputFloat32 command, int index, Database database, OperateType opType) {
        this.temp.add(ExpectedCommand.build(command, index));
        return CommandStatus.SUCCESS;
    }

    @Override
    public CommandStatus operate(AnalogOutputDouble64 command, int index, Database database, OperateType opType) {
        this.temp.add(ExpectedCommand.build(command, index));
        return CommandStatus.SUCCESS;
    }
}
