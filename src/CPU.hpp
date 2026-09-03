#ifndef CPU_HPP
#define CPU_HPP

#include <systemc>
#include <vector>
#include <iostream>
#include "CPU_Mem_IF.hpp"
#include "CPU_ACC_IF.hpp"

using namespace std;
using namespace sc_core;

class CPU : public sc_module {
public:
    sc_port<CPU_Mem_IF> mem_port;
    sc_port<CPU_ACC_IF> acc_port;
    

    SC_CTOR(CPU) {
        SC_THREAD(process);
    }

private:
    void process() {
        cout << sc_time_stamp() << " [CPU] process() thread started." << endl;
        wait(5, SC_NS);  // Initial boot delay

        // Receive layer sizes
        mem_port->wait_layer_ready();
        mem_port->ack_layer_received();
        vector<int> layer_sizes = mem_port->get_layer_sizes();
        cout << sc_time_stamp() << " [CPU] Received layer sizes.\n";
        acc_port->set_layer_sizes(layer_sizes);

        // Receive input vector
        mem_port->wait_input_ready();
        mem_port->ack_input_received();
        vector<float> input = mem_port->get_input_vector();
        cout << sc_time_stamp() << " [CPU] Received input vector.\n ";
        acc_port->set_input_vector(input);

        for (size_t i = 0; i < layer_sizes.size() - 1; ++i) {
            acc_port->send_start();
            acc_port->wait_done();
        }

        acc_port->wait_ready();

        vector<float> output = acc_port->get_output_vector();
        cout << sc_time_stamp() << " [CPU] Sending output to Memory." << endl;
        mem_port->set_output_vector(output);
        mem_port->wait_output_ack();
        cout << sc_time_stamp() << " [CPU] Output transfer completed." << endl;

    }

};

#endif
