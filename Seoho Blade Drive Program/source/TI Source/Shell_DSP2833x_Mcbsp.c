  // TI File $Revision: /main/16 $
// Checkin $Date: October 3, 2007   14:50:19 $
//###########################################################################
//
// FILE:	DSP2833x_McBSP.c
//
// TITLE:	DSP2833x Device McBSP Initialization & Support Functions.
//
//###########################################################################
// $TI Release: DSP2833x Header Files V1.10 $
// $Release Date: February 15, 2008 $
//###########################################################################

#include "All_Const.h"
#include "Core_DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "Core_DSP2833x_Examples.h"   // DSP2833x Examples Include File

//---------------------------------------------------------------------------
// MCBSP_INIT_DELAY determines the amount of CPU cycles in the 2 sample rate
// generator (SRG) cycles required for the Mcbsp initialization routine.
// MCBSP_CLKG_DELAY determines the amount of CPU cycles in the 2 clock
// generator (CLKG) cycles required for the Mcbsp initialization routine.
// For the functions defined in Mcbsp.c, MCBSP_INIT_DELAY and MCBSP_CLKG_DELAY
// are based off of either a 150 MHz SYSCLKOUT (default) or a 100 MHz SYSCLKOUT.
//
// CPU_FRQ_100MHZ and CPU_FRQ_150MHZ are defined in DSP2833x_Examples.h
//---------------------------------------------------------------------------

#define MCBSP_SRG_FREQ       CPU_CLOCK/4                    // SRG input is LSPCLK (SYSCLKOUT/4) for examples

#define CLKGDV_VAL           1
#define MCBSP_INIT_DELAY     2*(CPU_CLOCK/MCBSP_SRG_FREQ)                  // # of CPU cycles in 2 SRG cycles-init delay
#define MCBSP_CLKG_DELAY     2*(CPU_CLOCK/(MCBSP_SRG_FREQ/(1+CLKGDV_VAL))) // # of CPU cycles in 2 CLKG cycles-init delay
//---------------------------------------------------------------------------
// InitMcbsp:
//---------------------------------------------------------------------------
// This function initializes the McBSP to a known state.
//

void delay_loop(void);		// Delay function used for SRG initialization
void clkg_delay_loop(void); // Delay function used for CLKG initialization

void InitMcbsp(void)
{
	InitMcbspa();
	#if DSP28_MCBSPB
	  InitMcbspb();
	#endif               // end DSP28_MCBSPB
}

