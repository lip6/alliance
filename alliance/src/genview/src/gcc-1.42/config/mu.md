;; -------------------------------------------------------------

;;
;; Description de la machine universelle
;;

;;
;; Ecrit par Arnaud COMPAN & Francois PECHEUX ** avril-juin 1990
;; Quelques modifs par Frederic Petrot, 91/92

;; -------------------------------------------------------------

(define_insn "cmpqi"
  [(set (cc0)
	(compare (match_operand:QI 0 "general_operand" "g")
		 (match_operand:QI 1 "general_operand" "g")))]
  ""
  "cmp.qi %0,%1")

(define_insn "cmphi"
  [(set (cc0)
	(compare (match_operand:HI 0 "general_operand" "g")
		 (match_operand:HI 1 "general_operand" "g")))]
  ""
  "cmp.hi %0,%1")

(define_insn "cmpsi"
  [(set (cc0)
	(compare (match_operand:SI 0 "general_operand" "g")
		 (match_operand:SI 1 "general_operand" "g")))]
  ""
  "cmp.si %0,%1")

(define_insn "cmpsf"
  [(set (cc0)
	(compare (match_operand:SF 0 "general_operand" "g")
		 (match_operand:SF 1 "general_operand" "g")))]
  ""
  "cmp.sf %0,%1")

(define_insn "cmpdf"
  [(set (cc0)
	(compare (match_operand:DF 0 "general_operand" "g")
		 (match_operand:DF 1 "general_operand" "g")))]
  ""
  "cmp.df %0,%1")

(define_insn "tstqi"
  [(set (cc0)
        (match_operand:QI 0 "general_operand" "g"))]
  ""
  "cmp.qi %0,#0")

(define_insn "tsthi"
  [(set (cc0)
        (match_operand:HI 0 "general_operand" "g"))]
  ""
  "cmp.hi %0,#0")

(define_insn "tstsi"
  [(set (cc0)
        (match_operand:SI 0 "general_operand" "g"))]
  ""
  "cmp.si %0,#0")

(define_insn "tstsf"
  [(set (cc0)
        (match_operand:SF 0 "general_operand" "g"))]
  ""
  "cmp.sf %0,#0")

(define_insn "tstdf"
  [(set (cc0)
        (match_operand:DF 0 "general_operand" "g"))]
  ""
  "cmp.df %0,#0")

(define_insn "movqi"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(match_operand:QI 1 "general_operand" "g"))]
  ""
  "mov.qi %0,%1")

(define_insn "movhi"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(match_operand:HI 1 "general_operand" "g"))]
  ""
  "mov.hi %0,%1")

(define_insn "movsi"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(match_operand:SI 1 "general_operand" "g"))]
  ""
  "mov.si %0,%1")

(define_insn "movsf"
  [(set (match_operand:SF 0 "general_operand" "=g")
	(match_operand:SF 1 "general_operand" "g"))]
  ""
  "mov.sf %0,%1")

(define_insn "movdf"
  [(set (match_operand:DF 0 "general_operand" "=g")
	(match_operand:DF 1 "general_operand" "g"))]
  ""
  "mov.df %0,%1")

(define_insn "extendqihi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
        (sign_extend:HI
         (match_operand:QI 1 "general_operand" "g")))]
  ""
  "ext.qi.hi %1,%0")

(define_insn "extendqisi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
        (sign_extend:SI
         (match_operand:QI 1 "general_operand" "g")))]
  ""
  "ext.qi.si %1,%0")

(define_insn "extendhisi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
        (sign_extend:SI
         (match_operand:HI 1 "general_operand" "g")))]
  ""
  "ext.hi.si %1,%0")

(define_insn "zero_extendhisi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
        (zero_extend:SI
         (match_operand:HI 1 "general_operand" "g")))]
  ""
  "ext0.hi.si %1,%0")

(define_insn "zero_extendqihi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
        (zero_extend:HI
         (match_operand:QI 1 "general_operand" "g")))]
  ""
  "ext0.qi.hi %1,%0")

(define_insn "zero_extendqisi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
        (zero_extend:SI
         (match_operand:QI 1 "general_operand" "g")))]
  ""
  "ext0.qi.si %1,%0")

(define_insn "extendsfdf2"
  [(set (match_operand:DF 0 "general_operand" "=g")
        (float_extend:DF
         (match_operand:SF 1 "general_operand" "g")))]
  ""
  "ext.sf.df %1,%0")

