#include "Utils.hpp"

// The macro plots either TH1 with the same name but contained
// in different files, or TH1 with different name, but in the
// same root file.
std::vector<TString> root_files   =  {
};

// If any subfolder, name can be "subfolder1/th1_name;1"
std::vector<TString> histo_names = {
};

// Leave it empty if the th1_names are already descriptive
std::vector<TString> histo_titles = {
};

// Settings
TString title_x = "xx";
TString title_y = "yy";

double fill_alpha = 0.; // Opacity of filling color
bool show_marker  = 1;

double min_y = 1.; // If using log_y, set it > 0

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

  std::vector<TH1*> histos;
  std::vector<TEfficiency*> efficiencies;

  for (size_t i = 0; i < n_graph; i++) {
    TString file_name = loop_on_files ? root_files[i] : root_files[0];
    TFile* input_file = TFile::Open(file_name);
    if (!input_file || input_file->IsZombie()) continue;

    TString obj_name = loop_on_tgraphs ? histo_names[i] : histo_names[0];
    TObject* obj = input_file->Get(obj_name);
    if (!obj) {
      std::cout << "Object " << obj_name << " not found in " << file_name << std::endl;
      continue;
    }

    if (obj->InheritsFrom(TH1::Class())) {
      TH1* h = static_cast<TH1*>(obj);
      if (!h) continue;

      make_histo_cute(h, line_width, i, fill_alpha, show_marker);
      if (!histo_titles.empty()) h->SetTitle(histo_titles[i]);

      histos.push_back(h);

      if (x_min > h->GetXaxis()->GetXmin()) x_min = h->GetXaxis()->GetXmin();
      if (x_max < h->GetXaxis()->GetXmax()) x_max = h->GetXaxis()->GetXmax();
      if (y_min > h->GetMinimum()) y_min = h->GetMinimum();
      if (y_max < h->GetMaximum()) y_max = h->GetMaximum();
    }
    else if (obj->InheritsFrom(TEfficiency::Class())) {
      TEfficiency* eff = static_cast<TEfficiency*>(obj);
      if (!eff) continue;

      make_histo_cute(eff, line_width, i, fill_alpha, show_marker);
      if (!histo_titles.empty()) eff->SetTitle(histo_titles[i]);

      efficiencies.push_back(eff);


      auto* htot = eff->GetTotalHistogram();
      if (htot) {
        if (x_min > htot->GetXaxis()->GetXmin()) x_min = htot->GetXaxis()->GetXmin();
        if (x_max < htot->GetXaxis()->GetXmax()) x_max = htot->GetXaxis()->GetXmax();
      }

      y_min = 0.0;
      y_max = 1.05;
    }
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
 
  for (auto& h : histos) {
    CenterTitles(h);
    if (show_marker) h->Draw("hist same E2");
    else h->Draw("hist same");
  }

  for (auto& eff : efficiencies) {
    if (show_marker) eff->Draw("same E2");
    else eff->Draw("same");
    gPad->Update();
    auto* h_axis = eff->GetPaintedGraph()->GetHistogram();
    if (h_axis) {
      h_axis->GetXaxis()->CenterTitle();
      h_axis->GetYaxis()->CenterTitle();
    }
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
