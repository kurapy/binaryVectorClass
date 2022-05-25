#ifndef RAND__CLASS
#define RAND__CLASS

#include <iostream>
#include <ctime>
#include <random>
#include <vector>

#ifndef PI
#define PI 3.14159265358979
#endif

class MyRnd{
//private:
public:
  std::random_device seed_gen;
  std::mt19937_64 mt;
  MyRnd(): mt((std::random_device{}.entropy() == 0.0)? std::time(NULL) : seed_gen()),dist(0,1),distRad(0,2*PI)
  {
    //
    if(std::random_device{}.entropy() == 0.0){
      //std::cout<<"notice: random_device{}.entropy()==0"<<std::endl;
    }
  }
  std::uniform_real_distribution<double> dist;
  std::uniform_real_distribution<double> distRad;

  //(0,1)uniform distribution
  void uniform01(std::vector<double>& vec){
      for(auto& e:vec) e=dist(mt);
  }
  
  //(0,1)uniform distribution
  double uniform01(){
    return dist(mt);
  }

  //(0,2*pi)uniform distribution
  void uniform02pi(std::vector<double>& vec){
    for(auto& e:vec) e=distRad(mt);
  }
  double getrnd(){
    return mt();
  }
  uint_fast64_t getrndul(){
    return mt();
  }
};

//defined grobal scope
MyRnd myrnd;

#endif