(define_insn "truncsiqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
        (truncate:QI
         (match_operand:SI 1 "general_operand" "g")))]
  ""
  "trunc.si.qi %1,%0")

(define_insn "truncsihi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
        (truncate:HI
         (match_operand:SI 1 "general_operand" "g")))]
  ""
  "trunc.si.hi %1,%0")

(define_insn "trunchiqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
        (truncate:QI
         (match_operand:HI 1 "general_operand" "g")))]
  ""
  "trunc.hi.qi %1,%0")

(define_insn "truncdfsf2"
  [(set (match_operand:SF 0 "general_operand" "=g")
        (float_truncate:SF
         (match_operand:DF 1 "general_operand" "g")))]
  ""
  "trunc.df.sf %1,%0")

(define_insn "floatqisf2"
  [(set (match_operand:SF 0 "general_operand" "=g")
        (float:SF (match_operand:QI 1 "general_operand" "g")))]
  ""
  "float.qi.sf %1,%0")

(define_insn "floathisf2"
  [(set (match_operand:SF 0 "general_operand" "=g")
        (float:SF (match_operand:HI 1 "general_operand" "g")))]
  ""
  "float.hi.sf %1,%0")

(define_insn "floatsisf2"
  [(set (match_operand:SF 0 "general_operand" "=g")
        (float:SF (match_operand:SI 1 "general_operand" "g")))]
  ""
  "float.si.sf %1,%0")

(define_insn "floatqidf2"
  [(set (match_operand:DF 0 "general_operand" "=g")
        (float:DF (match_operand:QI 1 "general_operand" "g")))]
  ""
  "float.qi.df %1,%0")

(define_insn "floathidf2"
  [(set (match_operand:DF 0 "general_operand" "=g")
        (float:DF (match_operand:HI 1 "general_operand" "g")))]
  ""
  "float.hi.df %1,%0")

(define_insn "floatsidf2"
  [(set (match_operand:DF 0 "general_operand" "=g")
        (float:DF (match_operand:SI 1 "general_operand" "g")))]
  ""
  "float.si.df %1,%0")

(define_insn "fix_truncsfqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
        (fix:QI (fix:SF (match_operand:SF 1 "general_operand" "g"))))]
  ""
  "fix_trunc.sf.qi %1,%0")  

(define_insn "fix_truncsfhi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
        (fix:HI (fix:SF (match_operand:SF 1 "general_operand" "g"))))]
  ""
  "fix_trunc.sf.hi %1,%0")
 
(define_insn "fix_truncsfsi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
        (fix:SI (fix:SF (match_operand:SF 1 "general_operand" "g"))))]
  ""
  "fix_trunc.sf.si %1,%0")

(define_insn "fix_truncdfqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
        (fix:QI (fix:DF (match_operand:DF 1 "general_operand" "g"))))]
  ""
  "fix_trunc.df.qi %1,%0")
 
(define_insn "fix_truncdfhi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
        (fix:HI (fix:DF (match_operand:DF 1 "general_operand" "g"))))]
  ""
  "fix_trunc.df.hi %1,%0")
 
(define_insn "fix_truncdfsi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
        (fix:SI (fix:DF (match_operand:DF 1 "general_operand" "g"))))]
  ""
  "fix_trunc.df.si %1,%0")
 
(define_insn "fixunssfqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(unsigned_fix:QI (fix:SF (match_operand:SF 1 "general_operand" "g"))))]
  ""
  "fixuns.sf.qi %1,%0")

(define_insn "fixunssfhi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(unsigned_fix:HI (fix:SF (match_operand:SF 1 "general_operand" "g"))))]
  ""
  "fixuns.sf.hi %1,%0")

(define_insn "fixunssfsi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(unsigned_fix:SI (fix:SF (match_operand:SF 1 "general_operand" "g"))))]
  ""
  "fixuns.sf.si %1,%0")

(define_insn "fixunsdfqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(unsigned_fix:QI (fix:DF (match_operand:DF 1 "general_operand" "g"))))]
  ""
  "fixuns.df.qi %1,%0")

(define_insn "fixunsdfhi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(unsigned_fix:HI (fix:DF (match_operand:DF 1 "general_operand" "g"))))]
  ""
  "fixuns.df.hi %1,%0")

(define_insn "fixunsdfsi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(unsigned_fix:SI (fix:DF (match_operand:DF 1 "general_operand" "g"))))]
  ""
  "fixuns.df.si %1,%0")

