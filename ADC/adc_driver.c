
#include "../TM4C123GH6PM.h"
#include "adc_driver.h"

ADC0_Type* ADC0_CFGS= (ADC0_Type *)  ADC0_BASE;
#define SYSCTL_RCGCADC_R        (*((volatile unsigned long *)0x400FE638))
	#define NVIC_EN0_R              (*((volatile unsigned long *)0xE000E100))
#define NVIC_EN1_R              (*((volatile unsigned long *)0xE000E104))

void ADCInit(sample_sequencer seq,emux_selector emux_sel, ssmux_selector seqmux_sel  ){
	//ADC 0, Sequencer 3, PEl(AN2)
  //Enable the ADC clock using the RCGCADC register 
   SYSCTL_RCGCADC_R |= (UP<<0);//Enable ADC module 0
 
	//Enable the clock to the appropriate GPIO modules via the RCGCGPIO register
  //SYSCTL_RCGCGPIO_R |= (1 << 4) | (1 << 5); //Enable port E and port F
 
	//Enable alternate function for PEl
  //adcData=3;
  //GPIO_PORTE_AFSEL_R |= (1 << 1);
	
  //Set direction to input PE1
	//GPIO_PORTE_DIR_R &= ~(1 << 1);
	
	//Disable digital function for PEl
 // GPIO_PORTE_DEN_R &= ~(1 << 1) ;

  //Enable analog functionality for PE1
  //GPIO_PORTE_AMSEL_R |= (1 << 1);
	
	//Sample Sequencer 3 is disabled
	//ADC0_ACTSS_R &= ~(1 << 3);
	ADC0_CFGS->ACTSS |= seq;
	//Sample Sequencer 3 is always running 
	//ADC0_EMUX_R = (0xf<<12);
	ADC0_CFGS->EMUX |= emux_sel;
	//Sample Sequencer 3 reads from AIN2
	//ADC0_SSMUX3_R = 2;
	ADC0_CFGS->SSMUX3 |= seqmux_sel;
	//End bit (1) must be set and bit 2 to enable interrupt for Sample Sequencer 3
	//ADC0_SSCTL3_R |= ( 1 << 1) | ( 1 << 2 ); 
	ADC0_CFGS->SSCTL3 |= ( UP<<1) | (UP <<2);
	//Enable interrupt mask for Sample Sequencer 3
	//ADC0_IM_R = (1 << 3);
	ADC0_CFGS->IM |=( UP << seq);
	//Clear interrupt status for Sample Sequencer 3
	//ADC0_RIS_R |= (1 << 3);
		ADC0_CFGS->RIS |=( UP << seq);
	//Sample Sequencer 3 is enabled
//	ADC0_ACTSS_R |= (1 << 3);
	ADC0_CFGS->ACTSS |=( UP << seq);
	//ADC0_PSSI_R |= (1 <<3 );
	
	//Enable IRQ for ADC0SS3
   if (seq == 0 || seq== 1){
    NVIC_EN0_R|=(1<< (30+seq));
	 }else{
		 NVIC_EN1_R|=(1<<(seq-2));
	 }
}


//void ADC0Seq3_Handler(void){
//		if((ADC0_CFGS->RIS & (1 << 3)) == (1 << 3))
//			{
//				ADC0_CFGS->ISC |= (1 << 3); //clear the interrupt bit
//				adcData = ADC0_SSFIFO3_R;
//				adcData=adcData/sqrt(adcData);
//				
//		
//		  }
//	
//}