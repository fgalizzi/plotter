#include "TStyle.h"
#include "PlotterUtils.hpp"

// The macro plots either TGraphs with the same name but contained
// in different files, or TGraphs with different name, but in the
// same root file.
// You can easily modify the macro to plot TGraph / TGraphErrors

// ***************************************************************
// *** Files, TGraphs and settings *******************************
TString root_file_name   =  "";

// If any subfolder, name can be "subfolder1/trgraph_name;1"
TString histo2D_name =  "";

// ***************************************************************
// ***************************************************************

// Settings
bool display_opstat = true;
int  OptStat = 1110;

TString title   = "";
TString title_x = "";
TString title_y = "";

bool log_x  = 0;
bool log_y  = 0;
bool log_z  = 0;
bool grid_h = 0;
bool grid_v = 0;

double white_position = 0.5; // Position of the white color in the gradient (0.0 - 1.0)

int rebin_x = 3; // Rebinning in x (no-rebin if 1)
int rebin_y = 3; // Rebinning in y

double x_axis_low = 0.; // Use default if both are 0.
double x_axis_up  = 0.;
double y_axis_low = 0.;
double y_axis_up  = 0.;
double z_axis_low = 0.;
double z_axis_up  = 0.;

void Bidistribution(){
  SetMyStyle();

  if (display_opstat){
    gStyle->SetOptFit(1111); gStyle->SetOptTitle(0);
    gStyle->SetStatX(0.88); gStyle->SetStatY(0.89);
    gStyle->SetOptStat(OptStat);
    gStyle->SetStatFontSize(0.035);
    gStyle->SetStatBorderSize(0);
  }
  else {
    gStyle->SetOptStat(0000);
  }
  
  TFile* root_file = TFile::Open(root_file_name, "READ");
  TH2F* h2_distr = nullptr;
  root_file->GetObject(histo2D_name, h2_distr);
  if (!h2_distr){
    std::cout << "Try to change TH2D->TH2F..." <<  std::endl;
    return;
  }


  if (h2_distr->GetXaxis()->GetTitle() != title_x && title_x != "") {
    h2_distr->GetXaxis()->SetTitle(title_x);
  }
  if (h2_distr->GetYaxis()->GetTitle() != title_y && title_y != "") {
    h2_distr->GetYaxis()->SetTitle(title_y);
  }
  if (h2_distr->GetTitle() != title && title != "") {
    h2_distr->SetTitle(title);
  }
  CenterTitles(h2_distr);
  
  if(rebin_x>1) h2_distr->RebinX(rebin_x);
  if(rebin_y>1) h2_distr->RebinY(rebin_y);
  
  if(x_axis_low!=0. || x_axis_up!=0.)
    h2_distr->GetXaxis()->SetRangeUser(x_axis_low, x_axis_up);
  if(y_axis_low!=0. || y_axis_up!=0.)
    h2_distr->GetYaxis()->SetRangeUser(y_axis_low, y_axis_up);
  if(z_axis_low!=0. || z_axis_up!=0.)
    h2_distr->GetZaxis()->SetRangeUser(z_axis_low, z_axis_up);
  

  // Gradient: Sky Blue → White → Orange
  const Int_t NColors = 999; // Number of colors in the palette
  Int_t palette[NColors];

  const Int_t nStops = 3;
  Double_t stops[nStops] = {0.0, white_position, 1.0};

  Double_t red[nStops]   = {0.35, 1.0, 0.90};
  Double_t green[nStops] = {0.70, 1.0, 0.60};
  Double_t blue[nStops]  = {0.90, 1.0, 0.00};

  TCanvas* gc = new TCanvas("gc", "gc", 0, 0, canvas_width, canvas_height);
  gc->cd();
  gc->SetLogy(log_y); gc->SetLogx(log_x); gc->SetLogz(log_z);
  gc->SetGrid(grid_v, grid_h);
  
  h2_distr->SetTitle("");
  h2_distr->GetXaxis()->SetTitleFont(font); h2_distr->GetYaxis()->SetTitleFont(font);
  h2_distr->GetXaxis()->SetTitleSize(title_font_size);    h2_distr->GetYaxis()->SetTitleSize(title_font_size);
  h2_distr->GetXaxis()->SetTitleOffset(title_offset_x); h2_distr->GetYaxis()->SetTitleOffset(title_offset_y);
  h2_distr->GetXaxis()->SetLabelFont(font); h2_distr->GetYaxis()->SetLabelFont(font);
  h2_distr->GetXaxis()->SetLabelSize(label_font_size);    h2_distr->GetYaxis()->SetLabelSize(label_font_size);
  h2_distr->GetXaxis()->SetLabelOffset(label_offset_x); h2_distr->GetYaxis()->SetLabelOffset(label_offset_y);
  
  TColor::CreateGradientColorTable(nStops, stops, red, green, blue, NColors);
  h2_distr->SetContour(NColors);
  h2_distr->Draw("COLZ");
  gc->Modified(); gc->Update();
}
