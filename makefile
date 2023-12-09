all: server client

server:                csapp   server   hanafuda-card   hanafuda-deck   hanafuda-hands   serv-koikoi   serv-playgame
	g++ -o hserver.out csapp.o server.o hanafuda-card.o hanafuda-deck.o hanafuda-hands.o serv-koikoi.o serv-playgame.o
client:                csapp   final-client
	g++ -o hclient.out csapp.o final-client.o

hanafuda-card:
	g++ -Wall -g -c hanafuda-card.cpp -o hanafuda-card.o
hanafuda-deck:
	g++ -Wall -g -c hanafuda-deck.cpp -o hanafuda-deck.o
hanafuda-hands:
	g++ -Wall -g -c hanafuda-hands.cpp -o hanafuda-hands.o
serv-koikoi:
	g++ -Wall -g -c serv-koikoi.cpp -o serv-koikoi.o
serv-playgame:
	g++ -Wall -g -c serv-playgame.cpp -o serv-playgame.o
server:
	g++ -Wall -g -c server.cpp -o server.o
final-client:
	g++ -Wall -g -c final-client.cpp
csapp:
	gcc -g -O -c csapp.c -o csapp.o

clean:
	rm ./*.o
	rm ./*.out
