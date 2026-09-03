#ifndef ACCELERATOR_HPP
#define ACCELERATOR_HPP

#include <systemc>
#include <vector>
#include <algorithm>
#include <iostream>
#include "CPU_ACC_IF.hpp"
#include "Mem_ACC_IF.hpp"

using namespace sc_core;
using namespace std;

class Accelerator : public sc_module {
public:
    sc_port<CPU_ACC_IF> cpu_port;
    sc_port<MemACC_IF> mem_port;

    SC_CTOR(Accelerator) {
        SC_THREAD(process);
    }

private:
    void process() {
        cout << sc_time_stamp() << " [Accelerator] process() thread started." << endl;

        // Wait for CPU to start
        cpu_port->wait_for_start();

        // Receive network configuration and input
        vector<int> layer_sizes = cpu_port->get_layer_sizes();
        vector<float> current = cpu_port->get_input_vector();

        for (size_t l = 0; l < layer_sizes.size() - 1; ++l) {
            int in_size = layer_sizes[l];
            int out_size = layer_sizes[l + 1];
            cout << sc_time_stamp() << " [Accelerator] Starting layer " << l << " (" 
                 << in_size << " -> " << out_size << ")" << endl;


            // Request weights and biases from memory
            mem_port->request_layer(l, in_size, out_size);
            mem_port->wait_data_ready();
            vector<float> weights = mem_port->get_weights();   // size: out_size * in_size
            vector<float> biases = mem_port->get_biases();     // size: out_size

            // Compute layer output
            vector<float> output(out_size, 0.0f);
            for (int i = 0; i < out_size; ++i) {
                for (int j = 0; j < in_size; ++j) {
                    output[i] += current[j] * weights[i * in_size + j];
                }
                output[i] += biases[i];
                output[i] = max(0.0f, output[i]);  // ReLU activation
            }

            current = output;             // Pass output to next layer

            // Simulate layer latency based on number of MACs (multiply-accumulate ops)
            double delay_ns = in_size * out_size * 0.01;  // 0.01ns per MAC
            wait(delay_ns, SC_NS);

            cpu_port->notify_done();      // Inform CPU this layer is done

        }

        // Final output to CPU
        cpu_port->set_output_vector(current);
        cpu_port->notify_ready();  // Signal completion
        cout << sc_time_stamp() << " [Accelerator] All layers completed. Final output sent." << endl;
    }
};

#endif
