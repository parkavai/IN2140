#
# Idea for a Makefile that can be used for local & remote compiling
# and local & remote testing if you use SSHFS
# Works for MAC and Linux
#
# On Mac and Linux, just must first do something like this:
#     cd ~
#     mkdir ifi-home
#     sshfs griff@login.ifi.uio.no: ifi-home
#

run: 
	gcc oppgave-1.c -o oppgave-1

test: 
	./oppgave-1
		

clean:
	rm -f oppgave-1
