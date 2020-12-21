#include <core/Logger.h>

#ifdef _DEBUG
bool _trace(char* format, ...);
#define TRACE _trace
#else
#define TRACE false && _trace
#endif

Shared<Logger> Logger::m_coreLogger = createShared<Logger>();