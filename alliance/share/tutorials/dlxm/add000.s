;  ###----------------------------------------------------------------###
;  #									#
;  # file	: add000.s						#
;  # date	: Oct 21 1994						#
;  #									#
;  # origin	: this description has been developed by CAO-VLSI team	#
;  #		  at MASI laboratory, University Pierre et Marie Curie	#
;  #		  URA CNRS 818, Institut Blaise Pascal			#
;  #		  4 Place Jussieu 75252 Paris Cedex 05 - France		#
;  # E-mail      :      cao-vlsi@masi.ibp.fr                            #
;  #									#
;  # descr.	: functional test for dlx				#
;  #									#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   initialization prgram (RESET)				#
	;  ###--------------------------------------------------------###

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

other_causes 	j	other_causes

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

		end
