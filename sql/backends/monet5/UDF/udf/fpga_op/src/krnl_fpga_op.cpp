// FPGA operator kernel code

#include "krnl_fpga_op.h"

extern "C" {
void krnl_fpga_op(const v_dt* in1,             // Read-Only Vector 1
                  v_dt* out_double,            // Output Result for ADD
                  const unsigned int size      // Size in integer
) {
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = out_double offset = slave bundle = gmem2

#pragma HLS INTERFACE s_axilite port = in1
#pragma HLS INTERFACE s_axilite port = out_double
#pragma HLS INTERFACE s_axilite port = size
#pragma HLS INTERFACE s_axilite port = return

    v_dt tmpIn1;

    // Auto-pipeline is going to apply pipeline to this loop
    for (int i = 0; i < size; i++) {
        tmpIn1 = in1[i];
        out_double[i] = tmpIn1 << 1;
    }

}
}
