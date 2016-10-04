'SYSTEM MESSAGE HANDLING
'sys_mssg.inc  for Domination Tube
'Revision Date: 7 Aug 2010
'By Jim Robertson 

    DISABLE
'*************************************************************
'**************** SYSTEM MESSAGE HANDLER *********************
'*************************************************************
sys_msg_handler:
	message_ID = databyte1 & %01111111		'mask message_ID
	message = databyte

	GoSub shift_data_in				        'get third byte, leave it in "databyte"
	IF bad_data = 1 Then message_exit		'if any bit errors, ignore
	'if databyte != 232 THEN message_exit   'ignore - works with all MT systems
	
valid_message:
    LOW lampA
    LOW lampB
    IF message_ID = 3 THEN commands
    IF message_ID = 99 THEN time_set
    
message_exit:
    IF state != 3 THEN start      'continue game
    GOTO game_over
'------------------------------------------------------------
time_set: '99

    WRITE 50, message
    PAUSE 100
    
    GOTO initialize
    
'------------------------------------------------------------ 
'------------------------------------------------------------         	
commands:  '3
    IF message = 5 THEN mssg_new_game           'Reset for New Game

    GOTO message_exit

'------------------------------------------------------------ 
mssg_new_game:          'Reset For New Game

    GOTO initialize
'------------------------------------------------------------ 
'------------------------------------------------------------     