(define_insn "addqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(plus:QI (match_operand:QI 1 "general_operand" "%g")
		 (match_operand:QI 2 "general_operand" "g")))]
  ""
  "add.qi %0,%1,%2")

(define_insn "addhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(plus:HI (match_operand:HI 1 "general_operand" "%g")
		 (match_operand:HI 2 "general_operand" "g")))]
  ""
  "add.hi %0,%1,%2")

(define_insn "addsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(plus:SI (match_operand:SI 1 "general_operand" "%g")
		 (match_operand:SI 2 "general_operand" "g")))]
  ""
  "add.si %0,%1,%2")

(define_insn "addsf3"
  [(set (match_operand:SF 0 "general_operand" "=g")
	(plus:SF (match_operand:SF 1 "general_operand" "%g")
		 (match_operand:SF 2 "general_operand" "g")))]
  ""
  "add.sf %0,%1,%2")

(define_insn "adddf3"
  [(set (match_operand:DF 0 "general_operand" "=g")
	(plus:DF (match_operand:DF 1 "general_operand" "%g")
		 (match_operand:DF 2 "general_operand" "g")))]
  ""
  "add.df %0,%1,%2")

(define_insn "mulqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(mult:QI (match_operand:QI 1 "general_operand" "%g")
		 (match_operand:QI 2 "general_operand" "g")))]
  ""
  "mul.qi %0,%1,%2")

(define_insn "mulhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(mult:HI (match_operand:HI 1 "general_operand" "%g")
		 (match_operand:HI 2 "general_operand" "g")))]
  ""
  "mul.hi %0,%1,%2")

(define_insn "mulsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(mult:SI (match_operand:SI 1 "general_operand" "%g")
		 (match_operand:SI 2 "general_operand" "g")))]
  ""
  "mul.si %0,%1,%2")

(define_insn "mulsf3"
  [(set (match_operand:SF 0 "general_operand" "=g")
	(mult:SF (match_operand:SF 1 "general_operand" "%g")
		 (match_operand:SF 2 "general_operand" "g")))]
  ""
  "mul.sf %0,%1,%2")

(define_insn "muldf3"
  [(set (match_operand:DF 0 "general_operand" "=g")
	(mult:DF (match_operand:DF 1 "general_operand" "%g")
		 (match_operand:DF 2 "general_operand" "g")))]
  ""
  "mul.df %0,%1,%2")

(define_insn "divqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(div:QI (match_operand:QI 1 "general_operand" "g")
		 (match_operand:QI 2 "general_operand" "g")))]
  ""
  "div.qi %0,%1,%2")

(define_insn "divhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(div:HI (match_operand:HI 1 "general_operand" "g")
		 (match_operand:HI 2 "general_operand" "g")))]
  ""
  "div.hi %0,%1,%2")

(define_insn "divsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(div:SI (match_operand:SI 1 "general_operand" "g")
		 (match_operand:SI 2 "general_operand" "g")))]
  ""
  "div.si %0,%1,%2")

(define_insn "divsf3"
  [(set (match_operand:SF 0 "general_operand" "=g")
	(div:SF (match_operand:SF 1 "general_operand" "g")
		 (match_operand:SF 2 "general_operand" "g")))]
  ""
  "div.sf %0,%1,%2")

(define_insn "divdf3"
  [(set (match_operand:DF 0 "general_operand" "=g")
	(div:DF (match_operand:DF 1 "general_operand" "g")
		 (match_operand:DF 2 "general_operand" "g")))]
  ""
  "div.df %0,%1,%2")

(define_insn "subqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(minus:QI (match_operand:QI 1 "general_operand" "g")
		  (match_operand:QI 2 "general_operand" "g")))]
  ""
  "sub.qi %0,%1,%2")

(define_insn "subhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(minus:HI (match_operand:HI 1 "general_operand" "g")
		  (match_operand:HI 2 "general_operand" "g")))]
  ""
  "sub.hi %0,%1,%2")

(define_insn "subsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(minus:SI (match_operand:SI 1 "general_operand" "g")
		  (match_operand:SI 2 "general_operand" "g")))]
  ""
  "sub.si %0,%1,%2")

(define_insn "subsf3"
  [(set (match_operand:SF 0 "general_operand" "=g")
	(minus:SF (match_operand:SF 1 "general_operand" "g")
		  (match_operand:SF 2 "general_operand" "g")))]
  ""
  "sub.sf %0,%1,%2")

