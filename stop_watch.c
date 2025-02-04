/*
 * main.c
 *
 *  Created on: Sep 11, 2024
 *  Author: Mariam hassan
 *  description: stop-watch
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/* to count complete one second due to calculation*/
#define count_register_for_one_second 16000
/* initiate three variable to count secs,mins,hours*/
unsigned char seconds=0,minutes=0,hours=0;
/* variable modes -flagmode for checking button  and mode to control which mode is active */
unsigned char mode=0,flagmode=0;
/*flag_one_sec AFTER ONE IT INCREMENT INSIDE ISR*/
unsigned char flag_1_sec=0;


/*============================================================================================*/
/* FUNCTION ENABLE TIMER1*/
void timer1_ctc(void)
{
	/* SET FOC AS NON PWM MODE AND THE REST ARE ZEROS ACCORDING TO MODE SETTINGS*/
	TCCR1A=(1<<FOC1A)|(1<<FOC1B);
	/*SELECT COMPARE MATCH MODE AND PRESCALAR 1024*/
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS12);
	/* INTIATE TIMER REGISTER BY ZERO*/
	TCNT1=0;
	/* ACCORDING TO CALCULATION 1 SEC OCCUR AFTER 16000 TICK*/
	OCR1A=count_register_for_one_second;
	/* ENABLE TIMER1 INTERRUPT WITH CTC*/
	TIMSK|=(1<<OCIE1A);
	/* enable global interrupt bit (I-bit)*/
	SREG|=(1<<7);

}

/*============================================================================================*/
/*enable interrupt zero*/
void INT0_FALLING(void)
{
	/*MAKE PD2 INPUT*/
	DDRD&=~(1<<2);
	/* ENABLE INTERNAL PULL UP*/
	PORTD|=(1<<2);
	/* SELECT FALLING EDGE*/
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);
	/* ENABLE MODULE INTERRUPT*/
	GICR|=(1<<INT0);
	/* ENABLE GLOBAL INTERRUPT*/
	SREG|=(1<<7);
}

/*============================================================================================*/
/* ENABLE INTERRUPT ONE*/
void INT1_raising(void)
{
	/*MAKE PD3 INPUT*/
	DDRD&=~(1<<3);
	/* SELECT raising EDGE*/
	MCUCR|=(1<<ISC11)|(1<<ISC10);
	/* ENABLE MODULE INTERRUPT*/
	GICR|=(1<<INT1);
	/* ENABLE GLOBAL INTERRUPT*/
	SREG|=(1<<7);
}

/*============================================================================================*/
void INT2_FALLING(void)
{
	/*MAKE PB2 INPUT*/
	DDRB&=~(1<<2);
	/* ENABLE INTERNAL PULL UP*/
	PORTB|=(1<<2);
	/* FALLING EDGE*/
	MCUCSR &= ~(1<<ISC2);
	/* ENABLE INTERRUPT MODULE*/
	GICR|=(1<<INT2);
	/* ENABLE GLOBAL INTERRUPT*/
	SREG|=(1<<7);

}

/*============================================================================================*/
ISR(INT2_vect)
{
	/* RE-ENABLE THE CLOCK PRESCALER 1024*/
	TCCR1B|=(1<<CS10)|(1<<CS12);
	TCCR1B&=~(1<<CS11);
}

/*============================================================================================*/
ISR(INT1_vect)
{  /* DISENABLE THE CLOCK FOR RESUMING*/
	TCCR1B&=~(1<<CS10)&~(1<<CS11)&~(1<<CS12);
}

/*============================================================================================*/
ISR(INT0_vect)
{
	/* THE CODE WILL RESET BY PUTTING ALL SEC,MIN,HOUR, PUTTING ZERO*/
	seconds=0;
	minutes=0;
	hours=0;
}
/*============================================================================================*/

ISR(TIMER1_COMPA_vect)
{
	/*put one to this flag */
   flag_1_sec=1;
}
/*============================================================================================*/


