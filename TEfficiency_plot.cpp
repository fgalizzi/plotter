#include "TFile.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include "PlotterUtils.hpp"

TString root_file   =  "";
TString root_folder =  "";
TString tgraph_name =  "";

bool TEfficiency_to_TGraphErrors = false;

// Settings
TString title_x = "x_axis";
TString title_y = "y_axis";

bool log_x  = 0;
bool log_y  = 0;
bool grid_h = 1;
bool grid_v = 0;


void TEfficiency_plot(){
  // SetDuneStyle();

  TFile input_file(root_file, "READ");
  TEfficiency* g = nullptr;
  input_file.GetObject(root_folder+tgraph_name, g);
  
  TCanvas* gc = new TCanvas(root_file, root_file, 0, 0, 800, 600);
  gc->cd();

  // Modify the Graph according to the settings
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  g->SetLineWidth(4);
  CenterTitles(g);
  // g->GetXaxis()->CenterTitle();
  // g->GetYaxis()->CenterTitle();

  // g->SetLineColor(colors::kOkabeItoOrange);

  g->Draw();

  // input_file.Close();
}
