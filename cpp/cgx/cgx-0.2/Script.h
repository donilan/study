#pragma once

#define LINE_LENGTH 1024

#define _NUMBERS		TEXT("0123456789")
#define CN_HEAL			TEXT("����")
#define CN_CHANGE_MAP	TEXT("תͼ")
#define CN_TALK			TEXT("�Ի�")
#define CN_AGAIN		TEXT("����")
#define CN_FIND_ENEMY	TEXT("����")
#define CN_SALE			TEXT("��ħʯ")
#define CN_AUTO_FIGHT	TEXT("�Զ�ս��")
#define CN_LOGOUT		TEXT("�ǳ�")
#define CN_BACK_TO_CITY TEXT("�س�")
#define CN_MAZE			TEXT("�Թ�"
#define CN_YES			TEXT("��")
#define CN_NO			TEXT("��")
#define CN_SURE			TEXT("ȷ��")
#define CN_CANCEL		TEXT("ȡ��")

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

