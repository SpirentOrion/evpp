#pragma once

#include "evpp/platform_config.h"

#ifdef __cplusplus
#define GOOGLE_GLOG_DLL_DECL           // 使用静态glog库时，必须定义这个
#define GLOG_NO_ABBREVIATED_SEVERITIES // 没这个编译会出错,传说因为和Windows.h冲突

#include <glog/logging.h>

#ifdef GOOGLE_STRIP_LOG

#if GOOGLE_STRIP_LOG == 0
#define EVPP_LOG_TRACE LOG(INFO)
#define EVPP_LOG_DEBUG LOG(INFO)
#define EVPP_LOG_INFO  LOG(INFO)
#define EVPP_DLOG_TRACE LOG(INFO) << __PRETTY_FUNCTION__ << " this=" << this << " "
#else
#define EVPP_LOG_TRACE if (false) LOG(INFO)
#define EVPP_LOG_DEBUG if (false) LOG(INFO)
#define EVPP_LOG_INFO  if (false) LOG(INFO)
#define EVPP_DLOG_TRACE if (false) LOG(INFO)
#endif

#if GOOGLE_STRIP_LOG <= 1
#define EVPP_LOG_WARN  LOG(WARNING)
#define EVPP_DLOG_WARN LOG(WARNING) << __PRETTY_FUNCTION__ << " this=" << this << " "
#else
#define EVPP_LOG_WARN  if (false) LOG(WARNING)
#define EVPP_DLOG_WARN if (false) LOG(WARNING)
#endif

#define EVPP_LOG_ERROR LOG(ERROR)
#define EVPP_LOG_FATAL LOG(FATAL)

#else
#define EVPP_LOG_TRACE if (false) std::cout << __FILE__ << ":" << __LINE__ << " "
#define EVPP_LOG_DEBUG if (false) std::cout << __FILE__ << ":" << __LINE__ << " "
#define EVPP_LOG_INFO  if (false) std::cout << __FILE__ << ":" << __LINE__ << " "
#define EVPP_LOG_WARN  if (false) std::cout << __FILE__ << ":" << __LINE__ << " "
#define EVPP_LOG_ERROR if (false) std::cout << __FILE__ << ":" << __LINE__ << " "
#define EVPP_LOG_FATAL if (false) std::cout << __FILE__ << ":" << __LINE__ << " "
#define CHECK_NOTnullptr(val) if (false) EVPP_LOG_ERROR << "'" #val "' Must be non nullptr";
#define EVPP_DLOG_TRACE if (false) std::cout
#define EVPP_DLOG_WARN if (false) std::cout
#endif
#endif // end of define __cplusplus

//#ifdef _DEBUG
//#ifdef assert
//#undef assert
//#endif
//#define assert(expr)  { if (!(expr)) { EVPP_LOG_FATAL << #expr ;} }
//#endif