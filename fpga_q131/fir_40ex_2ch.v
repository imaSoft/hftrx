// megafunction wizard: %FIR Compiler v13.1%
// GENERATION: XML

// ============================================================
// Megafunction Name(s):
// 			fir_40ex_2ch_ast
// ============================================================
// Generated by FIR Compiler 13.1 [Altera, IP Toolbench 1.3.0 Build 182]
// ************************************************************
// THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
// ************************************************************
// Copyright (C) 1991-2018 Altera Corporation
// Any megafunction design, and related net list (encrypted or decrypted),
// support information, device programming or simulation file, and any other
// associated documentation or information provided by Altera or a partner
// under Altera's Megafunction Partnership Program may be used only to
// program PLD devices (but not masked PLD devices) from Altera.  Any other
// use of such megafunction design, net list, support information, device
// programming or simulation file, or any other related documentation or
// information is prohibited for any other purpose, including, but not
// limited to modification, reverse engineering, de-compiling, or use with
// any other silicon devices, unless such use is explicitly licensed under
// a separate agreement with Altera or a megafunction partner.  Title to
// the intellectual property, including patents, copyrights, trademarks,
// trade secrets, or maskworks, embodied in any such megafunction design,
// net list, support information, device programming or simulation file, or
// any other related documentation or information provided by Altera or a
// megafunction partner, remains with Altera, the megafunction partner, or
// their respective licensors.  No other licenses, including any licenses
// needed under any third party's intellectual property, are provided herein.


module fir_40ex_2ch (
	clk,
	reset_n,
	ast_sink_data,
	ast_sink_valid,
	ast_source_ready,
	ast_sink_sop,
	ast_sink_eop,
	ast_sink_error,
	ast_source_data,
	ast_sink_ready,
	ast_source_valid,
	ast_source_sop,
	ast_source_eop,
	ast_source_channel,
	ast_source_error);


	input		clk;
	input		reset_n;
	input	[31:0]	ast_sink_data;
	input		ast_sink_valid;
	input		ast_source_ready;
	input		ast_sink_sop;
	input		ast_sink_eop;
	input	[1:0]	ast_sink_error;
	output	[31:0]	ast_source_data;
	output		ast_sink_ready;
	output		ast_source_valid;
	output		ast_source_sop;
	output		ast_source_eop;
	output		ast_source_channel;
	output	[1:0]	ast_source_error;


	fir_40ex_2ch_ast	fir_40ex_2ch_ast_inst(
		.clk(clk),
		.reset_n(reset_n),
		.ast_sink_data(ast_sink_data),
		.ast_sink_valid(ast_sink_valid),
		.ast_source_ready(ast_source_ready),
		.ast_sink_sop(ast_sink_sop),
		.ast_sink_eop(ast_sink_eop),
		.ast_sink_error(ast_sink_error),
		.ast_source_data(ast_source_data),
		.ast_sink_ready(ast_sink_ready),
		.ast_source_valid(ast_source_valid),
		.ast_source_sop(ast_source_sop),
		.ast_source_eop(ast_source_eop),
		.ast_source_channel(ast_source_channel),
		.ast_source_error(ast_source_error));
endmodule

