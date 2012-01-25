 == Exercice 8 ==

Quel est la commande pour configurer une IP Adresse en ppp ?
	-> 

Quel est la commande pour avoir l encapsulation ppp ?
	-> encapsulation ppp

Si je veut une horloge , quelle commande doit je rajouter sur le router1 & Router2
	-> Router1: clock rate 64000
	-> Router2: no clock rate

 == Exercice 9 ==
Pour avoir l authentification chap que doit je rajouter sur le router1 & router2 sachant que la partie ppp est déjà opérationnelle ?
	-> Routeur1: ppp authentification chap
	-> Routeur1: username _hostname_routeur2_ password _password_routeur2_
	-> Routeur1: enable secret _password_routeur1_
	
	-> Routeur2: ppp authentification chap
	-> Routeur2: username _hostname_routeur1_ password _password_routeur1_
	-> Routeur2: enable secret _password_routeur2_


 == Exercice 10 ==
Comment au niveau de packet tracer limiter le trafic à TCP & http ?
	-> Utiliser un filtre

Citez les couches mises en œuvre dans l exercice ?
	-> 1: Couche physique
	-> 2: Couche réseau
	-> 3: Couche de liaison
	-> 4: Couche de transport

Une requete http est encapsulée dans ?
	-> Un segment TCP

Un segment TCP est encapsulé dans ?
	-> Un packet IP

Un paquet IP est encapsulé dans ?
	-> Une trame Ethernet

Entre le PC1 et le routeur1 quelles adresses IP sont insérées dans le paquet IP ?
	-> Source (PC) et destination (Routeur)

Entre le Routeur1 et le routeur 2 quelles adresses IP sont insérées dans le paquet IP ?
	-> Source (Routeur1) et destination (Routeur2)

Entre le Routeur2 et le serveur quelles adresses IP sont insérées dans le paquet IP ?
	-> Source (Routeur2) et destination (Server1)

Le champ TTL vaut quoi respectivement au PC0, routeur1,router2, au Hub, au serveur ?
	-> PC0:	Départ: 128
	-> Routeur1: Arrivé: 128, départ: 127
	-> Routeur2: Arrivé: 127, départ: 126
	-> Hub: Arrivé: 126, départ: 126
	-> Serveur: Arrivé: 126

Quand TCP fait une ouverture de connexion quel flag utilise –t-il ?
	-> SYN

Quand le Serveur répond à la demande de connexion TCP quel flag utilise-t-il ?
	-> SYN+ACK

 == Exercice 11 ==
Switch0 Fa0/1: up, link 3rd
Switch0 Fa0/2: up, link primary

Switch1 Fa0/1: orange, link 3rd
Switch1 Fa0/2: up, link primary

Switch2 Fa0/1: up, link 2nd
Switch2 Fa0/2: up, link 3rd

A quoi sert le spanning Tree ?
	-> A ne pas avoir de redondance (boucle) dans les chemins

Que fait le spanning Tree concretement ?
	-> Il défini un chemin a utiliser, sur base d'une priorité, pour savoir quel port/chemin
	   utiliser pour accéder à une destination.

Comment savoir qui est la racine de l arbre ?
	-> Celui qui a le BID le plus faible (si égalité, MAC la plus faible)

Que faut-il faire pour qu un switch devienne racine de l arbre ?
	-> Le mettre avec une priorité plus basse que les autres

 == Exercice 12 ==
A quoi sert VTP ?
	-> C'est un protocol (couche 2) pour garder une cohérences des vlans sur les periferique Cisco.

Comment Définit-on un serveur VTP ?
	-> (config) vtp domain __name__
	-> (config) vtp mode server

Comment Définit – on un client VTP ?
	-> (config) vtp domain __name__
	-> (config) vtp mode client

Comment définit – t on un Trunk entre deux switchs ?
	-> (config-if) switchport mode dynamic desirable
	-> (config-if) switchport mode dynamic auto

Comment crée-t-on un VLAN et comment le nomme t-on ?
	-> (config) vlan X (X = numéro)
	-> (config-vlan) name __name__

Comment assigne-t-on un port à un vlan ?
	-> (config) interface __interface__
	-> (config-if) switchport mode access
	-> (config-if) switchport access vlan __id_vlan__

Comment vérifier si les Vlan Fonctionnent ?
	-> Ping !
