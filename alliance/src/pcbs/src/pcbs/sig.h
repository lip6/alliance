/*****************************************************************************/
/*fermer les ports, desallouer la memoire et exit*/
void pcbs_exit(int code);

/*sortie avec message sur interruption*/
void emergency(int sig);

/*mise en place des interruptions*/
void error_signal_handler();
/*****************************************************************************/