void InitMcbspa(void)
{
/*
// McBSP-A register settings


    McbspaRegs.SPCR2.all=0x0000;		// Reset FS generator, sample rate generator & transmitter
	McbspaRegs.SPCR1.all=0x0000;		// Reset Receiver, Right justify word
	McbspaRegs.SPCR1.bit.DLB = 1;       // Enable loopback mode for test. Comment out for normal McBSP transfer mode.


	McbspaRegs.MFFINT.all=0x0;			// Disable all interrupts

    McbspaRegs.RCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Receive)
    McbspaRegs.RCR1.all=0x0;

    McbspaRegs.XCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Transmit)
    McbspaRegs.XCR1.all=0x0;

    McbspaRegs.PCR.bit.FSXM = 1;		// FSX generated internally, FSR derived from an external source
	McbspaRegs.PCR.bit.CLKXM = 1;		// CLKX generated internally, CLKR derived from an external source

    McbspaRegs.SRGR2.bit.CLKSM = 1;		// CLKSM=1 (If SCLKME=0, i/p clock to SRG is LSPCLK)
	McbspaRegs.SRGR2.bit.FPER = 31;		// FPER = 32 CLKG periods

    McbspaRegs.SRGR1.bit.FWID = 0;              // Frame Width = 1 CLKG period
    McbspaRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;	// CLKG frequency = LSPCLK/(CLKGDV+1)

    delay_loop();                // Wait at least 2 SRG clock cycles

    McbspaRegs.SPCR2.bit.GRST=1; // Enable the sample rate generator
	clkg_delay_loop();           // Wait at least 2 CLKG cycles
	McbspaRegs.SPCR2.bit.XRST=1; // Release TX from Reset
	McbspaRegs.SPCR1.bit.RRST=1; // Release RX from Reset
    McbspaRegs.SPCR2.bit.FRST=1; // Frame Sync Generator reset

*/
  
//*************** RESET MCBSP
   McbspaRegs.SPCR2.bit.FRST=0; // Frame Sync generator reset
   McbspaRegs.SPCR2.bit.GRST=0; // Sample Rate generator Reset
   McbspaRegs.SPCR2.bit.XRST=0; // Transmitter reset
   McbspaRegs.SPCR1.bit.RRST=0; // Receiver reset

//*************** Initialize McBSP Registers
// McBSP register settings for Digital loop back
   McbspaRegs.SPCR2.all=0x0000; // XRST =0
//   McbspaRegs.SPCR1.all=0x8000; // RRST =0, DLB enabled
   McbspaRegs.SPCR1.all=0x0000; // RRST =0, DLB enabled
 McbspaRegs.PCR.all=0x0F08;           //(CLKXM=CLKRM=FSXM=FSRM= 1, FSXP = 1)

//   McbspaRegs.SPCR1.bit.DLB=0;  //chy
   McbspaRegs.SPCR1.bit.CLKSTP = 2;     // Together with CLKXP/CLKRP determines clocking scheme

   McbspaRegs.PCR.bit.CLKXP = 1;		 // CPOL = 0, CPHA = 0 rising edge no delay
   McbspaRegs.PCR.bit.CLKRP = 0;
    McbspaRegs.RCR2.bit.RDATDLY=01;      // FSX setup time 1 in master mode. 0 for slave mode (Receive)
    McbspaRegs.XCR2.bit.XDATDLY=01;      // FSX setup time 1 in master mode. 0 for slave mode (Transmit)

//   McbspaRegs.RCR2.all=0x0001;  // RDATDLY = 1
//   McbspaRegs.RCR1.all=0x0;
//   McbspaRegs.XCR2.all=0x0001;  // XDATDLY = 1
//   McbspaRegs.XCR1.all=0x0;
	McbspaRegs.RCR1.bit.RWDLEN1=5;     // 32-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=5;     // 32-bit word
 

//   McbspaRegs.SRGR2.all=0x3140;
//   McbspaRegs.SRGR1.all=0x010f;

    McbspaRegs.SRGR2.all= 0x2000; 	 	 // CLKSM=1, FPER = 1 CLKG periods
    McbspaRegs.SRGR1.all= 0x008F;	//8F     // Frame Width = 1 CLKG period, CLKGDV=16

//   McbspaRegs.MCR2.all=0x0;
//   McbspaRegs.MCR1.all=0x0;
//   McbspaRegs.PCR.all=0x0A00;

   McbspaRegs.MFFINT.bit.XINT = 1; // Enable Transmit Interrupts
   McbspaRegs.MFFINT.bit.RINT = 1; // Enable Receive Interrupts

//************* Enable Sample rate generator
   McbspaRegs.SPCR2.bit.GRST=1;
   delay_loop();                   // Wait at least 2 SRG clock cycles

//************ Enable TX/RX unit
   McbspaRegs.SPCR2.bit.XRST=1;
   McbspaRegs.SPCR1.bit.RRST=1;

//************ Frame Sync generator reset
   McbspaRegs.SPCR2.bit.FRST=1;
}


