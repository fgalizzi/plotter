#include "DUNEStyle.h"

#include <TFile.h>

TString root_file   =  "";
TString tgraph_name =  "";

// Settings
TString title_x = "";
TString title_y = "";

bool log_x  = false;
bool log_y  = false;
bool grid_h = true;
bool grid_v = true;

using namespace dunestyle;

void TGraph_line(){
  SetDuneStyle();

  TFile input_file(root_file, "READ");
  TGraph* g = nullptr;
  TCanvas* gc = new TCanvas(root_file, root_file, 0, 0, 800, 600);
  input_file.GetObject(tgraph_name, g);
  // Modify the Graph according to the settings
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  CenterTitles(g);
  g->GetXaxis()->SetTitle(title_x);
  g->GetYaxis()->SetTitle(title_y);
  g->SetLineWidth(4);
  g->SetLineColor(colors::kOkabeItoOrange);


  g->Draw("AL");

  // input_file.Close();
}
