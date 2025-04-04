#include <TFile.h>
#include <TH1.h>
#include <cstddef>

#include "DUNEStyle.h"
#include "Rtypes.h"
#include "color_list.hpp"

// The macro plots either TH1 with the same name but contained
// in different files, or TH1 with different name, but in the
// same root file.

std::vector<TString> root_files   =  {
  "",
  };
// If any subfolder, name can be "subfolder1/th1_name;1"
std::vector<TString> histo_names = {
  "",
};
// Leave it empty if the th1_names are already descriptive
std::vector<TString> histo_titles = {};

// Settings
TString title_x = "x-axis name";
TString title_y = "y-axes name";

bool fill_histo = false;

bool log_x  = false;
bool log_y  = false;
bool grid_h = false;
bool grid_v = false;

// If (low==0 and up==0) it will set it automatically
double x_axis_low = 0.;
double x_axis_up  = 0.;
double y_axis_low = 0.;
double y_axis_up  = 0.;

// ***************************************************************
// ***************************************************************


using namespace dunestyle;

void Histo_1D(){
  SetDuneStyle();

  bool loop_on_files, loop_on_tgraphs;
  size_t n_graph = 0;
  if(root_files.size()>histo_names.size()){
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = false;
  }
  else{
    n_graph = histo_names.size();
    loop_on_files = false;
    loop_on_tgraphs = true;
  }
  TH1D* h[n_graph];


  for(size_t i=0; i<n_graph; i++){
    TString file_name;
    if(loop_on_files==true) file_name = root_files[i];
    else file_name = root_files[0];
    
    TFile* input_file = TFile::Open(root_files[0]);

    TString tg_name;
    if(loop_on_tgraphs==true){
      tg_name = histo_names[i];
    }
    else{
      tg_name = histo_names[0];
    }
    input_file->GetObject(tg_name, h[i]);
    // Modify the Graph according to the settings
    h[i]->SetLineWidth(5);
    h[i]->SetLineColor(color_list[i%color_list.size()].GetNumber());
    h[i]->SetMarkerColor(color_list[i%color_list.size()].GetNumber());
    if (fill_histo==true){
      h[i]->SetFillStyle(1001);
      h[i]->SetFillColor(color_list[i%color_list.size()].GetNumber());
    }
    CenterTitles(h[i]);
    // h[i]->SetMarkerStyle(21); h[i]->SetMarkerColor(kBlack);
    if (histo_titles.size()>0) h[i]->SetTitle(histo_titles[i]); 
    
    // input_file.Close();
  }

  TCanvas* gc = new TCanvas("gc", "gc", 0, 0, 1200,9900);
  gc->cd();
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  for(size_t i=0; i<n_graph; i++){
    if (i==0) {
      h[i]->GetXaxis()->SetTitle(title_x);
      h[i]->GetYaxis()->SetTitle(title_y);
      h[i]->Draw();
    }
    else {
      h[i]->Draw("same");
    }
  }

  gc->BuildLegend();
  gc->Modified(); gc->Update();
  return;
}
