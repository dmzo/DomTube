'ASSEMBLY SUBROUTINES
'asm_rout.inc  for Domination Tube
'Revision Date: 7 Aug 2010
'By Jim Robertson 


'*************************************************************
'***************** ASSEMBLY CODE ROUTINES ********************
'*************************************************************
    disable
	Asm
;---------------------------------------------		
_get_pulse
    movlw   0			; max pulse = 255 (4.08 mS)
    movwf   TMR0		; set Timer 0
    bcf     INTCON,2		; clear Timer 0 OVF flag
    
header_loop    
    btfsc   PORTA,2		; check for end of header
     Goto   pulse_ok		; end of header found

    btfss   INTCON,2		; Timer 0 overflow?
     Goto header_loop		; if no, keep going

too_long                    	; Timer 0 overflow    
	movlw	255
	movwf	_length		 
	Return	

pulse_ok
    movf    TMR0,W
    movwf   _length
    Return

;---------------------------------------------
;---------------------------------------------	
_get_bit
measure_space
    movlw   195             ; max space = 60 (.96 mS)
    movwf   TMR0           ; set Timer 0
    bcf     INTCON,2        ; clear Timer 0 OVF flag
	
wait_edge2					; wait for edge
	btfss	PORTA,2			; end of space?
	 GoTo	measure_bit		; yes, go measure bit length
	 
    btfss   INTCON,2        ; Timer 0 overflow?        
	 GoTo	wait_edge2		; no, keep going
	GoTo	bad_bit			; yes, space was too long

measure_bit
	movlw	105				; max bit = 150 (2.4 mS)
    	movwf   TMR0           ; set Timer 0
    	bcf     INTCON,2        ; clear Timer 0 OVF flag
    
bit_loop
	btfsc	PORTA,2			; check for end of bit
	 GoTo pulse_ok_2		; end of bit found
	 
    	btfss   INTCON,2        ; Timer 0 overflow?        
	 GoTo	bit_loop		; no, keep going
	;fall through			; yes, space was too long
	
bad_bit
	bsf	_bad_data
	Return

pulse_ok_2
    movf    TMR0,W
    movwf   _length
    Return




;-------------------------------------------------	
;----------Vulnerable Pause Routine---------------
;-------------------------------------------------
_vpause					;call with desired ms delay in "vdelay"	
	bcf _hit_detect, 0
	
vpauseloop
	movlw 200				;(1)
	movwf _vcount			;(1)
	
vpauseloop2					; 1ms loop (10 x 200) + 3
	Call rtrn				;(4) delay 4 cycles
	nop						;(1)
	btfss PORTA,2			;(2) (1)  is IR input low?
	 GoTo vpausehit			;    (2)  yes, go check for data
	decfsz	_vcount,F		;(1)      
	 GoTo vpauseloop2		;(2)
	 
	decfsz  _vdelay,F		;(1) subtract 1 from vdelay
	 GoTo vpauseloop		;(2) repeat 1ms delay
 	Return 					; vdelay = 0 so return
 		
vpausehit					;if hit during pause
	bsf _hit_detect, 0
rtrn
	Return
	
;----------------------------------------------------
_intpause					;call with vdelay = 2   (for  .1mS)
                            ;call with vdelay = 20  (for  1mS delay)
                            ;call with vdelay = 200 (for 10mS delay)	
	
intpauseloop
	movlw 19				    ;(1)     (19)
	movwf _vcount			;(1)
	
intpauseloop2				;   50uS loop (19 x 5) + 5 = 100
	btfsc INTCON,1          ;(2) (1)  RB0 INTERRUPT FLAG?
	 Return                 ;    (2)  yes, quit delay

	decfsz	_vcount,F		;(1)      
	 GoTo intpauseloop2		;(2)
	 
	decfsz  _vdelay,F		;(1) subtract 1 from vdelay
	 GoTo intpauseloop		;(2) repeat 1ms delay
 	Return 					; vdelay = 0 so return


	EndAsm



