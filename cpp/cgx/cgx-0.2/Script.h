#pragma once

#define LINE_LENGTH 1024

class CScript
{
public:
	enum COMMANDS {
		UNKNOW, WALK, HEAL, TALK, CHANGE_MAP, AGAIN, 
		FIND_ENEMY, LOGOUT, MAZE, AUTO_FIGHT, BACK_TO_CITY
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

