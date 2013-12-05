namespace DNP3.Interface
{
  /// <summary>
  /// Enumeration for log levels
  /// </summary>
  public enum LogLevel : int
  {
    Event = 0x1,
    Error = 0x2,
    Warning = 0x4,
    Info = 0x8,
    Interpret = 0x10,
    Comm = 0x20,
    Debug = 0x40
  }
}
