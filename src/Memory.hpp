#ifndef MEMORY_H
#define MEMORY_H

#include <systemc>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "CPU_Mem_IF.hpp"
#include "CPU_Mem_Channel.hpp"
#include "Mem_ACC_IF.hpp"  // ✅ Needed for Accelerator communication

using namespace std;
using namespace sc_core;

class Memory : public sc_module {
public:
    // Ports for CPU and Accelerator interfaces
    sc_port<CPU_Mem_IF> cpu_if;
    sc_port<MemACC_IF> acc_if;  // ✅ Added to support MemACC_Channel

    SC_CTOR(Memory) {
        SC_THREAD(load_and_send);
    }

private:
   void load_and_send() {

        wait(5, SC_NS);  // Let CPU start
        // Load file
        string line;
        vector<int> layer_sizes;
        vector<float> input_vec;

        ifstream file("mlp_data.csv");
        if (!file.is_open()) {
            SC_REPORT_ERROR("Memory", "Unable to open mlp_data.csv");
            return;
        }

        // Read layer sizes (first line)
        if (getline(file, line)) {
            istringstream ss(line);
            int n;
            while (ss >> n) layer_sizes.push_back(n);
        }

        // Read input vector (second line)
        if (getline(file, line)) {
            istringstream ss(line);
            float val;
            while (ss >> val) input_vec.push_back(val);
        }

        cout << sc_time_stamp() << " [Memory] Loaded layer sizes: ";
        for (int size : layer_sizes) cout << size << " ";
        cout << endl;

        cout << sc_time_stamp() << " [Memory] Loaded input vector: ";
        for (float val : input_vec) cout << val << " ";
        cout << endl;

        // Send layer sizes
        cpu_if->set_layer_sizes(layer_sizes);
        wait(1, SC_NS);  // Allow CPU to reach wait_layer_ready()
        cpu_if->ack_layer_received(); // ✅ notify CPU

        // Send input vector
        cpu_if->set_input_vector(input_vec);
        wait(1, SC_NS);  // Allow CPU to reach wait_input_ready()
        cpu_if->ack_input_received(); // ✅ notify CPU

        wait(5, SC_NS);

       while (true) {

            if (cpu_if->has_output()) {
                cout << sc_time_stamp() << " [Memory] Output received." << endl;

                vector<float> output = cpu_if->get_final_output();

                ofstream out("output.csv");
                if (out.is_open()) {
                    for (float val : output) out << val << " ";
                    out.close();
                    cout << sc_time_stamp() << " [Memory] Output written to file." << endl;
                } else {
                    SC_REPORT_WARNING("Memory", "Failed to open output.csv for writing");
                }

                cpu_if->ack_output_received();
                cpu_if->reset_output_flag();

                break;  
            }

            wait(1, SC_NS);  
        }


    }

};

#endif
