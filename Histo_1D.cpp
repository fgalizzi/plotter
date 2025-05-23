#include "Utils.hpp"

// The macro plots either TH1 with the same name but contained
// in different files, or TH1 with different name, but in the
// same root file.
std::vector<TString> root_files   =  {
  "../Compute_Likelihood_fiducial_norefl_30fake.root"
};

// If any subfolder, name can be "subfolder1/th1_name;1"
std::vector<TString> histo_names = {
  "h_Fq;1",
  "h_Fq_fake;1",
};

// Leave it empty if the th1_names are already descriptive
std::vector<TString> histo_titles = {
  "Likelihood_{TrueEvent}",
  "Likelihood_{FalseEvent}",
};

// Settings
TString title_x = "-log(Likelihood)";
TString title_y = "Counts";

bool fill_histo = false;
double fill_alpha = 0.3;

double min_y = 1.; // If using log_y, set it > 0

bool log_x  = false;
bool log_y  = true;
bool grid_h = true;
bool grid_v = false;

// If (low==0 and up==0) it will set it automatically
double x_axis_low = 0.;
double x_axis_up  = 0.;
double y_axis_low = 0.;
double y_axis_up  = 0.;

// ***************************************************************
// ***************************************************************

void Histo_1D(){
  SetMyStyle();
  
  double x_min = 1e100;
  double x_max = -1e100;
  double y_min = 1.e100;
  double y_max = -1.e100;

  bool loop_on_files, loop_on_tgraphs;
  size_t n_graph = 0;
  if (root_files.size()>histo_names.size()){
    std::cout << "Looping on files" << std::endl;
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = false;
  }
  else if (root_files.size()<histo_names.size()){
    std::cout << "Looping on tgraphs" << std::endl;
    n_graph = histo_names.size();
    loop_on_files = false;
    loop_on_tgraphs = true;
  }
  else{
    std::cout << "Looping on both" << std::endl;
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = true;
  }

  TH1D* h[n_graph];

  for(size_t i=0; i<n_graph; i++){
    TString file_name;
    if(loop_on_files==true) file_name = root_files[i];
    else file_name = root_files[0];
   
    TFile* input_file = TFile::Open(file_name);

    TString tg_name;
    if(loop_on_tgraphs==true){
      tg_name = histo_names[i];
    }
    else{
      tg_name = histo_names[0];
    }
    input_file->GetObject(tg_name, h[i]);
    if (h[i]==nullptr) {
      std::cout << "TGraph " << tg_name << " not found in file " << file_name << std::endl;
      continue;
    }
    CenterTitles(h[i]);
    h[i]->SetLineWidth(6);
    h[i]->SetLineColor(color_list[i%color_list.size()].GetNumber());
    h[i]->SetMarkerColor(color_list[i%color_list.size()].GetNumber());
    if (fill_histo==true){
      h[i]->SetFillColorAlpha(color_list[i%color_list.size()].GetNumber(), fill_alpha);
    }
    // h[i]->SetMarkerStyle(21); h[i]->SetMarkerColor(kBlack);
    if (histo_titles.size()>0) h[i]->SetTitle(histo_titles[i]); 
    
    if (x_min > h[i]->GetXaxis()->GetXmin()) x_min = h[i]->GetXaxis()->GetXmin();
    if (x_max < h[i]->GetXaxis()->GetXmax()) x_max = h[i]->GetXaxis()->GetXmax();
    if (y_min > h[i]->GetMinimum()) y_min = h[i]->GetMinimum();
    if (y_max < h[i]->GetMaximum()) y_max = h[i]->GetMaximum();
  }


  std::cout << x_min << " " << x_max << " " << y_min << " " << y_max << std::endl;

  TCanvas* gc = new TCanvas("gc", "gc", 0, 0, canvas_width, canvas_height);
  gc->cd();

  if (y_min == 0 && log_y) y_min = min_y;
  
  if (!log_x) {
    double dx = x_max - x_min;
    x_min = x_min - dx*0.04;
    x_max = x_max + dx*0.04;
  }
  else {
    x_min = x_min - abs(x_min*0.55);
    x_max = x_max + abs(x_max*0.55);
  }

  if (!log_y) {
    double dy = y_max - y_min;
    y_min = y_min - dy*0.05;
    y_max = y_max + dy*0.05;
  }
  else {
    if (y_min != 0.) y_min = y_min - abs(y_min*0.55);
    y_max = y_max + abs(y_max*0.55);
  }


  auto frame = gc->DrawFrame(x_min, y_min, x_max, y_max);
  frame->GetXaxis()->SetTitle(title_x);   frame->GetYaxis()->SetTitle(title_y); 
  frame->GetXaxis()->CenterTitle();       frame->GetYaxis()->CenterTitle();
  frame->GetXaxis()->SetTitleFont(42);   frame->GetYaxis()->SetTitleFont(42);
  frame->GetXaxis()->SetTitleOffset(1.5); frame->GetYaxis()->SetTitleOffset(1.3);
  frame->GetXaxis()->SetTitleSize(0.04);    frame->GetYaxis()->SetTitleSize(0.04);

  gc->SetLogy(log_y); gc->SetLogx(log_x);
  gc->SetGrid(grid_v, grid_h);
  
  for(int i=0; i<n_graph; i++){
    h[i]->Draw("same");
  }

  auto legend = gc->BuildLegend();
  legend->SetFillStyle(0);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.026);
  legend->SetTextFont(42);
  legend->SetMargin(0.15);
  legend->SetEntrySeparation(0.2);
  auto entries = legend->GetListOfPrimitives();
  auto entry = static_cast<TLegendEntry*>(entries->At(0));
  entries->Remove(entry);
  legend->Draw();
  gc->Modified(); gc->Update();
  return;
}
