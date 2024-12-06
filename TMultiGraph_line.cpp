#include <TFile.h>
#include <TMultiGraph.h>
#include <algorithm>
#include <iostream>
#include <string>

#include "DUNEStyle.h"
#include "Rtypes.h"
#include "color_list.hpp"

// The macro plots either TGraphs with the same name but contained
// in different files, or TGraphs with different name, but in the
// same root file

// ***************************************************************
// *** Files, TGraphs and settings *******************************
std::vector<TString> root_files   =  {
  "~/CERN/M1/cb_nov_24/cb/Daphne_StandAlone/FFT_20241204_M3_AFE0_Ch0_Vgain1000_raw.root",
  "~/CERN/M1/cb_nov_24/cb/Daphne_StandAlone/FFT_20241204_M3_AFE0_Ch2_Vgain1000_raw.root",
  "~/CERN/M1/cb_nov_24/cb/Daphne_StandAlone/FFT_20241204_M4_AFE0_Ch1_Vgain1000_raw.root",
  "~/CERN/M1/cb_nov_24/cb/Daphne_StandAlone/FFT_20241204_M4_AFE0_Ch3_Vgain1000_raw.root",
  };


TString root_dir = "";  

std::vector<TString> tgraph_names =  {
  "Graph;1",
  };

std::vector<TString> tgraph_titles = {
  "M3 - AFE 0 - Ch 0",
  "M3 - AFE 0 - Ch 2",
  "M4 - AFE 0 - Ch 1",
  "M4 - AFE 0 - Ch 3",
};

// ***************************************************************
// ***************************************************************

// Settings
TString title_x = "Frequency [MHz]";
TString title_y = "FFT [db]";

bool log_x  = true;
bool log_y  = false;
bool grid_h = true;
bool grid_v = true;

// If (low==0 and up==0) it will set it automatically
double x_axis_low = 0.;
double x_axis_up  = 0.;
double y_axis_low = -72.5;
double y_axis_up  = -44.;

// ***************************************************************
// ***************************************************************

using namespace dunestyle;

void TMultiGraph_line(){
  SetDuneStyle();

  bool loop_on_files, loop_on_tgraphs;
  size_t n_graph = 0;

  if(root_files.size()>tgraph_names.size()){
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = false;
  }
  else{
    n_graph = tgraph_names.size();
    loop_on_files = false;
    loop_on_tgraphs = true;
  }
  TGraph* g[n_graph];
  TMultiGraph* mg = new TMultiGraph();

  TCanvas* gc;
 
  if(loop_on_files==true)   gc = new TCanvas(root_files[0], root_files[0], 0, 0, 800, 600);
  if(loop_on_tgraphs==true) gc = new TCanvas(tgraph_names[0], tgraph_names[0], 0, 0, 800, 600);
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  for(size_t i=0; i<n_graph; i++){
    TString file_name;
    if(loop_on_files==true) file_name = root_files[i];
    else file_name = root_files[0];
    
    TFile input_file(file_name, "READ");

    TString tg_name;
    if(loop_on_tgraphs==true){
      tg_name = tgraph_names[i];
    }
    else{
      tg_name = tgraph_names[0];
    }
    input_file.GetObject(root_dir+tg_name, g[i]);
    // Modify the Graph according to the settings
    g[i]->SetLineWidth(4);
    g[i]->SetLineColor(color_list[i%color_list.size()].GetNumber());
    g[i]->SetTitle(tgraph_titles[i]); 
    
    mg->Add(g[i], "l");
    input_file.Close();
  }


  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  mg->GetXaxis()->SetTitle(title_x);
  mg->GetYaxis()->SetTitle(title_y);
  if(x_axis_low!=0. || x_axis_up!=0.)
    mg->GetXaxis()->SetRangeUser(x_axis_low, x_axis_up);
  if(y_axis_low!=0. || y_axis_up!=0.)
    mg->GetYaxis()->SetRangeUser(y_axis_low, y_axis_up);
  mg->Draw("al");

  gc->BuildLegend();
  gc->SetCanvasSize(1100, 800);
  gc->SetWindowSize(1100, 800);
  gc->Modified(); gc->Update();
}
