#include <systemc>
#include "CPU.hpp"
#include "Memory.hpp"
#include "Accelerator.hpp"
#include "CPU_Mem_Channel.hpp"
#include "CPU_ACC_Channel.hpp"
#include "Mem_ACC_Channel.hpp"

using namespace sc_core;

int sc_main(int argc, char* argv[]) {
    // Instantiate modules
    CPU cpu("cpu");
    Memory mem("memory");
    Accelerator acc("accelerator");

    // Instantiate channels
    CPU_Mem_Channel cpu_mem_ch("cpu_mem_channel");
    CPU_ACC_Channel cpu_acc_ch("cpu_acc_channel");
    MemACC_Channel mem_acc_ch("mem_acc_channel");

    

    // Bind ports to channels
    cpu.mem_port(cpu_mem_ch);
    mem.cpu_if(cpu_mem_ch);

    cpu.acc_port(cpu_acc_ch);
    acc.cpu_port(cpu_acc_ch);

    acc.mem_port(mem_acc_ch);
    mem.acc_if(mem_acc_ch);  

    // Start simulation
    sc_start(1000, SC_NS);

    return 0;
}

