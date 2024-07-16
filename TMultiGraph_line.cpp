#include "DUNEStyle.h"

TString root_file   =  "NoiseRun_27565.root";
std::vector<TString> tgraph_name =  {"Channel_11147", "Channel_11145",
  "Channel_11147", "Channel_11145"};

// Settings
TString title_x = "x_axis";
TString title_y = "y_axis";

bool log_x  = true;
bool log_y  = false;
bool grid_h = true;
bool grid_v = true;

using namespace dunestyle;

void TMultiGraph_line(){
  SetDuneStyle();

  TFile input_file(root_file, "READ");
  size_t n_graph = tgraph_name.size();
  TGraph* g[n_graph];
  TMultiGraph* mg = new TMultiGraph();

  TCanvas* gc = new TCanvas(root_file, root_file, 0, 0, 800, 600);
  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  for(size_t i=0; i<n_graph; i++){
    input_file.GetObject(tgraph_name[i], g[i]);
    // Modify the Graph according to the settings
    g[i]->SetLineWidth(4);
    auto col = dunestyle::colors::NextColor();
    g[i]->SetLineColor(col);
    
    mg->Add(g[i], "lp");
  }


  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  mg->GetXaxis()->SetTitle(title_x);
  mg->GetYaxis()->SetTitle(title_y);
  mg->Draw("al");

  gc->Modified(); gc->Update();
  input_file.Close();
}