#if (DSP28_MCBSPB)
void InitMcbspb(void)
{

// McBSP-B register settings

    McbspbRegs.SPCR2.all=0x0000;		// Reset FS generator, sample rate generator & transmitter
	McbspbRegs.SPCR1.all=0x0000;		// Reset Receiver, Right justify word
	McbspbRegs.SPCR1.bit.DLB = 1;       // Enable loopback mode for test. Comment out for normal McBSP transfer mode.

	McbspbRegs.MFFINT.all=0x0;			// Disable all interrupts

    McbspbRegs.RCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Receive)
    McbspbRegs.RCR1.all=0x0;

    McbspbRegs.XCR2.all=0x0;			// Single-phase frame, 1 word/frame, No companding	(Transmit)
    McbspbRegs.XCR1.all=0x0;

    McbspbRegs.SRGR2.bit.CLKSM = 1;		// CLKSM=1 (If SCLKME=0, i/p clock to SRG is LSPCLK)
	McbspbRegs.SRGR2.bit.FPER = 31;		// FPER = 32 CLKG periods

    McbspbRegs.SRGR1.bit.FWID = 0;              // Frame Width = 1 CLKG period
    McbspbRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;	// CLKG frequency = LSPCLK/(CLKGDV+1)

   	McbspbRegs.PCR.bit.FSXM = 1;		// FSX generated internally, FSR derived from an external source
	McbspbRegs.PCR.bit.CLKXM = 1;		// CLKX generated internally, CLKR derived from an external source
    delay_loop();                // Wait at least 2 SRG clock cycles
    McbspbRegs.SPCR2.bit.GRST=1; // Enable the sample rate generator
	clkg_delay_loop();           // Wait at least 2 CLKG cycles
	McbspbRegs.SPCR2.bit.XRST=1; // Release TX from Reset
	McbspbRegs.SPCR1.bit.RRST=1; // Release RX from Reset
    McbspbRegs.SPCR2.bit.FRST=1; // Frame Sync Generator reset

}


#endif // end DSP28_MCBSPB

// McBSP-A Data Lengths
void InitMcbspa8bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=0;     // 8-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=0;     // 8-bit word
}

void InitMcbspa12bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=1;     // 12-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=1;     // 12-bit word
}

void InitMcbspa16bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=2;      // 16-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=2;      // 16-bit word
}

void InitMcbspa20bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=3;     // 20-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=3;     // 20-bit word
}

void InitMcbspa24bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=4;     // 24-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=4;     // 24-bit word
}

void InitMcbspa32bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1=5;     // 32-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=5;     // 32-bit word
}

// McBSP-B Data Lengths
#if (DSP28_MCBSPB)

void InitMcbspb8bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=0;     // 8-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=0;     // 8-bit word
}

void InitMcbspb12bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=1;     // 12-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=1;     // 12-bit word
}

void InitMcbspb16bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=2;      // 16-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=2;      // 16-bit word
}

void InitMcbspb20bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=3;     // 20-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=3;     // 20-bit word
}

void InitMcbspb24bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=4;     // 24-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=4;     // 24-bit word
}

void InitMcbspb32bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1=5;     // 32-bit word
    McbspbRegs.XCR1.bit.XWDLEN1=5;     // 32-bit word
}

#endif //end DSP28_MCBSPB



void InitMcbspGpio(void)
{
	InitMcbspaGpio();
	#if DSP28_MCBSPB
	  InitMcbspbGpio();
	#endif               // end DSP28_MCBSPB
}

