#include	<All_Header.h>
#include	<All_Extern_Variables.h>
#pragma CODE_SECTION(VF_Controller, "ramfuncs");
void VF_Controller()
{
	double a;
	static double Theta_VF= 0.; 

	freq_vf_set = Final_reference*Wrpm_rated/30.;
	if(freq_vf > freq_vf_set) {
		freq_vf -= 10.*Tsamp_VF;
		if(freq_vf < freq_vf_set) freq_vf = freq_vf_set;	
	}
	else {
		freq_vf += 10.*Tsamp_VF;
		if(freq_vf > freq_vf_set) freq_vf = freq_vf_set;	
	}
	We = (2.*PI)*freq_vf;
	a = Theta_VF + We*Tsamp_VF;
	Theta_VF = BOUND_PI(a);
	Sin_Theta_vtg = sin(Theta_VF);    	Cos_Theta_vtg = cos(Theta_VF);


	// Stationary -> Synchronous 
	Idse =  Cos_Theta*Idss + Sin_Theta*Iqss;
   	Iqse = -Sin_Theta*Idss + Cos_Theta*Iqss;

	#if (VOLTAGE_CLASS==0) // 200V
		Vdse_ref = 3. * freq_vf;
	#elif (VOLTAGE_CLASS==1) // 400V
		Vdse_ref = 6. * freq_vf;
	#endif
	Vqse_ref = 0.1;

	Ias_ref = Ias;	Ibs_ref = Ibs;	Ics_ref = Ics;
	if (!Driver_ON)	freq_vf=Theta=0.;

}




