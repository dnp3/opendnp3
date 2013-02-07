package com.automatak.dnp3;

public class OutstationStackConfig {

    /**
     * Link layer configuration
     */
    public final LinkLayerConfig linkConfig = new LinkLayerConfig(false);

    /**
     * Application layer configuration
     */
    public final AppLayerConfig appConfig = new AppLayerConfig();

    /**
     * Configuration for the outstation (everything but the database layout)
     */
    public final OutstationConfig outstationConfig = new OutstationConfig();

    /**
     * Outstation database configuration
     */
    public final OutstationDatabaseConfig database;

    /**
     * Constructor for an outstation stack with sensible defaults
     * @param database Database layout for the outstation
     */
    public OutstationStackConfig(OutstationDatabaseConfig database)
    {
        this.database = database;
    }
}
