#ifndef __LOG_TERMINAL_EXTENSION_H_
#define __LOG_TERMINAL_EXTENSION_H_

#include <APL/Log.h>
#include <APL/LogToFile.h>
#include <APL/MetricBuffer.h>

#include "TerminalInterfaces.h"

namespace apl
{

class IExecutor;
class ITimer;

class LogTerminalExtension : public ITerminalExtension, public INotifier
{
public:
	LogTerminalExtension(EventLog*);

	virtual ~LogTerminalExtension();

	void RedirectedLine(const std::string&);

	// Implements INotifier so that we can process new log messages
	void Notify();

private:

	EventLog* mpLog;
	IExecutor* mpExecutor;

	//implement from ITerminalExtension
	void _BindToTerminal(ITerminal* apTerminal);

	std::string GetLogString(LogEntry& arEntry);
	void ResetActiveColumns();
	void HandleLogTimer();

	enum LogColumns {
	    COL_TIME,
	    COL_FILTER,
	    COL_DEVICE,
	    COL_LOCATION,
	    COL_MESSAGE
	};

	void StopLoggingToFile();

	std::vector<LogColumns> mActiveColumns;
	LogEntryCircularBuffer mBuffer;
	MetricBuffer mMetrics;
	LogToFile* mpFileLogger;

	//print
	retcode HandlePrintLog(std::vector<std::string>&);
	retcode HandlePrintLoggers(std::vector<std::string>&);
	retcode HandlePrintVars(std::vector<std::string>&);
	//run
	retcode HandleRunLog(std::vector<std::string>& arTokens);
	//set
	retcode HandleSetFilterOrLevel(std::vector<std::string>& arTokens, bool aSetLevel);
	retcode HandleSetLogCol(std::vector<std::string>& arTokens);

	retcode HandleStartToFileLogging(std::vector<std::string>& arTokens);
	retcode HandleStopToFileLogging(std::vector<std::string>& arTokens);

	void PrintLog();
};
}

#endif

