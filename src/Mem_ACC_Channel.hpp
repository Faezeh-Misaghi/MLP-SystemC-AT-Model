#ifndef MEM_ACC_CHANNEL_H
#define MEM_ACC_CHANNEL_H

#include <systemc>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "Mem_ACC_IF.hpp"

using namespace std;
using namespace sc_core;

class MemACC_Channel : public sc_module, public MemACC_IF {
public:
    SC_CTOR(MemACC_Channel) {
        SC_THREAD(loader);

        load_csv("weights.csv", weights_mem);
        load_csv("biases.csv", biases_mem);
    }

    void request_layer(int layer_index, int in_size_, int out_size_) override {

        layer_idx = layer_index;
        in_size = in_size_;
        out_size = out_size_;

        // Option 1: Load from file (if available)
        if (!weights_mem[layer_idx].empty() && !biases_mem[layer_idx].empty()) {
            layer_request_event.notify(SC_ZERO_TIME);
        }
        // Option 2: Use dummy data
        else {
            weights_flat.clear();
            biases_flat.clear();

            for (int i = 0; i < out_size * in_size; ++i)
                weights_flat.push_back(0.1f);
            for (int i = 0; i < out_size; ++i)
                biases_flat.push_back(0.05f);

            data_ready_event.notify(SC_ZERO_TIME);
        }
    }


    void wait_data_ready() override {
        wait(data_ready_event);
    }

    vector<float> get_weights() override { return weights_flat; }
    vector<float> get_biases() override { return biases_flat; }

private:
    vector<vector<float>> weights_mem[10];
    vector<vector<float>> biases_mem[10];
    vector<float> weights_flat;
    vector<float> biases_flat;

    sc_event layer_request_event, data_ready_event;
    int layer_idx = 0, in_size = 0, out_size = 0;

   void loader() {
        while (true) {
            wait(layer_request_event);  

            weights_flat.clear();
            biases_flat.clear();

            for (int i = 0; i < out_size; ++i)
                for (int j = 0; j < in_size; ++j)
                    weights_flat.push_back(weights_mem[layer_idx][i][j]);

            for (int i = 0; i < out_size; ++i)
                biases_flat.push_back(biases_mem[layer_idx][i][0]);

            data_ready_event.notify(SC_ZERO_TIME);
        }
    }


    void load_csv(const string& filename, vector<vector<float>> storage[10]) {
        ifstream file(filename);
        string line;
        int layer = -1;

        while (getline(file, line)) {
         
            if (line == "layer") {
                ++layer;
                continue;
            }

            vector<float> row;
            stringstream ss(line);
            string val;
            while (getline(ss, val, ',')) {
                if (!val.empty())
                    row.push_back(stof(val));
            }

            storage[layer].push_back(row);

        }
        file.close();
    }
};

#endif
