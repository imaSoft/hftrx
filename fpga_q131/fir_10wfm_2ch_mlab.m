%                                                                                                          
%THIS IS A WIZARD GENERATED FILE. DO NOT EDIT THIS FILE!                                                   
%                                                                                                          
%--------------------------------------------------------------------------------------------------------- 
%This Model Only Support Single Channel Input Data. 
%Please input:                                      
%data vector: 		stimulation(1:n)                
%FILTER PARAMETER  
%Input Data Width: 32
%Decimation Factor: 10
%MSB Truncated Bits:
%MSB Satuated Bits :
%LSB Truncated bits:
%LSB Rounded bits  :
%FIR Width (Full Calculation Width Before Output Width Adjust) :32 
%-----------------------------------------------------------------------------------------------------------
%MegaWizard Scaled Coefficient Values
 function  output = fir_10wfm_2ch_mlab(stimulation, output);
 coef_matrix=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-6,-7,-9,-10,-10,-9,-8,-6,0,0,5,9,13,16,19,20,20,19,16,11,6,0,-9,-16,-23,-29,-33,-35,-35,-32,-27,-19,-9,0,14,26,37,46,53,56,55,50,42,30,14,0,-21,-39,-55,-69,-78,-82,-81,-74,-61,-43,-21,0,30,56,79,98,110,116,114,104,86,60,29,-5,-41,-77,-108,-134,-151,-159,-155,-141,-116,-82,-40,7,56,103,144,178,201,211,206,187,154,108,53,-9,-73,-134,-189,-233,-262,-275,-268,-243,-199,-140,-68,11,93,172,242,299,336,352,343,310,255,179,87,-14,-118,-218,-306,-377,-424,-443,-433,-391,-320,-225,-110,17,147,272,383,471,529,553,539,487,399,280,138,-20,-181,-336,-472,-581,-653,-682,-664,-600,-491,-345,-170,24,222,411,578,711,798,833,811,733,600,422,208,-27,-268,-498,-700,-861,-967,-1009,-982,-886,-726,-510,-252,32,322,599,843,1036,1163,1214,1181,1066,873,614,304,-36,-384,-715,-1007,-1238,-1389,-1449,-1410,-1272,-1042,-733,-364,41,455,850,1196,1470,1650,1721,1674,1510,1237,871,434,-46,-536,-1002,-1412,-1736,-1948,-2031,-1976,-1782,-1460,-1028,-513,52,629,1178,1660,2040,2290,2388,2323,2096,1717,1210,605,-57,-733,-1376,-1941,-2387,-2679,-2793,-2717,-2451,-2009,-1416,-710,64,853,1604,2264,2784,3125,3258,3170,2860,2345,1654,832,-69,-988,-1862,-2629,-3235,-3631,-3787,-3685,-3325,-2727,-1925,-970,76,1144,2158,3050,3752,4213,4395,4277,3861,3168,2238,1131,-81,-1318,-2495,-3528,-4343,-4878,-5090,-4955,-4474,-3672,-2597,-1315,89,1521,2884,4082,5027,5648,5895,5740,5186,4259,3015,1532,-93,-1753,-3332,-4721,-5817,-6539,-6828,-6652,-6012,-4940,-3501,-1783,101,2025,3858,5471,6746,7588,7927,7726,6987,5747,4078,2084,-105,-2343,-4476,-6356,-7844,-8830,-9231,-9004,-8149,-6708,-4765,-2442,112,2727,5223,7425,9173,10335,10814,10558,9565,7883,5610,2885,-115,-3192,-6135,-8737,-10809,-12193,-12772,-12485,-11325,-9347,-6664,-3439,122,3781,7291,10404,12892,14565,15281,14962,13596,11243,8035,4164,-124,-4546,-8803,-12594,-15642,-17711,-18623,-18275,-16646,-13801,-9892,-5150,129,5602,10898,15644,19492,22139,23354,22994,21017,17491,12592,6597,-130,-7158,-14017,-20224,-25321,-28903,-30645,-30332,-27880,-23342,-16915,-8937,135,9740,19250,28007,35361,40715,43566,43543,40440,34237,25117,13467,-134,-14934,-30039,-44460,-57162,-67113,-73350,-75029,-71481,-62252,-47142,-26226,138,31310,66397,104278,143656,183113,221176,256382,287349,312842,331832,343547,347506,343547,331832,312842,287349,256382,221176,183113,143656,104278,66397,31310,138,-26226,-47142,-62252,-71481,-75029,-73350,-67113,-57162,-44460,-30039,-14934,-134,13467,25117,34237,40440,43543,43566,40715,35361,28007,19250,9740,135,-8937,-16915,-23342,-27880,-30332,-30645,-28903,-25321,-20224,-14017,-7158,-130,6597,12592,17491,21017,22994,23354,22139,19492,15644,10898,5602,129,-5150,-9892,-13801,-16646,-18275,-18623,-17711,-15642,-12594,-8803,-4546,-124,4164,8035,11243,13596,14962,15281,14565,12892,10404,7291,3781,122,-3439,-6664,-9347,-11325,-12485,-12772,-12193,-10809,-8737,-6135,-3192,-115,2885,5610,7883,9565,10558,10814,10335,9173,7425,5223,2727,112,-2442,-4765,-6708,-8149,-9004,-9231,-8830,-7844,-6356,-4476,-2343,-105,2084,4078,5747,6987,7726,7927,7588,6746,5471,3858,2025,101,-1783,-3501,-4940,-6012,-6652,-6828,-6539,-5817,-4721,-3332,-1753,-93,1532,3015,4259,5186,5740,5895,5648,5027,4082,2884,1521,89,-1315,-2597,-3672,-4474,-4955,-5090,-4878,-4343,-3528,-2495,-1318,-81,1131,2238,3168,3861,4277,4395,4213,3752,3050,2158,1144,76,-970,-1925,-2727,-3325,-3685,-3787,-3631,-3235,-2629,-1862,-988,-69,832,1654,2345,2860,3170,3258,3125,2784,2264,1604,853,64,-710,-1416,-2009,-2451,-2717,-2793,-2679,-2387,-1941,-1376,-733,-57,605,1210,1717,2096,2323,2388,2290,2040,1660,1178,629,52,-513,-1028,-1460,-1782,-1976,-2031,-1948,-1736,-1412,-1002,-536,-46,434,871,1237,1510,1674,1721,1650,1470,1196,850,455,41,-364,-733,-1042,-1272,-1410,-1449,-1389,-1238,-1007,-715,-384,-36,304,614,873,1066,1181,1214,1163,1036,843,599,322,32,-252,-510,-726,-886,-982,-1009,-967,-861,-700,-498,-268,-27,208,422,600,733,811,833,798,711,578,411,222,24,-170,-345,-491,-600,-664,-682,-653,-581,-472,-336,-181,-20,138,280,399,487,539,553,529,471,383,272,147,17,-110,-225,-320,-391,-433,-443,-424,-377,-306,-218,-118,-14,87,179,255,310,343,352,336,299,242,172,93,11,-68,-140,-199,-243,-268,-275,-262,-233,-189,-134,-73,-9,53,108,154,187,206,211,201,178,144,103,56,7,-40,-82,-116,-141,-155,-159,-151,-134,-108,-77,-41,-5,29,60,86,104,114,116,110,98,79,56,30,0,-21,-43,-61,-74,-81,-82,-78,-69,-55,-39,-21,0,14,30,42,50,55,56,53,46,37,26,14,0,-9,-19,-27,-32,-35,-35,-33,-29,-23,-16,-9,0,6,11,16,19,20,20,19,16,13,9,5,0,0,-6,-8,-9,-10,-10,-9,-7,-6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
 INTER_FACTOR  = 1;         
 DECI_FACTOR  =  10;
 MSB_RM  = 2;
 MSB_TYPE  = 0;
 LSB_RM  = 22;
 LSB_TYPE  = 0;
 FIR_WIDTH  = 32 + MSB_RM + LSB_RM;
 OUT_WIDTH  = 32 ;  %32
  DATA_WIDTH = 32;
 data_type = 1;

 % check size of inputs. 
 [DX,DY] = size(stimulation);
 [CX,CY] = size(coef_matrix); 
 if (CX ~= DY * INTER_FACTOR) 
    fprintf('WARNING : coef_matrix size and input data size is not match\n'); 
 end 
   
 %fill coef_matrix to length of data with the latest coef set 
 if (CX < DY * INTER_FACTOR) 
   for i= CX +1:DY * INTER_FACTOR 
     coef_matrix(i,:) = coef_matrix(CX,:); 
   end 
 end
  
        %check if input is integer 
       	int_sti=round(stimulation); 
	    T = (int_sti ~= stimulation); 
	    if (max(T)~=0) 
	        fprintf('WARNING : Integer Input Expected: Rounding Fractional Input to Nearest Integer...\n'); 
	    end 
	     
	    %Input overflow check 
	    switch  data_type 
	    case 1 
	        %set max/min for signed 
	        maxdat = 2^(DATA_WIDTH-1)-1; 
	        mindat = -maxdat-1; 
	    case 2 
	        %set max/min for unsigned 
	        maxdat = 2^DATA_WIDTH-1; 
	        mindat = 0; 
	    end 

	    if(data_type == 2)
	    	if(abs(coef_matrix) == coef_matrix)
	    		FIR_WIDTH = FIR_WIDTH +1;
	    	end
	    end

	    %Saturating Input Value 
	    a=find(int_sti>maxdat); 
	    b=find(int_sti<mindat); 
	    if (~isempty(a)|~isempty(b)) 
	 	    fprintf('WARNING : Input Amplitude Exceeds MAXIMUM/MINIMUM allowable values - saturating input values...\n'); 
	            lena = length (a); 
	            lenb = length (b); 
	            for i =1:lena 
	        	    fprintf('%d > %d \n', int_sti(a(i)), maxdat); 
			        int_sti(a(i)) = maxdat; 
		        end 
		    for i =1:lenb 
			    fprintf('%d < %d \n', int_sti(b(i)), mindat); 
			    int_sti(b(i)) = mindat; 
		    end 
	    end 
         
	    % Add interpolation 
   	    inter_sti = zeros(1, INTER_FACTOR * length(int_sti)); 
	    inter_sti(1:INTER_FACTOR:INTER_FACTOR * length(int_sti)) = int_sti; 
 
         
        for i = 1 : DY *INTER_FACTOR 
    	    coef_current = coef_matrix(i,:); 
            output_temp(i) = simp_adaptive (inter_sti, coef_current, i); 
        end 
	% Truncate output 
	len1 = length(output_temp); 
	 
	    switch  LSB_TYPE 
	    case 0 
	        %truncate 
            out_dec = bi_trunc_lsb(output_temp,LSB_RM,FIR_WIDTH); 
	    case 1 
	        %round 
            out_dec = bi_round(output_temp,LSB_RM, FIR_WIDTH); 
	    end 
         
 	    switch  MSB_TYPE 
	    case 0 
	        %truncate 
            out_dec = bi_trunc_msb(out_dec,MSB_RM,FIR_WIDTH-LSB_RM); 
	    case 1 
	        %round 
            out_dec = bi_satu(out_dec,MSB_RM, FIR_WIDTH-LSB_RM); 
	    end 
 	    
    	% choose decimation output in phase=DECI_FACTOR-1  
     	if(DECI_FACTOR == 1) 
     		output = out_dec; 
     	else
     		output = out_dec;%(DECI_FACTOR:DECI_FACTOR:len1);
 	end  
 	       
  	function[output, outindex] = simp_adaptive (int_sti, coef_current, data_index, output) 
	%Simulation is the whole input sequence 
	%coef_current is the current coefficient set 
	%data_index gives the last data to use 
	%outputs are the sum of input and coef multiplication 
	%outindex is the next data_index 
    
	sti_current = zeros(length(coef_current),1); 
	 
	data_length = length(int_sti); 
	 
	%Check data index 
	if (data_index > data_length) 
		fprintf('ERROR: DATA INDEX IS LARGER THAN DATA LENGTH!!!\n'); 
		return; 
	end 
	for i = 1: length(coef_current) 
	   if ((data_index -i+1)>0 & (data_index - i+1)<=data_length) 
	      sti_current(i,1) = int_sti(data_index - i+1); 
	   end 
	end 
	 
	outindex= data_index+1; 
	output = coef_current * sti_current; 
	% end of function simp_adaptive 
 
	function output = bi_round(data_in,LSB_RM,ORI_WIDTH, output) 
	% LSB_RM is the bit to lose in LSB 
	% ORI_WIDTH is the original data width
	data = round (data_in / 2^LSB_RM);       
	output = bi_satu(data,0,ORI_WIDTH - LSB_RM); 
	%end of function bi_trunc_lsb 
	 
	function output = bi_trunc_lsb(data_in,LSB_RM,ORI_WIDTH, output) 
	% LSB_RM is the bit to lose in LSB 
	% ORI_WIDTH is the original data width 
	%2's complement system 
	output = bitshift(2^ORI_WIDTH*(data_in<0) + data_in, -LSB_RM) - 2^(ORI_WIDTH-LSB_RM) *(data_in<0); 
	% end of function bi_round 
	 
	function output = bi_trunc_msb(data_in,MSB_RM,ORI_WIDTH, output) 
	% MSB_RM is the bit to lose in LSB 
	% ORI_WIDTH is the original data width 
	%2's complement system 
	data = 2^ORI_WIDTH * (data_in < 0)+ data_in; 
	erase_num = 2^(ORI_WIDTH - MSB_RM) - 1; 
	data = bitand(data, erase_num); 
	output = data - 2^(ORI_WIDTH - MSB_RM)*(bitget(data,ORI_WIDTH - MSB_RM)); 
	%end of bi_trunc_msb 
	 
	function output = bi_satu(data_in,MSB_RM,ORI_WIDTH, output) 
	% MSB_RM is the bit to lose in LSB 
	% ORI_WIDTH is the original data width 
	%2's complement system 
	maxdat = 2^(ORI_WIDTH - MSB_RM - 1)-1; 
	mindat = 2^(ORI_WIDTH - MSB_RM - 1)*(-1); 
	data_in(find(data_in > maxdat)) = maxdat; 
	data_in(find(data_in < mindat)) = mindat; 
	output = data_in; 
	%end of bi_satu  
         
