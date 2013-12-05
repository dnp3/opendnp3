namespace DNP3.Interface
{
  /// <summary>
  /// An enumeration of result codes received from an outstation in response to command request.
  /// These correspond to those defined in the DNP3 standard
  /// </summary>
  public enum CommandStatus : byte
  {
    /// <summary>
    /// command was successfully received and handled
    /// </summary>
    SUCCESS = 0,
    /// <summary>
    /// command timed out before completing
    /// </summary>
    TIMEOUT = 1,
    /// <summary>
    /// command requires being selected before operate, configuration issue
    /// </summary>
    NO_SELECT = 2,
    /// <summary>
    /// bad control code or timing values
    /// </summary>
    FORMAT_ERROR = 3,
    /// <summary>
    /// command is not implemented
    /// </summary>
    NOT_SUPPORTED = 4,
    /// <summary>
    /// command is all ready in progress or its all ready in that mode
    /// </summary>
    ALREADY_ACTIVE = 5,
    /// <summary>
    /// something is stopping the command, often a local/remote interlock
    /// </summary>
    HARDWARE_ERROR = 6,
    /// <summary>
    /// the function governed by the control is in local only control
    /// </summary>
    LOCAL = 7,
    /// <summary>
    /// the command has been done too often and has been throttled
    /// </summary>
    TOO_MANY_OPS = 8,
    /// <summary>
    /// the command was rejected because the device denied it or an RTU intercepted it
    /// </summary>
    NOT_AUTHORIZED = 9,
    /// <summary>
    /// 10 to 126 are currently reserved
    /// </summary>
    UNDEFINED = 127
  }
}
