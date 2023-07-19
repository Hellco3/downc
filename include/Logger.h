#ifndef ___MYLOGGER_H__
#define ___MYLOGGER_H__

#include <log4cpp/Category.hh>
using namespace log4cpp;

class Mylogger
{
public:
	static Mylogger * getInstance()
	{
		if(nullptr == _pInstance) {
			_pInstance = new Mylogger();
		}
		return _pInstance;
	}

	static void destroy()
	{
		if(_pInstance) {
			delete _pInstance;
		}
	}

	void setLogFile(const char * filepath);

	void setLogLevel(Priority::Value priority);

	void warn(const char * msg);

	template <class... Args>
	void warn(const char * msg, Args... args)
	{	
		_mycat.warn(msg, args...);
	}

	void info(const char * msg);

	template <class... Args>
	void info(const char * msg, Args... args)
	{	
		_mycat.info(msg, args...);
	}

	void debug(const char * msg);

	template <class... Args>
	void debug(const char * msg, Args... args)
	{	
		_mycat.debug(msg, args...);
	}

	void error(const char * msg);

	template <class... Args>
	void error(const char * msg, Args... args)
	{	
		_mycat.error(msg, args...);
	}

private:
	Mylogger();
	~Mylogger();

private:
	static Mylogger * _pInstance;
	Category & _mycat; }; 
 
#define addprefix(msg)	std::string("[")\
	.append(__FILE__).append(":")\
	.append(__func__).append(":")\
	.append(std::to_string(__LINE__)).append("] ")\
	.append(msg).c_str()


#define logError(msg, ...) Mylogger::getInstance()->error(addprefix(msg), ##__VA_ARGS__)
#define logInfo(msg, ...) Mylogger::getInstance()->info(addprefix(msg), ##__VA_ARGS__)
#define logDebug(msg, ...) Mylogger::getInstance()->debug(addprefix(msg), ## __VA_ARGS__)
#define logWarn(msg, ...) Mylogger::getInstance()->warn(addprefix(msg), ##__VA_ARGS__)


#endif
