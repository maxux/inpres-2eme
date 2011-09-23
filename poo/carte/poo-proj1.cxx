#include <iostream>
#include "carte.hxx"

using namespace std;

int main(void) {
	Carte carte1;
	
	cout << "# Pass 1: Creating empty object" << endl;
	carte1.display();
	
	cout << endl << "# Pass 2: Setting up ID" << endl;
	carte1.set_id(1);
	carte1.display();
	
	cout << endl << "# Pass 2.1: Setting up Name" << endl;
	carte1.set_name("Lenny");
	carte1.display();
	
	cout << endl << "# Pass 2.1: Setting up Serie" << endl;
	carte1.set_serie("Toy Story");
	carte1.display();
	
	cout << endl << "# Pass 3: Creating defined object" << endl;
	Carte carte2("Woody", "Toy Story", 17);
	carte2.display();
	
	cout << endl << "# Pass 4: Creating object from another one" << endl;
	Carte carte3(carte2);
	carte3.display();
	
	return 0;
}
