all: server client

server:                csapp.o server.o hanafuda-card.o hanafuda-deck.o hanafuda-hands.o serv-koikoi.o serv-playgame.o
	g++ -o hserver.out csapp.o server.o hanafuda-card.o hanafuda-deck.o hanafuda-hands.o serv-koikoi.o serv-playgame.o
client:                csapp.o final-client.o
	g++ -o hclient.out csapp.o final-client.o

hanafuda-card.o:
	g++ -Wall -g -c hanafuda-card.cpp -o hanafuda-card.o
hanafuda-deck.o:
	g++ -Wall -g -c hanafuda-deck.cpp -o hanafuda-deck.o
hanafuda-hands.o:
	g++ -Wall -g -c hanafuda-hands.cpp -o hanafuda-hands.o
serv-koikoi.o:
	g++ -Wall -g -c serv-koikoi.cpp -o serv-koikoi.o
serv-playgame.o:
	g++ -Wall -g -c serv-playgame.cpp -o serv-playgame.o
server.o:
	g++ -Wall -g -c server.cpp -o server.o
final-client.o:
	g++ -Wall -g -c final-client.cpp
csapp.o:
	gcc -g -O -c csapp.c -o csapp.o

clean:
	rm ./*.o
	rm ./*.out