#pragma CODE_SECTION(Flux_Estimation, "ramfuncs");
void Flux_Estimation()
{
	
	double a, b;
 	double alpha=0.;
	double Sin_WeT=0.,Cos_WeT=0.;
	double Sin_alpha=0., Cos_alpha=1.;
	double Sin_Thetar=0., Cos_Thetar=1.;
	

//	double LAMdrs=0., LAMqrs=0.; 
//	double LAMdrs_cm=0., LAMqrs_cm=0.;
	double LAMdrs_cm_alpha=0., LAMqrs_cm_alpha=0.;
	double LAMdrs_vm_alpha=0., LAMqrs_vm_alpha=0.;
	double LAMdrs_vm_predict=0., LAMqrs_vm_predict=0.;

	static int alpha_enb=0;
//	static double Thetar= 0.;
	static double Vdss=0., Vqss=0.;
	static double Idsr=0., Iqsr=0.;
	static double LAMdss_vm=0., LAMqss_vm=0.;
	static double LAMdrs_vm=0., LAMqrs_vm=0.;
	static double LAMdrr_cm=0., LAMqrr_cm=0.;
	static double Edss=0., Eqss=0.;
	static double Err_LAMdst=0., Err_LAMqst=0.;
	// Sensorless
	double Sin_Thetae=0., Cos_Thetae=1.;
	static double LAMdre_ref_flt= 0.;
//	long i,j; 
//	static int Dir_theta= 1;
//	static long Theta_rm_counter=0;

 
//	static double LAMdss=0., LAMqss=0.;
//	static double Wc_Fst=0., Kp_Fst=0., Ki_Fst=0.,  Freq_Fst=10.;
//	static double LAMqre=0., LAMdse=0., LAMqse=0., LAMdre_cm=0., LAMqre_cm=0.;

/*
	j = (EQep1Regs.QPOSLAT)&(0xffff);
	i = j - Theta_rm_counter;
	if(i>(32768))		i = i - (65536);
	else if(i<-(32768))	i = i + (65536);
	Theta_rm_counter=j;

	// Direction 
	a= (double)i;
   	if(i*Dir_theta<0) {
		a +=Dir_theta*1.;
		Dir_theta*=-1;
	}  
	a = Thetar + (2.*PI)/(PPR*4.)*a*Pole_pair;
*/

	// Flux Estimation  
    // Current Model 
	a = Thetar + Wr*Tsamp_CC;

	Thetar = (float)(Driver_ON*Flux_build_up)*BOUND_PI(a);	 												  
   	Sin_Thetar = sin(Thetar);    	Cos_Thetar = cos(Thetar);


//	K_CM1=(1.-(Tsamp_CC/2.)*(Rr/Lr))/(1.+(Tsamp_CC/2.)*(Rr/Lr));
//	K_CM2=(Tsamp_CC/2.)*(Rr/Lr)*Lm/(1.+(Tsamp_CC/2.)*(Rr/Lr));
	if (P.G01.P06_Control_method== 3)
	{
		a= Cos_Thetar*Idss + Sin_Thetar*Iqss;
	       LAMdrr_cm=K_CM1*LAMdrr_cm+K_CM2*(a+Idsr);		Idsr=a;
		a= -Sin_Thetar*Idss + Cos_Thetar*Iqss;
	       LAMqrr_cm=K_CM1*LAMqrr_cm+K_CM2*(a+Iqsr);		Iqsr=a;

		LAMdrs_cm= Cos_Thetar*LAMdrr_cm-Sin_Thetar*LAMqrr_cm;
		LAMqrs_cm= Sin_Thetar*LAMdrr_cm+Cos_Thetar*LAMqrr_cm; 
	}
	else if (P.G01.P06_Control_method== 4)
	{	
		Sin_Thetae = sin(Thetae);    	Cos_Thetae = cos(Thetae);
		LAMdre_ref_flt+= (Rr/Lr*Tsamp_CC)*(Lm*Idse_ref-LAMdre_ref_flt);
		LAMdrs_cm= LAMdre_ref_flt*Cos_Thetae;
		LAMqrs_cm= LAMdre_ref_flt*Sin_Thetae;
	}

	// Delay Angle Compensation 
	a= We*Tsamp_CC;

	Sin_WeT = sin(a);    	Cos_WeT = cos(a);

	LAMdrs_vm_predict =  Cos_WeT*LAMdrs_vm - Sin_WeT*LAMqrs_vm;
	LAMqrs_vm_predict =  Sin_WeT*LAMdrs_vm + Cos_WeT*LAMqrs_vm; 

	// alpha 
	a = Ki_Fst*(Lr/Lm) - We*We;		b = Kp_Fst*(Lr/Lm)*We;


	a = PI-atan2(b,a);
	alpha = (double)alpha_enb*BOUND_PI(a);

    Sin_alpha = sin(alpha);    	Cos_alpha = cos(alpha);

	// Flux Estimation Controller 
	a=LAMdrs_cm-LAMdrs_vm_predict; 
	Edss=Edss+Kp_Fst*(a-Err_LAMdst)+Ki_Fst*Tsamp_CC*a;
   	Err_LAMdst=a;
	Edss = BOUND(Edss,20.,-20.);

	a=LAMqrs_cm-LAMqrs_vm_predict;  
	Eqss=Eqss+Kp_Fst*(a-Err_LAMqst)+Ki_Fst*Tsamp_CC*a;
	Err_LAMqst=a;
	Eqss = BOUND(Eqss,20.,-20.);

    // Voltage Model 
	a=-Rs*Idss+Edss; 
	LAMdss_vm=LAMdss_vm+Tsamp_CC*Vdss_ref_old+(Tsamp_CC/2.)*(a+Vdss);	
	Vdss=a;
    LAMdrs_vm=(Lr/Lm)*(LAMdss_vm-Lsigma*Idss);

	a=-Rs*Iqss+Eqss; 
	LAMqss_vm=LAMqss_vm+Tsamp_CC*Vqss_ref_old+(Tsamp_CC/2.)*(a+Vqss);	
	Vqss=a;
    LAMqrs_vm=(Lr/Lm)*(LAMqss_vm-Lsigma*Iqss);


	// alpha compensation 
   	LAMdrs_cm_alpha =  Cos_alpha*LAMdrs_cm + Sin_alpha*LAMqrs_cm;
    LAMqrs_cm_alpha = -Sin_alpha*LAMdrs_cm + Cos_alpha*LAMqrs_cm;
		
   	LAMdrs_vm_alpha =  Cos_alpha*LAMdrs_vm + Sin_alpha*LAMqrs_vm;
    LAMqrs_vm_alpha = -Sin_alpha*LAMdrs_vm + Cos_alpha*LAMqrs_vm;

	LAMdrs = LAMdrs_cm - LAMdrs_cm_alpha + LAMdrs_vm_alpha;
	LAMqrs = LAMqrs_cm - LAMqrs_cm_alpha + LAMqrs_vm_alpha;


//	LAMdss = (Lm/Lr)*LAMdrs + Lsigma*Idss;
//	LAMqss = (Lm/Lr)*LAMqrs + Lsigma*Iqss;

	if (!Driver_ON)	{
		Edss = Eqss = LAMdss_vm = LAMqss_vm = 0.;
	}


	//---- Flux Angle Detection ----//
	Theta = (float)(Driver_ON*Flux_build_up)*atan2(LAMqrs, LAMdrs); //hhh

    Sin_Theta = sin(Theta);    	Cos_Theta = cos(Theta);

	// Stationary -> Synchronous 
  	LAMdre =  Cos_Theta*LAMdrs + Sin_Theta*LAMqrs;
//  LAMqre = -Sin_Theta*LAMdrs + Cos_Theta*LAMqrs;  
// 	LAMdse =  Cos_Theta*LAMdss + Sin_Theta*LAMqss;
//  LAMqse = -Sin_Theta*LAMdss + Cos_Theta*LAMqss;
// 	LAMdre_cm =  Cos_Theta*LAMdrs_cm + Sin_Theta*LAMqrs_cm;
//  LAMqre_cm = -Sin_Theta*LAMdrs_cm + Cos_Theta*LAMqrs_cm; 




} 


