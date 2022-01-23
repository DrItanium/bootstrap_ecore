# BOOTSTRAP_ECORE

While working on the sim_ecore project, I realized I needed some way to test
the viability of the core itself. I realized the easiest way to do this would
be to write a very tiny 64k program which would always be run on program
startup. It would run various operations to test functionality and then boot an
external program as normal.

This is a separate project because it is a fork of hitagimon but cut down
significantly.

