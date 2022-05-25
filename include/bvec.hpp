#include <iostream>
#include <bitset>
#include <initializer_list>

#include "RndClass.hpp"

#ifndef THROW_ERROR
#define THROW_EEEOR
#include <stdexcept>
inline void throwErr(const char *msg) { throw std::runtime_error(msg); }
#endif

//bitset class
class bvec
{
public:
  //直接アクセスせずlen()を利用する
  int _len;
  bool *ptr;
  //デフォルトコンストラクタ
  bvec():_len(0),ptr(nullptr){
  }
  //bvecコンストラクタ(explicit) 第2引数を設定するとその値に初期化
  explicit bvec(int length,bool set=0): _len(length),ptr(new bool[_len]){
    for(int i=0;i<_len;++i){
      ptr[i]=set;
    }
  }
  //リスト初期化用コンストラクタ 左から順番に0th,1stというビットに割り当てられるので注意
  bvec(std::initializer_list<bool> init): _len(init.size()),ptr(new bool[_len]){
    auto it=init.begin();
    for(int i=0;i<_len;++i,++it){
      ptr[i]=*it;
    }
  }
  //bvecデストラクタ
  ~bvec()
  {
    if(ptr!=nullptr) delete[] ptr;
  }
  //bvecムーブコンストラクタ
  bvec(bvec&& a):_len(a._len){
    if(ptr!=nullptr) delete[] ptr;
    ptr=a.ptr;
    a.ptr=nullptr;
  }

  //bvecコピーコンストラクタ
  bvec(const bvec& a):_len(a.len())
  {
    ptr=new bool[_len];
    for(int i=0;i<_len;++i){
      ptr[i]=a[i];
    }
  }

  void setLength(int newlen){
    if(newlen>0){
      if(ptr!=nullptr){
        bool* newptr= new bool[newlen];
        int min,max;
        if(newlen>_len){
          for(int i=0;i<_len;++i){
            newptr[i]=ptr[i];
          }
          for(int i=_len;i<newlen;++i){
            newptr[i]=0;
          }
        }else{
          for(int i=0;i<newlen;++i){
            newptr[i]=ptr[i];
          }
        }
        delete[] ptr;
        ptr=newptr;
        newptr=nullptr;
      }else{
        ptr= new bool[newlen];
        for(int i=0;i<newlen;++i){
          ptr[i]=0;
        }
      }
      _len=newlen;
    }else{
      _len=0;
      if(ptr!=nullptr) delete[] ptr;
      ptr=nullptr;
    }
  }

  //bvec要素数を取得
  int len() const {return this->_len;}

  //1埋め
  void set();

  //0埋め
  void reset();

  //0or1 ランダム埋め
  void setRnd();

  //uint to bvec
  void setulong(unsigned int val);

  //bvec to ulong
  unsigned int toulong();

  //ハミング重み
  int weight();

  //all-0 vec?
  bool isZero();

  //0の数を左からカウント
  int countl() const;

  //bvec同士の結合
  bvec& append(const bvec& v);

  //bvec要素の部分抽出
  bvec slice(int begin,int end);

  //8ビット区切りで出力 右から0th デフォルトの出力ストリームはcout
  void print(std::ostream& s);

  //8ビット区切りで出力 左から0th デフォルトの出力ストリームはcout
  void printr(std::ostream& s);

  //access per byte
  unsigned char getHex(const int i);

  //要素アクセス(const)(要素同士の和はboolなので+と^を混同しないように)
  const bool operator[](int i) const
  {
  #ifdef VEC_RANGE_CHECK
    if (i < 0 || i >= _len) throwErr("index out of range in Vec");
  #endif
    return this->ptr[i];
  }

  //要素アクセス 参照返しのため代入が可能
  bool& operator[](int i)
  {
  #ifdef VEC_RANGE_CHECK
    if (i < 0 || i >= _len){
      std::cout<<"_len:"<<_len<<" i:"<<i<<std::endl;
      throwErr("index out of range in Vec");
    }
  #endif
    return this->ptr[i];
  }

  //移動代入
  bvec& operator=(bvec&& other) noexcept;

  //コピー代入
  bvec& operator=(const bvec& a);
};

bvec& bvec::operator=(bvec&& other) noexcept
{
  if(this != &other){
    if(ptr!=nullptr) delete[] ptr;
    ptr=other.ptr;
    _len=other._len;
    other.ptr=nullptr;
    other._len=0;
  }
  return *this;
}

