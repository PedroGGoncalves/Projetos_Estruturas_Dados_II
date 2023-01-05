#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
struct estrutura
  	{
    	char nome[50], filme[50], genero[50];
  	}film; 	
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
void insercao(FILE  *out,FILE *insere)
{
    int cod_cliente,cod_filme,temp,temp_2=0,tam_reg,tam_arq,tam_arq2;
	char reg[160],reg1[160*4],reg2[160*4],codfilme;

	fseek(insere,0,SEEK_END); //coloca o arquivo no fim
	tam_arq=ftell(insere);   //pega tamnho do arquivo
	fseek(insere,0,0);	     //coloca o arquivo no início
	
	fseek(out,0,SEEK_END); 
	tam_arq2=ftell(out);   //pega tamnho do arquivo
	fseek(out,0,0);
	tam_reg=pega_registro(out,reg);
	if (tam_reg==0) //nada no arquivo
	{
		fread(&cod_cliente,sizeof(int),1,insere);
   		fread(&cod_filme,sizeof(int),1,insere);
   		fread(&film,sizeof(film),1,insere); 
   		temp=(ftell(insere)+2)/160; //temp que será utilizada para se mover no arquivo insere
   		//coloca dados no registro reg
   		sprintf(reg,"##%d#%d#%s#%s#%s#%c",cod_cliente,cod_filme,film.nome,film.filme,film.genero,temp);
   		temp_2=strlen(reg); //temp2 armazenará o tamanho do registro reg que será usado para se locomover no arquivo saida
   		sprintf(reg,"%c#%d#%d#%s#%s#%s#%c",temp_2,cod_cliente,cod_filme,film.nome,film.filme,film.genero,temp);
   		fwrite(reg, sizeof(char), strlen(reg), out); //registro é escrito no arquivo
	}
	else //já tem conteúdo
	{	
		fseek(out,1,0); //pula o primeiro registro
		fread(&codfilme,sizeof(char),1,out); //salva o registro que marca se foi removido ou não
	  	fseek(out,0,SEEK_END); //arquivo pro fim
		fseek(out,ftell(out)-1,0); //arquivo na penultima posição
		fread(&temp_2,sizeof(char),1,out); //será lido a temp_2 para saber em que parte do arquivo está
		while(((int) temp_2)=='-') //retorna enquando temp_2 for =  '-' 
	   	{
	   		fseek(out,-1,SEEK_CUR);
	   		fread(&temp_2,sizeof(char),1,out);
	   		fseek(out,-1,SEEK_CUR);
		}
		fseek(out,0,SEEK_END); //coloca o arquivo saida na ultima posição para inserção
		fseek(insere,((int)temp_2)*160,0); //locomove o arquivo insere com a temp_2 salva

		if((((int)temp_2)*160)>=tam_arq)  //se a temp_2 for maior que o tamanho arquivo insere significa que chegou ao fim
		{ 
			printf("\nArquivo insere.bin chegou ao fim\n"); return;
		}
		fread(&cod_cliente,sizeof(int),1,insere); //le dados para iserir no registro
   		fread(&cod_filme,sizeof(int),1,insere);
   		fread(&film,sizeof(film),1,insere); 
   		temp=(ftell(insere)+2)/160; //temp que será utilizada para se mover no arquivo insere
   		//coloca dados no registro reg
   		sprintf(reg,"##%d#%d#%s#%s#%s#%c",cod_cliente,cod_filme,film.nome,film.filme,film.genero,temp);
   		temp_2=strlen(reg);//temp2 armazenará o tamanho do registro reg que será usado para se locomover no arquivo saida
   		sprintf(reg,"%c#%d#%d#%s#%s#%s#%c",temp_2,cod_cliente,cod_filme,film.nome,film.filme,film.genero,temp);
   		int salva_pos = ftell(out);
   		int relembre=(int)temp_2,relembre2;
   		fseek(out,0,0);
   		char codfilme2;
   		int salvei;
		do
   		{
   			fread(&codfilme2,sizeof(char),1,out); //le o tamanho que será necessario se mover até o proximo
   			printf("%c",codfilme2);
		
			fread(&codfilme,sizeof(char),1,out);
			if(((int)codfilme)=='-')
			{
				do
				{
					fread(&codfilme,sizeof(char),1,out);
				}while(((int)codfilme)=='-');
				fread(&codfilme,sizeof(char),1,out);
			}
			if(((int)codfilme)=='*')
			{
				salvei = ftell(out)-1;
				
			}
			fseek(out,-2,SEEK_CUR);
			fseek(out,((int)codfilme2),SEEK_CUR);
			
		   }while ((int)codfilme!='*' && tam_arq2>=ftell(out));
		if (((int)codfilme)=='*') //se for encontrado o marcador de remoção
		{
			fseek(out,0,0); //coloca arquivo no inicio do arquivo
   			fread(&codfilme,sizeof(char),1
			   ,out); //le o tamanho que será necessario se mover até o proximo
			fseek(out,salvei-1,0);
			fread(&temp_2,sizeof(char),1,out);
	   		if(temp_2>((int)codfilme)) //se o tamanho da string for maior que a anterior - não haverá inserção nessa posição
	   		{
		   		fseek(out,temp_2,0); //locomove o tamanho da string atual
		   		fwrite(reg, sizeof(char), strlen(reg), out); //inserção ocorrera no fim do aqruivo///
		    }//descobrir pq escrevendo em cima
		    else //se ele for menor haverá inserção nessa posição
		    {
		   	 	fseek(out,0,0); //locomove para a posição codfilme 
				fread(reg1,salvei,1,out); //todos dados até então serão salvos
				fseek(out,salvei-1,0); //locomove para a posição codfilme
				fread(&codfilme,sizeof(char),1,out);
				fseek(out,(int)codfilme-1,SEEK_CUR);
				fread(reg2,sizeof(reg2),1,out);
				fclose(out);
				if ((out = fopen("saida.bin","w+b")) == NULL)//o arquivo é reaberto com o objetivo de reescrever ele do começo
				{
					printf("Nao foi possivel abrir o arquivo");	return ;
				}			 
			   	 //o que foi salvo no registrador reg será escrito//
			   	
			   	relembre2=(int)codfilme;
			   	int cont=1;
			   	
				if (temp==2) //quando teve um elemento inserido apenas e ele foi removido
				{
					fseek(out,-1,SEEK_CUR);
					fwrite(reg1, sizeof(char), strlen(reg), out);
					fseek(out,-1,SEEK_CUR); //volta 1 posição para substituir a temp_2//
			   		temp_2=temp; //temp_2 terá o valor da temp (valor que ajuda a se mover no arquivo insere)
			   		sprintf(reg1,"%c",'-'); //salva a temp_2 no fim do reg1
	   				fwrite(reg1, sizeof(char), 1, out); //reg1 é escrito no arquivo
	   				
				}
				else 
				{
					//fseek(out,-1,SEEK_CUR);
					fwrite(reg1, sizeof(char), strlen(reg1)-1, out);
					fwrite(reg, sizeof(char), strlen(reg), out);
					while(cont<(((int)codfilme)-relembre)) //preenche  '-' a qtd de vezes que é obtido em b
	   			{
	   				sprintf(reg,"%c",'-');
	   				fwrite(reg, sizeof(char), 1, out);
	   				cont++;
				}
					fwrite(reg2, sizeof(char), strlen(reg2), out);//salva isso no reg1 no equivalente ao espaço sobrando//
				   	fseek(out,-1,SEEK_CUR); //volta 1 posição para substituir a temp_2//
				   	temp_2=temp; //temp_2 terá o valor da temp (valor que ajuda a se mover no arquivo insere)
				   	sprintf(reg1,"%c",temp_2); //salva a temp_2 no fim do reg1
					fwrite(reg1, sizeof(char), 1, out); //reg1 é escrito no arquivo
				}
		    }
		}
		else //se não foi removida
			{
				fseek(out,0,salva_pos);
				fwrite(reg, sizeof(char), strlen(reg), out); //escrever no final do arquivo
	}
			}
	fclose(out); fclose(insere);
}
void remocao(FILE  *remove, FILE *out)
{
	int cod_cliente=0,cod_filme=0,temp1,temp2,tam_arq,tam_arqq,contt;
	char codcliente,codfilme,reg[160],reg1[300],reg2[300];

	fseek(out,0,SEEK_END);//coloca o arquivo no fim
	tam_arq=ftell(out);//pega tamnho do arquivo
	fseek(remove,0,SEEK_END);//coloca o arquivo no fim
	tam_arqq=ftell(remove);//pega tamnho do arquivo
	fseek(out,0,0); //coloca o arquivo no início
	fseek(remove,0,0);//coloca o arquivo no início

	do //faça enquanto o  arquivo remove não chegou no final
	{
		fread(&cod_cliente,sizeof(int),1,remove); //ler cod_cliente para remover
		fread(&cod_filme,sizeof(int),1,remove); //ler cod_filme para remover
		fseek(out,0,0); //coloca o arquivo no início
		do{ //faça enquanto o  arquivo saida não chegou no final
			fseek(out,2,SEEK_CUR); //pula duas posições 
			fread(&codcliente,sizeof(char),1,out); //pega o cod_cliente no arquivo out
			fseek(out,1,SEEK_CUR); //pula uma posição
			fread(&codfilme,sizeof(char),1,out); //pega o cod_filme no arquivo out
			temp1= codcliente -'0'; //transforma em int 
			temp2= codfilme -'0';   //transforma em int 
			if(cod_cliente==temp1 && cod_filme==temp2) //se cods a serem removidos forem iguais aos encontrados
			{
				fseek(out,-4,SEEK_CUR); //volta 4 posições para remover desde o começo
				fread(&codcliente,sizeof(char),1,out); //pega o valor onde fica o marcador de remoção
				if(codcliente=='*') //se for encontrado o marcador de remoção irá pular para a próxima string
				{
					fseek(out,-2,SEEK_CUR);//volta 2 posições
					fread(&codcliente,sizeof(char),1,out); //pega o tamanho da string a ser removida
					temp2= (int)codcliente ; //converte em int
					fseek(out,temp2-1,SEEK_CUR); //pula a string com o tamanho pego e volta 1 posição
					fread(&codcliente,sizeof(char),1,out); //ver se tem fragmento interno
					contt=0;
					if(codcliente=='-')
					{
						while(codcliente=='-')//lê enquanto for  '-' 
						{
							contt++;
							fread(&codcliente,sizeof(char),1,out);	
						}
					}
					temp2= temp2+contt ; //soma o tamnho da string mais a quantidade de '-' para ir para a próxima string
					fseek(out,temp2-1,SEEK_CUR); //pula a string e volta uma posição
				}
				else //se for encontrado o marcador de remoção
				{
					fseek(out,-1,SEEK_CUR); //volta uma posição para ficar na posição do marcador de remoção
					codcliente='*';
					sprintf(reg,"%c",codcliente); //será adicionado nela o marcador de remoção
   					fwrite(reg, sizeof(char), 1, out); //escreve no arquivo saida
					break; //se foi feito a remoção o loop deve ser parado
				}		
			}
			else //se cods a serem removidos não  forem iguais aos encontrados
			{
				fseek(out,-5,SEEK_CUR); //volta 5 posições
				fread(&codcliente,sizeof(char),1,out);//le  o codigo q irá conter o tamanho
				temp2= (int)codcliente ; //converte para int
				fseek(out,temp2-1,SEEK_CUR); //pula o tamanho e volta 1
				fread(&codcliente,sizeof(char),1,out); //ver se tem fragmento interno	
				contt=0;
				if(codcliente=='-')
				{
					while(codcliente=='-')//lê enquanto for  '-' 
					{
						contt++;
						fread(&codcliente,sizeof(char),1,out);	
					}
					temp2= temp2+contt ;//soma o tamnho da string mais a quantidade de '-' para ir para a próxima string
				}
				fseek(out,-1,SEEK_CUR); //volta 1 posição
			}	
		}while(ftell(out)<tam_arq); 
		if(cod_cliente==temp1 && cod_filme==temp2 &&codcliente=='*') //if para sair do loop
			break;	
	}while(ftell(remove)<tam_arqq);
	fclose(out);fclose(remove);
}
void compactacao(FILE *out)
{
	FILE *out2;
	int temp,tam_arq,b;
	char codcliente,codfilme,reg1[300];
	
	if ((out2 = fopen("saidatemp.bin","w+b")) == NULL)//arquivo temp
	{
		printf("Nao foi possivel abrir o arquivo"); return ;
	}
	fseek(out,0,SEEK_END);//coloca o arquivo no fim
	tam_arq=ftell(out);		//pega tamanho do arquivo			 
	fseek(out,0,0);//coloca o arquivo no inicio
	do //faça enquanto o  arquivo saida não chegou no final
	{
		fseek(out,1,SEEK_CUR); //pula uma posição
		fread(&codcliente,sizeof(char),1,out);//le o local que pode ter o marcador de remoção
		fseek(out,-2,SEEK_CUR); //volta duas posições
		fread(&codfilme,sizeof(char),1,out); //pega o valor a ser pulado
		temp= (int)codfilme ; //converte em inteiro
		if(temp<=4)//se chegou ao fim break
			break;
	//	printf("%d ",a2);
		if(codcliente=='*') //se tiver o marcador de remoção e nem fragmento interno
		{
			fseek(out,temp,SEEK_CUR); //pula essa string com o valor obtido
			fread(&codcliente,sizeof(char),1,out); //le o local que pode ter fragmento interno
			if(codcliente=='-')
			{
				while(codcliente=='-') //ler enquanto tiver '-'
				{	
					fread(&codcliente,sizeof(char),1,out);	
				}
				fseek(out,1,SEEK_CUR);	 //pula uma posição	para depois conseguir voltar para onde fica o valor a ser pulado
			}
			if(ftell(out)>=tam_arq) //se chegou ao fim break
				break;	
			fseek(out,-2,SEEK_CUR);//volta duas posições onde fica o valor a ser pulado
		}
		else if(codcliente=='-') //se tiver fragmento interno
			{
				while(codcliente=='-')//ler enquanto tiver '-'
				{
					fread(&codcliente,sizeof(char),1,out);				
				}
				fseek(out,-1,SEEK_CUR);	//volta uma posição
			}
		else //se não tiver o marcador de remoção e nem fragmento interno
		{
			fseek(out,-1,SEEK_CUR); //volta uma posição
			fread(&codfilme,sizeof(char),1,out); //le onde dica o tamanho
			temp= (int)codfilme ; //converte em inteiro
			b=ftell(out);//posição atual
			sprintf(reg1,"%c",codfilme);//será salvo o tamanho na string
			fwrite(reg1, sizeof(char), 1, out2);//será escrito no arquivo
			fread(&reg1,sizeof(reg1),1,out); //será lido o restante do arquivo
			fwrite(reg1, sizeof(char), temp-1, out2);//será escrito no arquivo
			fseek(out,b+(temp-1),0); //pular posição atual +tamanho a ser pulado	
		}
		if(ftell(out)>=tam_arq)//se chegou ao fim break
				break;
	}while(ftell(out)<tam_arq);
	fclose(out);
	if ((out = fopen("saida.bin","w+b")) == NULL)//o arquivo é reaberto com o objetivo de reescrever ele do começo
	{
		printf("Nao foi possivel abrir o arquivo");	return ;
	 }
	fseek(out2,0,0);//volta ao inicio
	fread(&reg1,sizeof(reg1),1,out2);//ele é escrito no reg1
	fseek(out2,0,SEEK_END);//volta ao fim
	
	fwrite(reg1, sizeof(char), ftell(out2), out);//ele é passado para o arquivo saida
	fclose(out2);fclose(out);
}
int main()
{
	int op=1;
	FILE *insere,*remove,*out;

	while(op!=4)
	{
		printf("\n1- Insercao\n2-Remocao\n3-Compactacao\n4-Sair\nEscolha uma opcao:");
		scanf("%d",&op);
		printf("\n");
			switch(op)
				{
				case 1:
						if ((insere = fopen("insere.bin","r+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						  printf("\ninsere.bin carregado");
						if ((out = fopen("saida.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nsaida.bin carregado");
						insercao(out,insere);	
						break;
				case 2:
						 if ((remove = fopen("remove.bin","r+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						 printf("\nremove.bin carregado\n");
						 if ((out = fopen("saida.bin","r+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nsaida.bin carregado");
						remocao(remove,out);	
						break;
				case 3:
					if ((out = fopen("saida.bin","r+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						compactacao(out);
						break;
				case 4:
						printf("\nSaindo...\n");break;
				default: printf("\nErro\n");
				}
	}
	fclose(out); fclose(insere); fclose(remove);
}
