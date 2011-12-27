#include "FichierLog.hxx"
#include "Carte.hxx"
#include "FichierCarte.hxx"
#include "UserInterface.hxx"

int main(void) {
	UI ui;
	char running = 1;
	
	cout << endl << " Dossier C++ - Janvier 2011 - FanaDeCollection" << endl;
	cout << " =============================================" << endl << endl;
	
	while(running) {
		/* Waiting login */
		while(!ui.login());
		
		ui.prepare();
		
		running = ui.start_events();
	}
	
	return 0;
}
