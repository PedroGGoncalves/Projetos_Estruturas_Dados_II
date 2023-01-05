#include<stdio.h>
#include<conio.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct CliF {
        int CodCli;
        int CodF;
        char NomeCli[50];
        char NomeF[50];
        char Genero[50];
    } vet[6] = {{1, 3, "Nome-1", "Filme-3", "Gen-3"},              
                {1, 2, "Nome-1", "Filme-2", "Gen-2"},
				{2, 3, "Nome-2", "Filme-3", "Gen-3"},
				
				{3, 3, "Nome-3", "Filme-3", "Gen-3"},
				{1, 1, "Nome-1", "Filme-1", "Gen-1"},
				{2, 2, "Nome-2", "Filme-2", "Gen-2"},
				};			
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
    struct busca_p {
        int CodCli;
        int CodF;
    } vet_bp[6] = {{2,3},
                   {1,7},
                   {2,3},
                   
                   {3,3},
                   
                   {2,3},
                   {3,3},
                   };
    
    fd = fopen("busca_p.bin", "w+b");
    fwrite(vet_bp, sizeof(vet_bp), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	const int quantidade = 8;
    char nomes[quantidade][50] = {"Filme-3", "Filme-X", "Filme-2", 
	                              "Filme-3", 
								  "Filme-3", "Filme-2", "Filme-X", "Filme-1"};
	   
    fd = fopen("busca_s.bin", "w+b");
    for (int i=0; i<quantidade; i++)
       fwrite(nomes[i], sizeof(char), 50, fd);
    fclose(fd);
    
    /*char buffer[50];
    fd = fopen("busca_s.bin", "r+b");
    for (int i=0; i<quantidade; i++)
       {
         fread(buffer, sizeof(buffer), 1, fd);
         printf("\n%s",buffer);
       }
    fclose(fd);
    getch();*/
}

