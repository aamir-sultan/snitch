// Copyright 2020 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51

// Author: Fabian Schuiki <fschuiki@iis.ee.ethz.ch>
// Author: Paul Scheffler <paulsc@iis.ee.ethz.ch>

`include "common_cells/registers.svh"

module snitch_ssr_credit_counter #(
  parameter int unsigned NumCredits      = 0,
  /// Whether credit is full or empty on reset
  parameter bit          InitCreditEmpty = 1'b0,
  /// Derived parameters *Do not override*
  parameter int unsigned InitNumCredits  = InitCreditEmpty ? '0 : NumCredits,
  parameter type         credit_cnt_t    = logic [$clog2(NumCredits):0]
) (
  input  logic clk_i,
  input  logic rst_ni,

  output credit_cnt_t credit_o,

  input  logic credit_give_i,
  input  logic credit_take_i,
  input  logic credit_init_i,  // Reinitialize (soft-reset) credit; takes priority

  output logic credit_left_o,
  output logic credit_full_o
);

  credit_cnt_t credit_d, credit_q;

  always_comb begin
    credit_d = credit_q;
    if      (credit_take_i & ~credit_give_i)  credit_d = credit_q - 1;
    else if (~credit_take_i & credit_give_i)  credit_d = credit_q + 1;
  end

  logic credit_load;
  assign credit_load = 1'b1;
  `FFLARNC(credit_q, credit_d, credit_load, credit_init_i, InitNumCredits, clk_i, rst_ni)

  assign credit_o       = credit_q;
  assign credit_left_o  = (credit_q != '0);
  assign credit_full_o  = (credit_q == NumCredits-1);

endmodule
