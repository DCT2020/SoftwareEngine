// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <iostream>
#include <stdio.h>
#include <tchar.h>



// TODO: reference additional headers your program requires here
#include <Windows.h>


//defines
#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_DELARY(p) if(p != nullptr){ delete[] p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release; p = nullptr;}