(define_insn "subdf3"
  [(set (match_operand:DF 0 "general_operand" "=g")
	(minus:DF (match_operand:DF 1 "general_operand" "g")
		  (match_operand:DF 2 "general_operand" "g")))]
  ""
  "sub.df %0,%1,%2")

(define_insn "andqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(and:QI (match_operand:QI 1 "general_operand" "%g")
		(match_operand:QI 2 "general_operand" "g")))]
  ""
  "and.qi %0,%1,%2")

(define_insn "andhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(and:HI (match_operand:HI 1 "general_operand" "%g")
		(match_operand:HI 2 "general_operand" "g")))]
  ""
  "and.hi %0,%1,%2")

(define_insn "andsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(and:SI (match_operand:SI 1 "general_operand" "%g")
		(match_operand:SI 2 "general_operand" "g")))]
  ""
  "and.si %0,%1,%2")

(define_insn "iorqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(ior:QI (match_operand:QI 1 "general_operand" "%g")
		(match_operand:QI 2 "general_operand" "g")))]
  ""
  "or.qi %0,%1,%2")

(define_insn "iorhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(ior:HI (match_operand:HI 1 "general_operand" "%g")
		(match_operand:HI 2 "general_operand" "g")))]
  ""
  "or.hi %0,%1,%2")

(define_insn "iorsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(ior:SI (match_operand:SI 1 "general_operand" "%g")
		(match_operand:SI 2 "general_operand" "g")))]
  ""
  "or.si %0,%1,%2")

(define_insn "xorqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(xor:QI (match_operand:QI 1 "general_operand" "%g")
		(match_operand:QI 2 "general_operand" "g")))]
  ""
  "xor.qi %0,%1,%2")

(define_insn "xorhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(xor:HI (match_operand:HI 1 "general_operand" "%g")
		(match_operand:HI 2 "general_operand" "g")))]
  ""
  "xor.hi %0,%1,%2")

(define_insn "xorsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(xor:SI (match_operand:SI 1 "general_operand" "%g")
		(match_operand:SI 2 "general_operand" "g")))]
  ""
  "xor.si %0,%1,%2")

(define_insn "negqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(neg:QI (match_operand:QI 1 "general_operand" "g")))]
  ""
  "neg.qi %0,%1")

(define_insn "neghi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(neg:HI (match_operand:HI 1 "general_operand" "g")))]
  ""
  "neg.hi %0,%1")

(define_insn "negsi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(neg:SI (match_operand:SI 1 "general_operand" "g")))]
  ""
  "neg.si %0,%1")

(define_insn "one_cmplqi2"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(not:QI (match_operand:QI 1 "general_operand" "g")))]
  ""
  "not.qi %0,%1")

(define_insn "one_cmplhi2"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(not:HI (match_operand:HI 1 "general_operand" "g")))]
  ""
  "not.hi %0,%1")

(define_insn "one_cmplsi2"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(not:SI (match_operand:SI 1 "general_operand" "g")))]
  ""
  "not.si %0,%1")

(define_insn "ashlqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(ashift:QI (match_operand:QI 1 "general_operand" "g")
		   (match_operand:QI 2 "general_operand" "g")))]
  ""
  "asl.qi %0,%1,%2")

(define_insn "ashlhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(ashift:HI (match_operand:HI 1 "general_operand" "g")
		   (match_operand:HI 2 "general_operand" "g")))]
  ""
  "asl.hi %0,%1,%2")

(define_insn "ashlsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(ashift:SI (match_operand:SI 1 "general_operand" "g")
		   (match_operand:SI 2 "general_operand" "g")))]
  ""
  "asl.si %0,%1,%2")

(define_insn "ashrqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(ashiftrt:QI (match_operand:QI 1 "general_operand" "g")
		     (match_operand:QI 2 "general_operand" "g")))]
  ""
  "asr.qi %0,%1,%2")

(define_insn "ashrhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(ashiftrt:HI (match_operand:HI 1 "general_operand" "g")
		     (match_operand:HI 2 "general_operand" "g")))]
  ""
  "asr.hi %0,%1,%2")

(define_insn "ashrsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(ashiftrt:SI (match_operand:SI 1 "general_operand" "g")
		     (match_operand:SI 2 "general_operand" "g")))]
  ""
  "asr.si %0,%1,%2")

(define_insn "lshlqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(lshift:QI (match_operand:QI 1 "general_operand" "g")
		   (match_operand:QI 2 "general_operand" "g")))]
  ""
  "lsl.qi %0,%1,%2")

