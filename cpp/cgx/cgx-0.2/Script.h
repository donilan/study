#pragma once



class CScript
{
public:
	enum Commands {
		WALK
	};
	CScript();
	CScript(TCHAR*, int);
	~CScript(void);

	TCHAR* script;
	int charLength;
	void loadScript(TCHAR* s, const int size);
	int parse(void);

private:
	int pos;
	int step;
public:
	BOOL nextStep(int* command, LPVOID lpVoid1, LPVOID lpVoid2);
	void walkCommand( TCHAR* buff, LPVOID lpVoid1, LPVOID lpVoid2 );
};

