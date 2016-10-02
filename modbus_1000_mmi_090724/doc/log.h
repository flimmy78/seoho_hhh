/**
    @mainpage  Project : MODUS-1000_MMI
    @section intro �Ұ�
        - �Ұ� : datfile_t�� �ؽ�Ʈ ������ �̿��Ͽ� ���ڿ�, ����, �Ǽ�, Boolean ����
                 �����ڷ� �����ϸ鼭 �аų� �� �� �ֵ��� ó���Ѵ�.
        - ���� : main() �Լ����� datfile_t�� �̿��Ͽ� dat ���� ������ ����ϴ� ������ ����
 
 //-----------------------------------------------------------------------------------------------------
@section HISTORY �����丮
090223
	- ����(modbusw)�� �ҽ��� EPU-100���忡 ������
	- �ҽ��м��� ���� ���߰� ���ۿ� �̻��� �����
	- ���̽��ҽ��� ��ó�� ���ۿ��� �˻��� �����ȿ� PDF������ ����


090317
  - PCB�԰� : 4�� ����

090320
  �⺻���� �׽�Ʈ:
  - ���� MODUS�������� �ҽ��� �����Ͽ�  LED���� �׽�Ʈ

  - ����̹� �浹�� �߻��Ͽ�  AVR-JTAG�� �ν����� ���Ͽ���
   - ���α׷��� ���� ��ġ �Ͽ� �ذ�

090324
	- UART���� ����
		- PC�� ����Ͽ� ������ �ϴ��� Ȯ����
		- modbus�������� �ҽ��� �������ϴ��� Ȯ����
   		- ���ä�� 4ä���� ���ʷ� ����
     	-	������� �̻����
		- LCD����
		  - ���߿����� ���纸��� �޸����� ��� ��ǰ�� �����Ͽ�
		   LCD���� �Ųٷ� �Ǿ����
		  - ���� �������� �ٽÿ���
		 	- ������ �е尡 ������������ ������ ����
		   - �����ؾ���

090331
	- MMI���� SMPS���� ����
  	- ���������� ������
	- LCD�� �����ȣ������ ������
  - ���α׷� ���ߵ� �����ϰ� ��������� �����ʾ�
   ���� ���带 ������
   	- MCU�� �ٽ� ���̰� �⺻���� �׽�Ʈ
    :�̻����
	- LCD�� Ÿ�̹� ������ �κ��� ���α׷��� ����, 
   ���ʿ��� ��Ҹ� �����ϰ� ����ȭ��

090401
	 - LCD SHIFT ��ũ�� �Լ� ����
	  - �޴�ȭ�� ���α׷� �ۼ��� �� �Ʒ����� ��ũ�� ��Ŵ 
	 - GPIO�׽�Ʈ LED�� ������ ����� ��Ʈ�� ������ Ȯ����
	 - �������
	  - uart���� Ȯ�� :  �̻����
	 	- modbus �ҽ����� : ������ ��Ʈ�� ����� �����Ͽ� Ʃ���Ͽ��� ��
	  - LCD ��� : �̻����
	  - GPIO : ����Ȯ����
	 - ��������
	 	- adcŰ ����
	  - menu ���α׷�
	    -dsp���� modbus �������� ����
	
090402
	 - ADCŰ ���� ����
	  - ADC�ʱ�ȭ�ϴ� �������� �������� ������ �ټ� 
	   �����ߴ� �κ��� �־���
	   - xadc5�� xadc4~0�� ������������
	
	 - ���� 10��Ʈ �� ���� 2��Ʈ�� ������� �ʰ�
	   Ű����Ȯ����
	   - default 0xE0, 0x80, 0xC0, 0xAB, 0xC0, 0xD6, 0xDC����
	   Ű�� ������
	
	 - LCD�� �ѱ� ���� ������
	  : �ѱ���Ʈ ����
	 
	 - UART ���α׷� ����ȭ
	
	
	 - MENU ���α׷� ����
	  ; �ҽ� ������
	   
	090403
	- KEY������ �̿��Ͽ�
	  MENU �׺���̼� ������ ������
		 - �� : ����/10
	   - �� : ����*10
	   - �� : ���� +1
	   - �Ʒ� : ���� -1
	
	����� ������ �޴��ڵ鸵�� ���� �ڵ��� ��  
	
	
	090414
	 - ������ ������ ���� �� 
	  - DSP �����ʿ� ��Ŷ ������ �����Ͱ� ���ŵ��� �ʴ� ����
	  - MMI�� 485�̳��̺� ��ȣ�� �����ͺ��� ���� ������
	    - ���ͷ�Ʈ�� ó���� dsp�ʿ��� �����Ͱ� �� ���޹����� Ȯ����
	  - MODUS�������α׷����� ��Ŷfault�� ���۵��ϴ� ���� �ҽ�����
	  - MMI�� 485 TX �̳��̺��� RXD���ڰ� LOW�� �������� ����
	   - AVR�� �̰��� 0x00���� �ν���
	  - 485 �̳��̺� ��ȣ�� ������Ʈ�� disable�������� ���۵��� ����
	
	  - DSP���� �����ͼ����� ���� �۽��� �� 10us �����̸� ��
	   - �ۼ��ŵ������� ������ ���� 
	
	090415(modbus_1000_mmi_090415)
	 - MODBUS�������� ������ ����
	  - ���� ���Ǵ� ������ Ȯ���ϰ� �ҽ� ������
	
	 - MENUȭ�� ���� 
	  - LCD���� �ڵ带 ���� ��Ȯ�ϰ� ������
	  - ��ũ�� ����� �����۵� ��
	
	 - MENU�ڵ鷯(�����̸�)�� ���Ͽ� �޴�ȭ�� �̵� 
	  - adcŰ ��� UARTŰ�� ��ü
	
	- MENUȭ�� ���� 
	  - 1. ��������
	  - 2. ������ü
	  - 3. �̷º���
	  - 4. �Ķ��Ÿ ����
	\n;�������º��� ������������ ������κ��� �о��
	 LCD�� ����ϴ� ���α׷�
	
	090415(modbus_1000_mmi_090416)
	 - ������Ʈ ���
	
	
	090429(modbus_1000_mmi_090423)
	- �޸𸮸�(main_data_array): �޸�����
	- 3�� �޴��� 4�� �޴� ��ġ �ٲ�
	
	
	090506(modbus_1000_mmi_090506)
	- �����丮, �н�����, ��¥ ����
	- �̺�Ʈ �Է¹޾� ���

% release	
 //-----------------------------------------------------------------------------------------------------	
\n090527(modbus_1000_mmi_090527)
	 - MODBUS���õ� ���α׷� ����
	   DSP
	   - modbus -> data_array -> ����� -> data_array ->modbus
	    �� ���� �����Ͱ� ���޵Ǿ�� �ϴµ� ��������� data_array��
	    ���簡 ���� �̷���� ���ŵ����Ͱ� ������������
	   	 - void modbusStoreData(QUERY* query ) �Լ��� 
	   read_com_buf();�� ���������� �ذ�
	   - Bound_min_max(tmp, -32767, 32767)���� tmp����
	    ������ �� 0���� �Ǿ����
	@code
	/*
	  // Vin_offset
	  tmp = main_data_array[70];
	  Vin_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vin_gain
	  tmp = main_data_array[71];
	  Vin_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Ain_offset
	  tmp = main_data_array[72];
	  Ain_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Ain_gain
	  tmp = main_data_array[73];
	  Ain_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vout_offset
	  tmp = main_data_array[74];
	  Vout_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vout_gain
	  tmp = main_data_array[75];
	  Vout_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Aout_offset
	  tmp = main_data_array[76];
	  Aout_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Aout_gain
	  tmp = main_data_array[77];
	  Aout_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbyp_offset
	  tmp = main_data_array[78];
	  Vbyp_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbyp_gain
	  tmp = main_data_array[79];
	  Vbyp_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbat_offset
	  tmp = main_data_array[80];
	  Vbat_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vbat_gain
	  tmp = main_data_array[81];
	  Vbat_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Abat_offset
	  tmp = main_data_array[82];
	  Abat_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Abat_gain
	  tmp = main_data_array[83];
	  Abat_gain = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vdc_offset
	  tmp = main_data_array[84];
	  Vdc_offset = (Bound_min_max(tmp, -32767, 32767));
	  
	  // Vdc_gain
	  tmp = main_data_array[85];
	  Vdc_gain = (Bound_min_max(tmp, -32767, 32767));
	  */
	
	  // Vin_offset
	  Vin_offset = main_data_array[70];
	  // Vin_gain
	  Vin_gain = main_data_array[71];
	  // Ain_offset
	  Ain_offset = main_data_array[72];
	  // Ain_gain
	  Ain_gain = main_data_array[73];
	  // Vout_offset
	  Vout_offset = main_data_array[74];
	  // Vout_gain
	  Vout_gain = main_data_array[75];
	  // Aout_offset
	  Aout_offset = main_data_array[76];
	  // Aout_gain
	  Aout_gain = main_data_array[77];
	  // Vbyp_offset
	  Vbyp_offset = main_data_array[78];
	  // Vbyp_gain
	  Vbyp_gain = main_data_array[79];
	  // Vbat_offset
	  Vbat_offset = main_data_array[80];
	  // Vbat_gain
	  Vbat_gain = main_data_array[81];
	  // Abat_offset
	  Abat_offset = main_data_array[82];
	  // Abat_gain
	  Abat_gain = main_data_array[83];
	  // Vdc_offset
	  Vdc_offset = main_data_array[84];
	  // Vdc_gain
	  Vdc_gain = main_data_array[85];
	
	@endcode
	
	   MMI
	   - makeQuery1�Լ��� ���׹߰�/����
	    �μ� ���޺κ��� ����� �����Ǿ��־� Ư�������� �����Ͱ�
	    �߸����޵Ǵ� ����
	
	   - �ʱ� �޴����� 10�� �ڿ� ���뵵 ȭ������ ��ȯ
	@code
	void SYS_1(void)
	{
		static unsigned char flag=0;
	
		if(EventFlagC)
		{
			if(KeyState.KeyValue == ESC)naviMENU = naviMENU / 10;
			else if(KeyState.KeyValue == UP)naviMENU = naviMENU - 1;
			else if(KeyState.KeyValue == DN)naviMENU = naviMENU + 1;
			else if(KeyState.KeyValue == ENTER)naviMENU = naviMENU * 10;
			else if(KeyState.KeyValue == (ESC  & UP & DN & ENTER))
			{
				naviMENU = 0xFFFFFFFF;
			}
			
			if(KeyState.KeyValue != 0xFF)EventTimeC=0;
		}
		else 
		{
			if(KeyState.KeyValue != 0xFF)EventFlagC=1;
			if(naviMENU!=old_naveMENU)EventFlagC=1;
		}
	
		if((!EventFlagC)&&(flag))RefreshFlag=1;// event edge
		else if((EventFlagC)&&(!flag))RefreshFlag=1;// event edge
		flag = EventFlagC;
	}
	
	void PAGE_1(void)
	{
		if(EventFlagC)
		{
			GLCD_BufClear();	
			GLCD_print0508(5, 0,"--MODUS-1000 UPS--");
			GLCD_print0508(2, 2, "1.SYSTEM MEASURE");
			GLCD_print0508(2, 3, "2.SYSTEM OPERATION");
			GLCD_print0508(2, 4, "3.SYSTEM HISTORY");
			GLCD_print0508(2, 5, "4.PARAMETER SETTING");
			
			GLCD_inversion(12,2,112);
		}
		else
		{
			GLCD_loaddiagram();
		}
	}
	@endcode
	
	   - ������/���� ����޴�(4400..) ������ġ(posInpage)��
	     1�� �ڸ��� ������
	@code
			else if(KeyState.KeyValue == RIGHT){	posInpage=5;RefreshFlag=1;}
			else if(KeyState.KeyValue == LIFT)	{	posInpage=0;RefreshFlag=1;}
			else if(KeyState.KeyValue == ENTER){	posInpage=5;RefreshFlag=1;}
	@endcode
	
	
	090527(modbus_1000_mmi_090610)
	- �޸𸮸ʰ� �Ʒ� ������ ��ġ���� �ʾ���
	   Bat_Float_Volt
	   Bat_Equal_volt
	   Bat_Equal_Curr
	   �޸� ���� Ȯ���ϰ� �����Ͽ� ������
	
	
	- ������/������ ���� ������ ��  ��Ų��� ȭ���� �ߴ� ����
	@code
	else if(rc_poll==(BYTE)CMPLT_NO_HOPE)
	{
	start_time=0;//get_milliseconds();
	mseconds = 0;
	communication_fault = 0;
			communication_fault_cnt = 0;
	modbus_state=0;
	clr_rx() ;
	Poll_flag=0;
	}
	@endcode
	����� ���� �ξ� �ذ���
	3���� ����� ���� ����
	cmplt : �������
	cmplt_hope : ������ (����� �̷������ ������ Ÿ�Ӿƿ��߻�)
	CMPLT_NO_HOPE : ���۽���(����� ������´� �ƴ�)
	
	
	- �˶�/LED ���� ����
	- �����丮 �߻����� ����
 @code
	//===================================================
	//Vin OUT
			if((main_data_array[42] & 0x0008) && (!(event_array[2]&0x0008))){history_update(33);BuzzerFlag=1;AlarmLED=1;}				//
			else if(!(main_data_array[42] & 0x0008) && ((event_array[2]&0x0008))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	
	
			if((main_data_array[42] & 0x0080) && (!(event_array[2]&0x0080))){history_update(29);BuzzerFlag=1;AlarmLED=1;}				//
			else if(!(main_data_array[42] & 0x0080) && ((event_array[2]&0x0080))){AlarmLED=0;GLCD_BuzzerOnOff(0);}	
	
			if((main_data_array[42] & 0x0020) && (!(event_array[2]&0x0020))){history_update(31);}				//
			else if(!(main_data_array[42] & 0x0020) && ((event_array[2]&0x0020))){}	
	//====================================================
	
	//====================================================
	//Vin IN
			if((main_data_array[42] & 0x0004) && (!(event_array[2]&0x0004))){history_update(34);}				//
			else if(!(main_data_array[42] & 0x0004) && ((event_array[2]&0x0004))){}	
	
			if((main_data_array[42] & 0x0040) && (!(event_array[2]&0x0040))){history_update(30);}				//
			else if(!(main_data_array[42] & 0x0040) && ((event_array[2]&0x0040))){}	
	
			if((main_data_array[42] & 0x0010) && (!(event_array[2]&0x0010))){history_update(32);}				//
			else if(!(main_data_array[42] & 0x0010) && ((event_array[2]&0x0010))){}	
	//====================================================
@endcode	
	 - ��� ���� ���� ����
	  ; ����� �������µ� Ÿ�� �ƿ��� ��Ŷ�� 2ȸ �������� �� �� ��Ų��� �̺�Ʈ �߻� 
@code
		rc_poll=modbusPacketResponseComplete();
	
		if ( rc_poll==(BYTE)CMPLT_COMPLETE )
		{
			modbus_state++;
			 start_time=0;//get_milliseconds();
	 		 mseconds = 0;
			communication_fault = 0;
			communication_fault_cnt = 0;
			break;
		}
	
		/******************************************************************/
		/*       IF NO RESPONSE RECEIVED TIME OUT AND SEND NEXT POLL      */
		/******************************************************************/
		if ((get_milliseconds()-start_time)>=(UINT32)MB_POLL_TIMEOUT)
		{
			modbus_state=0;
			clr_rx() ;
	
			communication_fault_cnt++;
			if(2<communication_fault_cnt)
			{
				communication_fault = 1;
			}		
			Poll_flag=0;
		}
	         break;
@endcode
	
	
-	090728(modbus_1000_mmi_090724)
	- page44XXX SAVE/CANCEL ������ ���� ����
	- �� �� �Ҽ��� ���� ���� 
	  - ���� ��Ÿ , ���� ���� ����
	
	
	
	
	*/