#include "Logger.h"

#include <iostream>

#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>

using std::cout;
using std::endl;


Mylogger * Mylogger::_pInstance = nullptr;
 
Mylogger::Mylogger()
: _mycat(Category::getRoot().getInstance("downc"))
{
	auto ptnLayout1 = new PatternLayout();
	ptnLayout1->setConversionPattern("%d %c [%p] %m%n");

	auto pOs = new OstreamAppender("cout", &cout);
	pOs->setLayout(ptnLayout1);

	_mycat.addAppender(pOs);
	_mycat.setPriority(Priority::WARN);

}

Mylogger::~Mylogger()
{
	Category::shutdown();
}
 
void Mylogger::setLogFile(const char * filepath)
{
	RollingFileAppender * RAppender = new RollingFileAppender("Rollingfile", filepath);
	PatternLayout * pLayout = new PatternLayout();
	pLayout->setConversionPattern("%d [%c] [%p] %m%n");
	RAppender->setLayout(pLayout);
	_mycat.addAppender(RAppender);
}

void Mylogger::setLogLevel(Priority::Value priority)
{
	_mycat.setPriority(priority);
}

void Mylogger::warn(const char * msg)
{
	_mycat.warn(msg);
}

void Mylogger::info(const char * msg)
{
	_mycat.info(msg);
}

void Mylogger::debug(const char * msg)
{
	_mycat.debug(msg);
}

void Mylogger::error(const char * msg)
{
	_mycat.error(msg);
}
