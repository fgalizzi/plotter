#include "DUNEStyle.h"

#include <TFile.h>

TString root_file   =  "ignore_data/NoiseRun_27565.root";
TString tgraph_name =  "Channel_11147";

// Settings
TString title_x = "Dynamic range [p.e.]";
TString title_y = "SNR";

bool log_x  = false;
bool log_y  = false;
bool grid_h = true;
bool grid_v = true;

using namespace dunestyle;

void TGraph_line(){
  SetDuneStyle();

  // TFile input_file(root_file, "READ");
  // TGraph* g = nullptr;
  TCanvas* gc = new TCanvas(root_file, root_file, 0, 0, 800, 600);
  // input_file.GetObject(tgraph_name, g);
  std::vector<double> x = {208.738, 250.099,306.868,380.020,453.811,558.627,698.333,830.258,1278.72,1531.79,1938.5};

  std::vector<double> y = {
7.38449,
7.02538,
7.2273,
7.00522,
6.97921,
7.27842,
7.24258,
6.79223,
5.2799,
4.76192,
4.07069,
  };

  TGraph* g = new TGraph(y.size(),&x[0],&y[0]);
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
