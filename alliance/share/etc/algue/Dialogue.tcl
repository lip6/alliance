proc Dialogue.tcl {w titre texte bitmap defaut args} {
  
  # pompe dans  J.K OUSTERHOUT pp 269
  # La boite de dialogue peut contenir un "texte" avec eventuellement
  # un  "bitmap" si celui ci est  {} pas de bitmap.
  # Un des bouton peut etre specifie en en "defaut" (sinon -1).
  # Dans ce cas il insere dans une frame sunken. La boite attend 
  # une reponse. Dans ce cas la boite est detruite et retourne l'indice 
  # du bouton invoque. Sur un retour chariot est avec un "defaut"
  # specifie, l'indice du defautest retourne 

  global bouton_diag
  global fontes
  
  if {[winfo exists $w]} {destroy $w}
  #set w $w.dialogue
  
  # cree la boite et la divise en deux

  toplevel $w -class Dialog
  wm title $w $titre
  wm geometry $w +300+400
  
  raise $w  

  
 
  frame $w.haut -relief raised -bd 1
  pack $w.haut -side top -fill both
  frame $w.bas -relief raised -bd 1
  pack $w.bas -side bottom -fill both

  # on remplie le haut
  
  message $w.haut.msg -width 3i -text $texte \
            -font $fontes(1)
  pack $w.haut.msg -side right -expand 1 -fill both \
     -padx 3m -pady 3m
  if {$bitmap != ""} {
   label $w.haut.bitmap -bitmap $bitmap
   pack $w.haut.bitmap -side left -padx 3m -pady 3m
  }
  
  # cree une ligne de boutons
  set i 0
  foreach but $args {
    button  $w.bas.button$i -text $but -font $fontes(1) -command \
       "set bouton_diag $i"
    if {$i == $defaut} {
      frame $w.bas.defaut -relief sunken -bd 1
      raise $w.bas.button$i
      pack $w.bas.defaut -side left -expand 1 \
           -padx 3m -pady 2m
      pack $w.bas.button$i -in $w.bas.defaut \
         -side left -padx 2m -pady 2m -ipadx 2m -ipady 1m
    } else {
      pack $w.bas.button$i -side left -expand 1 \
         -padx 3m -pady 3m -ipadx 2m -ipady 1m
    }
    incr i
  }

  # execute une sortie sur RC  et garde le controle
  
  if {$defaut >= 0} {
   bind $w <Return> "$w.bas.button$defaut flash; \
    set bouton_diag $defaut"
  }
  bell  
  set oldFocus [focus]
  #grab set $w
  focus $w

  # attend une reponse et retourne l'indice du bouton selectionne

  tkwait variable bouton_diag
  destroy $w
  focus $oldFocus
  return $bouton_diag
  
  
}
