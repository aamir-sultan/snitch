// Copyright 2020 ETH Zurich and University of Bologna.
// Solderpad Hardware License, Version 0.51, see LICENSE for details.
// SPDX-License-Identifier: SHL-0.51

// Author: Florian Zaruba <zarubaf@iis.ee.ethz.ch>
// Author: Fabian Schuiki <fschuiki@iis.ee.ethz.ch>

// AUTOMATICALLY GENERATED by occamygen.py; edit the script instead.
<%
  cut_width = 1
  nr_clusters = int(cfg["s1_quadrant"]["nr_clusters"])
  ro_cache_cfg = cfg["s1_quadrant"].get("ro_cache_cfg", {})
  ro_cache_regions = ro_cache_cfg.get("address_regions", 1)
%>

/// Occamy Stage 1 Quadrant
module occamy_quadrant_s1
  import occamy_pkg::*;
(
  input  logic                         clk_i,
  input  logic                         rst_ni,
  input  logic                         test_mode_i,
  input  tile_id_t                     tile_id_i,
  input  logic [NrCoresS1Quadrant-1:0] debug_req_i,
  input  logic [NrCoresS1Quadrant-1:0] meip_i,
  input  logic [NrCoresS1Quadrant-1:0] mtip_i,
  input  logic [NrCoresS1Quadrant-1:0] msip_i,
  input  logic [4:0]                   isolate_i,
  output logic [4:0]                   isolated_o,
  input  logic                         ro_enable_i,
  input  logic                         ro_flush_valid_i,
  output logic                         ro_flush_ready_o,
  input  logic [${ro_cache_regions-1}:0][${soc_wide_xbar.in_s1_quadrant_0.aw-1}:0] ro_start_addr_i,
  input  logic [${ro_cache_regions-1}:0][${soc_wide_xbar.in_s1_quadrant_0.aw-1}:0] ro_end_addr_i,
  // HBI Connection
  output ${wide_xbar_quadrant_s1.out_hbi.req_type()}   quadrant_hbi_out_req_o,
  input  ${wide_xbar_quadrant_s1.out_hbi.rsp_type()}   quadrant_hbi_out_rsp_i,
  // Next-Level
  output ${soc_narrow_xbar.in_s1_quadrant_0.req_type()} quadrant_narrow_out_req_o,
  input  ${soc_narrow_xbar.in_s1_quadrant_0.rsp_type()} quadrant_narrow_out_rsp_i,
  input  ${soc_narrow_xbar.out_s1_quadrant_0.req_type()} quadrant_narrow_in_req_i,
  output ${soc_narrow_xbar.out_s1_quadrant_0.rsp_type()} quadrant_narrow_in_rsp_o,
  output ${soc_wide_xbar.in_s1_quadrant_0.req_type()}   quadrant_wide_out_req_o,
  input  ${soc_wide_xbar.in_s1_quadrant_0.rsp_type()}   quadrant_wide_out_rsp_i,
  input  ${soc_wide_xbar.out_s1_quadrant_0.req_type()}   quadrant_wide_in_req_i,
  output ${soc_wide_xbar.out_s1_quadrant_0.rsp_type()}   quadrant_wide_in_rsp_o,
  // SRAM configuration
  input  occamy_pkg::sram_cfg_quadrant_t sram_cfg_i
);

 // Calculate cluster base address based on `tile id`.
  addr_t [${nr_clusters-1}:0] cluster_base_addr;
  % for i in range(nr_clusters):
  assign cluster_base_addr[${i}] = ClusterBaseOffset + tile_id_i * NrClustersS1Quadrant * ClusterAddressSpace + ${i} * ClusterAddressSpace;
  %endfor

  ///////////////////
  //   CROSSBARS   //
  ///////////////////
  ${module}

  ///////////////////////////////
  // Narrow In + IW Converter //
  ///////////////////////////////
  <%
    narrow_cluster_in_iwc = soc_narrow_xbar.out_s1_quadrant_0 \
      .copy(name="narrow_cluster_in_iwc") \
      .declare(context) \
      .cut(context, cut_width) \
      .isolate(context, "isolate_i[0]", "narrow_cluster_in_isolate", isolated="isolated_o[0]", terminated=True) \
      .change_iw(context, narrow_xbar_quadrant_s1.in_top.iw, "narrow_cluster_in_iwc", to=narrow_xbar_quadrant_s1.in_top)
  %>
  assign narrow_cluster_in_iwc_req = quadrant_narrow_in_req_i;
  assign quadrant_narrow_in_rsp_o = narrow_cluster_in_iwc_rsp;

  ///////////////////////////////
  // Narrow Out + IW Converter //
  ///////////////////////////////
  <% narrow_cluster_out_iwc = narrow_xbar_quadrant_s1.out_top \
    .change_iw(context, soc_narrow_xbar.in_s1_quadrant_0.iw, "narrow_cluster_out_iwc") \
    .isolate(context, "isolate_i[1]", "narrow_cluster_out_isolate", isolated="isolated_o[1]") \
    .cut(context, cut_width)
   %>

  assign quadrant_narrow_out_req_o = ${narrow_cluster_out_iwc.req_name()};
  assign ${narrow_cluster_out_iwc.rsp_name()} = quadrant_narrow_out_rsp_i;

  /////////////////////////////////////////
  // Wide Out + RO Cache + IW Converter  //
  /////////////////////////////////////////
  <%
    wide_xbar_out_iwc = wide_xbar_quadrant_s1.out_top.change_iw(context, 3, "wide_cluster_out_iwc")
    if ro_cache_cfg:
      wide_cluster_out_ro_cache = wide_xbar_out_iwc \
      .add_ro_cache(context, "snitch_ro_cache", \
        ro_cache_cfg, \
        enable="ro_enable_i", \
        flush_valid="ro_flush_valid_i", \
        flush_ready="ro_flush_ready_o", \
        start_addr="ro_start_addr_i", \
        end_addr="ro_end_addr_i", \
        sram_cfg_data_t="sram_cfg_t", \
        sram_cfg_tag_t="sram_cfg_t", \
        sram_cfg_data_i="sram_cfg_i.rocache_data", \
        sram_cfg_tag_i="sram_cfg_i.rocache_tag")
    else:
      wide_cluster_out_ro_cache = wide_xbar_out_iwc

    wide_cluster_out_cut = wide_cluster_out_ro_cache \
      .isolate(context, "isolate_i[3]", "wide_cluster_out_isolate", isolated="isolated_o[3]", atop_support=False) \
      .cut(context, cut_width)

    assert soc_wide_xbar.in_s1_quadrant_0.iw == wide_cluster_out_cut.iw, "S1 Quadrant and Cluster Out IW mismatches."
  %>

  assign quadrant_wide_out_req_o = ${wide_cluster_out_cut.req_name()};
  assign ${wide_cluster_out_cut.rsp_name()} = quadrant_wide_out_rsp_i;

  ////////////////////
  // HBI Connection //
  ////////////////////
  <%
    wide_cluster_hbi_out_iwc = wide_xbar_quadrant_s1.out_hbi  \
      .isolate(context, "isolate_i[4]", "quadrant_hbi_out_isolate", isolated="isolated_o[4]", atop_support=False) \
      .cut(context, cut_width)
  %>

  assign quadrant_hbi_out_req_o = ${wide_cluster_hbi_out_iwc.req_name()};
  assign ${wide_cluster_hbi_out_iwc.rsp_name()} = quadrant_hbi_out_rsp_i;

  ////////////////////////////
  // Wide In + IW Converter //
  ////////////////////////////
  <%
    soc_wide_xbar.out_s1_quadrant_0 \
      .copy(name="wide_cluster_in_iwc") \
      .declare(context) \
      .cut(context, cut_width) \
      .isolate(context, "isolate_i[2]", "wide_cluster_in_isolate", isolated="isolated_o[2]", terminated=True, atop_support=False) \
      .change_iw(context, wide_xbar_quadrant_s1.in_top.iw, "wide_cluster_in_iwc", to=wide_xbar_quadrant_s1.in_top)
  %>
  assign wide_cluster_in_iwc_req = quadrant_wide_in_req_i;
  assign quadrant_wide_in_rsp_o = wide_cluster_in_iwc_rsp;

