proc Supervision_cmdunix.tcl { commande code_cmd } { 

  # genere une commande unix en tache de fond 
  # et gere le flot de sortie
  # adapte du B.B. Welch p 104
  # si code_cmd est a 1 , on court-circuite la commande
  # execution . 

            # ---------- procedures internes ------------

    proc Mk_cmdunix { } {
      # lance la commande et lit le flot de sortie
      global cmdunix entree blabla bouton 
       
      if [catch {open "|$cmdunix |& cat"} entree] {
        $blabla insert end $entree\n
      } else {
        fileevent $entree readable Supervision 
        $blabla insert end $cmdunix\n
        $bouton config -state disabled 
      }
    }
    proc Supervision { } {
      # supervise le flot de sortie
      global bouton num_process
      global entree blabla  code_erreur_comdiac
      if [eof $entree] {
        # bell ; # bell  
        Stop_cmdunix 0
      } else {
        gets $entree line
        if [string match *COMDIAC_PID* $line] {
          set num_process [lrange $line 1 1] 
          set num_process [expr $num_process-2]
        }
        if [string match *COMDIAC_OK* $line] {
          # bell 
          Stop_cmdunix 1  
        }
        $blabla insert end $line\n
        $blabla see end
  
      }
    }
    proc Stop_cmdunix { code } {
    # arrete et revalide l'execution 
      global entree bouton code_erreur_comdiac num_process
      if  { $code == 0 } {
        bell ;  bell
        set code_erreur_comdiac 0
      }
      if  { $code == 1 } { 
        set code_erreur_comdiac 1 
      }
      if  { $code == -1 } { 
        exec kill -9 $num_process 
        bell ;  bell
        set code_erreur_comdiac 0
      }
      catch {close $entree} 
      $bouton config -state normal   
    }                    

               #--------------------------------------

   global blabla cmdunix entree bouton
   global code_erreur_comdiac num_process
   global fontes

   set num_process 0
   set cmdunix $commande
   set w .cmd_unix
   set poub [winfo exists $w]
   if {$poub == 1} { 
     raise $w 
     if { $code_cmd == 1 } { Mk_cmdunix }
   }
   if {$poub == 0} { 
     toplevel $w -class Cmd_unix
     wm title  $w "ALGUE : console output"
     wm iconname $w Supervision
     wm geometry $w +30+30 
     raise $w

    # creation de la frame haute

    frame $w.haut -borderwidth 10
    pack  $w.haut -side top -fill x 
 
    # creation des boutons de commande
    
    frame $w.haut.1 
    pack  $w.haut.1 -side right -fill x 

    button $w.haut.1.stop -text STOP -font $fontes(1) \
      -command "Stop_cmdunix -1"
     
    $w.haut.1.stop config -cursor {hand2}
    set bouton [button $w.haut.1.exec -text EXECUTION -font $fontes(1) -command Mk_cmdunix]
    $w.haut.1.exec config -cursor {hand2}
    button $w.haut.1.sortie -text SORTIE -font $fontes(1) -command "destroy .cmd_unix"
    $w.haut.1.sortie config -cursor {hand2}
    pack $w.haut.1.sortie $w.haut.1.stop $w.haut.1.exec -side right

  
    # creation du label

    label $w.haut.label -font $fontes(1) -text "Commande UNIX: " -padx 0
    entry $w.haut.entree -width 80 -relief sunken -textvariable cmdunix
    pack $w.haut.label -side left
    pack $w.haut.entree -side left -fill x -expand true
    
    focus $w.haut.entree

    # creation de la frame basse pour scruter l'execution

    frame $w.bas 
#    set blabla [text $w.bas.text -width 80 -height 5 -font $fontes(1)

    set blabla [text $w.bas.text -width 80 -height 16  \
      -borderwidth 5 -relief ridge -setgrid true \
      -yscrollcommand {.cmd_unix.bas.scroll set}] 
    scrollbar $w.bas.scroll -command {.cmd_unix.bas.text yview}
    pack  $w.bas.text -side left -fill both -expand true
    pack  $w.bas.scroll -side right -fill y
    pack $w.bas -side top -fill both -expand true

    if { $code_cmd == 1 } { Mk_cmdunix }
  } 
    
}
