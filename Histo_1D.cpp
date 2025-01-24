#include "DUNEStyle.h"

#include <TFile.h>

TString root_file   =  "";
TString root_folder =  "";
TString histo_name  =  "";

// Settings
TString title_x = "";
TString title_y = "";

bool log_x  = false;
bool log_y  = false;
bool grid_h = false;
bool grid_v = false;

using namespace dunestyle;

void Histo_1D(){
  SetDuneStyle();

  TFile* input_file = TFile::Open(root_file);
  TH1D* h = nullptr;
  TCanvas* gc = new TCanvas("gc", "gc", 0, 0, 800, 600);
  input_file->GetObject(root_folder+"/"+histo_name, h);


  gc->cd();
  h->Draw();
  gc->Modified(); gc->Update();
  return;
  // Modify the Graph according to the settings
 /* gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  h->SetFillStyle(1000);
  CenterTitles(h);
  h->GetXaxis()->SetTitle(title_x);
  h->GetYaxis()->SetTitle(title_y);
  h->SetLineWidth(4);
  h->SetLineColor(kBlack);
  h->SetMarkerStyle(21); h->SetMarkerColor(kBlack);

  h->Draw();

  input_file.Close();
*/
}
