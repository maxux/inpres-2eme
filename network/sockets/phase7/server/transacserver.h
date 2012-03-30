#ifndef __TRANSAC_H
	#define __TARNSAC_H
	
	FILE * ticket_init();
	void ticket_lister(FILE *fp);
	transac_t ticket_sortir(FILE *fp, transac_t parent);
	transac_t ticket_payer(FILE *fp, transac_t parent);
	transac_t ticket_reserver(FILE *fp, transac_t parent);
	void ticket_write_header(FILE *fp, transac_t *transaction);
	
	transac_t ticket_read_header(FILE *fp);
	
	int transac_check_exists(FILE *fp, transac_t *trans);
#endif
