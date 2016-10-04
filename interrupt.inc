'INTERRUPT SUBROUTINES
'asm_rout.inc  for Domination Tube
'Revision Date: 7 Aug 2010
'By Jim Robertson 


'*************************************************************
'******************** INTERRUPT ROUTINES *********************
'*************************************************************
    DISABLE
hit_int:
	bad_data = 0

'--------------------------------------------------------
'--------------------------------------------------------
check_header_min:						    'check for valid header pulse
	Call get_pulse
	IF length > 137 Then check_header_max	'min header pulse is 2.2 mS
	goto int_exit						    'pulse too short, disregard (no warnings)

check_header_max:
	IF length > 163 Then int_exit			'max header pulse is 2.6 mS, go to near miss
                                            'pulse too long, go to nearmiss
get_first_byte:						
	GoSub shift_data_in                     	'get first byte
	IF bad_data = 1 Then int_exit			'if any bit errors, go to nearmiss
	databyte1 = databyte				'move byte to "databyte1"
	IF databyte1.7 = 0 THEN get_shot_byte		'is it a SHOT or a MESSAGE?

get_message_byte:					'"message" byte is 8 bits	
	GoSub shift_data_in				'get byte, leave it in "databyte"
	IF bad_data = 1 Then int_exit			'if any bit errors, go to nearmiss

        OPTION_REG.6 = 0       'switch to Falling Edge 
        INTCON.5 = 0        'disable TMR0 interrupt
        INTCON.1 = 0
        goto sys_msg_handler

get_shot_byte:						'"shot" byte is 6 bits
	GoSub shift_data_in_short			'get byte, leave it in "databyte"
	IF bad_data = 1 Then int_exit			'if any bit errors, go to nearmiss	

'--------------------------------------------------------
'--------------------------------------------------------
data_valid:
	IF state = 3 THEN int_exit             ' only allow hits in NORMAL mode
	player 	= databyte1 & %01111111			' mask player ID bits
	team	= databyte  & %00110000			' mask team ID bits
	team 	= team >> 4				        ' shift right 4 places
  
weapon_hit:
		IF team = 0 THEN 
			state = 1
			HIGH lampA
			LOW lampB			
		ENDIF
		IF team = 1 THEN 
			state = 2
			HIGH lampB
			LOW lampA
		ENDIF
		
        PAUSE 1000

    	goto int_exit
        
'--------------------------------------------------------
int_exit:
    INTCON.4   = 1        'enable INT
    OPTION_REG.6  = 0        'falling edge
    INTCON.5   = 0        'disable TMR0 interrupt 
    INTCON.1   = 0        'clear INT FLAG

    RESUME
    
    ENABLE


    





