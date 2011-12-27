#include "FichierLog.hxx"
#include "Carte.hxx"
#include "FichierCarte.hxx"
#include "UserInterface.hxx"

int main(void) {
	UI ui;
	char running = 1;
	
	while(running) {
		/* Waiting login */
		while(!ui.login());
		
		running = 0;
	}
	
	return 0;
}
