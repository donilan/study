// A.h
//

#ifndef LZZ_A_h
#define LZZ_A_h
#define LZZ_INLINE inline
class A
{
public:
  void f (int i);
  void g (int j = 0);
};
LZZ_INLINE void A::f (int i)
                            { return i;}
#undef LZZ_INLINE
#endif
