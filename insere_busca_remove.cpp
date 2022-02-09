#include<stdio.h>
#include<conio.h>
#include<string.h>

int main() {
    FILE *fd;
    
    //////////////////////////////   
    struct CliF {
        char CodCli[3];
        char CodF[3];
        char NomeCli[50];
        char NomeF[50];
        char Genero[50];
    } vet[18] = { {"01", "12", "Joao", "Filme-12", "Gen-12"},
                 {"01", "25", "Joao", "Filme-25", "Gen-25"},
                 {"01", "38", "Joao", "Filme-38", "Gen-38"},
                 {"01", "51", "Joao", "Filme-51", "Gen-51"},
                 
                 {"02", "12", "pedro", "Filme-12", "Gen-12"},
                 {"02", "25", "pedro", "Filme-25", "Gen-25"},
                 {"02", "38", "pedro", "Filme-38", "Gen-38"},
                 {"02", "51", "pedro", "Filme-51", "Gen-51"},
                 
                 {"03", "12", "lucas", "Filme-12", "Gen-12"},
                 {"03", "25", "lucas", "Filme-25", "Gen-25"},
                 {"03", "38", "lucas", "Filme-38", "Gen-38"},
                 {"03", "38", "lucas", "Filme-38", "Gen-38"},
                 {"03", "51", "lucas", "Filme-51", "Gen-51"},
                 
                 {"04", "12", "pandora", "Filme-12", "Gen-12"},
                 {"04", "25", "pandora", "Filme-25", "Gen-25"},
                 {"04", "38", "pandora", "Filme-38", "Gen-38"},
                 {"04", "38", "pandora", "Filme-38", "Gen-38"},
                 {"04", "51", "pandora", "Filme-51", "Gen-51"},
                 
			   };
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct busca {
        char CodCli[3];
        char CodF[3];
    } vet_b[8] = { 
                   {"02","38"},
				   {"02","70"},
				   
				   {"03","25"},
                   {"03","38"},
                   {"03","12"},
                   {"03","38"},
				   {"03","70"},
				   
				   {"01","51"}
				   };
    
    fd = fopen("busca.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);    
    
    //////////////////////////////
    struct remove {
        char CodCli[3];
        char CodF[3];
    } vet_r[6] = {	{"02","38"},
					{"01","25"}, 
					{"01","44"},
					{"02","51"},
					{"03","25"},
					{"02","44"}};
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

