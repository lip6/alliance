
;  ###----------------------------------------------------------------###
;  # file	: exc007.s						#
;  # date	: Mar 26 1996						#
;  # descr.	: functional test for mips				#
;  ###----------------------------------------------------------------###

	;  ###--------------------------------------------------------###
	;  #   exceptions :						#
	;  #     - adel                                  		#
	;  ###--------------------------------------------------------###

val1    	.equ	0x3C    		

user_prog	.equ	0x00400000
val             .equ    0x10                    ; adel

		.org	0x80000080
		.start	it_handler

	;  ###--------------------------------------------------------###
	;  #   interrupt and exception handler				#
	;  ###--------------------------------------------------------###

it_handler:

		sw	r29, 0 (r30)
		sw	r28, 4 (r30)

		addiu	r28, r0 ,val1
                addiu   r27,r0,val
		mfc0	r29,cause 
		nop
		and	r29, r29, r28		; data address violation ?
		bne	r27,r29, other_causes
		nop

		lw	r29, 0 (r30)
		lw	r28, 4 (r30)

          	j	return
	;  ###--------------------------------------------------------###
	;  #   restore return address before returning (from EXCEPTION)	#
	;  # to the user program (address must be saved by the user	#
	;  # prior to the EXCEPTION in epc)				#
	;  ###--------------------------------------------------------###

restore_return_adr:

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

