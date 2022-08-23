# Overfoere filene lokalt fra pcen over til IFI ved aa kopiere kommandoen under, over til terminalen
# NB!!!!, husk aa fylle ut feltene markert med anforselstegn. Usikker paa om denne funker for windows, men funker for mac og linux. 
# NB!!!!, funker kun om fjerninnlogge ikke gjoeres med aa taste inn passord. 

scp -p apple.c the_apple.c the_apple.h Makefile "ditt navn"@login.ifi.uio.no:~/"Directory for aa teste filene, min var f.eks IN2140/oblig/oppgave-3"
