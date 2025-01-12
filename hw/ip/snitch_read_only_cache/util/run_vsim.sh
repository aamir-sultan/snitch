#!/bin/bash
# Copyright 2020 ETH Zurich and University of Bologna.
# Solderpad Hardware License, Version 0.51, see LICENSE for details.
# SPDX-License-Identifier: SHL-0.51
#
# Fabian Schuiki <fschuiki@iis.ee.ethz.ch>
# Andreas Kurth  <akurth@iis.ee.ethz.ch>

set -e
ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)

[ ! -z "$VSIM" ] || VSIM=vsim

call_vsim() {
    echo "log -r /*; run -all" | $VSIM -coverage -voptargs='+acc +cover=sbecft' "$@" | tee vsim.log 2>&1
    grep "Errors: 0," vsim.log
}

# call_vsim snitch_read_only_cache_tb


vsim -coverage -voptargs="+acc +cover=sbecft" snitch_read_only_cache_tb -do 'log -r /*; do ../util/wave.tcl; run -all'
