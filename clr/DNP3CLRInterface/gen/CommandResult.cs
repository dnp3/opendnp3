namespace DNP3.Interface
{
  /// <summary>
  /// Opendnp3 API enum used for differentiating cases when a command sequence fails without a response from the outstation
  /// </summary>
  public enum CommandResult : int
  {
    /// <summary>
    /// A response was received from the outstation, check the CommandStatus enumeration
    /// </summary>
    RESPONSE_OK = 0,
    /// <summary>
    /// The operation timed out without a response
    /// </summary>
    TIMEOUT = 1,
    /// <summary>
    /// There is no communication with the outstation, and the command was not attempted
    /// </summary>
    NO_COMMS = 2
  }
}
