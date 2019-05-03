#include "pizzeria.h"
#include "queue.h"
#include "helper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
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
int pa_pizza = 1;
queue_t smart_deck;
int pizzariafechada;
pthread_t* pizzaiolos;

void* pizzaiolo_func (void* arg) {
	while (!pizzariafechada) {
		if(!queue_empty(&smart_deck)){

		pedido_t* pedido_recebido = queue_wait(&smart_deck); // TODO: TESTAR COM LOCK/UNLOCK SE DER DATA RACE

		pizza_t* pizza_montada = pizzaiolo_montar_pizza(pedido_recebido);
		
		lock pa
		if (tam_forno > 0) {
			pizzaiolo_colocar_forno(pizza_montada);
			tam_forno--;
		}
		unlock pa

		lock pa
		pizzaiolo_retirar_forno(pizza_montada);
		tam_forno++;
		coloca_balcao();
		unlock pa

		lock balcao
		garcom_chamar();
		sem_post(&garcom_sem);
		garcom_entregar(pizza_montada);
		unlock balcao

		}
	}
	return NULL;
}

void pizzeria_init(int tam_forno, int n_pizzaiolos, int n_mesas,
                   int n_garcons, int tam_deck, int n_grupos) {
	tam_forno = tam_forno;
	n_pizzaiolos = n_pizzaiolos;
	n_mesas = n_mesas;
	n_garcons = n_garcons;
	tam_deck = tam_deck;
	n_grupos = n_grupos;
	pizariafechada = 0
	pizzaiolos = malloc(n_pizzaiolos*sizeof(pthread_t)); // (pthread_t*)
	queue_init(&smart_deck, tam_deck);
	for (int i = 0; i < n_pizzaiolos ; i++) {
		pthread_create(pizzaiolos[i], NULL, pizzaiolo_func, NULL);
	}
}

void pizzeria_close() {
	/*while (n_mesas < 40) {
		// esperando todas pesas serem liberadas
	}*/
	pizzariafechada = 1;
}

void pizzeria_destroy() {
	queue_destroy(&smart_deck);
	free(pizzaiolos);
	for (int i = 0; i < n_pizzaiolos ; i++) {
		pthread_join(pizzaiolos[i]);
	}
}

void pizza_assada(pizza_t* pizza) {

}

int pegar_mesas(int tam_grupo) {
	if (pizzariafechada == 0) {
		return -1;
	} else {
		if (n_mesas*4 >= tam_grupo) {
			int mesas_nao_ocupadas = 1;
			int a1 = 0;
			int a2 = 4;
			int i = 1;
			while (mesas_nao_ocupadas) {
				if (tam_grupo > a1 && tam_grupo <= a2) {
					n_mesas -= i;
					mesas_nao_ocupadas = 0;
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
		int achou = 0;
		int a1 = 0;
		int a2 = 4;
		int i = 1;
		while (achou == 0) {
			if (tam_grupo > a1 && tam_grupo <= a2) {
				num_mesas_ocupadas = i;
				achou = 1;
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
	sem_wait(&garcom_sem);
}

void fazer_pedido(pedido_t* pedido) {
	void* pedido_v = (void*) pedido; // TALVEZ N PRECISE DAR CAST
	queue_push_back(&smart_deck, pedido_v);
}

int pizza_pegar_fatia(pizza_t* pizza) {
	if (pizza->fatias > 0) {
		lock PEGADOR
		pizza->fatias--;
		unlock PEGADOR
		return 0;
	} else {
		return -1; // PIZZA ACABO
	}
}
