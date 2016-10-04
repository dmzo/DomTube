'SUBROUTINES
'sub_rout.inc  for Domination Tube
'Revision Date: 7 Aug 2010
'By Jim Robertson 


'*************************************************************
'*************************************************************
'*************************************************************
    DISABLE

'-----------------------------------------
'-----------------------------------------
shift_data_in:							'get 8 bits of data and place in "databyte" variable
	For bit_counter = 1 TO 8
		databyte = databyte * 2				'rotate byte left
		Call get_bit					'measure bit
		IF bad_data = 0 Then pass1
		Return
pass1:     							'add 105 to bit values				
		IF length < 193 AND length > 130 Then pass2	'bit length = 0.4 mS to 1.4 mS
		bad_data = 1
		Return
pass2:
		IF length > 161 Then shiftone			' is bit length > 0.9 mS, if yes it's a "1" 
shiftzero:							' if no, it's a "0"
		databyte.0 = 0
		GoTo bypass
shiftone:
		databyte.0 = 1
bypass:		
	Next bit_counter
	
	Return
	
'-----------------------------------------
'-----------------------------------------
shift_data_in_short:						'get 6 bits of data and place in "databyte" variable
	For bit_counter = 1 TO 6
		databyte = databyte * 2				'rotate byte left
		Call get_bit					'measure bit
		IF bad_data = 0 Then pass1_short
		Return
pass1_short:     							'add 105 to bit values				
		IF length < 193 AND length > 130 Then pass2_short	'bit length = 0.4 mS to 1.4 mS
		bad_data = 1
		Return
pass2_short:
		IF length > 161 Then shiftone_short			' is bit length > 0.9 mS, if yes it's a "1" 
shiftzero_short:							' if no, it's a "0"
		databyte.0 = 0
		GoTo bypass_short
shiftone_short:
		databyte.0 = 1
bypass_short:		
	Next bit_counter

'read_zone:
'		Call get_bit					'measure bit
'		IF bad_data = 0 Then test_zone
'		Return
'test_zone:
'		IF length < 193 AND length > 130 Then pass_zone		'bit length = 0.4 mS to 1.4 mS
'		bad_data = 1
'		Return
'pass_zone:
'	zone_rx = 0
'	IF length > 161 Then 				' is bit length > 0.9 mS, if yes it's a "1" 
'		zone_rx = 1
'	ENDIF						' if no, it's a "0"
				
	RETURN
    
'******************************************************************
'******************************************************************	
	ENABLE

'-----------------------------------------
'-----------------------------------------	
intdelay:                       '10 = 1mS; 100 = 10mS
    vdelay = 2                  '.1ms resolution
    CALL intpause
    delay_time = delay_time - 1
    If delay_time > 0 then intdelay
    
    RETURN

    
    
    
    
    
