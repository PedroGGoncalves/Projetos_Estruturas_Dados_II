#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_HASH 13
/*
Grupo Gustavo Rosseto e Pedro Gonçalves
*/
//Fazer testes, reduzir, melhorar variaveis, CONFERIR TENTATIVAS, ACESSOS, POSIÇÃO
//Reduzir mais o cod - colocar no Github
//att linha 302 - procurachave
struct estrutura
  	{
    	char cliente[3],codfilme[3],nome[50], filme[50], genero[50];
  	}film; 
  	
 struct estrutura2
  	{
    	int chave, RRN, flag;
  	}hash;  	

char *identifica_campo(char *ts,int num)
{
	if(num==1)
		return strtok(ts,"#");
}	
int pega_registro(FILE *p_out, char *p_reg) //utilizado para saber se o registro está vazio ou não
{
     unsigned char bytes;
     unsigned int a=0 ;
     if (!fread(&bytes, sizeof(char), 1, p_out))
       return 0;
     else {
            fread(p_reg, bytes, 1, p_out); 
 			a= (int)bytes; //retorna em int
            return a;
          }  
}
void exibirTela (char *filme)			
{
	char *pch;
	pch = identifica_campo(filme,1);
	printf("Cod Cliente:%s\n",pch);
	pch = identifica_campo(NULL,1);
	printf("Cod Filme:%s\n",pch);
	pch = identifica_campo(NULL,1);
	printf("Nome Cliente:%s\n",pch);
	pch = identifica_campo(NULL,1);
	printf("Nome Filme:%s\n",pch);
	pch = identifica_campo(NULL,1);
	printf("Genero:%s\n",pch);
	pch = identifica_campo(NULL,1);
}
int contadorDigito (int temp)
{
	int conta_digito=0;
	do //qtos digitos tem
	{
		temp=temp/10; conta_digito++;
	}while(temp>=1);
	return conta_digito;
}
void salvaDigitoArquivo (int conta_digito, FILE *hashh)
{
	char reg[160];
	do
	{
		sprintf(reg,"%d",0);
		fwrite(reg, sizeof(char), strlen(reg), hashh);
		conta_digito++;
	}while(conta_digito<5);
}
void salvaDadosArquivo (int mat[13][3], int cont, int num, FILE *hashh)
{
	char reg[160];
	sprintf(reg,"%d",mat[cont][num]);
   	fwrite(reg, sizeof(char), strlen(reg), hashh); //registro é escrito no arquivo 
}
void salvaNoArquivo (int mat[13][3], FILE *hashh)
{
	int cont=0,temp=0,conta_digito;
	char reg[160];
	do
		{
			temp=mat[cont][0];			conta_digito=0;	
			conta_digito=contadorDigito(temp);
			salvaDigitoArquivo (conta_digito, hashh);
			salvaDadosArquivo (mat, cont, 0, hashh);
   			temp=mat[cont][1];			conta_digito=0;	
			conta_digito=contadorDigito(temp);
			salvaDigitoArquivo (conta_digito, hashh);
			salvaDadosArquivo (mat, cont, 1, hashh);
   			sprintf(reg,"%c",mat[cont][2]);
   			fwrite(reg, sizeof(char), 1, hashh); //registro é escrito no arquivo 
			cont++;
		}while (cont<TAM_HASH);
}
int lerNumero(char num1,char num2,char num3,char num4,char num5, FILE *hashh)
{
	fread(&num1,sizeof(char),1,hashh); //le dados para iserir no registro
	fread(&num2,sizeof(char),1,hashh); //le dados para iserir no registro
	fread(&num3,sizeof(char),1,hashh); //le dados para iserir no registro
	fread(&num4,sizeof(char),1,hashh); //le dados para iserir no registro
	fread(&num5,sizeof(char),1,hashh); //le dados para iserir no registro
					
	return (num1-'0')*10000+(num2-'0')*1000+(num3-'0')*100+(num4-'0')*10+(num5-'0');
}
int lendoAuxiliares(FILE *aux,int tam_reg, FILE *arq)
{
	char reg[160];
	int temp=0,cont=0;
	if(tam_reg==0)
	{
		sprintf(reg,"%c",1);
		fwrite(reg, sizeof(char), strlen(reg), aux);
	}
	else
	{
		fseek(aux,0,SEEK_END);	
		fseek(aux,ftell(aux)-1,0);	
		fread(&temp,sizeof(int),1,aux);		//por exemplo, se ja leu 1, entao temp4
		fseek(aux,ftell(aux),0); //fseek(aux,ftell(aux)+1,0);
		cont= temp; //contt -> valor da proxima posicao para ler do arquivo de remover
		cont=cont+1;
		sprintf(reg,"%c",cont); //escrevendo no arquivo auxremove.
		fwrite(reg, sizeof(char), strlen(reg), aux);
		fseek(arq,0,SEEK_END);
		
		if(ftell(arq)<=(cont-1)*6)	// verifica se tem arquivo pra remover
			return 1;	
		else
			fseek(arq,(cont-1)*6,0); //posiciona no registro pra remover	
	}
	return 0;
}
int procuraChave(int temp1, int mat[13][3], int *posi, int *cont)
{
	do
	{
		if(temp1==mat[*posi][0] && mat[*posi][2]==1)		// temp1==mat[posi][0] && mat[posi][2]==1
			return 1;
		if (mat[*posi][2]==0){	break;	} //Se encontrou 0, quer dizer q n existe a chave - P.54 Aula 10
		*posi=*posi +1;
		if(*posi>12)
			*posi=0;
		*cont=*cont+1;
	}while(*cont<TAM_HASH+1);
	return 0;
}
void insercao(FILE  *out,FILE *insere, FILE *hashh)
{
    int temp,temp_2=0,tam_reg,tam_arq,temp1,temp4=0,temp3,mat[13][3], posi=0,cont=0,teste=0,cond=0;
	char reg[160],reg2[160],temp2[100],num1,num2,num3,num4,num5;
	
	fseek(insere,0,SEEK_END); //coloca o arquivo no fim
	tam_arq=ftell(insere);   //pega tamnho do arquivo
	fseek(insere,0,0);	     //coloca o arquivo no início
	
	fseek(out,0,SEEK_END);	//mesma coisa mas do arquivo de saida
	int pos=ftell(out);
	fseek(out,0,0);
	
	tam_reg=pega_registro(out,reg); // tam_reg == 0, arquivo vazio
	
	if (tam_reg==0) //nada no arquivo principal, nada foi inserido
	{
   		fread(&film,sizeof(film),1,insere); 
   		temp=(ftell(insere))/156; 
   		sprintf(reg,"##%s#%s#%s#%s#%s#%c",film.cliente,film.codfilme,film.nome,film.filme,film.genero,temp);
   		temp_2=strlen(reg); //temp2 armazenará o tamanho do registro reg que será usado para se locomover no arquivo saida
   	
		sprintf(reg,"%c#%s#%s#%s#%s#%s#%c",temp_2,film.cliente,film.codfilme,film.nome,film.filme,film.genero,temp);
   		fwrite(reg, sizeof(char), strlen(reg), out); //registro é escrito no arquivo
		
		strcat(strcpy(temp2,film.cliente),film.codfilme); //copia o codigo cliente e concatena com o condifo do fime
   		temp1=atoi(temp2);	//transforma em inteiro o valor concatenado da chave
   		
		hash.chave=0; hash.RRN=0; hash.flag=0;	//zera as variaveis 
   		
   		int i;
   		for( i=0;i<TAM_HASH;i++)
   		{
   			mat[i][0]=hash.chave; mat[i][1]=hash.RRN; mat[i][2]=hash.flag;		//zerando a matriz inteira.
		}
		
   		fseek(hashh,0,0); //rewind hash
   		hash.chave=temp1;
   		hash.RRN=0;//posi no arq principal
   		hash.flag=1; //1 existe    		//0 vazio vdd   		//-1 vazio falso
   							
   		posi=hash.chave%TAM_HASH; //divisao inteira
   		printf("\n'%d'+'%d' (%d)\n",atoi(film.cliente),atoi(film.codfilme),temp1);
   		printf("Endereco: %d\n", posi);
   		printf("Chave inserida com sucesso\n");
   		
		mat[posi][0]=hash.chave; mat[posi][1]=hash.RRN; mat[posi][2]=hash.flag;
		salvaNoArquivo(mat,hashh);
   	}
	else //já tem conteúdo
	{	
		fseek(hashh,0,0);
		do //lendo arquivo hash para a matriz
		{	
			teste = lerNumero(num1,num2,num3,num4,num5,hashh);
			mat[cont][0]=teste;
			teste=0;
			teste = lerNumero(num1,num2,num3,num4,num5,hashh);
			mat[cont][1]=teste;
			teste=0;
			fread(&teste,sizeof(char),1,hashh);	mat[cont][2]=teste;
			cont++;		
		}while (cont<TAM_HASH);	
		
		fseek(out,0,SEEK_END); 
		temp3=ftell(out);
		fseek(out,temp3-1,0);
		fread(&temp_2,sizeof(int),1,out); //será lido a temp_2 para saber em que parte do arquivo de inserção está
		fseek(out,0,SEEK_END); //coloca o arquivo saida na ultima posição para inserção
		fseek(insere,(temp_2)*156,0); //locomove o arquivo insere com a temp_2 salva

		if((((int)temp_2)*156)>=tam_arq)  //se a temp_2 for maior que o tamanho arquivo insere significa que chegou ao fim
		{ 
			printf("\nArquivo insere.bin chegou ao fim\n");	 return;
		}
		
   		fread(&film,sizeof(film),1,insere); 
   		temp=(ftell(insere)+2)/156; //temp que será utilizada para se mover no arquivo insere
   		//coloca dados no registro reg
   		sprintf(reg,"##%s#%s#%s#%s#%s#%c",film.cliente,film.codfilme,film.nome,film.filme,film.genero,temp);
   		temp_2=strlen(reg); //temp2 armazenará o tamanho do registro reg que será usado para se locomover no arquivo saida
		
		strcat(strcpy(temp2,film.cliente),film.codfilme); //copia o codigo cliente e concatena com o condifo do fime
   		temp1=atoi(temp2);
        
   		hash.chave=temp1;
   		hash.RRN=temp3;//posi no arq principal
   		hash.flag=1; //1 existe    		//0 vazio vdd   		//-1 vazio falso
   	
   		posi=hash.chave%TAM_HASH;
   		printf("\n'%d'+'%d' (%d)\n",atoi(film.cliente),atoi(film.codfilme),temp1);
   		//duplicados
   		cont=0;
   		do
		{
			if(hash.chave==mat[cont][0])
			{
				printf("Duplicado\n");
				sprintf(reg,"#%c",temp);
   				fwrite(reg, sizeof(char), strlen(reg), out);
				fclose(out);  fclose(insere); fclose(hashh);
				return;
			}
			cont++;
		}while (cont<TAM_HASH);
   		
   		cond=0;
   		cont=0;
   		do 
   		{											//-1 assign unsegn 255
   			if (mat[posi][2]==0 || mat[posi][2]==255)
   				cond=1;
			else
			{
				posi=posi+1;
				if(posi>12)
					posi=0;
			}
			cont++;	
		}while(cond==0 && cont<TAM_HASH+1);
		
		if(cont>TAM_HASH)
		{
			printf("Lotado\n");
			sprintf(reg,"#%c",temp);
   			fwrite(reg, sizeof(char), strlen(reg), out);
   			fclose(out);  fclose(insere); fclose(hashh);
			return;
		}
		
		printf("Endereco: %d\n", hash.chave%TAM_HASH);
		if(cont-1!=0)
			printf("Colisao\nTentativa %d\n",cont-1);//se tentativa igual 0 n teve colisao
		printf("Chave inserida com sucesso\n");
   		mat[posi][0]=hash.chave;
   		mat[posi][1]=hash.RRN;
   		mat[posi][2]=hash.flag;
   		
   		fclose(hashh);
		if ((hashh = fopen("hash.bin","w+b")) == NULL)
			{
				printf("Nao foi possivel abrir o arquivo");	return ;
			}
		salvaNoArquivo(mat,hashh);
   		
   		sprintf(reg,"%c#%s#%s#%s#%s#%s#%c",temp_2,film.cliente,film.codfilme,film.nome,film.filme,film.genero,temp);
   		fwrite(reg, sizeof(char), strlen(reg), out); //registro é escrito no arquivo	
	}
	fclose(out);  fclose(insere); fclose(hashh);
}
void remocao(FILE *remove, FILE *hashh)
{
	int temp1=0,posi=0,cond=0,cont=0,mat[13][3],teste=0,valor=0,tam_reg=0;
	char temp2[100],reg2[160],reg[160],num1,num2,num3,num4,num5;
	
	FILE *aux;
	
	if ((aux = fopen("auxremove.bin","a+b")) == NULL)
	{
		printf("Nao foi possivel abrir o arquivo"); return ;
	}
	tam_reg=pega_registro(aux,reg);	//para ver se esta vazio
	valor=lendoAuxiliares(aux,tam_reg, remove);
	
	if(valor==1)
	{
		printf("\nFim de arquivo\n"); return;
	}	
	fread(&film.cliente,sizeof(film.cliente),1,remove); 
	fread(&film.codfilme,sizeof(film.codfilme),1,remove);
	strcat(strcpy(temp2,film.cliente),film.codfilme); //copia o codigo cliente e concatena com o condifo do fime
   	temp1=atoi(temp2); 
	posi=temp1%TAM_HASH;
	
	fseek(hashh,0,0);
	
		do 	//lendo a arvore hash dnv e passando pra matriz
		{
			teste = lerNumero(num1,num2,num3,num4,num5,hashh);
			mat[cont][0]=teste;
			teste = 0;
			teste = lerNumero(num1,num2,num3,num4,num5,hashh);
			mat[cont][1]=teste;
			teste = 0;
			fread(&teste,sizeof(char),1,hashh);	mat[cont][2]=teste;
			cont++;		
		}while (cont<TAM_HASH);	
		
	cont=0;	
	cond =procuraChave(temp1, mat, &posi, &cont);
	if (cond==1)
	{
			printf("\nEncontrou %d\n",mat[posi][0]);
			mat[posi][0]=0;  //por conta disso atencao na busca - fazer encontrou [0] e [2]!=0 e !=-1
			mat[posi][1]=0;
			mat[posi][2]=-1;	//se encontrou, entao eh vazio falso quando removemos
			printf("Chave removida com sucesso!\n");
	}
	else
	{
		printf("\nChave %d nao encontrada!\n", temp1);
		fclose(remove); fclose(hashh);fclose(aux);
		return;
	}
	
	fclose(hashh);
	if ((hashh = fopen("hash.bin","w+b")) == NULL)
		{
			printf("Nao foi possivel abrir o arquivo"); return ;
		}

	salvaNoArquivo(mat,hashh);
	fclose(remove); fclose(hashh);fclose(aux);
}

