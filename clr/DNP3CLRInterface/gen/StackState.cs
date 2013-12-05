namespace DNP3.Interface
{
  /// <summary>
  /// Enumeration for possible communication states of a stack
  /// </summary>
  public enum StackState : int
  {
    /// <summary>
    /// communications are online
    /// </summary>
    COMMS_UP = 0,
    /// <summary>
    /// communication difficulties
    /// </summary>
    COMMS_DOWN = 1,
    /// <summary>
    /// unknown state
    /// </summary>
    UNKNOWN = 2
  }
}