bvec& bvec::operator=(const bvec& a)
{
  if(this==&a) return *this;
  #ifdef VEC_RANGE_CHECK
   if(a.len()!=_len) throwErr("bvec op= error");
  #endif
  // 丸めないなら
  // if(a.len()!=_len) this->setLength(a.len());
  // 代入先サイズに丸めるなら
  int l=_len;
  if(a.len()<l) l=a.len(); 
  for(int i=0;i<l;++i){
    this->ptr[i]=a[i];
  }
  return *this;
}

//bvec a == bvec b
bool operator==(const bvec& a, const bvec& b)
{
  if(a.len()!=b.len()) return 0;
  int n=a.len();
  for(int i=0;i<n;++i){
    if(a[i]^b[i]) return 0;
  }
  return 1;
}
bool operator!=(const bvec& a, const bvec& b)
{
  if(a.len()!=b.len()) return 1;
  int n=a.len();
  for(int i=0;i<n;++i){
    if(a[i]!=b[i]) return 1;
  }
  return 0;
}

//add x=a&b (a.len>b.len)
void bvecand(bvec& x, const bvec& a, const bvec& b);

//add x=a|b (a.len>b.len)
void bvecor(bvec& x, const bvec& a, const bvec& b);

//xor x=a^b
void bvecxor(bvec& x, const bvec& a, const bvec& b);

//mul x(bvec)=a(bool)^b(bvec) //!!check
void mul(bvec& x, const bool a, const bvec& b);

//mul polynomial a,b over GF(2^m) x(bvec)=a(bvec)^b(bvec) 
void mulGF2m(bvec& x, const bvec& a, const bvec& b);

//bvec shift
void shiftR(bvec& x, const bvec& a, const int shift);

//bvec shift
void shiftL(bvec& x, const bvec& a, const int shift);

//remain q: a=p*b+q(mod2)
void rem(bvec& q, const bvec& a, const bvec& b);

//innner product return (a,b)(long)
bool innnerproduct(bool& x,const bvec& a,const bvec& b);

inline bvec operator+(const bvec& a,const bvec& b)
{
  bvec x(a.len());
  bvecor(x,a,b);
  return x;
}

inline bvec operator&=(bvec& a,const bvec& b)
{
  bvecand(a,a,b);
  return a;
}

inline bvec operator|=(bvec& a,const bvec& b)
{
  bvecor(a,a,b);
  return a;
}

inline bvec operator^(const bvec& a,const bvec& b)
{
  bvec x(a.len());
  bvecxor(x,a,b);
  return x;
}

inline bvec operator^=(bvec& a,const bvec& b)
{
  bvecxor(a,a,b);
  return a;
}

inline bool operator*(const bvec& a,const bvec& b)
{
  bool x;
  innnerproduct(x,a,b);
  return x;
}

inline bvec operator*(const bool a,const bvec& b)
{
  bvec x(b.len());
  mul(x,a,b);
  return x;
}

inline bvec operator*(const bvec& b,const bool a)
{
  bvec x(b.len());
  mul(x,a,b);
  return x;
}

//(n-vec,n-vec)=1-vec(bool)
inline bool operator*=(const bvec& a,const bvec& b){
  bool x=0;
  innnerproduct(x,a,b);
  return x;
}

//bvec全要素ストリーム出力
std::ostream& operator<<(std::ostream& s, const bvec& a)
{   
   int n =a.len();  
   s<<'['<<' ';
   for (int i=0;i<n;++i) {
      bool c=a[i];
      if(c==0)s<<"0";
      else s<<"1";
      s<<" ";
   }
   s<<']';
   return s;
}

//bvec shift 
inline bvec operator>>(const bvec& a,const int shift)
{
  bvec x(a.len());
  shiftR(x,a,shift);
  return x;
}

//bvec shift 丸めない
inline bvec operator<<(const bvec& a,const int shift)
{
  bvec x(a.len()+shift);
  shiftL(x,a,shift);
  return x;
}

//bvec shift 
inline bvec operator>>=(bvec& a,const int shift)
{
  shiftR(a,a,shift);
  return a;
}

//bvec shift 左辺のサイズに丸めて代入
inline bvec operator<<=(bvec& a,const int shift)
{
  shiftL(a,a,shift);
  return a;
}

//ここまでヘッダー