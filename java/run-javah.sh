javah -jni -d cpp -classpath ./maven/api/target/classes:./maven/bindings/target/classes \
com.automatak.dnp3.impl.ManagerImpl \
com.automatak.dnp3.impl.ChannelImpl \
com.automatak.dnp3.impl.MasterImpl \
com.automatak.dnp3.impl.OutstationImpl \
com.automatak.dnp3.impl.CommandProcessorImpl \
com.automatak.dnp3.impl.DataObserverImpl \
com.automatak.dnp3.impl.StackBase \
com.automatak.dnp3.impl.VTOEndpointImpl

