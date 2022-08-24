#pragma once

#ifdef CR_PLATFORM_WINDOWS
	#ifdef CR_BUILD_DLL
		#define CORONA_API __declspec(dllexport)
	#else
		//#define CORONA_API __declspec(dllimport)
		#define CORONA_API
	#endif
#else
	#error Corona only supports Windows!
#endif