// =========================================================
// FIR Compiler Wizard Data
// ===============================
// DO NOT EDIT FOLLOWING DATA
// @Altera, IP Toolbench@
// Warning: If you modify this section, FIR Compiler Wizard may not be able to reproduce your chosen configuration.
// 
// Retrieval info: <?xml version="1.0"?>
// Retrieval info: <MEGACORE title="FIR Compiler"  version="13.1"  build="182"  iptb_version="1.3.0 Build 182"  format_version="120" >
// Retrieval info:  <NETLIST_SECTION class="altera.ipbu.flowbase.netlist.model.FIRModelClass"  active_core="fir_40ex_2ch_ast" >
// Retrieval info:   <STATIC_SECTION>
// Retrieval info:    <PRIVATES>
// Retrieval info:     <NAMESPACE name = "parameterization">
// Retrieval info:      <PRIVATE name = "use_mem" value="1"  type="BOOLEAN"  enable="1" />
// Retrieval info:      <PRIVATE name = "mem_type" value="M512"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "filter_rate" value="Decimation"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "filter_factor" value="40"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "coefficient_scaling_type" value="Signed Binary Fractional"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "coefficient_scaling_factor" value="4194304.0"  type="STRING"  enable="0" />
// Retrieval info:      <PRIVATE name = "coefficient_bit_width" value="23"  type="INTEGER"  enable="0" />
// Retrieval info:      <PRIVATE name = "coefficient_binary_point_position" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "number_of_input_channels" value="2"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "input_number_system" value="Signed Binary"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "input_bit_width" value="32"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "input_binary_point_position" value="0"  type="INTEGER"  enable="0" />
// Retrieval info:      <PRIVATE name = "output_bit_width_method" value="Actual Coefficients"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_number_system" value="Custom Resolution"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_bit_width" value="32"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_bits_right_of_binary_point" value="32"  type="INTEGER"  enable="0" />
// Retrieval info:      <PRIVATE name = "output_bits_removed_from_lsb" value="22"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_lsb_remove_type" value="Truncate"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_msb_remove_type" value="Truncate"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "flow_control" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "flow_control_input" value="Slave Sink"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "flow_control_output" value="Master Source"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "device_family" value="Cyclone IV E"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "structure" value="Distributed Arithmetic : Fully Serial Filter"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "pipeline_level" value="1"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "clocks_to_compute" value="1"  type="INTEGER"  enable="0" />
// Retrieval info:      <PRIVATE name = "number_of_serial_units" value="2"  type="INTEGER"  enable="0" />
// Retrieval info:      <PRIVATE name = "data_storage" value="M9K"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "coefficient_storage" value="Logic Cells"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "multiplier_storage" value="Logic Cells"  type="STRING"  enable="0" />
// Retrieval info:      <PRIVATE name = "force_non_symmetric_structure" value="1"  type="BOOLEAN"  enable="0" />
// Retrieval info:      <PRIVATE name = "coefficients_reload" value="0"  type="BOOLEAN"  enable="0" />
// Retrieval info:      <PRIVATE name = "coefficients_reload_sgl_clock" value="0"  type="BOOLEAN"  enable="1" />
// Retrieval info:      <PRIVATE name = "max_clocks_to_compute" value="1"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "set_1" value="Low Pass Set, Floating, Low Pass, Blackman, 729, 4000.0, 91.0, 3750000.0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.23189E-6, -1.57865E-6, -1.94495E-6, -2.31894E-6, -2.68669E-6, -3.03251E-6, -3.33936E-6, -3.58935E-6, -3.76427E-6, -3.84623E-6, -3.81832E-6, -3.66531E-6, -3.37432E-6, -2.93551E-6, -2.34269E-6, -1.59391E-6, 0.0, 0.0, 1.535E-6, 2.8282E-6, 4.21112E-6, 5.65466E-6, 7.12501E-6, 8.58416E-6, 9.99069E-6, 1.13006E-5, 1.24683E-5, 1.34481E-5, 1.41948E-5, 1.46658E-5, 1.48218E-5, 1.46287E-5, 1.40584E-5, 1.30906E-5, 1.17136E-5, 9.92529E-6, 7.73413E-6, 5.15963E-6, 2.23273E-6, -1.0042E-6, -4.49754E-6, -8.18303E-6, -1.19866E-5, -1.58254E-5, -1.96097E-5, -2.32442E-5, -2.66301E-5, -2.96677E-5, -3.22583E-5, -3.4307E-5, -3.57255E-5, -3.64342E-5, -3.63651E-5, -3.54642E-5, -3.36937E-5, -3.10339E-5, -2.74848E-5, -2.30678E-5, -1.78257E-5, -1.1824E-5, -5.15016E-6, 2.08712E-6, 9.7589E-6, 1.7718E-5, 2.58012E-5, 3.38325E-5, 4.16261E-5, 4.89901E-5, 5.57313E-5, 6.16588E-5, 6.65893E-5, 7.03511E-5, 7.27896E-5, 7.3771E-5, 7.31872E-5, 7.09594E-5, 6.70419E-5, 6.14245E-5, 5.4135E-5, 4.52407E-5, 3.4849E-5, 2.31067E-5, 1.01994E-5, -3.65095E-6, -1.81894E-5, -3.31317E-5, -4.81693E-5, -6.29751E-5, -7.72101E-5, -9.053E-5, -1.02593E-4, -1.13068E-4, -1.21642E-4, -1.28027E-4, -1.31972E-4, -1.33264E-4, -1.3174E-4, -1.27291E-4, -1.19868E-4, -1.09484E-4, -9.62179E-5, -8.02178E-5, -6.16981E-5, -4.09393E-5, -1.82847E-5, 5.86416E-6, 3.10538E-5, 5.67866E-5, 8.25292E-5, 1.07723E-4, 1.31797E-4, 1.54175E-4, 1.74294E-4, 1.91614E-4, 2.0563E-4, 2.1589E-4, 2.21999E-4, 2.23637E-4, 2.20567E-4, 2.12644E-4, 1.99823E-4, 1.82161E-4, 1.59826E-4, 1.33093E-4, 1.02344E-4, 6.80669E-5, 3.0845E-5, -8.64825E-6, -4.96618E-5, -9.13795E-5, -1.32936E-4, -1.73431E-4, -2.11953E-4, -2.47592E-4, -2.79462E-4, -3.06725E-4, -3.28605E-4, -3.4441E-4, -3.53554E-4, -3.55566E-4, -3.50114E-4, -3.3701E-4, -3.16225E-4, -2.87893E-4, -2.52318E-4, -2.09967E-4, -1.61477E-4, -1.07637E-4, -4.93845E-5, 1.22119E-5, 7.59708E-5, 1.40618E-4, 2.04812E-4, 2.67168E-4, 3.26287E-4, 3.80789E-4, 4.29336E-4, 4.70669E-4, 5.03635E-4, 5.27218E-4, 5.40561E-4, 5.42996E-4, 5.34063E-4, 5.13528E-4, 4.81393E-4, 4.3791E-4, 3.83583E-4, 3.19163E-4, 2.45646E-4, 1.64256E-4, 7.64307E-5, -1.6204E-5, -1.11861E-4, -2.08625E-4, -3.04487E-4, -3.97387E-4, -4.85255E-4, -5.66052E-4, -6.37819E-4, -6.98719E-4, -7.4708E-4, -7.81438E-4, -8.00576E-4, -8.03555E-4, -7.89747E-4, -7.58859E-4, -7.10947E-4, -6.46432E-4, -5.66099E-4, -4.71097E-4, -3.62922E-4, -2.43403E-4, -1.14669E-4, 2.08829E-5, 1.60631E-4, 3.01775E-4, 4.4139E-4, 5.76488E-4, 7.04071E-4, 8.21202E-4, 9.25065E-4, 0.00101303, 0.00108272, 0.00113205, 0.0011593, 0.00116317, 0.0011428, 0.00109781, 0.00102832, 9.34992E-4, 8.18985E-4, 6.81986E-4, 5.26178E-4, 3.5421E-4, 1.69156E-4, -2.55334E-5, -2.26093E-4, -4.28511E-4, -6.28607E-4, -8.22112E-4, -0.00100476, -0.00117236, -0.00132092, -0.00144671, -0.00154635, -0.0016169, -0.00165594, -0.00166162, -0.00163274, -0.00156878, -0.00146992, -0.0013371, -0.00117198, -9.76972E-4, -7.55163E-4, -5.10314E-4, -2.46786E-4, 3.05306E-5, 3.16297E-4, 6.04828E-4, 8.90193E-4, 0.00116634, 0.0014272, 0.00166683, 0.00187955, 0.00206003, 0.00220344, 0.00230558, 0.00236295, 0.00237286, 0.00233354, 0.00224415, 0.00210489, 0.00191699, 0.00168271, 0.00140536, 0.00108925, 7.39643E-4, 3.62657E-4, -3.48105E-5, -4.45209E-4, -8.60463E-4, -0.00127212, -0.00167152, -0.00204995, -0.00239883, -0.00270989, -0.00297534, -0.00318808, -0.00334181, -0.00343124, -0.00345219, -0.00340175, -0.00327834, -0.00308185, -0.00281361, -0.00247651, -0.00207491, -0.00161466, -0.00110301, -5.48553E-4, 3.89621E-5, 6.48725E-4, 0.00126906, 0.00188764, 0.00249166, 0.00306814, 0.00360411, 0.0040869, 0.00450439, 0.00484527, 0.00509929, 0.00525749, 0.00531244, 0.00525845, 0.0050917, 0.00481047, 0.00441517, 0.00390851, 0.00329545, 0.00258327, 0.00178153, 9.01944E-4, -4.17009E-5, -0.00103372, -0.00205672, -0.00309185, -0.00411907, -0.00511744, -0.00606544, -0.0069413, -0.0077234, -0.00839059, -0.00892256, -0.00930023, -0.00950607, -0.00952444, -0.00934192, -0.00894757, -0.00833319, -0.00749353, -0.00642648, -0.00513312, -0.00361789, -0.00188853, 4.39005E-5, 0.00216514, 0.00445784, 0.00690177, 0.00947404, 0.0121494, 0.0149005, 0.0176984, 0.0205127, 0.0233123, 0.0260655, 0.0287408, 0.0313069, 0.0337335, 0.0359916, 0.0380542, 0.039896, 0.0414948, 0.0428307, 0.0438873, 0.0446516, 0.045114, 0.0452687, 0.045114, 0.0446516, 0.0438873, 0.0428307, 0.0414948, 0.039896, 0.0380542, 0.0359916, 0.0337335, 0.0313069, 0.0287408, 0.0260655, 0.0233123, 0.0205127, 0.0176984, 0.0149005, 0.0121494, 0.00947404, 0.00690177, 0.00445784, 0.00216514, 4.39005E-5, -0.00188853, -0.00361789, -0.00513312, -0.00642648, -0.00749353, -0.00833319, -0.00894757, -0.00934192, -0.00952444, -0.00950607, -0.00930023, -0.00892256, -0.00839059, -0.0077234, -0.0069413, -0.00606544, -0.00511744, -0.00411907, -0.00309185, -0.00205672, -0.00103372, -4.17009E-5, 9.01944E-4, 0.00178153, 0.00258327, 0.00329545, 0.00390851, 0.00441517, 0.00481047, 0.0050917, 0.00525845, 0.00531244, 0.00525749, 0.00509929, 0.00484527, 0.00450439, 0.0040869, 0.00360411, 0.00306814, 0.00249166, 0.00188764, 0.00126906, 6.48725E-4, 3.89621E-5, -5.48553E-4, -0.00110301, -0.00161466, -0.00207491, -0.00247651, -0.00281361, -0.00308185, -0.00327834, -0.00340175, -0.00345219, -0.00343124, -0.00334181, -0.00318808, -0.00297534, -0.00270989, -0.00239883, -0.00204995, -0.00167152, -0.00127212, -8.60463E-4, -4.45209E-4, -3.48105E-5, 3.62657E-4, 7.39643E-4, 0.00108925, 0.00140536, 0.00168271, 0.00191699, 0.00210489, 0.00224415, 0.00233354, 0.00237286, 0.00236295, 0.00230558, 0.00220344, 0.00206003, 0.00187955, 0.00166683, 0.0014272, 0.00116634, 8.90193E-4, 6.04828E-4, 3.16297E-4, 3.05306E-5, -2.46786E-4, -5.10314E-4, -7.55163E-4, -9.76972E-4, -0.00117198, -0.0013371, -0.00146992, -0.00156878, -0.00163274, -0.00166162, -0.00165594, -0.0016169, -0.00154635, -0.00144671, -0.00132092, -0.00117236, -0.00100476, -8.22112E-4, -6.28607E-4, -4.28511E-4, -2.26093E-4, -2.55334E-5, 1.69156E-4, 3.5421E-4, 5.26178E-4, 6.81986E-4, 8.18985E-4, 9.34992E-4, 0.00102832, 0.00109781, 0.0011428, 0.00116317, 0.0011593, 0.00113205, 0.00108272, 0.00101303, 9.25065E-4, 8.21202E-4, 7.04071E-4, 5.76488E-4, 4.4139E-4, 3.01775E-4, 1.60631E-4, 2.08829E-5, -1.14669E-4, -2.43403E-4, -3.62922E-4, -4.71097E-4, -5.66099E-4, -6.46432E-4, -7.10947E-4, -7.58859E-4, -7.89747E-4, -8.03555E-4, -8.00576E-4, -7.81438E-4, -7.4708E-4, -6.98719E-4, -6.37819E-4, -5.66052E-4, -4.85255E-4, -3.97387E-4, -3.04487E-4, -2.08625E-4, -1.11861E-4, -1.6204E-5, 7.64307E-5, 1.64256E-4, 2.45646E-4, 3.19163E-4, 3.83583E-4, 4.3791E-4, 4.81393E-4, 5.13528E-4, 5.34063E-4, 5.42996E-4, 5.40561E-4, 5.27218E-4, 5.03635E-4, 4.70669E-4, 4.29336E-4, 3.80789E-4, 3.26287E-4, 2.67168E-4, 2.04812E-4, 1.40618E-4, 7.59708E-5, 1.22119E-5, -4.93845E-5, -1.07637E-4, -1.61477E-4, -2.09967E-4, -2.52318E-4, -2.87893E-4, -3.16225E-4, -3.3701E-4, -3.50114E-4, -3.55566E-4, -3.53554E-4, -3.4441E-4, -3.28605E-4, -3.06725E-4, -2.79462E-4, -2.47592E-4, -2.11953E-4, -1.73431E-4, -1.32936E-4, -9.13795E-5, -4.96618E-5, -8.64825E-6, 3.0845E-5, 6.80669E-5, 1.02344E-4, 1.33093E-4, 1.59826E-4, 1.82161E-4, 1.99823E-4, 2.12644E-4, 2.20567E-4, 2.23637E-4, 2.21999E-4, 2.1589E-4, 2.0563E-4, 1.91614E-4, 1.74294E-4, 1.54175E-4, 1.31797E-4, 1.07723E-4, 8.25292E-5, 5.67866E-5, 3.10538E-5, 5.86416E-6, -1.82847E-5, -4.09393E-5, -6.16981E-5, -8.02178E-5, -9.62179E-5, -1.09484E-4, -1.19868E-4, -1.27291E-4, -1.3174E-4, -1.33264E-4, -1.31972E-4, -1.28027E-4, -1.21642E-4, -1.13068E-4, -1.02593E-4, -9.053E-5, -7.72101E-5, -6.29751E-5, -4.81693E-5, -3.31317E-5, -1.81894E-5, -3.65095E-6, 1.01994E-5, 2.31067E-5, 3.4849E-5, 4.52407E-5, 5.4135E-5, 6.14245E-5, 6.70419E-5, 7.09594E-5, 7.31872E-5, 7.3771E-5, 7.27896E-5, 7.03511E-5, 6.65893E-5, 6.16588E-5, 5.57313E-5, 4.89901E-5, 4.16261E-5, 3.38325E-5, 2.58012E-5, 1.7718E-5, 9.7589E-6, 2.08712E-6, -5.15016E-6, -1.1824E-5, -1.78257E-5, -2.30678E-5, -2.74848E-5, -3.10339E-5, -3.36937E-5, -3.54642E-5, -3.63651E-5, -3.64342E-5, -3.57255E-5, -3.4307E-5, -3.22583E-5, -2.96677E-5, -2.66301E-5, -2.32442E-5, -1.96097E-5, -1.58254E-5, -1.19866E-5, -8.18303E-6, -4.49754E-6, -1.0042E-6, 2.23273E-6, 5.15963E-6, 7.73413E-6, 9.92529E-6, 1.17136E-5, 1.30906E-5, 1.40584E-5, 1.46287E-5, 1.48218E-5, 1.46658E-5, 1.41948E-5, 1.34481E-5, 1.24683E-5, 1.13006E-5, 9.99069E-6, 8.58416E-6, 7.12501E-6, 5.65466E-6, 4.21112E-6, 2.8282E-6, 1.535E-6, 0.0, 0.0, -1.59391E-6, -2.34269E-6, -2.93551E-6, -3.37432E-6, -3.66531E-6, -3.81832E-6, -3.84623E-6, -3.76427E-6, -3.58935E-6, -3.33936E-6, -3.03251E-6, -2.68669E-6, -2.31894E-6, -1.94495E-6, -1.57865E-6, -1.23189E-6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "number_of_sets" value="1"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_full_bit_width" value="55"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_full_bits_right_of_binary_point" value="53"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "coefficient_reload_bit_width" value="24"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "logic_cell" value="2178"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "m512" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "m4k" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "m144k" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "m9k" value="12"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "m20k" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "mlab" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "megaram" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "dsp_block" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "input_clock_period" value="32"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "output_clock_period" value="1280"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "throughput" value="1280"  type="INTEGER"  enable="1" />
// Retrieval info:      <PRIVATE name = "memory_units" value="0"  type="INTEGER"  enable="1" />
// Retrieval info:     </NAMESPACE>
// Retrieval info:     <NAMESPACE name = "simgen_enable">
// Retrieval info:      <PRIVATE name = "matlab_enable" value="1"  type="BOOLEAN"  enable="1" />
// Retrieval info:      <PRIVATE name = "testbench_enable" value="1"  type="BOOLEAN"  enable="1" />
// Retrieval info:      <PRIVATE name = "testbench_simulation_clock_period" value="10.0"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "language" value="Verilog HDL"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "enabled" value="0"  type="BOOLEAN"  enable="1" />
// Retrieval info:     </NAMESPACE>
// Retrieval info:     <NAMESPACE name = "simgen">
// Retrieval info:      <PRIVATE name = "filename" value="fir_40ex_2ch.vo"  type="STRING"  enable="1" />
// Retrieval info:     </NAMESPACE>
// Retrieval info:     <NAMESPACE name = "quartus_settings">
// Retrieval info:      <PRIVATE name = "DEVICE" value="EP4CE22E22I7"  type="STRING"  enable="1" />
// Retrieval info:      <PRIVATE name = "FAMILY" value="Cyclone IV E"  type="STRING"  enable="1" />
// Retrieval info:     </NAMESPACE>
// Retrieval info:     <NAMESPACE name = "serializer"/>
// Retrieval info:    </PRIVATES>
// Retrieval info:    <FILES/>
// Retrieval info:    <PORTS/>
// Retrieval info:    <LIBRARIES/>
// Retrieval info:   </STATIC_SECTION>
// Retrieval info:  </NETLIST_SECTION>
// Retrieval info: </MEGACORE>
// =========================================================
