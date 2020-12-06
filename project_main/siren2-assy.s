	.arch msp430g2553
	.align 2

	.data			;static var in ram
siren2_state:
	.word -2		;2byte
	
	.data
frequency2:			;static var in ram
	.word 700
	
	.global siren2		;visible outside of function
				
siren2:
				;setting up in reverse logic to show the use of JGE and JL
	
	mov #-1, r13 		;move a -1 into r13 for comparison
	
	cmp r13, &siren2_state	;if -2 < -1, we want to move to case-1, B-A
	jl caseN1		;(N!=V), jump to case-1

	cmp &siren2_state, r13	;if -1 > -2, we want to move to -2, A-B
	jge caseN2		;(N==V), jump to case-2

	mov.b #-2, &siren2_state;if we get here, something went wrong and reset siren_state
	jmp siren2
	
caseN1:
	mov &frequency2, r12	;move frequency into r12 parameter
	call #buzzer_set_period	;call method with parameter already in r12

	add #50, &frequency2	;increment buzzer frequency by constant 50
	cmp #1600, &frequency2 	;if frequency < 1600 (a-b, cmp &b, &a)
	jnc break2		;jump on borrow
	
	jmp up2_state
up2_state:
	add #1, &siren2_state	;add 1 to siren state to get siren_state to -1
	jmp break2

caseN2:
	mov &frequency2,r12	;move frequency into r12 parameter
	call #buzzer_set_period	;call method with parameter already in r12

	sub #50, &frequency2	;decrement buzzer frequency by constant 50
	mov #700, r15		;move constant 700 value into r14 so we can compare
	cmp &frequency2, r15	;if frequency > 700 (b-a, cmp &a, &b)
	jnc break2		;jump on borrow
	jmp down2_state
down2_state:
	sub #1, &siren2_state 	;sub 1 to siren state to get siren_state to -2
	jmp break2
	
break2:
	pop r0			;break
