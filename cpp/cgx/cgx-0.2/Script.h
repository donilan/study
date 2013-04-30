#pragma once

#define LINE_LENGTH 1024

#define _NUMBERS		TEXT("0123456789")
#define CN_HEAL			TEXT("治疗")
#define CN_CHANGE_MAP	TEXT("转图")
#define CN_TALK			TEXT("对话")
#define CN_AGAIN		TEXT("重来")
#define CN_FIND_ENEMY	TEXT("遇敌")
#define CN_SALE			TEXT("卖魔石")
#define CN_AUTO_FIGHT	TEXT("自动战斗")
#define CN_LOGOUT		TEXT("登出")
#define CN_BACK_TO_CITY TEXT("回城")
#define CN_MAZE			TEXT("迷宫"
#define CN_YES			TEXT("是")
#define CN_NO			TEXT("否")
#define CN_SURE			TEXT("确定")
#define CN_CANCEL		TEXT("取消")

class CScript
{
public:
	enum COMMANDS {
		UNKNOW, WALK, HEAL, TALK, CHANGE_MAP, AGAIN, 
		FIND_ENEMY, LOGOUT, MAZE, AUTO_FIGHT, BACK_TO_CITY, SALE
	};
	CScript();
	CScript(TCHAR*);
	~CScript(void);
	void loadScript(const TCHAR* s);
	BOOL nextStep(void);

	int lastX;
	int lastY;
	int x;
	int y;
	int targetX;
	int targetY;
	COMMANDS lastCommand;
	COMMANDS command;
private:
	TCHAR* script;
	UINT scriptSize;
	TCHAR nextLine[LINE_LENGTH];
	inline int readNextLine();
	UINT pos;

protected:
	void _init(void);
	inline BOOL parseCommand();
	inline BOOL parseCoordinate(TCHAR*, int*, int*);
	inline BOOL parseChineseCommand(const TCHAR* pChar, COMMANDS* commandOut);
	inline BOOL split(const TCHAR* org, UINT* idx, TCHAR* out);

	inline void resetCommand(void);
public:
	void resetPos(void);
};

