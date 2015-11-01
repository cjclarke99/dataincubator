#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;
int main(void)
{
  ifstream file ( "nyc311calls.csv");
  string tmp, time, agency, complaint, borough, longitude, latitude, names[1000][3], shour, smin, ssec, date;
  double x, y, agencynums[1000], compvsbor[1000][1000], hour, min, sec, time24h, timediff;
  double averagex=0;
  double averagey=0;
  double averagetime=0;
  double stdevx=0;
  double stdevy=0;
  double stdevtime=0;
  double PI=acos(-1);
  double prevtime=-1;
  int foundcomp, foundagency, foundbor, hours[24], maxhour, minhour, hourint;
  int numagencys=0;
  int numcomps=0;
  int numbors=0;
  int line=0;
  int numwithxy=0;
  int numtimecalcs=0;

  for(int i=0; i<24; i++){
    hours[i]=0;
  }
  getline(file, tmp) ; 
  while(file.good())
  {
    getline(file, tmp, ',');
    if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
      getline(file, tmp, '\"');
      getline(file, tmp, ',');
    }
    getline(file, date, ' ');
    getline(file, shour, ':');
    getline(file, smin, ':');
    getline(file, ssec, ' ');
    getline(file, time, ',');
    getline(file, tmp, ',');
    if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
      getline(file, tmp, '\"');
      getline(file, tmp, ',');
    }
    getline(file, agency, ',');
    getline(file, tmp, ',');
    if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
      getline(file, tmp, '\"');
      getline(file, tmp, ',');
    }
    getline(file, complaint, ',');
    for(int i=0; i<18; i++){
      getline(file, tmp, ',');
      if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
        getline(file, tmp, '\"');
        getline(file, tmp, ',');
      }
    }
    getline(file, borough, ',');
    for(int i=0; i<25; i++){
      getline(file, tmp, ',');
      if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
        getline(file, tmp, '\"');
        getline(file, tmp, ',');
      }
    }
    getline(file, latitude, ',');
    getline(file, longitude, ',');
    getline(file, tmp);
    if(longitude.length()>0 && latitude.length()>0){
      averagex+=atof(longitude.c_str());
      averagey+=atof(latitude.c_str());
      numwithxy++;
      //cout<<"Total x = "<<averagex<<", total y = "<<averagey<<", total num = "<<numwithxy<<endl;
    }
    foundagency=0;
    for(int i=0; i<numagencys; i++){
      if(agency==names[i][0]) {
        agencynums[i]++;
        foundagency=1;
      }
    }
    if(!foundagency){
      names[numagencys][0]=agency;
      agencynums[numagencys]++;
      numagencys++;
    }
    foundcomp=0;
    foundbor=0;
    for(int i=0; i<numcomps; i++){
      for(int j=0; j<numbors; j++){
        if(complaint==names[i][1] && borough==names[j][2]) {
          compvsbor[i][j]++;
          foundcomp=1;
          foundbor=1;
        }
        else if(complaint==names[i][1]) {
          foundcomp=1;
        }
        else if(borough==names[j][2]) {
          foundbor=1;
        }
      }
    }
    if(foundcomp && !foundbor){
      for(int i=0; i<numcomps; i++){
        if(complaint==names[i][1]) {
          names[numbors][2]=borough;
          compvsbor[i][numbors]++;
          numbors++;
        }
      }
    }
    if(!foundcomp && foundbor){
      for(int i=0; i<numbors; i++){
        if(borough==names[i][2]) {
          names[numcomps][1]=complaint;
          compvsbor[numcomps][i]++;
          numcomps++;
        }
      }
    }
    if(!foundcomp && !foundbor){
      names[numcomps][1]=complaint;
      names[numbors][2]=borough;
      compvsbor[numcomps][numbors]++;
      numcomps++;
      numbors++;
    }
    line++;
    if(ssec!="00" || (smin!="00" && smin!="15" && smin!="30" && smin!="45" )){
      hourint=atoi(shour.c_str());
      if(time=="PM" && hourint!=12) hourint+=12;
      if(time=="AM" && hourint==12) hourint=0;
      hours[hourint]++;
      hour=atof(shour.c_str());
      min=atof(smin.c_str());
      sec=atof(ssec.c_str());
      if(time=="PM" && hour!=12) hour+=12;
      if(time=="AM" && hour==12) hour=0;
      time24h=sec+min*60+hour*3600;
      if(prevtime!=-1) {
        timediff=prevtime-time24h;
        if(timediff<0)timediff=86400+timediff;
        if(timediff<7200){
          averagetime+=timediff;
          numtimecalcs++;
        }
      }
      prevtime=time24h;
    }

  }

  int agency1[2]={-1,-1};
  int agency2[2];
  for(int i=0; i<numagencys; i++){
    if(agencynums[i]>agency1[0]){
      agency2[0]=agency1[0];
      agency1[0]=agencynums[i];
      agency2[1]=agency1[1];
      agency1[1]=i;
    }
  }
  cout<<"2nd most popular agency is "<<names[agency2[1]][0]<<" with "<<agency2[0]<<" calls."<<endl;

  double borprobs[numbors];
  for(int j=0; j<numbors; j++){
    borprobs[j]=0;
    for(int i=0; i<numcomps; i++){
      borprobs[j]+=compvsbor[i][j];
    }
    borprobs[j]=borprobs[j]/line;
  }

  double compprobs[numcomps];
  double suprisemax, probAandB, probAgivenB, supriseAB;
  int suprise[2];
  suprise[0]=-1;
  for(int i=0; i<numcomps; i++){
    compprobs[i]=0;
    for(int j=0; j<numbors; j++){
      compprobs[i]+=compvsbor[i][j];
    }
    compprobs[i]=compprobs[i]/line;
    for(int j=0; j<numbors; j++){
      //calc how suprising, compare to max, set max if necessary
      probAandB = compvsbor[i][j]/line;
      probAgivenB = probAandB/borprobs[j];
      supriseAB = probAgivenB/compprobs[i];
      if(supriseAB>suprisemax){
        suprisemax=supriseAB;
        suprise[0]=i;
        suprise[1]=j;
      }
    }
  }
  cout<<"Most suprising complaint is "<<names[suprise[0]][1]<<" in "<<names[suprise[1]][2]<<" with a value of "<<suprisemax<<"."<<endl;

  averagex=averagex/numwithxy;
  averagey=averagey/numwithxy;
  averagetime=averagetime/numtimecalcs;
  prevtime=-1;
  ifstream file2 ( "nyc311calls.csv"); // declare file stream: http://www.cplusplus.com/reference/iostream/ifstream/
  getline(file2, tmp) ; 
  while(file2.good())
  {
    getline(file2, tmp, ',');
    if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
      getline(file2, tmp, '\"');
      getline(file2, tmp, ',');
    }
    getline(file2, date, ' ');
    getline(file2, shour, ':');
    getline(file2, smin, ':');
    getline(file2, ssec, ' ');
    getline(file2, time, ',');
    for(int i=0; i<48; i++){
      getline(file2, tmp, ',');
      if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
        getline(file2, tmp, '\"');
        getline(file2, tmp, ',');
      }
    }
    getline(file2, latitude, ',');
    getline(file2, longitude, ',');
    getline(file2, tmp);
    if(longitude.length()>0 && latitude.length()>0){
      x=atof(longitude.c_str());
      y=atof(latitude.c_str());
      stdevx+=pow(x-averagex,2);
      stdevy+=pow(y-averagey,2);
      //cout<<"Stdev x = "<<stdevx<<", Stdev y = "<<stdevy<<endl;
    }
    if(ssec!="00" || (smin!="00" && smin!="15" && smin!="30" && smin!="45" )){
      hour=atof(shour.c_str());
      min=atof(smin.c_str());
      sec=atof(ssec.c_str());
      if(time=="PM" && hour!=12) hour+=12;
      if(time=="AM" && hour==12) hour=0;
      time24h=sec+min*60+hour*3600;
      if(prevtime!=-1) {
        timediff=prevtime-time24h;
        if(timediff<0)timediff=86400+timediff;
        if(timediff<7200) {
          cout<<timediff<<endl;
          stdevtime+=pow(timediff-averagetime,2);
        }
      }
      prevtime=time24h;
    }
  }
  stdevx=sqrt(stdevx/numwithxy);
  stdevy=sqrt(stdevy/numwithxy);
  //cout<<"x = "<<averagex<<" +/- "<<stdevx<<", y = "<<averagey<<" +/- "<<stdevy<<endl;
  double ninety=1.64485*stdevy-averagey;
  double ten=0.125661*stdevy-averagey;
  double ellipseWidth=acos(pow(sin(averagey*PI/180),2)+pow(cos(averagey*PI/180),2)*cos(stdevx*2*PI/180))*6371;
  double ellipseHeight=acos(sin((averagey-stdevy)*PI/180)*sin((averagey+stdevy)*PI/180)+cos((averagey-stdevy)*PI/180)*cos((averagey+stdevy)*PI/180))*6371;;
  double ellipseArea=PI*ellipseWidth*ellipseHeight;
  cout<<"Distance between area of 90 percentile and 10 percentile is "<<ninety-ten<<" degrees."<<endl;
  cout<<"Area covered by 1 standard deviation is "<<ellipseArea<<"km^2."<<endl;

  stdevtime=sqrt(stdevtime/numtimecalcs);
  int maxtime=-1;
  int mintime=1000000000;
  for(int i=0; i<24; i++){
    if(hours[i]>maxtime){
      maxtime=hours[i];
      maxhour=i;
    }
    if(hours[i]<mintime){
      mintime=hours[i];
      minhour=i;
    }
  }
  cout<<maxhour<<" is the most popular time to call. "<<minhour<<" is the least popular time to call. "<<maxtime-mintime<<" is the difference in the number of calls between the two."<<endl;
  cout<<"Average time between calls is "<<averagetime<<" +/- "<<stdevtime<<"."<<endl;

  return 0;
}
//compaints associated with 2nd most popular agency
  //array of agency and array of num of calls to them
//most suprising complaint conditioned on a borough
  //array of boroughs and complaints, 2d array of numbers for combinations
//distance in degrees latitude between 90% and 10% 
  //calc mean
//sq km of standard dev ellipse
  //calc mean rerun to calc stdev of lat & long
//difference betwen most and least popular hours
  //array for how many calls in each period of 24hr
//std dev in time between calls
  //calc mean rerun to calc stdev of time difference

