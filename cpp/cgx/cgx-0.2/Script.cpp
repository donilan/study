#include "StdAfx.h"
#include "Script.h"

#define NUMBERS __NUMBERS

const TCHAR* __NUMBERS = TEXT("0123456789");

CScript::CScript()
{
	charLength = 0;
	this->script = NULL;
	pos = 0;
}
CScript::CScript(TCHAR* script, int size)
{
	pos = 0;
	loadScript(script, size);
}

CScript::~CScript(void)
{
	if(this->script) delete[] script;
}


void CScript::loadScript(TCHAR* s, const int size)
{
	TRACE("Loading scripts...\n");
	charLength = size;
	this->script = new TCHAR[size];
	wcscpy(this->script, s);
	TRACE("Loaded scripts\n");
}

int CScript::parse(void)
{
	TCHAR ch = 0;
	TCHAR buff[1024] = {0};
	int idx = 0;
	for(UINT i = 0; i < wcslen(script); ++i)
	{
		//ch = script[i];

	}
	return 0;
}

BOOL CScript::nextStep(int* command, LPVOID lpVoid1, LPVOID lpVoid2)
{
	TCHAR ch = 0;
	TCHAR buff[512] = {0};
	int i = 0;
	while((ch != TEXT('\n')) && i < 512)
	{
		ch = script[pos++];
		buff[i++] = ch;
	}
	if(i == 512) {
		// error
		return FALSE;
	}
	TRACE("Command: %s\n", buff);
	for(i = 0; i < wcslen(NUMBERS); ++i)
	{
		if(buff[0] == NUMBERS[i])
		{
			*command = WALK;
			walkCommand(buff, lpVoid1, lpVoid2);
			break;
		}
	}
	return TRUE;
}

void CScript::walkCommand( TCHAR* buff, LPVOID lpVoid1, LPVOID lpVoid2 )
{
	TCHAR comma = TCHAR(",");
	BOOL foundComma = FALSE;
	int* x = (int*)lpVoid1;
	int* y = (int*)lpVoid2;
	*x = 0;
	*y = 0;
	for(UINT i = 0; i < wcslen(buff); ++i) {
		if(comma == buff[i]) {
			foundComma = TRUE;
			continue;
		}
		for(int z = 0; z < wcslen(NUMBERS); ++z)
		{
			if(buff[i] == NUMBERS[z] && foundComma) {
				*y = i + *y * 10;
			} else
				*x = i + *x * 10;
		}
	}
	
}