void buscar(FILE *out, FILE *busca, FILE *hashh)
{
    int temp1=0,posi=0,cond=0,cont=0,mat[13][3],teste=0,tam_reg=0,valor;
    char temp2[100],reg2[160],reg[160],num6;
    FILE *aux2;
    char num1,num2,num3,num4,num5;
    
    if ((aux2 = fopen("auxbusca.bin","a+b")) == NULL)
    {
        printf("Nao foi possivel abrir o arquivo"); return ;
    }
    
    tam_reg=pega_registro(aux2,reg);    //para ver se esta vazio
    valor=lendoAuxiliares(aux2,tam_reg, busca);
	if(valor==1)
	{
		printf("\nFim de arquivo\n"); return;
	}
    
    fread(&film.cliente,sizeof(film.cliente),1,busca); 
    fread(&film.codfilme,sizeof(film.codfilme),1,busca);    
    
    strcat(strcpy(temp2,film.cliente),film.codfilme); //copia o codigo cliente e concatena com o condifo do fime
    temp1=atoi(temp2); 
    posi=temp1%TAM_HASH;
    
    fseek(hashh,0,0);
    
        do     //lendo a arvore hash dnv e passando pra matriz
        {
            teste = lerNumero(num1,num2,num3,num4,num5,hashh);      
            mat[cont][0]=teste;
            teste = 0;
            teste = lerNumero(num1,num2,num3,num4,num5,hashh);       
            mat[cont][1]=teste;
            teste = 0;
            fread(&teste,sizeof(char),1,hashh);    mat[cont][2]=teste;
            cont++;       
        }while (cont<TAM_HASH);    
        
    cont=0;   
	cond =procuraChave(temp1, mat, &posi, &cont); 

	if (cond==1)
	{
		printf("\nChave %d encontrada, endereco %d, %d acesso\n", mat[posi][0],posi,cont+1);
        fseek(out,mat[posi][1],0);
        fread(&num6,sizeof(char),1,out);//posição para se locomover
        char filme[(int)num6];
        fread(&filme,sizeof(filme),1,out);
        exibirTela(filme);   
	}
	else
        printf("\nChave %d nao encontrada!\n", temp1);
    fclose(out); fclose(hashh);fclose(aux2); fclose(busca);
}
int main()
{
	int op=1;
	FILE *insere,*out, *hashh, *remove, *busca;

	while(op!=4)
	{
		printf("\n1-Insercao\n2-Remocao\n3-Busca\n4-Sair\nEscolha uma opcao:");
		scanf("%d",&op);
		printf("\n");
			switch(op)
				{
				case 1:
						if ((insere = fopen("insere.bin","r+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						  printf("\ninsere.bin carregado");
						if ((out = fopen("principal.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						printf("\nprincipal.bin carregado");
						
						if ((hashh = fopen("hash.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						printf("\nhashh.bin carregado");
						insercao(out,insere,hashh);	
						break;
				case 2:
					if ((remove = fopen("remove.bin","r+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						 printf("\nremove.bin carregado");
					if ((hashh = fopen("hash.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						printf("\nhashh.bin carregado");
						remocao(remove,hashh);	
					break;
				case 3:
					if ((busca = fopen("busca.bin","r+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						  printf("\nbuscar.bin carregado");
					if ((out = fopen("principal.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						printf("\nprincipal.bin carregado");
						
						if ((hashh = fopen("hash.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");	return 0;
						 }
						printf("\nhashh.bin carregado");
						buscar(out,busca,hashh);
					break;
				case 4:
						printf("\nSaindo...\n");break;
				default: printf("\nErro\n");
				}
	}
	fclose(out); fclose(insere); 
}
