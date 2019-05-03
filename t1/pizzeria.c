#include "pizzeria.h"
#include "queue.h"
#include "helper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
/*
Atentem aos seguintes recursos que devem ser gerenciados no simulador:

    1 Pá de pizza.
    1 Forno com capacidade para tam_forno pizzas.
    n_pizzaiolos pizzaiolos.
    n_mesas mesas de 4 lugares.
    1 Espaço vazio no balcão.
    1 Deck de pedidos capaz de conter tam_deck pedidos.
    n_garçons Garçons.

*/
int tam_forno, n_pizzaiolos, n_mesas, n_garcons, tam_deck, n_grupos;
int pa_pizze = 1;
queue_t smart_deck;
bool pizariafechada;
void pizzeria_init(int tam_forno, int n_pizzaiolos, int n_mesas,
                   int n_garcons, int tam_deck, int n_grupos) {
	tam_forno = tam_forno;
	n_pizzaiolos = n_pizzaiolos;
	n_mesas = n_mesas;
	n_garcons = n_garcons;
	tam_deck = tam_deck;
	n_grupos = n_grupos;
	pizariafechada = false;
	queue_init(&smart_deck, tam_deck);
}

void pizzeria_close() {
	/*while (n_mesas < 40) {
		// esperando todas pesas serem liberadas
	}*/
	pizariafechada = true;
}

void pizzeria_destroy() {
	queue_destroy(smart_deck);
}

void pizza_assada(pizza_t* pizza) {
}

int pegar_mesas(int tam_grupo) {
	if (pizariafechada == true) {
		return -1;
	} else {
		if (n_mesas*4 >= tam_grupo) {
			bool mesas_nao_ocupadas = true;
			int a1 = 0;
			int a2 = 4;
			int i = 1;
			while (mesas_nao_ocupadas) {
				if (tam_grupo > a1 && tam_grupo <= a2) {
					n_mesas -= i;
					mesas_nao_ocupadas = false;
				} else {
					i++;
					a1+=4;
					a2+=4;
				}
			}
			return 0;
		} else {
			printf("tamanho do grupo negativo ou igual a zero, ou tamanho do grupo maior do que lugares disponiveis\n");
		}
	} 
    return -1; //erro: não fui implementado (ainda)!
}

void garcom_tchau(int tam_grupo) {
	if (tam_grupo <= 0 || tam_grupo > n_mesas*4) {
		printf("tamanho do grupo invalido: tam_grupo <= 0 || tam_grupo > n_mesas*4\n");
	} else {
		int num_mesas_ocupadas = 0;
		bool achou = false;
		int a1 = 0;
		int a2 = 4;
		int i = 1;
		while (achou == false) {
			if (tam_grupo > a1 && tam_grupo <= a2) {
				num_mesas_ocupadas = i;
				achou = true;
			} else {
				i++;
				a1+=4;
				a2+=4;
			}
		}
		n_mesas += num_mesas_ocupadas;	
	}
}

void garcom_chamar() {
	/*while (!chegou) {
		espera();
	}*/
}

void fazer_pedido(pedido_t* pedido) {
	tam_deck--;
}

int pizza_pegar_fatia(pizza_t* pizza) {
    return -1; // erro: não fui implementado (ainda)!
}
