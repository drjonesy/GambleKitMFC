#pragma once

//#if defined (GKDLLCOMPILE)
//#	define GKDLLCLASS __declspec(dllexport)
//#	define GKDLLTEMPLATEMEMBER
//#else
//#	define GKDLLCLASS __declspec(dllimport)
//#	define GKDLLTEMPLATEMEMBER extern
//#endif

#	define GKDLLCLASS
#	define GKDLLTEMPLATEMEMBER
