#include "StdAfx.h"
#include "Script.h"

const TCHAR* _NUMBERS = TEXT("0123456789");
const TCHAR* CN_HEAL = TEXT("治疗");
const TCHAR* CN_CHANGE = TEXT("转图");
const TCHAR* CN_TALK = TEXT("对话");

CScript::CScript()
{
	_init();
}

CScript::CScript(TCHAR* buff)
{
	_init();
	loadScript(buff);
}


CScript::~CScript(void)
{
	if(script) delete[] script;
}


void CScript::loadScript(const TCHAR* s)
{
	if(script) delete[] script;
	scriptSize = wcslen(s);
	int count = scriptSize;
	script = new TCHAR[scriptSize+2];
	wcsncpy_s(script, scriptSize+2, s, count);
}


BOOL CScript::nextStep(void)
{
	int cunt = readNextLine();
	if(cunt == 0)
		return FALSE;
	
	return parseCommand();
}

int CScript::readNextLine()
{
	if(!script) return 0;
	memset(nextLine, 0, sizeof(TCHAR)*LINE_LENGTH);
	TCHAR ch = 0;
	int i = 0;
	while((ch = script[pos++])!= '\n' && pos < scriptSize)
	{
		nextLine[i++] = ch;
	}
	nextLine[i+1] = 0;
	return i;
}


void CScript::_init(void)
{
	script = NULL;
	pos = 0;
	y = 0;
	x = 0;
	lastX = 0;
	lastY = 0;
	targetX = 0;
	targetY = 0;
	command = UNKNOW;
}


void CScript::resetPos(void)
{
	pos = 0;
}



BOOL CScript::parseCommand(void)
{
	resetCommand();
	TCHAR tmp[64] = {0};
	UINT cmdIdx = 0;
	if(wcslen(nextLine) < 1)
		return FALSE;
	TCHAR ch = nextLine[0];
	for(UINT i = 0; i < wcslen(_NUMBERS); ++i)
	{
		if(ch == _NUMBERS[i])
		{
			command = WALK;
			return parseCoordinate(nextLine, &x, &y);
		}
	}
	
	//没有找到命令
	if(split(nextLine, &cmdIdx, tmp))
	{
		//没有找到中文命令
		if(parseChineseCommand(tmp, &command))
		{
			//如果是对话或者治疗
			if(command == TALK || command == HEAL || command == CHANGE_MAP)
			{
				if(split(nextLine, &cmdIdx, tmp))
				{
					//解析坐标
					return parseCoordinate(tmp, &targetX, &targetY);
				}
			}
		}
	}

	command = UNKNOW;
	return FALSE;
}


BOOL CScript::parseCoordinate(TCHAR* pChar, int* _x, int* _y)
{
	BOOL foundOne = FALSE;
	TCHAR ch = 0;
	*_x = 0;
	*_y = 0;
	for(UINT i = 0; i < wcslen(pChar); ++i)
	{
		ch = pChar[i];
		if(ch == ',')
		{
			foundOne = TRUE;
			continue;
		}
		for(UINT z = 0; z < wcslen(_NUMBERS); ++z)
		{
			if(ch == _NUMBERS[z])
			{
				if(foundOne)
					*_y = z + *_y*10;
				else
					*_x = z + *_x*10;
			}
		}
	}
	return TRUE;
}


BOOL CScript::parseChineseCommand(const TCHAR* pChar, COMMANDS* commandOut)
{
	if(_tcsncmp(pChar, CN_CHANGE, wcslen(CN_CHANGE)) == 0)
	{
		*commandOut = CHANGE_MAP;
	}
	else if(_tcsncmp(pChar, CN_HEAL, wcslen(CN_HEAL)) == 0)
	{
		*commandOut = HEAL;
	}
	else if(_tcsncmp(pChar, CN_TALK, wcslen(CN_TALK)) == 0)
	{
		*commandOut = TALK;
	} 
	else 
	{
		*commandOut = UNKNOW;
		return FALSE;
	}
	return TRUE;
}


BOOL CScript::split(const TCHAR* org, UINT* idx, TCHAR* out)
{
	TCHAR ch = 0;
	int cnt = 0;
	for(; *idx < wcslen(org); ++(*idx))
	{
		ch = org[*idx];
		if(ch == ' ')
		{
			++(*idx);
			break;
		}
		out[cnt++] = ch;
	}
	out[cnt] = 0;
	if(wcslen(out) > 0)
		return TRUE;
	return FALSE;
}




void CScript::resetCommand(void)
{
	lastX = x;
	lastY = y;
	x = 0;
	y = 0;
	targetX = 0;
	targetY = 0;
	command = UNKNOW;
}
