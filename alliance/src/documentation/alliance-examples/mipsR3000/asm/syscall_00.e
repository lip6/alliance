
;  ###----------------------------------------------------------------###
;  # file	: syscall_00.s						#
;  # date	: Mar 26 1996						#
;  # descr.	: functional test for mips				#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   exceptions :						#
	;  #     - syscall                                		#
	;  ###--------------------------------------------------------###

val1    	.equ	0x3C    	

val             .equ    0x20

		.org	0x80000080
		.start	it_handler

	;  ###--------------------------------------------------------###
	;  #   interrupt and exception handler				#
	;  ###--------------------------------------------------------###

it_handler:

		sw	r29, 0 (r30)
		sw	r28, 4 (r30)

		addi	r28, r0, val1
		mfc0	r29, cause 
		nop
		and	r29, r29, r28
                addi    r27, r0, val
		bne	r29, r27, not_good
		nop

		lw	r29, 0 (r30)
		lw	r28, 4 (r30)

	;  ###--------------------------------------------------------###
	;  #   restore return address before returning (from EXCEPTION)	#
	;  # to the user program (address must be saved by the user	#
	;  # prior to the EXCEPTION in epc)				#
	;  ###--------------------------------------------------------###

restore_return_adr:
		mfc0    r31,epc
		nop
                addi    r31,r31,8
                jr      r31
		rfe

not_good:
		mfc0    r31,epc
		nop
                addi    r31,r31,4
                jr      r31
		rfe

		.end

