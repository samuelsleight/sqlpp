#ifndef SQLLIB_DEFINES_HPP
#define SQLLIB_DEFINES_HPP

#ifdef _MSC_VER
#ifdef SQLLIB_EXPORT
#define SQLLIB_API __declspec(dllexport)
#else
#define SQLLIB_API __declspec(dllimport)
#endif
#else
#define SQLLIB_API
#endif

#define SQLLIB_NS_NAME sqlpp
#define SQLLIB_SQLITE3_NS_NAME sqlite3

#define SQLLIB_NS_(thing) ::SQLLIB_NS_NAME::thing
#define SQLLIB_SQLITE3_NS_(thing) ::SQLLIB_NS_NAME::SQLLIB_SQLITE3_NS_NAME::thing

#define SQLLIB_NS namespace SQLLIB_NS_NAME {
#define SQLLIB_NS_END }

#define SQLLIB_SQLITE3_NS namespace sqlite3 {
#define SQLLIB_SQLITE3_NS_END }

#endif
