#include "StdAfx.h"
#include "Script.h"

#define TO_COMMAND(val, T) \
	else if(_tcsncmp(pChar, CN_##T, wcslen(CN_##T)) == 0)\
	*##val = ##T;

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
	while((ch = script[pos++])!= '\n' && pos <= scriptSize)
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
	lastCommand = UNKNOW;
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
			if(command == CHANGE_MAP || command == FIND_ENEMY
				|| command == BACK_TO_CITY || command == TIME
				|| command == TALK || command == TEST || command == START_GAME)
			{
				//CHANGE_MAP 第一个是走路坐标，第二个是转图后坐标
				//FIND_ENEMY 限制坐标
				//BACK_TO_CITY 登出目标坐标
				//TIME 时间间隔
				if(split(nextLine, &cmdIdx, tmp))
				{
					parseCoordinate(tmp, &x, &y);
				}
			}

			//如果是对话或者治疗
			if(command == TALK || command == HEAL || command == CHANGE_MAP 
				|| command == FIND_ENEMY || command == SALE 
				|| command == BACK_TO_CITY || command == TIME
				|| command == TEST || command == START_GAME)
			{
				if(split(nextLine, &cmdIdx, tmp))
				{
					//解析坐标
					return parseCoordinate(tmp, &targetX, &targetY);
				}
			}
			return TRUE;
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
	if(FALSE);
	TO_COMMAND(commandOut, CHANGE_MAP)
	TO_COMMAND(commandOut, HEAL)
	TO_COMMAND(commandOut, TALK)
	TO_COMMAND(commandOut, AGAIN)
	TO_COMMAND(commandOut, FIND_ENEMY)
	TO_COMMAND(commandOut, LOGOUT)
	TO_COMMAND(commandOut, BACK_TO_CITY)
	TO_COMMAND(commandOut, AUTO_FIGHT)
	TO_COMMAND(commandOut, SALE)
	TO_COMMAND(commandOut, TIME)
	TO_COMMAND(commandOut, TEST)
	TO_COMMAND(commandOut, CLOSE_GAME)
	TO_COMMAND(commandOut, START_GAME)
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
	lastCommand = command;
	lastX = x;
	lastY = y;
	x = 0;
	y = 0;
	targetX = 0;
	targetY = 0;
	command = UNKNOW;
}
