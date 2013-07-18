#!/bin/bash
PROJECT_DIR=Tiralabra
# Tätä tiedostoa käytetään automatisoituun testien ajamiseen.
# Tiedoston ajaminen linuxilla ./run_tests.sh tulee aikaansaada
# ohjelmasi testien suorittaminen.
#
# Mikäli käytät pohjassa mukana tullutta ant projektia poista
# kommentit
# PROJECT_DIR=Tiralabra

# Mikäli käytät pohjassa mukana tullutta ant projektia poista
# kommentit
# PROJECT_DIR=Tiralabra_maven

# Mikäli käytät muun nimistä kansiota, korvaa <kansiosi_nimi>
# alla olevalta riviltä kansiosi nimeksi, ja poista sen rivin
# kommentti
# PROJECT_DIR=<kansiosi_nimi>

if [ -d Tiralabra_maven ]
	then
	PROJECT_DIR=Tiralabra_maven
fi
if [ -d Tiralabra ]
	then
	PROJECT_DIR=Tiralabra
fi

if [ -f $PROJECT_DIR/pom.xml ]
  then
  cd $PROJECT_DIR
  mvn test
else
  cd $PROJECT_DIR
  ant test
fi