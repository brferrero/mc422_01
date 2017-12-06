# Autores:
 * Bruno Ferrero n.USP: 3690142  Curso: BCC
 * Rodrigo Alves n.USP: 6800149  Curso: BCC
 *
 * Data: Set/2017

# Compilar
	make

# Execução
	./ep1sh

	./ep1 1 trace.txt saida.txt d

	/* se rodar sem o ultimo argmento nao gera saida em stderr */

	$ tree
	.
	├── analise_simulador.py
	├── apresentacao.pdf
	├── ep1.c
	├── ep1sh.c
	├── ep1sh.h
	├── gera_trace.sh
	├── LEIAME
	├── Makefile
	├── run_experiments.sh
	└── trace.txt

# Observações
1. `gera_trace.sh`: script para gerar arquivos trace aleatórios de diveros tamanhos
2. O script `run_experiments.sh` foi usado para gerar as execucoes do ep1 para diversos tipos de arquivo trace com varios tamanhos
3. `analise_simulador.py`: foi usado para gerar as analises dos resultados gerados para as varias execucoes do ep1
4. não trata o casa de eventos chegando no mesmo `t0`