void InitMcbspaGpio(void)
{
	EALLOW;

/* Configure McBSP-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be McBSP functional pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2;	// GPIO20 is MDXA pin
	GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 2;	// GPIO21 is MDRA pin
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;	// GPIO22 is MCLKXA pin
//    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;		// GPIO7 is MCLKRA pin (Comment as needed)
	//GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 1;	// GPIO58 is MCLKRA pin (Comment as needed)
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;	// GPIO23 is MFSXA pin
 //   GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;		// GPIO5 is MFSRA pin (Comment as needed)
	//GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 1;	// GPIO59 is MFSRA pin (Comment as needed)

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPAPUD.bit.GPIO20 = 0;     // Enable pull-up on GPIO20 (MDXA)
	GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;     // disable pull-up on GPIO21 (MDRA)
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 0;     // Enable pull-up on GPIO22 (MCLKXA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;      // Enable pull-up on GPIO7 (MCLKRA) (Comment as needed)
	//GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;   // Enable pull-up on GPIO58 (MCLKRA) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO23 = 0;     // Enable pull-up on GPIO23 (MFSXA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;      // Enable pull-up on GPIO5 (MFSRA) (Comment as needed)
	//GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;   // Enable pull-up on GPIO59 (MFSRA) (Comment as needed)

/* Set qualification for selected input pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.


    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;   // Asynch input GPIO21 (MDRA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;   // Asynch input GPIO22 (MCLKXA)
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;    // Asynch input GPIO7 (MCLKRA) (Comment as needed)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3; // Asynch input GPIO58(MCLKRA) (Comment as needed)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;   // Asynch input GPIO23 (MFSXA)
//    GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;    // Asynch input GPIO5 (MFSRA) (Comment as needed)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3; // Asynch input GPIO59 (MFSRA) (Comment as needed)

	EDIS;
}

#if DSP28_MCBSPB
void InitMcbspbGpio(void)
{
    EALLOW;
/* Configure McBSP-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be McBSP functional pins.
// Comment out other unwanted lines.

	//GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 3;	// GPIO12 is MDXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;	// GPIO24 is MDXB pin (Comment as needed)
	//GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;	// GPIO13 is MDRB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;	// GPIO25 is MDRB pin (Comment as needed)
	//GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 3;	// GPIO14 is MCLKXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;	// GPIO26 is MCLKXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 3;		// GPIO3 is MCLKRB pin (Comment as needed)
	//GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 1;	// GPIO60 is MCLKRB pin (Comment as needed)
	//GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 3;	// GPIO15 is MFSXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;	// GPIO27 is MFSXB pin (Comment as needed)
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 3;		// GPIO1 is MFSRB pin (Comment as needed)
	//GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 1;	// GPIO61 is MFSRB pin (Comment as needed)

/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;	    // Enable pull-up on GPIO24 (MDXB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;	// Enable pull-up on GPIO12 (MDXB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;	    // Enable pull-up on GPIO25 (MDRB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;	// Enable pull-up on GPIO13 (MDRB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;	    // Enable pull-up on GPIO26 (MCLKXB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;	// Enable pull-up on GPIO14 (MCLKXB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;		// Enable pull-up on GPIO3 (MCLKRB) (Comment as needed)
	//GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;	// Enable pull-up on GPIO60 (MCLKRB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO27 = 0;	    // Enable pull-up on GPIO27 (MFSXB) (Comment as needed)
	//GpioCtrlRegs.GPAPUD.bit.GPIO15 = 0;	// Enable pull-up on GPIO15 (MFSXB) (Comment as needed)
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;		// Enable pull-up on GPIO1 (MFSRB) (Comment as needed)
	//GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;	// Enable pull-up on GPIO61 (MFSRB) (Comment as needed)


/* Set qualification for selected input pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.
     GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;   // Asynch input GPIO25 (MDRB) (Comment as needed)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3; // Asynch input GPIO13 (MDRB) (Comment as needed)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;   // Asynch input GPIO26(MCLKXB) (Comment as needed)
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3; // Asynch input GPIO14 (MCLKXB) (Comment as needed)
    GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;    // Asynch input GPIO3 (MCLKRB) (Comment as needed)
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3; // Asynch input GPIO60 (MCLKRB) (Comment as needed)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;   // Asynch input GPIO27 (MFSXB) (Comment as needed)
	//GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3; // Asynch input GPIO15 (MFSXB) (Comment as needed)
	GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 3;    // Asynch input GPIO1 (MFSRB) (Comment as needed)
	//GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3; // Asynch input GPIO61 (MFSRB) (Comment as needed)


	EDIS;
}
#endif                // end DSP28_MCBSPB

void delay_loop(void)
{
    long      i;
    for (i = 0; i < MCBSP_INIT_DELAY; i++) {} //delay in McBsp init. must be at least 2 SRG cycles
}

void clkg_delay_loop(void)
{
    long      i;
    for (i = 0; i < MCBSP_CLKG_DELAY; i++) {} //delay in McBsp init. must be at least 2 SRG cycles
}
//===========================================================================
// No more.
//===========================================================================

void mcbsp_xmit(int a, int b)
{
    McbspaRegs.DXR2.all=b;
    McbspaRegs.DXR1.all=a;
}










