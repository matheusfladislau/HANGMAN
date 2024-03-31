#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>

#include "forca.h"

char palavrasecreta[TAMANHO_PALAVRA];
char chutes[LIMITE_CHUTES];
int chutesdados;

void clearscreen() {
	system("@cls||clear");
}

void abertura() {
	printf("===============================\n");
	printf("=         JOGO DA FORCA       =\n");
	printf("===============================\n\n");
}

void adicionapalavra() {
	getchar();
	char quer; 
	printf("Deseja adicionar uma nova palavra no jogo? (S/N)\n");
	scanf(" %c", &quer);

	if (quer == 'S') {
		char novapalavra[TAMANHO_PALAVRA];
		printf("Qual é a nova palavra?\n");
		scanf("%s", novapalavra);

		for (int i = 0; novapalavra[i] != '\0'; i++) {
			novapalavra[i] = toupper(novapalavra[i]);
		}

		FILE* f;
		f = fopen("palavras.txt", "r+");
		if (f == 0) {
			printf("Banco de dados não disponível.");
			exit(1);
		}

		int qtd;
		fscanf(f, "%d", &qtd);
		qtd++;

		fseek(f, 0, SEEK_SET);
		fprintf(f, "%d", qtd);

		fseek(f, 0, SEEK_END);
		fprintf(f, "\n%s", novapalavra);

		fclose(f);
	}
}

void escolhepalavra() {
	FILE* f;
	f = fopen("palavras.txt", "r");
	
	if (f == 0) {
		printf("Banco de dados não disponível.");
		exit(1);
	}

	int qtdpalavras;
	fscanf(f, "%d", &qtdpalavras);

	srand(time(NULL));
	int randomico = rand() % qtdpalavras;

	for (int i = 0; i <= randomico; i++) {
		fscanf(f, "%s", palavrasecreta);
	}

	fclose(f);
}

int jachutou(char letra) {
    int achou = 0;
    for(int i = 0; i < chutesdados; i++) {
        if(chutes[i] == letra) {
            achou = 1;
            break;
        }
    }
    return achou;
}

void desenhaforca() {
	clearscreen();
	abertura();

	int erros = chuteserrados();
	printf("Quantidade de erros: %d. O máximo é %d\n", erros, LIMITE_ERROS);

	printf("  _______      \n");
	printf(" |/      |     \n");
	printf(" |      %s     \n", (erros > 0 ? "(_)"  :  " "));
	printf(" |      %s     \n", (erros > 1 ? "\\|/" :  " "));
	printf(" |       %c    \n", (erros > 2 ? '|'    :  ' '));
	printf(" |      %s     \n", (erros > 3 ? "/ \\" :  " "));
	printf(" |             \n");
	printf("_|___          \n");
	printf("\n\n");

	for (int i = 0; i < strlen(palavrasecreta); i++) {
		int achou = jachutou(palavrasecreta[i]);

		if (achou)
			printf("%c ", palavrasecreta[i]);
		else
			printf("_ ");
	}
	printf("\n");
}

void chuta() {
	char chute;
	scanf(" %c", &chute);
	chutes[chutesdados] = toupper(chute);
	chutesdados++;
}

int acertou() {
	for (int i = 0; i < strlen(palavrasecreta); i++) {
		if (!jachutou(palavrasecreta[i])) {
			return 0;
		}
	}
	return 1;
}

int letraexiste(char letra) {
	for (int j = 0; j < strlen(palavrasecreta); j++) {
		if (palavrasecreta[j] == letra) {
			return 1;
		}
	}
	return 0;
}

int chuteserrados() {
	int erros = 0;
	
	for (int i = 0; i < chutesdados; i++) {
		if (!letraexiste(chutes[i])) {
			erros++;
		}
	}
	return erros;
}

int enforcou() {
	return chuteserrados() >= LIMITE_ERROS;
}

int main() {
	setlocale(LC_ALL, "Portuguese");

	escolhepalavra();

	do {
		desenhaforca();
		chuta();
	} while(!acertou() && !enforcou());

	if (acertou())
		printf("Parabéns! Você venceu!\n");
	else
		printf("Infelizmente você perdeu.\n");
	
	printf("A palavra era: %s\n", palavrasecreta);

	adicionapalavra();
	return 0;
}
