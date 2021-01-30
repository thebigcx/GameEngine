#include <core/Logger.h>

#ifdef _DEBUG
bool _trace(char* format, ...);
#define TRACE _trace
#else
#define TRACE false && _trace
#endif

namespace Engine
{

Logger* Logger::m_coreLogger = new Logger();

}