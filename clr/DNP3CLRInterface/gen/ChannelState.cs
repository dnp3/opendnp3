namespace DNP3.Interface
{
  /// <summary>
  /// Enumeration for possible states of a channel
  /// </summary>
  public enum ChannelState : int
  {
    /// <summary>
    /// offline and idle
    /// </summary>
    CLOSED = 0,
    /// <summary>
    /// trying to open
    /// </summary>
    OPENING = 1,
    /// <summary>
    /// waiting to open
    /// </summary>
    WAITING = 2,
    /// <summary>
    /// open
    /// </summary>
    OPEN = 3,
    /// <summary>
    /// stopped and will never do anything again
    /// </summary>
    SHUTDOWN = 4
  }
}
