#include "DUNEStyle.h"

#include "TFile.h"
#include "TCanvas.h"
#include "TEfficiency.h"

TString root_file   =  "./ignore_data/Mu_Expected_x30cm_yz50cm_fiducial_norefl.root";
// DON'T PUT ";1" and TERMINATE WITH "/"
TString root_folder =  "";
TString tgraph_name =  "HitProb_ExpPe";

bool TEfficiency_to_TGraphErrors = true;

// Settings
TString title_x = "x_axis";
TString title_y = "y_axis";

bool log_x  = true;
bool log_y  = false;
bool grid_h = true;
bool grid_v = false;

using namespace dunestyle;

void TEfficiency_plot(){
  SetDuneStyle();

  TFile input_file(root_file, "READ");
  TEfficiency* g = nullptr;
  input_file.GetObject(root_folder+tgraph_name, g);
  
  TCanvas* gc = new TCanvas(root_file, root_file, 0, 0, 800, 600);
  gc->cd();

  // Modify the Graph according to the settings
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  g->SetLineWidth(4);
  // g->GetXaxis()->CenterTitle();
  // g->GetYaxis()->CenterTitle();

  g->SetLineColor(colors::kOkabeItoOrange);

  g->Draw();

  // input_file.Close();
}
