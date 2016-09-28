package com.automatak.dnp3;

/**
 * A set of measurement changes that can be applied to a database
 */
public interface ChangeSet
{
    /**
     * Apply the change set
     * @param database The database to which to apply the changes
     */
    void apply(Database database);
}