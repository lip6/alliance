;  ###----------------------------------------------------------------###
;  #									#
;  # file	: exc008.s						#
;  # date	: Apr  1 1993						#
;  # descr.	: functional test for dlx				#
;  #									#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   exceptions :						#
	;  #     - illegal data address (segment)			#
	;  ###--------------------------------------------------------###

data_adr_viol	equ	X"0400"			; data address violation
reset_mask	equ	X"0008"			; reset

system_stack	equ	X"80000000"		; system stack address

user_status	equ	X"0003"
user_prog	equ	X"7fffff00"

		org	X"fffffff0"
		start	it_handler

it_handler :
		j	handler_body
		nop
return		rfe
		nop

		org	X"ffffff00"

handler_body:
		movs2i	r29, sr			; read Status to test it
		andi	r29, r29, reset_mask	; reset ?
		bnez	r29, hardware_reset
		nop

		movs2i	r29, sr
		andi	r29, r29, data_adr_viol	; data address violation ?
		bnez	r29, restore_return_adr
		nop

other_causes	j	other_causes
		nop

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

		loadi	r29, user_prog		; prepare return address
		movi2s	iar, r29		; copy user program address
						;+into IAR
		j	return
		nop

	;  ###--------------------------------------------------------###
	;  #   restore return address before returning (from EXCEPTION)	#
	;  # to the user program (address must be saved by the user	#
	;  # prior to the EXCEPTION in r31)				#
	;  ###--------------------------------------------------------###

restore_return_adr:
		movi2s	iar, r31
		j	return
		nop

		end
