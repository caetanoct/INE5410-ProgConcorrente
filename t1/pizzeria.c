
#include "pizzeria.h"
#include "queue.h"
#include "helper.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
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
int tam_forno, n_pizzaiolos, n_mesas, n_mesas0, n_garcons, tam_deck, n_grupos;
int pa_pizza = 1;
queue_t smart_deck;
int pizzariafechada;
pthread_t* pizzaiolos;
pthread_mutex_t pa, balcao, pegador, mut_mesas;
sem_t garcom_sem;

void* pizzaiolo_func (void* arg);

void* pizzaiolo_func (void* arg) {
	while (!pizzariafechada) {
		if(!queue_empty(&smart_deck)){
		pedido_t* pedido_recebido = queue_wait(&smart_deck); // TODO: TESTAR COM LOCK/UNLOCK SE DER DATA RACE
		pizza_t* pizza_montada = pizzaiolo_montar_pizza(pedido_recebido);
		pthread_mutex_lock(&pa);
		if (tam_forno > 0) {
			pizzaiolo_colocar_forno(pizza_montada);
			tam_forno--;
		}
		pthread_mutex_unlock(&pa);
		pthread_mutex_lock(&pa);
		pizzaiolo_retirar_forno(pizza_montada);
		tam_forno++;
		//coloca_balcao();
		pthread_mutex_unlock(&pa);
		pthread_mutex_lock(&balcao);
		garcom_chamar();
		sem_post(&garcom_sem);
		garcom_entregar(pizza_montada);
		pthread_mutex_unlock(&balcao);
		}
	}
	return NULL;
}

void pizzeria_init(int tam_forno, int n_pizzaiolos, int n_mesas,
                   int n_garcons, int tam_deck, int n_grupos) {
	tam_forno = tam_forno;
	n_pizzaiolos = n_pizzaiolos;
	n_mesas0 = n_mesas;
    n_mesas = n_mesas;
	n_garcons = n_garcons;
	tam_deck = tam_deck;
	n_grupos = n_grupos;
	pizzariafechada = 0;
	pizzaiolos = malloc(n_pizzaiolos*sizeof(pthread_t)); // (pthread_t*)
	queue_init(&smart_deck, tam_deck);
	for (int i = 0; i < n_pizzaiolos ; i++) {
		pthread_create(&pizzaiolos[i], NULL, pizzaiolo_func, NULL);
	}
    // Inicializacao mutex's
	pthread_mutex_init(&pa, NULL);
	pthread_mutex_init(&balcao, NULL);
	pthread_mutex_init(&pegador, NULL);
	pthread_mutex_init(&mut_mesas, NULL);
	// Inicializacao sem
    sem_init(&garcom_sem, 0, n_garcons);
}

void pizzeria_close() {
	pizzariafechada = 1;
}

void pizzeria_destroy() {
	queue_destroy(&smart_deck);
	free(pizzaiolos);
	for (int i = 0; i < n_pizzaiolos ; i++) {
		pthread_join(pizzaiolos[i],NULL);
	}
    pthread_mutex_destroy(&pa);
    pthread_mutex_destroy(&balcao);
    pthread_mutex_destroy(&pegador);
    pthread_mutex_destroy(&mut_mesas);
    sem_destroy(&garcom_sem);
}

void pizza_assada(pizza_t* pizza) {

}

int pegar_mesas(int tam_grupo) {
    if (pizzariafechada) return -1;
    //calcula quantidade de mesas baseado no tamanho do grupo
    int qt_mesas = tam_grupo/4 + (tam_grupo%4 != 0); // (tam_grupo/4) + 1 se tam_grupo nao for multiplo de 4
    pthread_mutex_lock(&mut_mesas);
    if (n_mesas > qt_mesas) {
        return -1; // erro, não há mesas disponíveis
    }
    n_mesas -= qt_mesas;
    pthread_mutex_unlock(&mut_mesas);
    return 0; //erro: não fui implementado (ainda)!
}

void garcom_tchau(int tam_grupo) {
    if (!pizzariafechada) {
	    //calcula qtde mesas a serem desocupadas
	    int qt_mesas = tam_grupo/4 + (tam_grupo%4 != 0);
	    pthread_mutex_lock(&mut_mesas);
	    n_mesas += qt_mesas;
	    pthread_mutex_unlock(&mut_mesas);
	    sem_post(&garcom_sem);
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
		pthread_mutex_lock(&pegador);
		pizza->fatias--;
		pthread_mutex_unlock(&balcao);
		return 0;
	} else {
		return -1; // PIZZA ACABO
	}
}
