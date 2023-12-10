# CliqueMax

Análise de Redes Sociais: encontrando a clique máxima em um grafo.

## WorkDIR

- **Arquivo [results.ipynb](results.ipynb)**: Todos o report, resultados obtidos, análises e conclusões se encontram nesse arquivo.
- **Pasta [src](./src)**: Possui todas implementações realizadas tanto no colab quanto no Cluster com as configurações do slurm (veja o arquivo [clique_exaustivo_mpi.slurm](src/slurm/clique_exaustivo_mpi.slurm) para mais detalhes.
- **Pasta [amostra_grafos](./amostra_grafos)**: Possui amostra de tamanhos do grafo variando de 20 até 120, para facilitar a análise e plote dos gráficos.
- **Pasta [compilacoes](./compilacoes)**: Todas compilações das implementações de acordo com amostra de grafos.
- **Pasta [out_mpi](./out_mpi)**: Outputs gerados nas compilações da implementação de MPI.
- **Pasta [utils](./utils)**: Arquivos que facilitam a geração do grafo e verificação da clique máxima do grafo.