% for i in range(nr_clusters):
  ///////////////
  // Cluster ${i} //
  ///////////////
  <%
    narrow_cluster_in = narrow_xbar_quadrant_s1.__dict__["out_cluster_{}".format(i)].change_iw(context, 2, "narrow_in_iwc_{}".format(i)).cut(context, cut_width)
    narrow_cluster_out = narrow_xbar_quadrant_s1.__dict__["in_cluster_{}".format(i)].copy(name="narrow_out_{}".format(i)).declare(context)
    narrow_cluster_out.cut(context, cut_width, to=narrow_xbar_quadrant_s1.__dict__["in_cluster_{}".format(i)])
    wide_cluster_in = wide_xbar_quadrant_s1.__dict__["out_cluster_{}".format(i)].change_iw(context, 2, "wide_in_iwc_{}".format(i)).cut(context, cut_width)
    wide_cluster_out = wide_xbar_quadrant_s1.__dict__["in_cluster_{}".format(i)].copy(name="wide_out_{}".format(i)).declare(context)
    wide_cluster_out.cut(context, cut_width, to=wide_xbar_quadrant_s1.__dict__["in_cluster_{}".format(i)])
  %>

  logic [9:0] hart_base_id_${i};
  assign hart_base_id_${i} = HartIdOffset + tile_id_i * NrCoresS1Quadrant + ${i} * NrCoresCluster;

  occamy_cluster_wrapper i_occamy_cluster_${i} (
    .clk_i (clk_i),
    .rst_ni (rst_ni),
    .debug_req_i (debug_req_i[${i}*NrCoresCluster+:NrCoresCluster]),
    .meip_i (meip_i[${i}*NrCoresCluster+:NrCoresCluster]),
    .mtip_i (mtip_i[${i}*NrCoresCluster+:NrCoresCluster]),
    .msip_i (msip_i[${i}*NrCoresCluster+:NrCoresCluster]),
    .hart_base_id_i (hart_base_id_${i}),
    .cluster_base_addr_i (cluster_base_addr[${i}]),
    .clk_d2_bypass_i (1'b0),
    .narrow_in_req_i (${narrow_cluster_in.req_name()}),
    .narrow_in_resp_o (${narrow_cluster_in.rsp_name()}),
    .narrow_out_req_o  (${narrow_cluster_out.req_name()}),
    .narrow_out_resp_i (${narrow_cluster_out.rsp_name()}),
    .wide_out_req_o  (${wide_cluster_out.req_name()}),
    .wide_out_resp_i (${wide_cluster_out.rsp_name()}),
    .wide_in_req_i (${wide_cluster_in.req_name()}),
    .wide_in_resp_o (${wide_cluster_in.rsp_name()}),
    .sram_cfgs_i (sram_cfg_i.cluster)
  );

% endfor
endmodule
