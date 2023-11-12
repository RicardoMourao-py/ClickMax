python3 gera_grafo.py
g++ -Wall -O3 -g acha_clique.cpp -o acha_clique
./acha_clique
g++ -Wall -O3 -g clique_heuristica.cpp -o clique_heuristica
./clique_heuristica
g++ -Wall -O3 -g -fopenmp clique_threads.cpp -o clique_threads
./clique_threads
python3 clique_correto.py