#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

//       (pai)      
//         |        
//    +----+----+
//    |         |   
// filho_1   filho_2


// ~~~ printfs  ~~~
// pai (ao criar filho): "Processo pai criou %d\n"
//    pai (ao terminar): "Processo pai finalizado!\n"
//  filhos (ao iniciar): "Processo filho %d criado\n"

// Obs:
// - pai deve esperar pelos filhos antes de terminar!


int main(int argc, char** argv) {

	int pid = fork();

	if (pid > 0) {
		printf("Processo pai criou %d\n", pid);
		int pid2 = fork();
		if (pid2 > 0) {
			printf("Processo pai criou %d\n", pid2);
			wait(NULL);
		} else if (pid2 == 0) {
			printf("Processo filho %d criado\n", getpid());
			exit(0);
		} else {
			printf("Erro %d na criação do processo filho\n", pid2);	
		}
		wait(NULL);
	} else if (pid == 0) {
		printf("Processo filho %d criado\n", getpid());
		exit(0);
	} else {
		printf("Erro %d na criação do processo filho\n", pid);
	}
    // ....

    printf("Processo pai finalizado!\n");   
    return 0;
}
