;  ###----------------------------------------------------------------###
;  # file	: exc024.s						#
;  # date	: Mar 26 1996						#
;  # descr.	: functional test for mips				#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   exceptions :						#
	;  #     - instruction bus error  	               		#
	;  ###--------------------------------------------------------###

xcode_mask	.equ	0x3C			; exc mask
xcode_ibe	.equ    0x18    		; instruction bus error

		.org	0x80000080
		.start	it_handler

	;  ###--------------------------------------------------------###
	;  #   interrupt and exception handler				#
	;  ###--------------------------------------------------------###

it_handler:

		sw	r29, 0 (r30)
		sw	r28, 4 (r30)

		mfc0	r29,cause 
		nop
		addiu	r28,r0,xcode_mask
		and	r29, r29, r28		; instruction bus error ?
                addiu   r27,r0,xcode_ibe
		bne	r29,r27, other_causes
		nop

		lw	r29, 0 (r30)
		lw	r28, 4 (r30)

restore_return_adr:
		j	return
		nop

other_causes:	j	other_causes
		nop

	;  ###--------------------------------------------------------###
	;  #   return to the interrupted program			#
	;  ###--------------------------------------------------------###
                .org    0x800000d0
return:
		rfe
                jr      r31

		.end
