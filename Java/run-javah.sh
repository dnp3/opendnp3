"$JAVA_HOME/bin/javah" -jni -d cpp -classpath ./maven/target/classes \
com.automatak.dnp3.impl.ManagerImpl \
com.automatak.dnp3.impl.ChannelImpl \
com.automatak.dnp3.impl.MasterImpl \
com.automatak.dnp3.impl.OutstationImpl

