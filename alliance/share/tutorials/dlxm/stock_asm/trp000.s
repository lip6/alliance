;  ###----------------------------------------------------------------###
;  #									#
;  # file	: trp000.s						#
;  # date	: Oct 21 1994						#
;  # descr.	: functional test for dlx				#
;  #									#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   initialization prgram (RESET)				#
	;  ###--------------------------------------------------------###

not_trap_mask	equ	X"ff08"

reset_mask	equ	X"0008"
system_stack	equ	X"80000000"

user_status	equ	X"0003"
user_prog	equ	X"7fffff00"

		org	X"fffffff0"
		start	it_handler

it_handler:
		j	handler_body
		nop
return:
		rfe				; return from exception
		nop

		org	X"ffffff00"
handler_body:
		movs2i	r29, sr			; read Status to test it
		andi	r29, r29, reset_mask	; reset ?
		bnez	r29, hardware_reset
		nop

		movs2i	r29, sr			; read Status to test it
		andi	r29, r29, not_trap_mask ; any other cause ?
		beqz	r29, software_interrupt
		nop

other_causes	j	other_causes

	;  ###--------------------------------------------------------###
	;  #   initialization prgram (hardware RESET)			#
	;  ###--------------------------------------------------------###

hardware_reset:
		loadi	r30, system_stack	; init system stack pointer

		movs2i	r29, sr			; load Status to initialize
		ori	r28, r0 , X"ffff"	; clear high order bits of
		and	r29, r29, r28		;+the status register
		lhi     r28, user_status	; init status register
		or	r29, r29, r28		;
		movi2s	sr , r29

		loadi	r29, user_prog
		movi2s	iar, r29
		j	return
		nop

	;  ###--------------------------------------------------------###
	;  #   software interrupt (Trap instruction)			#
	;  #								#
	;  #   restore return address before returning (from EXCEPTION)	#
	;  # to the user program (address must be saved by the user	#
	;  # prior to the EXCEPTION in r31)				#
	;  ###--------------------------------------------------------###

software_interrupt:
		movs2i	r1 , tvr
		slli	r1 , r1 , 4
		srli	r1 , r1 , 6
		movi2s	iar, r31
		j	return
		nop

		end
