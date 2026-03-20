#pragma once

#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/core.hpp>

namespace Logger {

inline void init(const std::string& filename) {
    namespace logging  = boost::log;
    namespace keywords = boost::log::keywords;
    namespace expr     = boost::log::expressions;

    logging::add_common_attributes();

    // File sink — all severity levels
    logging::add_file_log(
        keywords::file_name  = filename,
        keywords::format     = "[%TimeStamp%] [%Severity%] %Message%",
        keywords::auto_flush = true
    );

    // Console sink — info and above only
    logging::add_console_log(
        std::cout,
        keywords::format = "[%Severity%] %Message%",
        keywords::filter = logging::trivial::severity >= logging::trivial::info
    );

    logging::core::get()->set_filter(
        logging::trivial::severity >= logging::trivial::debug
    );
}

} // namespace Logger

#define LOG_TRACE   BOOST_LOG_TRIVIAL(trace)
#define LOG_DEBUG   BOOST_LOG_TRIVIAL(debug)
#define LOG_INFO    BOOST_LOG_TRIVIAL(info)
#define LOG_WARNING BOOST_LOG_TRIVIAL(warning)
#define LOG_ERROR   BOOST_LOG_TRIVIAL(error)
