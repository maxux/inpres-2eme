#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "chemin.h"
#include "metro_protocol.h"
#include "ipc_messages.h"
#include "debug.h"

metro_nodes_t nodes[METRO_MAX_STATION];

global_t sys;

int main(void) {
	int CheminSuivit[55];
	int mkey_id;
	ask_pathway_t ask;
	pathway_t pathway;
	message_t message;
	
	/* System Global */
	sys.mkey_id = &mkey_id;
	
	debugn("FRK: (DBG) Init...\n");

	if((mkey_id = msgget(MESSAGE_KEY_ID, IPC_CREAT | 0666)) < 0) {
		perror("msgget");
		return 1;
	}
	
	/* Waiting data from control */
	debug("FRK: (DBG) Reading data...\n");
	read_message(&message);
	
	if(message.request != QRY_SEARCH_DATA) {
		debugc("FRK: (DBG) Invalid answer from server (0x%x)\n", message.request);
		return 1;
	}
	
	/* Copy data */
	memcpy(&ask, message.text, sizeof(ask));
	
	/* Downloading nodes list */
	debug("FRK: (QRY) Nodes List\n");
	if(!send_message(QRY_NODESLIST, (void*) "Wantz Nodes List", 1L, 0))
		debugc("Cannot downloading nodes\n");
	
	read_message(&message);
	
	if(message.request == ACK_NODESLIST) {
		printf("FRK: (ACK) Nodes List\n");
		memcpy(nodes, message.text, sizeof(nodes));
		
	} else debugc("FRK: (ERR) Wrong opcode: 0x%x\n", message.request);
	
	debug("FRK: (DBG) Searching path from (%d) to (%d)...\n", ask.from, ask.to);
	
	/* Init pathway struct */
	pathway.nbstation = 0;
	
	/* Searching path */
	Recherche(ask.from, ask.to, 0, CheminSuivit, &(pathway.step[1]), &(pathway.nbstation));
	
	/* Override first station */
	pathway.nbstation++;
	pathway.step[0] = ask.from;

	/* Display path */
	AffChemin(&pathway);

	/* Sending message */
	debug("FRK: (DBG) Sending data to client (%d)\n", ask.client);
	send_message(ACK_SEARCH, (void*) &pathway, ask.client, sizeof(pathway));
	
	exit(0);
}


int Recherche(int NoeudCourant, int NoeudDestination, int TailleActuelle, int CheminSuivit[], int CheminOptimum[], int *TailleChemin) {
	int rc, Mrc = 0, i;

	if(nodes[NoeudCourant].visite)
		return 0;
		
	if(NoeudCourant == NoeudDestination) {
		if(TailleActuelle < *TailleChemin || *TailleChemin == 0) {
			memcpy(CheminOptimum, CheminSuivit, TailleActuelle * sizeof(int));
			*TailleChemin = TailleActuelle;
			return 1;
		}
		
		return 0;
	}
	
	nodes[NoeudCourant].visite = 1;

	i = 0;
	while(i < 6 && nodes[NoeudCourant].suivant[i] != 0) {
		CheminSuivit[TailleActuelle] = nodes[NoeudCourant].suivant[i];
		rc = Recherche(nodes[NoeudCourant].suivant[i], NoeudDestination,TailleActuelle + 1, CheminSuivit,CheminOptimum,TailleChemin);
		
		if(rc != 0)
			Mrc = rc;
		
		i++;
	}
	
	nodes[NoeudCourant].visite = 0;
	
	return Mrc;
}

void AffChemin(pathway_t *pathway) {
	int i = 0;
	char szBuffer[64] = {0};
	char szBuffer1[64] = {0};

	debug("FRK: (DBG) Length: %d\n", pathway->nbstation);
	
	while(i < pathway->nbstation) {
		sprintf(szBuffer1, "%d -> ", pathway->step[i++]);
		strcat(szBuffer, szBuffer1);
	}
	
	debug("FRK: (DBG) Path: %s\n", szBuffer);
	return;
}
