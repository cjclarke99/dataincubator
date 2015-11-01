#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{
  double N=100000000;
  double x, y, hasbeen5, hasbeen10, hasbeenEast, steps, first;
  double N10m3b=0;//number of times end 3 blocks away after 10 moves
  double N10m5be=0;//number of times 5 blocks away at least once during 10 moves
  double N10mEtoW=0;//number of times went east then ended west after 10 moves
  double N60m10b=0;//number of times end 10 blocks away after 60 moves
  double N60m10be=0;//number of times 10 blocks away at least once during 60 moves
  double N30mEtoW=0;//number of times went east then ended west after 30 moves
  double N10b=0;//total number of moves to get 10 blocks away N times
  double N60b=0;//total number of moves to get 60 blocks away N times
  double direction;// 0=north, 1=east, 2=south, 3=west
  double distance;
  srand (time(NULL));  //initialize random numbers
  for(int j=0; j<N; j++){
    x=0;
    y=0;
    hasbeen5=0;
    hasbeen10=0;
    hasbeenEast=0;
    for(int i=1; i<61; i++){
      direction = rand() % 4;
      if(direction == 0) y++;//north
      else if(direction == 1) x++;//east
      else if(direction == 2) y--;//south
      else if(direction == 3) x--;//west
      distance = sqrt(x*x+y*y);
      //printf("i= %i, x=%i, y=%i\n", i, x, y );
      if(distance>=5) hasbeen5=1;
      if(distance>=10) hasbeen10=1;
      if(y>1) hasbeenEast=1;
      if(i==10){
        if(distance>=3) N10m3b++;
        if(hasbeen5) N10m5be++;
        if(hasbeenEast && y<-1) N10mEtoW++;
      }
      else if(i==30){
        if(hasbeenEast && y<-1) N30mEtoW++;
      }
      else if(i==60){
        if(distance>=10) N60m10b++;
        if(hasbeen10) N60m10be++;
      }
    }
    steps=0;
    x=0;
    y=0;
    first=1;
    while(distance<60){
      direction = rand() % 4;
      if(direction == 0) y++;//north
      else if(direction == 1) x++;//east
      else if(direction == 2) y--;//south
      else if(direction == 3) x--;//west
      distance = sqrt(x*x+y*y);
      steps++;
      //printf("i= %i, x=%i, y=%i\n", steps, x, y );
      if(distance>=10 && first) {
        N10b+=steps;
        first=0;
      }
      if(distance>=60) N60b+=steps;
    }
    //printf("N10b = %f, N60b = %f\n", N10b, N60b );
  }
  double P10m3b = N10m3b/N;//probability of 3 blocks away after 10 moves
  double P10m5be = N10m5be/N;//probability of 5 blocks away at least once during 10 moves
  double P10mEtoW = N10mEtoW/N;//probability of went east then ended west after 10 moves
  double P60m10b = N60m10b/N;//probability of end 10 blocks away after 60 moves
  double P60m10be = N60m10be/N;//probability of 10 blocks away at least once during 60 moves
  double P30mEtoW = N30mEtoW/N;//probability of went east then ended west after 30 moves
  double A10b = N10b/N;//average number of moves to get 10 blocks away
  double A60b = N60b/N;//average number of moves to get 60 blocks away

  printf("Probability of 3 blocks away after 10 moves %.10f\n", P10m3b );
  printf("Probability of 5 blocks away at least once during 10 moves %.10f\n", P10m5be );
  printf("Probability of went east then ended west after 10 moves %.10f\n", P10mEtoW );
  printf("Probability of end 10 blocks away after 60 moves %.10f\n", P60m10b );
  printf("Probability of 10 blocks away at least once during 60 moves %.10f\n", P60m10be );
  printf("Probability of went east then ended west after 30 moves %.10f\n", P30mEtoW );
  printf("Average number of moves to get 10 blocks away %.10f\n", A10b );
  printf("Average number of moves to get 60 blocks away %.10f\n", A60b );

  return 0;
}
