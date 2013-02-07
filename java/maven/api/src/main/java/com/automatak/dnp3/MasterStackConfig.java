package com.automatak.dnp3;

/**
 * Highest level configuration class for a master stack
 */
public class MasterStackConfig {

    public final MasterConfig masterConfig;
    public final AppLayerConfig appConfig;
    public final LinkLayerConfig linkConfig;

    /**
     * Constructs all sub-config objects with reasonable defaults
     */
    public MasterStackConfig()
    {
        this.masterConfig = new MasterConfig();
        this.appConfig = new AppLayerConfig();
        this.linkConfig = new LinkLayerConfig(true);
    }
}
