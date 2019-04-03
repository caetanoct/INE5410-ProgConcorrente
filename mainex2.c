#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

//                          (principal)
//                               |
//              +----------------+--------------+
//              |                               |
//           filho_1                         filho_2
//              |                               |
//    +---------+-----------+          +--------+--------+
//    |         |           |          |        |        |
// neto_1_1  neto_1_2  neto_1_3     neto_2_1 neto_2_2 neto_2_3

// ~~~ printfs  ~~~
//      principal (ao finalizar): "Processo principal %d finalizado\n"
// filhos e netos (ao finalizar): "Processo %d finalizado\n"
//    filhos e netos (ao inciar): "Processo %d, filho de %d\n"

// Obs:
// - netos devem esperar 5 segundos antes de imprmir a mensagem de finalizado (e terminar)
// - pais devem esperar pelos seu descendentes diretos antes de terminar

void filhoImprime() {
	printf("Processo %d, filho de %d\n", getpid(), getppid());
}
void imprimeFinalizado() {
	printf("Processo %d finalizado\n", getpid());
}

int main(int argc, char** argv) {
    
//	int pid = fork();
//
//	if (pid > 0) {
//		int pid2 = fork();
//		if (pid2 > 0) {
//
//		} else if (pid2 == 0) {
//			filhoImprime();
//			exit(0);
//		} else {
//			printf("erro");
//		}
//	} else if (pid == 0) {
//		filhoImprime();
//		cria3netos();
//		for (int i = 0 ; i < 2 ; i++) {
//			int pid3 = fork()
//		}
//		exit(0);
//	} else {
//		printf("Erro");
//	}
	for (int i = 0 ; i < 2 ; i++) {
		
		int pid = fork();
		
		if (pid > 0) {
			
		} else if (pid == 0) {
			filhoImprime();
			for (int j = 0 ; j < 3 ; j++) {
				pid = fork();
				if (pid > 0) {
					
				} else if (pid == 0) {
					filhoImprime();
					sleep(5);
					imprimeFinalizado();
					exit(0);
				} else {
					printf("erro");
				}
			}
			imprimeFinalizado();
			exit(0);
		} else {
			printf("erro");
		}
	}
	while(wait(NULL) > 0);
    printf("Processo principal %d finalizado\n", getpid());    
    return 0;
}
