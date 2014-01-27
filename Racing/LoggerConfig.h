#pragma once
#include "stdafx.h"

namespace logconfig{
	enum severity_level
	{
		log_debug,
		log_info,
		log_warning,
		log_error,
		log_critical
	};
	
	
	static boost::log::sources::severity_logger< severity_level > error_lg(boost::log::keywords::severity = log_info);
}

#define LOG_DEBUG BOOST_LOG_SEV(logconfig::error_lg, logconfig::log_debug) << __FILE__ << ' '
#define LOG_INFO BOOST_LOG_SEV(logconfig::error_lg, logconfig::log_info) << __FILE__ << ' '
#define LOG_WARN BOOST_LOG_SEV(logconfig::error_lg, logconfig::log_warning) << __FILE__ << ' '
#define LOG_ERR BOOST_LOG_SEV(logconfig::error_lg, logconfig::log_error) << __FILE__ << ' '
#define LOGG_CRIT BOOST_LOG_SEV(logconfig::error_lg, logconfig::log_critical) << __FILE__ << ' '


