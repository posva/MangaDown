#! /bin/sh
#Créer les dossier nécessaires (UNIX)


if [ ! -e  "$1" ] 
then
echo "Creating directory $1" 
mkdir "$1" 
fi 

if [ ! -e  "$2" ] 
then
echo "Creating directory $2" 
mkdir "$2" 
fi 

for I in $3/*/. 
do D=$(echo "$I" | sed -e "s/$3/$2/" -e 's/\.$//')
if [ ! "$D" = "$2/*/" ]
then
	echo "Checking for directory $D"
	if [ ! -e  "$D" ] 
	then
		echo "Creating directory $D" 
		mkdir "$D" 
	fi 
fi
done