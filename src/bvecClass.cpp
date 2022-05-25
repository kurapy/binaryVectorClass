#ifndef BVECCLASS
#define BVECCLSSS

#include "../include/bvec.hpp"

#include <iostream>
#include <bitset>

//ここからソースコード

void bvecand(bvec& x, const bvec& a, const bvec& b)
{
  #ifdef VEC_RANGE_CHECK
   if(a.len()!=b.len()) std::cout<<"notice:not the same length (bvecand)";
  #endif
  
  int alen=a.len(),blen=b.len();
  if(alen!=blen){
    if(alen>blen){
      for(int i=blen;i<alen;++i){
        x[i]=0;
      }
      alen=blen;
    }else{
      for(int i=alen;i<blen;++i){
        x[i]=0;
      }
    }
  }
  for(int i=0;i<alen;++i){
    if(a[i]&b[i]) x[i]=1;
    else x[i]=0;
  }
}

void bvecor(bvec& x, const bvec& a, const bvec& b)
{
  #ifdef VEC_RANGE_CHECK
   if(a.len()!=b.len()) std::cout<<"notice:not the same length (bvecor)";
  #endif
  int alen=a.len(),blen=b.len();
  if(alen!=blen){
    if(alen>blen){
      for(int i=blen;i<alen;++i){
        x[i]=a[i];
      }
      alen=blen;
    }else{
      for(int i=alen;i<blen;++i){
        x[i]=b[i];
      }
    }
  }
  for(int i=0;i<alen;++i){
    if(a[i]|b[i]) x[i]=1;
    else x[i]=0;
  }
}

void bvecxor(bvec& x, const bvec& a, const bvec& b)
{
  //要素数が違う場合0とのXORをとってる
  int alen=a.len(),blen=b.len();
  if(alen!=blen){
    if(alen>blen){
      for(int i=blen;i<alen;++i){
        x[i]=a[i]^0;
      }
      alen=blen;
    }else{
      for(int i=alen;i<blen;++i){
        x[i]=b[i]^0;
      }
    }
  }
  for(int i=0;i<alen;++i){
    if(a[i]^b[i]) x[i]=1;
    else x[i]=0;
  }
}

void mul(bvec& x, const bool a, const bvec& b)
{
  int n=b.len();
  for(int i=0;i<n;++i){
    if(a^b[i]) x[i]=1;
    else x[i]=0;
  }
}

void mulGF2m(bvec& x, const bvec& a, const bvec& b)
{
  int alen=a.len(),blen=b.len();
  x.setLength(alen+blen-1);
  x.reset();

  for(int i=0;i<blen;++i){
    for(int j=0;j<alen;++j){
      if(a[j]&b[i]) x[j+i]^=1;
      else x[j+i]^=0;
    }
  }
}

void shiftR(bvec& x, const bvec& a, const int shift)
{
  int n=a.len()-shift;
  for(int i=0;i<n;++i){
    x[i]=a[i+shift];
  }
  for(int i=n,n=a.len();i<n;++i){
    x[i]=0;
  }
}

void shiftL(bvec& x, const bvec& a, const int shift)
{
  int xlen=x.len(),alen=a.len();
  if(xlen+shift>alen){
    // 代入先のほうが大きい
    for(int i=0;i<shift;++i){
      x[i]=0;
    }
    for(int i=0;i<alen;++i){
      x[i+shift]=a[i];
    }
    for(int i=alen+shift;i<xlen;++i){
      x[i]=0;
    }
  }else{
    // 代入先のほうが小さい(切り捨てあり)
    for(int i=0;i<shift;++i){
      x[i]=0;
    }
    for(int i=shift;i<xlen;++i){
      x[i]=a[i-shift];
    }
  }
}

void rem(bvec& q, const bvec& a, const bvec& b)
{
  q.setLength(a.len());
  q=a;
  int offset=b.countl()+a.len()-b.len();

  while(true){
    int shift=offset-q.countl();
    if(shift<0) break;
    q^=b<<shift;
  }
  return;
}

bool innnerproduct(bool& x,const bvec& a,const bvec& b)
{
  int n=a.len();
  #ifdef VEC_RANGE_CHECK
    if(n!=b.len()) throw("innnerproduct: length error");
  #endif
  for(int i=0;i<n;++i){
    x^=a[i]&b[i];
  }
  return x;
}

void bvec::set()
{
  for(int i=0;i<_len;++i) ptr[i]=1;
}

void bvec::reset()
{
  for(int i=0;i<_len;++i) ptr[i]=0;
}

void bvec::setRnd()
{
  uint_fast64_t rndval;
  for(int i=_len-1,j=0;i!=-1;--i,--j){
    if(j==0){
      #pragma omp critical
        rndval=myrnd.getrndul();
      j=64;
    }
    ptr[i]=rndval&1;
    rndval>>=1;
  }
}

void bvec::setulong(unsigned int val)
{
  int l=_len;
  if(l>32) l=32;
  for(int i=0;i<l;++i){
    ptr[i]=val&1;
    val>>=1;
  }
}

unsigned int bvec::toulong()
{
  int l=_len;
  unsigned int val=0;
  if(l>32) l=32;
  for(int i=0;i<l;++i){
    val+=ptr[i]*(1<<i);
  }
  return val;
}

int bvec::weight()
{
  int c=0;
  for(int i=0;i<_len;++i) if(ptr[i]==1) c++;
  return c;
}

bool bvec::isZero()
{
  for(int i=0;i<_len;++i) if(ptr[i]==1) return 0;
  return 1;
}

int bvec::countl() const
{
  int c=0;
  for(int i=_len-1;i>=0;--i){
    if(ptr[i]==0) c++;
    else break;
  }
  return c;
}

bvec& bvec::append(const bvec& v)
{
  int vlen=v.len();
  if(ptr!=nullptr){
    int thislen=_len;
    this->setLength(thislen+vlen);
    for(int i=0;i<vlen;++i){
      ptr[i+thislen]=v[i];
    }
  }else{
    this->setLength(vlen);
    for(int i=0;i<vlen;++i){
      ptr[i]=v[i];
    }
  }
  return *this;
}

bvec bvec::slice(int begin,int end)
{
#ifdef VEC_RANGE_CHECK
  if(end<begin||end>_len) throw("slice: length error");
#endif
  int newlen=end-begin+1;
  bvec newvec(newlen);
  for(int i=0;i<newlen;++i){
    newvec[i]=ptr[begin+i];
  }
  return newvec;
}


void bvec::print(std::ostream& s=std::cout)
{
  for (int i=_len-1;i>=0;) {
    if(ptr[i]==0)s<<'0';
    else s<<'1';
    if((i--)%8==0)s<<' ';//デクリメント前の値が評価値になる
  }
  std::cout<<std::endl;
}

void bvec::printr(std::ostream& s=std::cout)
{
  for (int i=0;i<_len;) {
    if(ptr[i]==0)s<<'0';
    else s<<'1';
    if((++i)%8==0)s<<' ';
  }
  std::cout<<std::endl;
}

unsigned char bvec::getHex(const int i)
{
  unsigned char c;
  int byte=i*8;
  for(int j=0;j<8;++j){
    c+=ptr[byte+j]*(1<<j);
  }
  return c;
}
#endif