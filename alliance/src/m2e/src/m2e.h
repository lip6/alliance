/* USER field codes  */
#define DEDALE	  7683
#define PLACEMENT       "/PLACEMENT"
#define CLASSE 	        "/CLASS"
#define MODEL_PIN	"/MODELE"
#define ALIM_PIN	"/ALIM"
#define PL_ALIM_PIN	"/PLAN_ALIM"
#define PS_ALIM_PIN	"/PISTE_ALIM"
#define CONN_PIN	"/CONNECTEURS"
#define IMPL_DED	/IMPLANTATION

#define	MAXL 		127		/* nombre max de car par ligne en entree	*/
#define	MAXREP		5		/* nombre de "repere,broche" par ligne		*/

#define	FIGURE 		0
#define	INSTANCE	1

#define	SIZE_BUF	512		/* taille du tampon pour la concatenation	*/

int LigneSuiv(char *pbuf);
int LigneSlash(char *pbuf);
int OpenFileOut(char *name);
int OpenTempOut(void);
int GetPtrFichPin(char * pnom);
int GenRepFig(char *pbuf,char *prep);
int GenAlimFig(char *pbuf);
int GenAlimInst(char *pbuf);
void GenPatteAlim(char *pbuf,char *prep,char instance);
int NotAlimName(losig_list *ps);
void SauvPinCon(locon_list *pcon,char *pbuf,char *pb,char *pd);
int TraitConnect(locon_list *pcon,char *pbuf,char *prep);
int TraitFichFig(lofig_list *ptfig);
int GenRepIns(char *pbuf,loins_list *ptins);
int TraitFichInst(loins_list *ptins);
int TraitListInst(lofig_list *ptfig);
void ConcatFich();
void TraitSignaux(lofig_list *ptfig);
int ParseCommand(int argc,char *argv[]);
int LoadPinPath(void);
int ChercheSignal(losig_list *psig,char *pname);
int TraitFichClass(lofig_list *ptfig);
loins_list* ChercheInst(loins_list *pins,char *pname);
