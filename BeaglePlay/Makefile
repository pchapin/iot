all:  observer archivist

observer: observer.o sensor.o
	gcc -g -o observer observer.o sensor.o -lm -lpq

observer.o:  observer.c sensor.h
	gcc -g -std=c99 -Wall -c -I/usr/include/postgresql observer.c

sensor.o:    sensor.c sensor.h
	gcc -g -std=c99 -Wall -c sensor.c

# Archivist needs sensor.o because of `get_system_id`
archivist: archivist.o sensor.o
	gcc -o archivist archivist.o sensor.o -lm -lpq

archivist.o:  archivist.c sensor.h
	gcc -std=c99 -Wall -c -I/usr/include/postgresql archivist.c

clean: 
	rm -f *.o observer archivist 