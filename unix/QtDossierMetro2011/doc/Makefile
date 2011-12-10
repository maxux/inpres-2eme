all:
	pod2man -c "Metro 2011 Informations" metro2011.pod > metro2011.1

read: metro2011.1
	nroff -man metro2011.1 | less
