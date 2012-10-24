#prama once

#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

#ifdef _DBGOUT
ostringstream errs;
#else
extern ostringstream errs;
#endif

template<class _Elem, class _Traits> inline basic_ostream<_Elem, _Traits>&
__CLRCALL_OR_CDECL endd(basic_ostream<_Elem, _Traits>& _Ostr)
{
  ostringstream& oo = (ostringstream&)_Ostr;
  OutputDebugStringA(oo.str(), c_str());
  oo.clear();
  oo.str("");
  return (_Ostr);
}


class errTchar
{
public:
  explicit errTchar(wchar_t* lpstr):lpstr_(lpstr) {}
public:
  wchar_t* lpstr_;
  template<class _Elem, class _Traits>
  friend basic_ostream<_Elem, _Traits>& operator<<(basic_ostream<_Elem, _Traits>& _Ostr, const errTchar& m)
  {
    int len = wcslen(m.lpstr_);
    ++len;
    len *= sizeof(wchar_t);
    WideCharToMultiByte(CP_ACP, 0, m.lpstr_, -1, buf, len, 0, 0);
    _Ostr << buf;
    delete[] buf;
    return (_Ostr);
  }
}
