#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"

// --- UTILS FUNCTION -----------------------------------------------
// Read a file.txt of n_colums and create a vector<vector> in which each vector
// is a line
void Read_n_columns(std::string fileName, std::vector<std::vector<double>>& y,
                    size_t n_colums) {
  double t;
  y.clear();
  y.resize(n_colums);

  std::ifstream file(fileName.c_str());
  if (!file.is_open()){
    std::cout << "Error opening " << fileName << std::endl;
    return;
  }

  while (true) {
    std::vector<double> line;

    for (size_t i=0; i<n_colums; i++){
      if (!(file >> t)) {
        // if (i==0){
        //   std::cout << "Error reading data from " << fileName << std::endl;
        //   return;
        // }
        // else {
        //   std::cerr << "Error reading data from " << fileName 
        //     << ". The file may not have enough columns." << std::endl;
        // }
        std::cout << "Done" << fileName << std::endl;
        return;
      }
      line.push_back(t);
    }
    for (size_t i=0; i<n_colums; i++){
      y[i].push_back(line[i]);
      if (i==0) std::cout << line[0] << std::endl;
    }
  }
  return;
}
//**********************************************************
std::vector<std::pair<std::string, std::vector<double>>> read_vec_pair_CSV(const std::string& filename) {
//**********************************************************
  std::ifstream infile(filename);
  if (!infile.is_open()) {
    throw std::runtime_error("Could not open file: " + filename);
  }

  std::vector<std::pair<std::string, std::vector<double>>> data;
  std::string line;

  // Read the header line
  if (std::getline(infile, line)) {
    std::stringstream ss(line);
    std::string header;
    while (std::getline(ss, header, ',')) {
      if (!header.empty()) {
        data.emplace_back(header, std::vector<double>{});
      }
    }
  }

  // Read the data lines
  while (std::getline(infile, line)) {
    std::stringstream ss(line);
    std::string value;
    size_t col_index = 0;

    while (std::getline(ss, value, ',') && col_index < data.size()) {
      try {
        if (!value.empty()) {
          data[col_index].second.push_back(std::stod(value)); // Convert to double
        }
      } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number at column " << col_index + 1 << std::endl;
        data[col_index].second.push_back(0.0); // Default value for invalid numbers
      }
      col_index++;
    }
  }

  infile.close();
  return data;
}

// --- CREATE ROOT OBJECTS FUNCTIONS --------------------------------

void create_tgraph_from_txt(std::string txt_file_name, 
                            std::string tgraph_name,
                            std::string folder =  "") {
  std::cout << "The file must be header free and only with two colums\n"
            << "x-column \t y-column" << std::endl;
  
  std::vector<std::vector<double>> data;
  Read_n_columns(txt_file_name, data, 2);

  TGraph* graph = new TGraph(data[0].size(), &data[0][0], &data[1][0]);
  graph->SetName(tgraph_name.c_str()); graph->SetTitle(tgraph_name.c_str());

  TFile* file = TFile::Open((folder+tgraph_name+".root").c_str(), "RECREATE");
  file->cd();
  graph->Write();
  file->Close();

  return;
}

void create_tgrapherrors_from_txt(std::string txt_file_name,
                                  std::string tgraph_name,
                                  std::string folder = "") {
  std::cout << "The file must be header free and with four colums\n" 
            << "x-column \t y-column \t x-error \t y-error" << std::endl;
  
  std::vector<std::vector<double>> data;
  Read_n_columns(txt_file_name, data, 4);

  TGraphErrors* graph_errors = new TGraphErrors(data[0].size(), &data[0][0], &data[1][0],
                                                &data[2][0], &data[3][0]);
  graph_errors->SetName(tgraph_name.c_str()); graph_errors->SetTitle(tgraph_name.c_str());

  TFile* file = TFile::Open((folder+tgraph_name+".root").c_str(), "RECREATE");
  file->cd();
  graph_errors->Write();
  file->Close();

  return;
}

void create_tgraph_from_csv(std::string csv_file_name,
                            std::string tgraph_name,
                            std::string x_col_name = "",
                            std::string y_col_name = "",
                            std::string folder = "") {
  std::vector<std::pair<std::string, std::vector<double>>> data;
  std::cout << "d" << std::endl;
  data = read_vec_pair_CSV(csv_file_name);
  std::cout << "d" << std::endl;

  if (x_col_name == "") {
    x_col_name = data[0].first; // Default to the first column
  }
  if (y_col_name == "") {
    y_col_name = data[1].first; // Default to the second column
  }

  std::cout << "d" << std::endl;
  TGraph* graph = new TGraph(data[0].second.size(),
                            &data[0].second[0], &data[1].second[0]);
  graph->SetName(tgraph_name.c_str());
  graph->SetTitle(tgraph_name.c_str());

  std::cout << "d" << std::endl;
  TFile* file = TFile::Open((folder+tgraph_name+".root").c_str(), "RECREATE");
  file->cd();
  graph->Write();
  file->Close();
  
  return;
}

void create_tgrapherrors_from_csv(std::string csv_file_name,
                                  std::string tgraph_name,
                                  std::string x_col_name = "",
                                  std::string y_col_name = "",
                                  std::string err_x_col_name = "",
                                  std::string err_y_col_name = "",
                                  std::string folder = "") {
  std::vector<std::pair<std::string, std::vector<double>>> data;
  data = read_vec_pair_CSV(csv_file_name);

  if (x_col_name == "") {
    x_col_name = data[0].first; // Default to the first column
  }
  if (y_col_name == "") {
    y_col_name = data[1].first; // Default to the second column
  }
  if (err_x_col_name == "") {
    x_col_name = data[2].first;
  }
  if (err_y_col_name == "") {
    y_col_name = data[3].first;
  }

  TGraphErrors* graph_errors = new TGraphErrors(data[0].second.size(), &data[0].second[0], &data[1].second[0],
                                                &data[2].second[0], &data[3].second[0]);

  graph_errors->SetName(tgraph_name.c_str()); graph_errors->SetTitle(tgraph_name.c_str());

  TFile* file = TFile::Open((folder+tgraph_name+".root").c_str(), "RECREATE");
  file->cd();
  graph_errors->Write();
  file->Close();

  return;
}
