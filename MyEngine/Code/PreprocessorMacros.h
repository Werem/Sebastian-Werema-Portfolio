#ifndef PREPROCESSOR_MACROS
#define PREPROCESSOR_MACROS

static const float nearZ = 1.0f;

#include <Windows.h>

#define DEBUG_MODE //Controls debug message types

#define SIMPLE_MESSAGEBOX_ERROR(a) MessageBox(NULL, a, NULL, MB_OK); return false

#ifdef DEBUG_MODE
	#define DEBUG_LINE_AND_FILE { \
	wchar_t messageInt[10]; \
	_itow_s(__LINE__, messageInt, 10); \
	MessageBox(NULL, __FILEW__ , messageInt, MB_OK); return false; }

	#define IFFALMESSAGE(a, b) if(!(a)) DEBUG_LINE_AND_FILE
	#define HREIFMESSAGE(b) if(FAILED(hResult)) DEBUG_LINE_AND_FILE
#else
	#define DEBUG_LINE_AND_FILE return false
	#define IFFALMESSAGE(a, b) if(!a) SIMPLE_MESSAGEBOX_ERROR(b)
	#define HREIFMESSAGE(b) if(FAILED(hResult)) SIMPLE_MESSAGEBOX_ERROR(b)
#endif

#define IFFAL(a) if(!(a)) DEBUG_LINE_AND_FILE
#define HREIF if(FAILED(hResult)) DEBUG_LINE_AND_FILE
#define IFEXIST(a) if(!a) DEBUG_LINE_AND_FILE

#define SHUTDOWN(a) if(a) a->Shutdown()
#define ReleaseCOM(a) if(a) a->Release()

#define ZMEM(a) ZeroMemory(&a, sizeof(a))

#endif