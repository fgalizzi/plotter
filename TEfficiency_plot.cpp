#include "DUNEStyle.h"

TString root_file   =  "Self_FOM.root";
TString root_folder =  "eff";
TString tgraph_name =  "TRIGGER_15";

bool TEfficiency_to_TGraphErrors = true;

// Settings
TString title_x = "x_axis";
TString title_y = "y_axis";

bool log_x  = false;
bool log_y  = false;
bool grid_h = false;
bool grid_v = false;

using namespace dunestyle;

void TEfficiency_plot(){
  SetDuneStyle();

  TFile input_file(root_file, "READ");
  TEfficiency* g = nullptr;
  input_file.GetObject(root_folder+"/"+tgraph_name, g);
  
  TCanvas* gc = new TCanvas(root_file, root_file, 0, 0, 800, 600);
  gc->cd();

  // Modify the Graph according to the settings
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  g->SetLineWidth(4);

  g->SetLineColor(colors::kOkabeItoOrange);

  g->Draw();

  input_file.Close();
}
