#ifndef LOGGER_HPP
#define LOGGER_HPP

/** @namespace 项目命名空间。*/
namespace _utils
{
    constexpr const char* const BLACK_TEXT    = "\x1b[30m";
    constexpr const char* const RED_TEXT      = "\x1b[31m";
    constexpr const char* const GREEN_TEXT    = "\x1b[32m";
    constexpr const char* const YELLOW_TEXT   = "\x1b[33m";
    constexpr const char* const BLUE_TEXT     = "\x1b[34m";
    constexpr const char* const MAGENTA_TEXT  = "\x1b[35m";
    constexpr const char* const CYAN_TEXT     = "\x1b[36m";
    constexpr const char* const WHITE_TEXT    = "\x1b[37m";
    constexpr const char* const BLACK_BG      = "\x1b[40m";
    constexpr const char* const RED_BG        = "\x1b[41m";
    constexpr const char* const GREEN_BG      = "\x1b[42m";
    constexpr const char* const YELLOW_BG     = "\x1b[43m";
    constexpr const char* const BLUE_BG       = "\x1b[44m";
    constexpr const char* const MAGENTA_BG    = "\x1b[45m";
    constexpr const char* const CYAN_BG       = "\x1b[46m";
    constexpr const char* const WHITE_BG      = "\x1b[47m";
    constexpr const char* const RESET         = "\x1b[0m";    
    constexpr const char* const BOLD          = "\x1b[1m";      
    constexpr const char* const UNDERLINE     = "\x1b[4m"; 

    /**
     * @class 日志记录
     */
    class logger
    {
    };
}

#endif // !LOGGER_HPP
