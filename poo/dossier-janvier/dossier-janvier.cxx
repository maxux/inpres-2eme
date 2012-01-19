#include "FichierLog.hxx"
#include "Carte.hxx"
#include "FichierCarte.hxx"
#include "UserInterface.hxx"
#include "dossier-janvier.hxx"

#include <signal.h>
#include <setjmp.h>

#include <list>
#include "Collectionneur.hxx"

jmp_buf jump_buffer;

int main(void) {
	UI ui;
	LinkCarte glob;
	char running = 1;
	
	/* Intercept SIGINT: Closing */
	signal_intercept(SIGINT, sig_handler);
	
	cout << endl << " Dossier C++ - Janvier 2011 - FanaDeCollection" << endl;
	cout << " =============================================" << endl << endl;
	
	if(!setjmp(jump_buffer)) {
		while(running) {
			/* Waiting login */
			while(!ui.login());
			
			ui.prepare();
			
			running = ui.start_events(&glob);
		}
	}
	
	/* Destructing objects... */
	
	return 0;
}

void sig_handler(int signum) {
	switch(signum) {
		case SIGINT:
			cout << endl << "[+] Closing..." << endl;
			longjmp(jump_buffer, SIGINT);
		break;
	}
}

int signal_intercept(int signal, void (*function)(int)) {
	struct sigaction sig;
	int ret;
	
	/* Building empty signal set */
	sigemptyset(&sig.sa_mask);
	
	/* Building Signal */
	sig.sa_handler	 = function;
	sig.sa_flags	 = 0;
	/* sig.sa_sigaction = NULL; */
	
	/* Installing Signal */
	if((ret = sigaction(signal, &sig, NULL)) == -1)
		perror("sigaction");
	
	return ret;
}