#pragma CODE_SECTION(Current_Controller, "ramfuncs");
void Current_Controller()
{
	static double Vdse_ref_integ= 0., Vdse_ref_ff= 0.;
	static double Vqse_ref_ff= 0.; 

	double a, b;
	double Theta_vtg= 0.;
	double Idse_flt=0., Iqse_flt=0.;
	double Err_Idse= 0., Err_Iqse= 0.; 


//	static double Vdse_ref_fb=0., Vqse_ref_fb=0.; // 관측용 변수 


//	Kp_cc = Lsigma*1250.;
//	Ki_cc = Rs*2.*1250;
//	Ka_cc = 3./Kp_cc;
	a = Theta + We*(1.5*Tsamp_CC);

	Theta_vtg = BOUND_PI(a);				
	Sin_Theta_vtg = sin(Theta_vtg);    	Cos_Theta_vtg = cos(Theta_vtg);

	// Stationary -> Synchronous 
	a =  Cos_Theta*Idss + Sin_Theta*Iqss;
   	b = -Sin_Theta*Idss + Cos_Theta*Iqss;

	Idse_flt = a;
	Iqse_flt = b;

	Idse = a;
	Iqse = b;

	// d-axis Current Control 
	Err_Idse = Idse_ref-Idse_flt;
    Vdse_ref_integ+= Ki_cc*Tsamp_CC*(Err_Idse-Ka_cc*(Vdse_ref_fb+Vdse_ref_ff-Vdse_ref)); 
	Vdse_ref_fb=Vdse_ref_integ+Kp_cc*Err_Idse; // Vdse_ref_fb 은 0~Rs*Idse -> 관측용 전역변수 
	Vdse_ref_ff=-We*Lsigma*Iqse-Rr*Lm/(Lr*Lr)*LAMdre;
		
//	Vdse_ref_ff = 0.;
	Vdse_ref = Vdse_ref_fb + Vdse_ref_ff;  

	// q-axis Current Control 
	Err_Iqse = Iqse_ref-Iqse_flt;
	Vqse_ref_integ+= Ki_cc*Tsamp_CC*(Err_Iqse-Ka_cc*(Vqse_ref_fb+Vqse_ref_ff-Vqse_ref));
	Vqse_ref_fb=Vqse_ref_integ+Kp_cc*Err_Iqse;  // Vqse_ref_fb 은 0~Rs*Iqse -> 관측용 전역변수
	Vqse_ref_ff=We*Lsigma*Idse+Wr*Lm/Lr*LAMdre;
//	Vqse_ref_ff = 0.;
	Vqse_ref = Vqse_ref_fb + Vqse_ref_ff; 
	 

//	Vdse_ref = BOUND(Vdse_ref,20., -20.);
//	Vqse_ref = BOUND(Vqse_ref,20., -20.);

	// Current controller disable
	if (!Driver_ON)	{
		Vdse_ref=Vqse_ref=Vdse_ref_integ=Vqse_ref_integ=0.;
	}
	Idss_ref = Ias_ref = Cos_Theta_vtg*Idse_ref - Sin_Theta_vtg*Iqse_ref;
	Iqss_ref = Sin_Theta_vtg*Idse_ref + Cos_Theta_vtg*Iqse_ref;
	Ibs_ref = -0.5*(Idss_ref - SQRT3*Iqss_ref);
	Ics_ref = -(Ias_ref+Ibs_ref);
	
//	Ias_ref= Ias; Ibs_ref= Ibs; Ics_ref= Ics; 

} 
#pragma CODE_SECTION(SVPWM, "ramfuncs");
void SVPWM()
{
	long i;
	double a, b;
	double Vas_ref= 0., Vbs_ref= 0., Vcs_ref= 0.;
	double Van_ref= 0., Vbn_ref= 0., Vcn_ref= 0.;
	double V_max= 0., V_min= 0., V_offset= 0.;
	double V_dt= 0., K_dt= 0., inv_Vdc= 0.;
	
	#if (DUAL_PWM_INTERRUPT)
		static int Flag_UPDN_PWM= 0;
	#endif   

	// Synchronous -> Stationary 
	Vdss_ref_old = Vdss_ref;		Vqss_ref_old = Vqss_ref;
	Vdss_ref = Vas_ref = Cos_Theta_vtg*Vdse_ref - Sin_Theta_vtg*Vqse_ref;
	Vqss_ref = Sin_Theta_vtg*Vdse_ref + Cos_Theta_vtg*Vqse_ref;
	
	// Stationary -> 3 Phases 
	Vbs_ref = -0.5*(Vdss_ref - SQRT3*Vqss_ref);
	Vcs_ref = -(Vas_ref+Vbs_ref);
	
	// Max & Min 
	if (Vas_ref > Vbs_ref)	{ 
		V_max = Vas_ref; 			V_min = Vbs_ref;
		if (Vcs_ref > V_max) 		V_max = Vcs_ref;
		else if (Vcs_ref < V_min)	V_min = Vcs_ref;
	}
	else { 
		V_max = Vbs_ref; 			V_min = Vas_ref;
		if (Vcs_ref > V_max) 		V_max = Vcs_ref;
		else if (Vcs_ref < V_min)	V_min = Vcs_ref;
 	}

	// Calculate and Add voltage offset, V_offset = voltage offset 
	V_offset = -(V_max+V_min)/2.;
	Van_ref = Vas_ref + V_offset;
	Vbn_ref = Vbs_ref + V_offset;
	Vcn_ref = Vcs_ref + V_offset;

	// Overmodulation 
//	Flag_OM=0; 
	if( (V_max-V_min) > Vdc) {
		a = Vdc/2.;		b=-a;

		Van_ref = BOUND(Van_ref,a,b);
		Vbn_ref = BOUND(Vbn_ref,a,b);
		Vcn_ref = BOUND(Vcn_ref,a,b);

		Vas_ref = (2.*Van_ref-Vbn_ref-Vcn_ref)/3.;
		Vbs_ref = (2.*Vbn_ref-Vcn_ref-Van_ref)/3.;
		Vcs_ref = -(Vas_ref + Vbs_ref);
		  
		Vdss_ref = Vas_ref;
		Vqss_ref = (Vbs_ref-Vcs_ref)/SQRT3;
		Vdse_ref =  Cos_Theta_vtg*Vdss_ref + Sin_Theta_vtg*Vqss_ref;
		Vqse_ref = -Sin_Theta_vtg*Vdss_ref + Cos_Theta_vtg*Vqss_ref;
//		Flag_OM=1; 
	}

	// Dead_Time Compensation 
	V_dt = Vdc*( (T_DEAD_US-T_dead_tuning)/(Tsamp_CC*1.e+6) );	
	K_dt = V_dt/(I_DT);
	

	#if (DUAL_PWM_INTERRUPT)
		Flag_UPDN_PWM=1-Flag_UPDN_PWM;
		if(Flag_UPDN_PWM) {			// next down mode 
			a = K_dt*(Ias_ref+I_DT);	a = BOUND_HHH(a,V_dt,0.);		Van_ref += a;
			a = K_dt*(Ibs_ref+I_DT);	a = BOUND_HHH(a,V_dt,0.);		Vbn_ref += a;
			a = K_dt*(Ics_ref+I_DT);	a = BOUND_HHH(a,V_dt,0.);		Vcn_ref += a;
		}
		else {						// next up mode 
			a = K_dt*(Ias_ref-I_DT);	a = BOUND_HHH(a,0,(-V_dt));		Van_ref += a;
			a = K_dt*(Ibs_ref-I_DT);	a = BOUND_HHH(a,0,(-V_dt));		Vbn_ref += a;
			a = K_dt*(Ics_ref-I_DT);	a = BOUND_HHH(a,0,(-V_dt));		Vcn_ref += a;
		}
		
	#else
		a= K_dt*Ias_ref;	a= BOUND_HHH(a, V_dt, (-V_dt)); Van_ref+= a;
		a= K_dt*Ibs_ref;	a= BOUND_HHH(a, V_dt, (-V_dt));	Vbn_ref+= a; 
		a= K_dt*Ics_ref;	a= BOUND_HHH(a, V_dt, (-V_dt));	Vcn_ref+= a; 
	#endif


	// Write PWM counter to ASIC 
	// Reference voltage --> Counter Value  	// max : dead_time 고려 
	inv_Vdc = 1./Vdc;
	i = (int)( (Van_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[u]=BOUND(i,EPwmPeriodCount,0);
	i = (int)( (Vbn_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[v]=BOUND(i,EPwmPeriodCount,0);
	i = (int)( (Vcn_ref*inv_Vdc+0.5)*EPwmPeriodCount+0.5);	DutyCount[w]=BOUND(i,EPwmPeriodCount,0);

//	EPwm1Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[u];
//	EPwm2Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[v];
//	EPwm3Regs.CMPA.half.CMPA = EPwmPeriodCount - DutyCount[w];

	EPwm1Regs.CMPA.half.CMPA = DutyCount[u];
	EPwm2Regs.CMPA.half.CMPA = DutyCount[v];
	EPwm3Regs.CMPA.half.CMPA = DutyCount[w];


}



#pragma CODE_SECTION(Field_Weakening, "ramfuncs");
void Field_Weakening()
{
	double a, b;
	double Vs_max= 0.;
	double We_fw1= 0., We_fw2= 0.; 


//	Vs_max = (Vdc*0.95)/SQRT3;
//	Vs_max = (Vdc*0.9)/SQRT3;
/*
	b= (double)P.G01.P09_Supply_voltage*SQRT2*1.05;
	if(Vdc>b) a = b;
	else		a = Vdc;
*/
	Vs_max = (Vdc*0.9)/SQRT3;

    We_fw1= sqrt( (Vs_max*Vs_max) / K_fw1 );
	We_fw2= K_fw2*(Vs_max/Is_max);
	Wrpm_fw1=We_fw1/Pole_pair*WRM2RPM;		Wrpm_fw2=We_fw2/Pole_pair*WRM2RPM;

   	a=fabs(Wr)+fabs(Rr*Lm/Lr*Iqse_max_fw/LAMdre_ref);

    if(a<We_fw1) { 								
        LAMdre_ref=LAMdre_rate;
        b = LAMdre_ref/Lm;		  				Iqse_max_fw=sqrt(Is_max*Is_max-b*b);
		Flag.Monitoring.bit.FW_MODE=0.;
	}		
    else if( (a>We_fw1) && (a<We_fw2) ) { 			
        b= ( (Vs_max*Vs_max)/(a*a)-(Lsigma*Is_max)*(Lsigma*Is_max) ) / (Ls*Ls-Lsigma*Lsigma);
        LAMdre_ref=Lm*sqrt(b);        			Iqse_max_fw=sqrt(Is_max*Is_max-b);
		Flag.Monitoring.bit.FW_MODE=1.;
	}
    else {                               		
        LAMdre_ref=Lm*Vs_max/(SQRT2*a*Ls);		Iqse_max_fw=Vs_max/(SQRT2*a*Lsigma);
		Flag.Monitoring.bit.FW_MODE=2.;
    }
}
#pragma CODE_SECTION(Flux_Controller, "ramfuncs");
void Flux_Controller()
{
	double a;
	double Err_flux= 0.;
	static double Alpha_fc= 0.;
	static double Idse_ref_integ= 0., Idse_ref_ff= 0., Idse_ref_fb= 0.;

//	Wc_fc=50.;
//	Kp_fc = (Lr/Lm)*Wc_fc/Rr;
//	Ki_fc = Wc_fc/Lm;
//	Ka_fc = 2./Kp_fc; 

	LAMdre_flt+= ((Wc_fc*5.)*Tsamp_FC)*(LAMdre-LAMdre_flt);
	if(!Driver_ON) {	  
		Idse_ref = Idse_ref_fb = Idse_ref_ff = Idse_ref_integ = 0.;
		Flux_build_up=0;
	}
	else {
//		if(LAMdre_flt>(0.99*LAMdre_ref))	Flux_build_up=1;
		if(LAMdre_flt>(0.49*LAMdre_ref))	Flux_build_up=1;

		// IP로 사용중 -> 모터 소음이 작다 
		Err_flux = LAMdre_ref - LAMdre_flt;
		Idse_ref_integ += Ki_fc*Tsamp_FC*(Err_flux-Ka_fc*(Idse_ref_fb+Idse_ref_ff-Idse_ref));
		Idse_ref_fb = Alpha_fc*Kp_fc*Err_flux + Idse_ref_integ - (1.-Alpha_fc)*Kp_fc*LAMdre_flt;
		Idse_ref_ff = LAMdre_ref/Lm;
		a = Idse_ref_fb + Idse_ref_ff;
		// Limit 
		Idse_ref  = BOUND(a, Idse_ref_max, (-Idse_ref_max));

	}	
}

#pragma CODE_SECTION(Speed_Controller, "ramfuncs");
void Speed_Controller()
{
	double a, b;
	double Wrm_ref= 0., Err_Wrm= 0.;
//	double Wsl= 0.;
	
	static double Wrm_flt= 0., Alpha_sc= 0.;
	static double Iqse_ref_integ= 0., Iqse_ref_fb= 0., Iqse_ref_ff= 0.;

	#if (VF_MODE==0)
		if(!OP.Run_stop.bit.MOTOR_TUNING)	Wrpm_ref = Final_reference*Wrpm_rated;
	#endif 

	Wrm_flt+= (1250.*Tsamp_SC)*(Wrm_det_flt-Wrm_flt);
//	Wc_sc = 250.;

	a= fabs(Wrm_flt / (RPM2WRM*Wrpm_rated));

	if (a>0.3)	b= Wc_sc*0.5;
	else if (a<0.1) b= Wc_sc;
	else b= Wc_sc*(1.-2.5*(a-0.1));  // Wc_sc*(1.-1.*(a-0.1));

	Kp_sc = Jm*b/Kt;
	Ki_sc = Kp_sc*(b/5.); 
	Ka_sc = 2./Kp_sc; 

//	Kp_sc = Jm*Wc_sc/Kt;
//	Ki_sc = Kp_sc*(Wc_sc/5.); 
//	Ka_sc = 2./Kp_sc; 

	Wrm_ref = RPM2WRM * Wrpm_ref;
	Err_Wrm = Wrm_ref - Wrm_flt;
	// Driver Off 
	if( (!Driver_ON) || (!Flux_build_up) ) {	
		Iqse_ref = Iqse_ref_fb = Iqse_ref_ff = Iqse_ref_integ = 0.;
		Wsl = We = 0.;


	}
	// Driver On 
	else {
		a = sqrt(Is_max*Is_max - Idse_ref*Idse_ref); 
		if(a<Iqse_max_fw)	Iqse_ref_max = a;
		else				Iqse_ref_max = Iqse_max_fw;

		if (P.G01.P06_Control_method== 3)		Alpha_sc= 1.;
		else if (P.G01.P06_Control_method== 4)	Alpha_sc= 0.;
		else									Alpha_sc= 1.;

		// PI Controller
		Iqse_ref_integ += Ki_sc*Tsamp_SC*(Err_Wrm-Ka_sc*(Iqse_ref_fb+Iqse_ref_ff-Iqse_ref));
		Iqse_ref_fb = Alpha_sc*Kp_sc*Err_Wrm + Iqse_ref_integ - (1.-Alpha_sc)*Kp_sc*Wrm_flt;
		Iqse_ref_ff = 0.;
		

		a = Iqse_ref_fb + Iqse_ref_ff;
		// Limit 
		Iqse_ref  = BOUND(a, Iqse_ref_max, (-Iqse_ref_max));
		// Angular Frequency 
		// 옮겨야 되지 않을까??????
		Wsl = Rr*Lm/Lr*Iqse_ref/LAMdre_ref;
		We = Wr + Wsl;	
	}

}


// end of file