(define_insn "lshlhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(lshift:HI (match_operand:HI 1 "general_operand" "g")
		   (match_operand:HI 2 "general_operand" "g")))]
  ""
  "lsl.hi %0,%1,%2")

(define_insn "lshlsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(lshift:SI (match_operand:SI 1 "general_operand" "g")
		   (match_operand:SI 2 "general_operand" "g")))]
  ""
  "lsl.si %0,%1,%2")

(define_insn "lshrqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(lshiftrt:QI (match_operand:QI 1 "general_operand" "g")
		     (match_operand:QI 2 "general_operand" "g")))]
  ""
  "lsr.qi %0,%1,%2")

(define_insn "lshrhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(lshiftrt:HI (match_operand:HI 1 "general_operand" "g")
		     (match_operand:HI 2 "general_operand" "g")))]
  ""
  "lsr.hi %0,%1,%2")

(define_insn "lshrsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(lshiftrt:SI (match_operand:SI 1 "general_operand" "g")
		     (match_operand:SI 2 "general_operand" "g")))]
  ""
  "lsr.si %0,%1,%2")

/*
(define_insn "rotlqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(rotate:QI (match_operand:QI 1 "general_operand" "g")
		   (match_operand:QI 2 "general_operand" "g")))]
  ""
  "rotl.qi %0,%1,%2")

(define_insn "rotlhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(rotate:HI (match_operand:HI 1 "general_operand" "g")
		   (match_operand:HI 2 "general_operand" "g")))]
  ""
  "rotl.hi %0,%1,%2")

(define_insn "rotlsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(rotate:SI (match_operand:SI 1 "general_operand" "g")
		   (match_operand:SI 2 "general_operand" "g")))]
  ""
  "rotl.si %0,%1,%2")

(define_insn "rotrqi3"
  [(set (match_operand:QI 0 "general_operand" "=g")
	(rotatert:QI (match_operand:QI 1 "general_operand" "g")
		     (match_operand:QI 2 "general_operand" "g")))]
  ""
  "rotr.qi %0,%1,%2")

(define_insn "rotrhi3"
  [(set (match_operand:HI 0 "general_operand" "=g")
	(rotatert:HI (match_operand:HI 1 "general_operand" "g")
		     (match_operand:HI 2 "general_operand" "g")))]
  ""
  "rotr.hi %0,%1,%2")

(define_insn "rotrsi3"
  [(set (match_operand:SI 0 "general_operand" "=g")
	(rotatert:SI (match_operand:SI 1 "general_operand" "g")
		     (match_operand:SI 2 "general_operand" "g")))]
  ""
  "rotr.si %0,%1,%2")
*/

(define_insn "beq"
  [(set (pc)
	(if_then_else (eq (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "beq %l0")

(define_insn "bne"
  [(set (pc)
	(if_then_else (ne (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bne %l0")

(define_insn "bgt"
  [(set (pc)
	(if_then_else (gt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bgt %l0")

(define_insn "bgtu"
  [(set (pc)
	(if_then_else (gtu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bgtu %l0")

(define_insn "blt"
  [(set (pc)
	(if_then_else (lt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "blt %l0")

(define_insn "bltu"
  [(set (pc)
	(if_then_else (ltu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bltu %l0")

(define_insn "bge"
  [(set (pc)
	(if_then_else (ge (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bge %l0")

(define_insn "bgeu"
  [(set (pc)
	(if_then_else (geu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bgeu %l0")

(define_insn "ble"
  [(set (pc)
	(if_then_else (le (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "ble %l0")

(define_insn "bleu"
  [(set (pc)
	(if_then_else (leu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "bleu %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (eq (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "bne %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (ne (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "beq %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (gt (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "ble %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (gtu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "bleu %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (lt (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "bge %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (ltu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "bgeu %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (ge (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "blt %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (geu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "bltu %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (le (cc0)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "bgt %l0")

(define_insn ""
  [(set (pc)
	(if_then_else (leu (cc0)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "bgtu %l0")

(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "bra %l0")

(define_insn "tablejump"
  [(set (pc) (match_operand:SI 0 "general_operand" "g"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "bra %0")

(define_insn "call"
  [(call (match_operand:SI 0 "ever_good" "g")
	 (match_operand:SI 1 "ever_good" "g"))]
  ""
  "call %0")

(define_insn "call_value"
  [(set (match_operand 0 "" "g")
	(call (match_operand:SI 1 "ever_good" "g")
	      (match_operand:SI 2 "ever_good" "g")))]
  ""
  "call %1")

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop")

