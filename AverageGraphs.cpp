#include "TFile.h"
#include "TGraphErrors.h"
#include <TGraph.h>
#include <iostream>
#include <ostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>


int N = 1000; // Number of points for the averaged graph
// ******************************************************************
// *** Files, TGraphs and settings **********************************
std::vector<TString> root_files   =  {
  "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_Ch10400.root",
  "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_Ch10402.root",
  "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_Ch10403.root",
};

// If any subfolder, name can be "subfolder1/trgraph_name;1"
std::vector<TString> tgraph_names =  {
  "g_effAt3peFit_ThrCal_it_10400;1",
  "g_effAt3peFit_ThrCal_it_10402;1",
  "g_effAt3peFit_ThrCal_it_10403;1",
};

TString out_file_name = "./projects/Thesis/data/Chapter4/CB_ST_SelfTriggerResults_3PEeff_Avg.root";

void AverageGraphs()
{
  bool loop_on_files, loop_on_tgraphs;
  size_t n_graph = 0;

  if(root_files.size()>tgraph_names.size()){
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = false;
  }
  else if(root_files.size()<tgraph_names.size()){
    n_graph = tgraph_names.size();
    loop_on_files = false;
    loop_on_tgraphs = true;
  }
  else{
    std::cout << "All true" << std::endl;
    n_graph = root_files.size();
    loop_on_files = true;
    loop_on_tgraphs = true;
  }


  printf("Averaging %zu graphs\n", n_graph);
  std::vector<TGraph*> graphs;

  for(size_t i=0; i<n_graph; i++){
    printf("Reading graph %zu\n", i);
    TString file_name;
    if(loop_on_files==true) file_name = root_files[i];
    else file_name = root_files[0];

    TFile input_file(file_name, "READ");

    TString tg_name;
    if(loop_on_tgraphs==true){
      tg_name = tgraph_names[i];
    }
    else{
      tg_name = tgraph_names[0];
    }
    std::cout << i << std::endl;
    input_file.GetObject(tg_name, graphs.emplace_back());


    if (graphs[i]==nullptr) {
      std::cout << "TGraph " << tg_name << " not found in file " << file_name << std::endl;
      continue;
    }
    std::cout << "TGraph " << graphs[i]->GetName() << std::endl;

  }


  if (N < 2)
    throw std::invalid_argument("AverageGraphs: N must be >= 2");

  // Find the common domain:
  // xmin = max of the lower limits
  // xmax = min of the upper limits
  std::vector<double> xmins, xmaxs;
  double ymin = -std::numeric_limits<double>::infinity();
  double ymax =  std::numeric_limits<double>::infinity();

  for (const auto* g : graphs) {

    if (!g || g->GetN() == 0)
      throw std::invalid_argument("AverageGraphs: invalid or empty graph");

    double gxmin, gxmax;
    g->ComputeRange(gxmin, ymin, gxmax, ymax);

    xmins.push_back(gxmin);
    xmaxs.push_back(gxmax);
  }

  sort(xmins.begin(), xmins.end());
  sort(xmaxs.begin(), xmaxs.end());
  double xmin = xmins[1]; // max of lower limits
  double xmax = xmaxs[xmaxs.size() - 2]; // min of upper limits

  if (xmin >= xmax)
    throw std::runtime_error("AverageGraphs: graphs have no common domain");

  // Create the averaged graph
  TGraphErrors* avg = new TGraphErrors(N);
  avg->SetName("averaged_graph");
  avg->SetTitle("Averaged Graph");
  printf("Averaging %zu graphs over the domain [%.3f, %.3f] with %d points\n", graphs.size(), xmin, xmax, N);

  for (int i = 0; i < N; ++i) {

    // Uniformly spaced x
    double x = xmin + i * (xmax - xmin) / (N - 1);

    // Average the values of all graphs
    double sum = 0.0;
    double norm = 0.0;

    for (const auto* g : graphs){
      // sum if in range
      if (x >= g->GetX()[0] && x <= g->GetX()[g->GetN() - 1]) {
        sum += g->Eval(x);
        norm += 1.0;
      }
    }

    double y = sum / norm;

    avg->SetPoint(i, x, y);
  }

  // writthe avg graph to a root file
  TFile out_file(out_file_name, "RECREATE");
  out_file.cd();
  avg->Write();
  out_file.Close();
  printf("Averaged graph written to %s\n", out_file_name.Data());

  return;
}
