;  ###----------------------------------------------------------------###
;  # file	: it005.s						#
;  # date	: Mar 26 1996						#
;  # descr.	: functional test for mips				#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   hardware interrupt					#
	;  ###--------------------------------------------------------###

hardware_it0	.equ	0x00000400
hardware_it1	.equ	0x00000800
hardware_it2	.equ	0x00001000
hardware_it3	.equ	0x00002000
hardware_it4	.equ	0x00004000
hardware_it5	.equ	0x00008000

timer_0		.equ	0x40000100
timer_1		.equ	0x40000104
timer_2		.equ	0x40000108
timer_3		.equ	0x4000010C
timer_4		.equ	0x40000110
timer_5		.equ	0x40000114
timer_rst	.equ	0x40000118
timer_sts	.equ	0x4000011C

		.org	0x80000080
		.start	handler_body

	;  ###--------------------------------------------------------###
	;  #   interrupt and exception handler				#
	;  ###--------------------------------------------------------###


handler_body:
		sw	r29, 0 (r30)
		sw	r28, 4 (r30)

		loadi	r28, hardware_it5
		mfc0	r29, cause
		nop
		and	r29, r29, r28		; hardware interrupt 5 ?
		bne	r29, r28,other_causes
		nop
		; ###---------------------------------------------###
		; # On releve la ligne it                           #
		; ###---------------------------------------------###
		loadi	r28, timer_sts
		sw	r0, 0(r28)

		lw	r29, 0 (r30)
		lw	r28, 4 (r30)

	;  ###--------------------------------------------------------###
	;  #   restore return address before returning (from INTERRUPT)	#
	;  # to the user program (address must be saved by the user	#
	;  # prior to the EXCEPTION in r31)				#
	;  ###--------------------------------------------------------###

return:
		rfe				; return from exception
                jr       r31

other_causes:	j	other_causes
		nop

	;  ###--------------------------------------------------------###
	;  #   return to the interrupted program			#
	;  ###--------------------------------------------------------###


		.end

