#include <TH2.h>
#include <TH3.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

void crime() {

  gStyle->SetLabelSize(0.05, "Y");
  gStyle->SetLabelSize(0.05, "X");
  gStyle->SetLabelSize(0.05, "Z");
  gStyle->SetTitleOffset(1.2, "Y");
  gStyle->SetTitleOffset(1.1, "X");
  gStyle->SetTitleOffset(1.5, "Z");
  gStyle->SetTitleSize(0.04, "Y");
  gStyle->SetTitleSize(0.04, "X");
  gStyle->SetTitleSize(0.04, "Z");
//  gStyle->SetHatchesLineWidth(2);
  gStyle->SetHistLineWidth(2);
  gStyle->SetHistLineColor(1);
  gStyle->SetOptStat(0);
  gStyle->SetPadLeftMargin(0.15);

  f = new TFile("Crime_histos.root", "RECREATE");

  string tmp, time, crime, longitude, latitude,  shour, smin, ssec, date;
  double x, y, hour, min;
  TH1I *times = new TH1I("times","Crimes vs Time;Number of Crimes Reported;Time",48,0.0,24.0);
  TH2I *location = new TH2I("location","Crimes vs Location;Longitude;Latitude;Nuber of Crimes Reported",70,-87.9,-97.6,50,41.6,42.1);

  ifstream file ( "Crimes_-_2001_to_present.csv");

  getline(file, tmp) ; 
  while(file.good())
  {

//cout<<"loop"<<endl;
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
    getline(file, tmp, ',');
    if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
      getline(file, tmp, '\"');
      getline(file, tmp, ',');
    }
    getline(file, crime, ',');
    for(int i=0; i<13; i++){
      getline(file, tmp, ',');
      if(tmp.length()>0 && tmp.at(0)=='\"' && tmp.at(tmp.length()-1)!='\"'){
        getline(file, tmp, '\"');
        getline(file, tmp, ',');
      }
    }
    getline(file, latitude, ',');
    getline(file, longitude, ',');
    //cout<<"x="<<longitude<<", y="<<latitude<<endl;
    getline(file, tmp);
    if(longitude.length()>0 && latitude.length()>0){
      x=atof(longitude.c_str());
      y=atof(latitude.c_str());
      location->Fill(x,y);
    }
    hour=atof(shour.c_str());
    min=atof(smin.c_str());
    if(time=="PM" && hour!=12) hour+=12;
    if(time=="AM" && hour==12) hour=0;
    hour+=min/60;
    times->Fill(hour);


  }

  TCanvas* c = new TCanvas( "c", "", 1);//probably need a pad
  //c->SetLogx(1);

  times->Draw();
  c->Print("time_hist.png");
  //gStyle->SetTitleOffset(2.0, "Y");
  //gStyle->SetTitleOffset(1.6, "X");
  //gStyle->SetTitleOffset(1.8, "Z");
//       h->GetXaxis()->SetTitleOffset(1.4);
  location->SetTitleOffset(1.6, "X");
  location->SetTitleOffset(2.0, "Y");
  location->SetTitleOffset(1.8, "Z");
  location->Draw("lego2");
  c->Print("location_hist.png");

  f->cd();
  f->Write();
}