int main ()
{
	/* output of four bit decoder */
     DDRC|=0X0F;
     /* intaite them by zeros*/
     PORTC&=0XF0;
     /* output 6 -7segment*/
     DDRA|=0X3F;
     /* intiate all on*/
     PORTA|=0X3F;
     DDRB=0X00;
     PORTB=0XFF;

     /*output pins led detect mode */
     DDRD|=(1<<4)|(1<<5);
     /*BUZZER PIN*/
     DDRD|=(1<<0);
     /*enable them internal pull up*/
     PORTD|=(1<<4);
     /*make PORTB INPUT - ENABLE THEM INTERNAL OULL UP*/
     DDRB=0X00;
     PORTB=0XFF;

     /* flag to check button decrement seconds*/
      unsigned char flag_seconds_decrement=0;
     /* flag to check button increment seconds*/
      unsigned char flag_seconds_increment=0;
      /* flag to check button increment minutes*/
      unsigned char flag_minutes_increment=0;
      /* flag to check button decrement minutes*/
      unsigned char flag_minutes_decrement=0;
      /* flag to check button decrement seconds*/
      unsigned char flag_hours_decrement=0;
      /* flag to check button increment seconds*/
      unsigned char flag_hours_increment=0;

     /* timer1 & INT0/1/2 enable function*/
     timer1_ctc();
     INT0_FALLING();
     INT1_raising();
     INT2_FALLING();
	while (1)
	{
		/*===================================================================*/
          /* adjustment of seconds increment*/
		    if (!(PINB & (1 << 6)))
			    {
				 /* de-bouncing check*/
			        _delay_ms(30);
			        if (!(PINB & (1 << 6)))
			        {
			           if (flag_seconds_increment==0)
			           {
			            if (seconds < 59)
			            {
			            	seconds++;
			            }else if(seconds==59)
			            {
			                /*nothing*/
			            }
			            flag_seconds_increment=1;
			           }
			      }
			    }else
		           {
		        	   flag_seconds_increment=0;
		           }

		    /*===================================================================*/
		    /* adjustment of seconds decrement*/
		    if (!(PINB & (1 << 5)))
		    	    {
		    		 /* de-bouncing check*/
		    	        _delay_ms(30);
		    	        if (!(PINB & (1 << 5)))
		    	        {
		                   if(flag_seconds_decrement==0)
		                   {
		    	            if (seconds > 0)
		    	            {
		    	            	 seconds--;
		    	            }else
		    	            {
		    	                seconds =0;
		    	            }
		    	            flag_seconds_decrement=1;
		    	            }
		    	        }
		    }else
	        {
	        	flag_seconds_decrement=0;
	        }


		    /*===================================================================*/
		    /* adjustment of minutes increment*/
		    if (!(PINB & (1 << 4)))
		        {
		    	 /* de-bouncing check*/
		            _delay_ms(30);
		            if (!(PINB & (1 << 4)))
		            {
		            	if(flag_minutes_increment==0)
		            	{
		                   if (minutes < 59)
		                   {
		                	   minutes++;
		                   }else if (minutes==59)
		                   {
		                	   /*nothing*/
		                   }
		                   flag_minutes_increment=1;
		            	}
		            }
		        }else
		        {
		        	flag_minutes_increment=0;
		        }
		    /*===================================================================*/
		    /* adjustment of minutes decrement*/
		    if (!(PINB & (1 << 3)))
		        {
		    	/* de-bouncing check*/
		            _delay_ms(30);
		            if (!(PINB & (1 << 3)))
		            {
		            	if (flag_minutes_decrement==0)
		            	{
		            		 if (minutes > 0)
		            		 {
		            		     minutes--;
		            		 }else
		            		 {
		            		  minutes = 59;
		            	     }
		            		 flag_minutes_decrement=1;
		               }
		            }
		        }else
		        {
		        	flag_minutes_decrement=0;
		        }


		    /*===================================================================*/
		    /* adjustment of hours decrement*/


		    if (!(PINB & (1 << 0)))
		       {
		    	   /* de-bouncing check*/
		           _delay_ms(30);
		           if (!(PINB & (1 << 0)))
		           {
		        	   if(flag_hours_decrement==0)
		        	   {
		               if (hours > 0)
		                   hours--;
		               else
		                   hours = 23;
		        	   }
		        	   flag_hours_decrement=1;
		           }
		       }else
		       {
		    	   flag_hours_decrement=0;
		       }

		    /*===================================================================*/
		   /* adjustment of hours increment*/
		    if (!(PINB & (1 << 1)))
		       {
		    	/* de-bouncing check*/
		           _delay_ms(30);
		           if (!(PINB & (1 << 1)))
		           {
		        	   if(flag_hours_increment==0)
		        	   {
		               if (hours < 23)
		                   hours++;
		               else if (hours==24)
		               {
		            	   /*nothing*/
		               }
		        	   }
		        	   flag_hours_increment=1;
		           }
		       }else
		       {
		    	   flag_hours_increment=0;
		       }


		    /*===================================================================*/

		    if(!(PINB & (1 << 7)))  // Check if the button is pressed
		    	    {
		    	        _delay_ms(30);  /*de-bouncing delay*/
		    	        if (!(PINB & (1 << 7)))  // Confirm button is still pressed
		    	        {
		    	        	if (flagmode==0)
		    	        	{
		    	        	/*change from 1 to zero & zero to one*/
		    	        	mode^=1;
		    	        	flagmode=1;
		    	            }
		    	        }

		    	    }else
		    	    {
		    	    	flagmode=0;
		    	    }

if (flag_1_sec==1)
{
			 if (mode==0)
			 {

			            /*enable red led*/
			            PORTD|=(1<<4);
				        /*dis-enable yellow led*/
					    PORTD&=~(1<<5);
		                /* dis-enble buzzer*/
					    PORTD&=~(1<<0);

						/* TIMER WILL INTERRUPT AFTER SEC ACCORDING TO CALCULATON*/
										seconds++;
										/* WHEN SECONDS REACH 60 INCREMENT MINUTES by one and put seconds zero*/
										if(seconds==60)
										{
											 minutes++;
											 seconds=0;
										}
									    /* when minutes reach 60 increment hours by 1 and put minutes zero*/
										 if (minutes==60)
										 {
										      hours++;
											  minutes=0;
										 }
									   /* when hours reach 24 increment hours put hours zero*/
									     if (hours==24)
									     {
										      hours=0;
										 }

			 }else if (mode==1)
			 {
				 PORTD|=(1<<5);
				 	        	PORTD&=~(1<<4);
				 	            // Check if all time units are zero stop the count down if true
				 	            if ((hours == 0) && (minutes == 0) && (seconds == 0))
				 	            {
				 	                //TIMSK &= ~(1 << OCIE1A);  // Disable Timer1 Compare A Match interrupt & enable the buzzer

				 	                PORTD|=(1<<0);
				 	            }

				 	            // Decrement seconds first
				 	            if (seconds > 0)
				 	            {
				 	                seconds--;
				 	            }
				 	            else
				 	            {
				 	                /* If seconds reach 0, decrement minutes and reset seconds to 59*/
				 	                if (minutes > 0)
				 	                {
				 	                    minutes--;
				 	                    seconds = 59;
				 	                }
				 	                else
				 	                {
				 	                    /*If minutes reach 0, decrement hours and reset minutes and seconds*/
				 	                    if (hours > 0)
				 	                    {
				 	                        hours--;
				 	                        minutes = 59;
				 	                        seconds = 59;
				 	                    }
				 	                }
				 	            }

			 }
			                                flag_1_sec=0;
			 }
		    /*===================================================================*/


		        /* DISPLAY UNITS OF SECONDS & ENABLE SEGMENT OF IT*/
		            PORTA = (PORTA & 0xC0) | 0x20;
		    		PORTC = (PORTC & 0xF0) | ((seconds%10) );

		    		_delay_ms(2);
		    		/* DISPLAY DIGITS OF SECONDS & ENABLE SEGMENT OF IT*/
		    		PORTA = (PORTA & 0xC0) | 0x10;
		    		PORTC = (PORTC & 0xF0) | ((seconds/10) );

		    		_delay_ms(2);
		    		/* DISPLAY UNITS OF MINUTES & ENABLE SEGMENT OF IT*/
		    		PORTA = (PORTA & 0xC0) | 0x08;
		    		PORTC = (PORTC & 0xF0) | ((minutes%10) );

		    		_delay_ms(2);
		    		/* DISPLAY DIGITS OF MINUTES  & ENABLE SEGMENT OF IT */
		    		PORTA = (PORTA & 0xC0) | 0x04;
		    	    PORTC = (PORTC & 0xF0) | ((minutes/10) );

		    	    _delay_ms(2);

		    	    /* DISPLAY UNITS OF HOURS & ENABLE SEGMENT OF IT*/
		    	    PORTA = (PORTA & 0xC0) | 0x02;
		    	    PORTC = (PORTC & 0xF0) | ((hours%10) );

		    	    _delay_ms(2);
		    	    /* DISPLAY DIGITS OF HOURS & ENABLE SEGMENT OF IT */
		    	    PORTA = (PORTA & 0xC0) | 0x01;
		    	    PORTC = (PORTC & 0xF0) | ((hours/10) );

		    	    _delay_ms(2);
                  /* DELAY 10 ms for visibility (time slices)*/


	}
}
