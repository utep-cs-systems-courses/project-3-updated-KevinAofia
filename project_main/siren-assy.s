	.arch msp430g2553

	.data			;static var in ram
siren_state:
	.word 0			;2bytes
	
	.data
frequency:			;static var in ram
	.word 700

	.text			;store in flash(immutable)
	.align 2		;keep even indexing 
jt:
	.word case0		;jt[0]
	.word case1		;jt[1]

	.global siren		;visible outside of function
siren:	
	mov &siren_state, r12	;move static var in r12
	add r12,r12		;allow for proper indexing for shorts
	mov jt(r12), r0		;move what is at jt index r12 into our program counter
case0:
	mov &frequency, r12	;move frequency into r12 parameter
	call #buzzer_set_period	;call method with parameter already in r12

	add #50, &frequency	;increment buzzer frequency by constant 50
	cmp #1600, &frequency 	;if frequency < 1600 (a-b, cmp &b, &a)
	jnc break		;jump on borrow/!carry
	
	jmp up_state
up_state:
	mov #1, &siren_state	;setup next switch case state
	jump break
case1:
	mov &frequency,r12	;move frequency into r12 parameter
	call #buzzer_set_period	;call method with parameter already in r12

	sub #50, &frequency	;decrement buzzer frequency by constant 50
	mov #700, r14		;move constant 700 value into r14 so we can compare
	cmp &frequency, r14	;if frequency > 700 (b-a, cmp &a, &b)
	jnc break		;jump on borrow/!carry
	
	jmp down_state
down_state:
	mov #0, &siren_state	;setup previous switch case state
	jump break
break:
	pop r0			;break

