
;  ###----------------------------------------------------------------###
;  # file	: mftlo_00.s						#
;  # date	: Mar 26 1996						#
;  # descr.	: functional test for mips				#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   exceptions :						#
	;  #     - break                                		#
	;  ###--------------------------------------------------------###

value   	.equ	0x12344444
ill_codop	.equ	0x28
mask		.equ	0x3C

		.org	0x80000080
		.start	it_handler

	;  ###--------------------------------------------------------###
	;  #   interrupt and exception handler				#
	;  ###--------------------------------------------------------###

it_handler:

		sw	r29, 0 (r30)
		sw	r28, 4 (r30)

		;addiu	r28, r0, mask
		;mfc0	r29, cause
		;nop
		;and	r29, r29, r28
		;addiu	r28, r0, ill_codop
		;bne	r28, r29, not_good

		loadi	r28, value
		mtlo	r28 
		nop
                mflo	r29
		nop
		bne	r28, r29, not_good

		lw	r29, 0 (r30)
		lw	r28, 4 (r30)

	;  ###--------------------------------------------------------###
	;  #   restore return address before returning (from EXCEPTION)	#
	;  # to the user program (address must be saved by the user	#
	;  # prior to the EXCEPTION in epc)				#
	;  ###--------------------------------------------------------###

restore_return_adr:
		mfc0    r31, epc
		nop
                addiu    r31, r31, 8
                jr      r31
		rfe

not_good:
		mfc0    r31, epc
		nop
                addiu    r31, r31, 4
                jr      r31
		rfe

		.end

