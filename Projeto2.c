#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
///primario codcod pos
//sec 1 nome\n space pos
//sec 2 codcod space sinal/pos
struct estrutura
  	{
    	char nome[50], filme[50], genero[50];
  	}film; 
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
void insercao(FILE  *out,FILE *insere,FILE *out2,FILE *out3, FILE *out4)
{
    int cod_cliente,cod_filme,temp,temp_2=0,tam_reg,tam_arq,tam_arqq,conta_digito=0,temp3,valor=1,valor2=1,tamanho;
	char reg[160],reg1[160*4],reg2[160*4],codfilme[50],codnum,vet[1000][1000];
	int i,j,vet_int[1000],vet2_int[1000];
	FILE *ordena1; FILE *ordena2;
	
	fseek(insere,0,SEEK_END); //coloca o arquivo no fim
	tam_arq=ftell(insere);   //pega tamnho do arquivo
	fseek(insere,0,0);	     //coloca o arquivo no início
	
	tam_reg=pega_registro(out,reg);
	if (tam_reg==0) //nada no arquivo
	{
		fread(&cod_cliente,sizeof(int),1,insere);
   		fread(&cod_filme,sizeof(int),1,insere);
   		fread(&film,sizeof(film),1,insere); 
   		temp=(ftell(insere)+2)/160; 
   		sprintf(reg,"##%d#%d#%s#%s#%s#%c",cod_cliente,cod_filme,film.nome,film.filme,film.genero,temp);
   		temp_2=strlen(reg); //temp2 armazenará o tamanho do registro reg que será usado para se locomover no arquivo saida
   		sprintf(reg,"%c#%d#%d#%s#%s#%s#%c",temp_2,cod_cliente,cod_filme,film.nome,film.filme,film.genero,temp);
   		fwrite(reg, sizeof(char), strlen(reg), out); //registro é escrito no arquivo
   		
   		sprintf(reg1,"#%d%d ",cod_cliente,cod_filme);
   		fwrite(reg1, sizeof(char), strlen(reg1), out2);
		temp3=0;
		temp=0;
		do //qtos digitos tem
		{
			temp=temp/10; conta_digito++;
		}while(temp>=1);
		do
		{
			sprintf(reg1,"%d",temp3);
			fwrite(reg1, sizeof(char), strlen(reg1), out2);
			conta_digito++;
		}while(conta_digito<5);//ira armazenar numeros de 5 digitos
		//posições de até 5 digitos
		sprintf(reg1,"%d",temp3);
   	
   		fwrite(reg1, sizeof(char), strlen(reg1), out2); //registro é escrito no arquivo	
		fseek(out4,0,0);
		sprintf(reg1,"%s\n %c",film.filme,1); 
   		fwrite(reg1, sizeof(char), strlen(reg1), out3);
		sprintf(reg1,"%d%d %c",cod_cliente,cod_filme,'-');
		fwrite(reg1, sizeof(char), strlen(reg1), out4); //registro é escrito no arquivo		
	}
	else //já tem conteúdo
	{	
		fseek(out,0,SEEK_END); 
		temp3=ftell(out);
		fseek(out,ftell(out)-1,0); //arquivo na penultima posição
		fread(&temp_2,sizeof(char),1,out); //será lido a temp_2 para saber em que parte do arquivo está
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
		fwrite(reg, sizeof(char), strlen(reg), out); //escrever no final do arquivo
		
		sprintf(reg1,"#%d%d ",cod_cliente,cod_filme);
   		fwrite(reg1, sizeof(char), strlen(reg1), out2);
		temp=temp3;
		do
		{
			temp=temp/10; conta_digito++;
		}while(temp>=1); //conta digitos
		do
		{
			sprintf(reg1,"%d",0);
			fwrite(reg1, sizeof(char), strlen(reg1), out2);
			conta_digito++;
		}while(conta_digito<5); //ira armazenar numeros de 5 digitos
		//posições de até 5 digitos
		sprintf(reg1,"%d",temp3);
   		fwrite(reg1, sizeof(char), strlen(reg1), out2);  
   		fseek(out3,0,SEEK_END); //coloca o arquivo no fim
		tam_arqq=ftell(out3);   //pega tamnho do arquivo
		fseek(out3,0,0);	     //coloca o arquivo no início
   		
   		while(ftell(out3)<=tam_arqq)//enquanto n chegar no fim do arquivo
   		{
			fgets(codfilme,sizeof(codfilme),out3);
			
			if (strncmp(codfilme,film.filme,strlen(film.filme)) == 0) //ve se ta tem algum filme com esse codigo na lista
      		{
         		temp=ftell(out3);
         		fseek(out3,1,SEEK_CUR);
         		
         		fread(&codnum,sizeof(char),1,out3);
         		valor=0;
         	
         		fseek(out3,0,0);
         		fread(reg1,temp,1,out3);
         		fseek(out3,2,SEEK_CUR);
         		tamanho=tam_arqq-ftell(out3);//
         		if(ftell(out3)<tam_arqq) //se ele n tiver no fim do arquivo
         		{
         			fread(reg2,sizeof(reg2),1,out3); //sera salvo o q vem dps
         			//sera inserido no meio do arquivo
         			//para substituir valor
        			valor2=0;
				}
         		fclose(out3);
				if ((out3 = fopen("secundaria1.bin","w+b")) == NULL)//o arquivo é reaberto com o objetivo de reescrever ele do começo
				{
					printf("Nao foi possivel abrir o arquivo");	return ;
				}	
				fwrite(reg1, sizeof(char), temp, out3); 
         		valor=0;
         		break;
      		}	
			fseek(out3,2,SEEK_CUR);
		}
		fseek(out3,0,SEEK_END);
		if(valor==1) //n tinha filme com aquele cod
		{
			fseek(out4,0,SEEK_END);
			sprintf(reg1,"%s\n %c",film.filme,ftell(out4));
   			fwrite(reg1, sizeof(char), strlen(reg1), out3); //registro é escrito no arquivo	
   			sprintf(reg1,"%d%d %c",cod_cliente,cod_filme,'-');
			fwrite(reg1, sizeof(char), strlen(reg1), out4);
		}
		else if (valor==0)
		{
			fseek(out4,0,SEEK_END);
			sprintf(reg1," %c",ftell(out4));//novo valor da pos 
			fwrite(reg1, sizeof(char), strlen(reg1), out3); 
			if(valor2==0) //escreve restp dp aqruivo
				fwrite(reg2, sizeof(char), tamanho, out3);
			sprintf(reg1,"%d%d %c",cod_cliente,cod_filme,codnum);
			fwrite(reg1, sizeof(char), strlen(reg1), out4);
		}
	}
	/////////////ordenação/////////////
	fseek(out3,0,SEEK_END); //coloca o arquivo no fim
	tam_arqq=ftell(out3); 
	fseek(out3,0,0);
	conta_digito=0;
	while(ftell(out3)<tam_arqq){
		fgets(codfilme,sizeof(codfilme),out3);
		fseek(out3,2,SEEK_CUR);
		strcpy(vet[conta_digito],codfilme);
		conta_digito++;
	}
	for (j=0; j<conta_digito; j++)
	{
        for (i=j+1; i<conta_digito; i++) 
		{
            if (strcmp(vet[j], vet[i]) > 0) 
			{
                char temp[100];
                strcpy(temp,vet[j]);
                strcpy(vet[j],vet[i]);
                strcpy(vet[i],temp);
            }
        }
    }
	if ((ordena1 = fopen("ordena1.bin","w+b")) == NULL)
	{
		printf("Nao foi possivel abrir o arquivo"); return ;
	}
	fseek(out3,0,0);
	
	for(i=0;i<conta_digito;i++)
	{
		while(ftell(out3)<tam_arqq)
		{
			fgets(codfilme,sizeof(codfilme),out3);
			fseek(out3,1,SEEK_CUR);
			fread(&codnum,sizeof(char),1,out3);
		   	
		   	if(strncmp(codfilme,vet[i],strlen(codfilme)) == 0)
		   	{
		   		sprintf(reg1,"%s %c",codfilme,codnum);
	   			fwrite(reg1, sizeof(char), strlen(reg1), ordena1);
	   			break;
			}
		}
		fseek(out3,0,0);
	}
	fseek(ordena1,0,0);//volta ao inicio
	fread(&reg1,sizeof(reg1),1,ordena1);//ele é escrito no reg1
	fseek(ordena1,0,SEEK_END);//volta ao fim
	fclose(out3);
	if ((out3 = fopen("secundaria1.bin","w+b")) == NULL)//o arquivo é reaberto com o objetivo de reescrever ele do começo
	{
		printf("Nao foi possivel abrir o arquivo");	return ;
	}
	fwrite(reg1, sizeof(char), ftell(ordena1), out3);
	
	/////////////ordenação/////////////
	conta_digito=0;
	fseek(out2,0,SEEK_END); //coloca o arquivo no fim
	tam_arqq=ftell(out2); 
	fseek(out2,0,0);
	
	while(ftell(out2)<tam_arqq)
	{
		fseek(out2,1,SEEK_CUR);
		fread(&vet_int[conta_digito],sizeof(char),1,out2); 
		fread(&vet2_int[conta_digito],sizeof(char),1,out2); 
		vet_int[conta_digito]=vet_int[conta_digito]-'0';
		vet2_int[conta_digito]=vet2_int[conta_digito]-'0';
		fseek(out2,6,SEEK_CUR);
		conta_digito++;
	}
	for (j=0; j<conta_digito; j++)
	{
        for (i=j+1; i<conta_digito; i++) 
		{
            if ( vet_int[j] > vet_int[i] || (vet_int[j] == vet_int[i] && vet2_int[j] > vet2_int[i] )) 
			{
                temp = vet_int[j];
        		vet_int[j] = vet_int[i];
        		vet_int[i] = temp;
        			
        		temp = vet2_int[j];
        		vet2_int[j] = vet2_int[i];
        		vet2_int[i] = temp;
           }
        }
    }
	if ((ordena2 = fopen("ordena2.bin","w+b")) == NULL)
	{
		printf("Nao foi possivel abrir o arquivo"); return ;
	}
	fseek(out2,0,0);
	
	for(i=0;i<conta_digito;i++)
	{
		while(ftell(out2)<tam_arqq)
		{
			fseek(out2,1,SEEK_CUR);
			fread(&cod_cliente,sizeof(char),1,out2); 
   			fread(&cod_filme,sizeof(char),1,out2);
			cod_cliente=cod_cliente-'0';
			cod_filme=cod_filme-'0';

		   	if((cod_cliente==vet_int[i])&&(cod_filme==vet2_int[i]))
		   	{
		   		sprintf(reg1,"#%d%d ",cod_cliente,cod_filme);
	   			fwrite(reg1, sizeof(char), strlen(reg1), ordena2);
	   			fseek(out2,1,SEEK_CUR);
	   			
				fread(&codnum,sizeof(char),1,out2);
				sprintf(reg1,"%d",codnum-'0');
	   			fwrite(reg1, sizeof(char), strlen(reg1), ordena2);
	   			fread(&codnum,sizeof(char),1,out2);
				sprintf(reg1,"%d",codnum-'0');
	   			fwrite(reg1, sizeof(char), strlen(reg1), ordena2);
	   			fread(&codnum,sizeof(char),1,out2);
				sprintf(reg1,"%d",codnum-'0');
	   			fwrite(reg1, sizeof(char), strlen(reg1), ordena2);
	   			fread(&codnum,sizeof(char),1,out2);
				sprintf(reg1,"%d",codnum-'0');
	   			fwrite(reg1, sizeof(char), strlen(reg1), ordena2);
	   			fread(&codnum,sizeof(char),1,out2);
				sprintf(reg1,"%d",codnum-'0');
	   			fwrite(reg1, sizeof(char), strlen(reg1), ordena2);
	   			break;
			}
			else
				fseek(out2,6,SEEK_CUR);
		}
		fseek(out2,0,0);
	}
	fclose(out2);
	fseek(ordena2,0,0);//volta ao inicio
	fread(&reg1,sizeof(reg1),1,ordena2);//ele é escrito no reg1
	fseek(ordena2,0,SEEK_END);//volta ao fim
	fclose(out2);
	if ((out2 = fopen("primaria.bin","w+b")) == NULL)//o arquivo é reaberto com o objetivo de reescrever ele do começo
	{
		printf("Nao foi possivel abrir o arquivo");	return ;
	}
	fwrite(reg1, sizeof(char), ftell(ordena2), out2);
	
	fclose(ordena1); remove("ordena1.bin"); fclose(ordena2); remove("ordena2.bin");    
	/////////////fim ordenação/////////////
	fclose(out); fclose(out2); fclose(out3); fclose(out4); fclose(insere);
}
void pesquisaPrimaria (FILE *out,FILE *out2, FILE *consultei) 
{
	FILE *pesquisaP;
	int cod_cliente,cod_filme,cond=0,cont=0,ler_cont,tam,tam2,tam_consultei,numero;//melhorar nomes e ver se uso todas
	char cod_cliente2,cod_filme2,reg[160],num1,num2,num3,num4,num5,*pch;
	
	if ((pesquisaP = fopen("busca_p.bin","r+b")) == NULL)
	{
		printf("Nao foi possivel abrir o arquivo");return ;
	}
		
	fseek(pesquisaP,0,SEEK_END); //fim arquivo
	fseek(out2,0,SEEK_END);
	fseek(consultei,0,SEEK_END);
	
	tam = ftell(pesquisaP); //tamanho dos arquivos
	tam2 = ftell(out2);
	tam_consultei=ftell(consultei);
	
	fseek(pesquisaP,0,0); //inicio arquivo
	fseek(out2,0,0);
	fseek(consultei,0,0);
	
	do
	{
		cont++;
		fread(&cod_cliente,sizeof(int),1,pesquisaP); //ver qual deve ser consultado
   		fread(&cod_filme,sizeof(int),1,pesquisaP);
		fseek(consultei,0,0);
		do
		{
			fread(&ler_cont,sizeof(char),1,consultei); //conferir se ja foi consultado
			if(ler_cont-'0'==cont)
			{
				cond=1;
				break;
			}
		}while(ftell(consultei)<tam_consultei); //fim arquivo consultei
		if (cond!=1)
		{
			fseek(consultei,0,0);
	   		do
	   		{
	   			fseek(out2,1,SEEK_CUR);
	   			fread(&cod_cliente2,sizeof(char),1,out2); 
	   			fread(&cod_filme2,sizeof(char),1,out2);
	   			fseek(out2,1,SEEK_CUR);
	   			
	   			if(cod_cliente==cod_cliente2-'0' && cod_filme==cod_filme2-'0') //se encontrei filme no arquivo
	   			{
	   				printf("\nCod cliente:%d\nCod filme:%d\n",cod_cliente,cod_filme);
	   				sprintf(reg,"%d",cont);
					fwrite(reg, sizeof(char), strlen(reg), consultei);
					
					fread(&num1,sizeof(char),1,out2); //le dados para iserir no registro
					fread(&num2,sizeof(char),1,out2); //le dados para iserir no registro
					fread(&num3,sizeof(char),1,out2); //le dados para iserir no registro
					fread(&num4,sizeof(char),1,out2); //le dados para iserir no registro
					fread(&num5,sizeof(char),1,out2); //le dados para iserir no registro
					
					numero=(num1-'0')*10000+(num2-'0')*1000+(num3-'0')*100+(num4-'0')*10+(num5-'0');
					
					fread(&num5,sizeof(char),1,out);//posição para se locomover
					fseek(out,numero+5,0); 
			
					char filme[(int)num5];
					
					fread(&filme,sizeof(filme),1,out);
					pch = identifica_campo(filme,1);
					printf("Nome:%s\n",pch);
					pch = identifica_campo(NULL,1);
					printf("Nome Filme:%s\n",pch);
					pch = identifica_campo(NULL,1);
					printf("Genero:%s\n",pch);
					pch = identifica_campo(NULL,1);
			
					cond=1;
				}
				fseek(out2,5,SEEK_CUR);
			} while(ftell(out2)<tam2 && cond==0);//enquanto n chegar no fim
	   		fseek(out2,0,0);
			  // /*
			  if (cond!=1)
			  {
			  	sprintf(reg,"%d",cont);
				fwrite(reg, sizeof(char), strlen(reg), consultei);
				cond=1;
			  }	 //se tirar isso fica como antes, se deixar ele vai um por um
			  // */
		}
		else
			cond=0;	
	} while(ftell(pesquisaP)<tam && cond==0);//enquanto n chegar no fim
	fclose(out); fclose(out2); fclose(consultei);
}
void pesquisaSecundaria (FILE *out,FILE *out2,FILE *out3,FILE *out4, FILE *consultei2) //fazer mais laguns testes, dps fazer prints correspondentes (terei q consultar aquivo principal atraves dos indices encontrados
{
	FILE *pesquisaS;
	int cod_cliente,cod_filme,cond=0,cont=0,tam,tam2,tam3,tam_consultei,teste,teste2,numero;
	char cod_cliente2[50],reg[160],codfilme[50],num,cod_cliente3,cod_filme3,num1,num2,num3,num4,num5,*pch;
	if ((pesquisaS = fopen("busca_s.bin","r+b")) == NULL)
		{
			printf("Nao foi possivel abrir o arquivo"); return ;
		}
	fseek(pesquisaS,0,SEEK_END); //fim
	fseek(out2,0,SEEK_END);
	fseek(out3,0,SEEK_END);
	fseek(consultei2,0,SEEK_END);
	
	tam = ftell(pesquisaS); //tamanho
	tam2 = ftell(out3);
	tam3 = ftell(out2);
	tam_consultei=ftell(consultei2);
	
	fseek(pesquisaS,0,0); //inicio
	fseek(out2,0,0);
	fseek(out3,0,0);
	fseek(consultei2,0,0);
	
	do
	{
		cont++;
		fread(&cod_cliente2,sizeof(cod_cliente2),1,pesquisaS); 
		
		fseek(consultei2,0,0);
		do
		{
			fread(&num,sizeof(char),1,consultei2);//se ja foi consultado
			if(num-'0'==cont)
			{
				cond=1;
				break;
			}
		}while(ftell(consultei2)<tam_consultei); //fim
		if (cond!=1)
		{
			fseek(consultei2,0,0);
	   		do
	   		{
	   			fgets(codfilme,sizeof(codfilme),out3); 
	   			if(strncmp(codfilme,cod_cliente2,strlen(cod_cliente2)) == 0) //achei filme com msm nome
	   				{
	   					fseek(out3,1,SEEK_CUR);
	   					fread(&num,sizeof(char),1,out3);
	   					if((int)num==1) //se pos for igual a 1, inicio do arquivo
	   						fseek(out4,0,0);
	   					else
							fseek(out4,(int)num,0);
	   					
	   					sprintf(reg,"%d",cont);
						fwrite(reg, sizeof(char), strlen(reg), consultei2);
	   					do
	   					{
	   						fread(&num,sizeof(char),1,out4);
	   						teste2=num-'0';
	   						fread(&num,sizeof(char),1,out4);
	   						teste=num-'0';
							fseek(out2,0,0);
							do
					   		{
					   			fseek(out2,1,SEEK_CUR);
					   			
					   			fread(&cod_cliente3,sizeof(char),1,out2); //le dados para iserir no registro
					   			fread(&cod_filme3,sizeof(char),1,out2);
					   	
					   			fseek(out2,1,SEEK_CUR);
					   			if(teste2==cod_cliente3-'0' && teste==cod_filme3-'0') //achei os cods iguais
					   			{
					   				printf("\nCod cliente:%d\nCod filme:%d\n",teste2,teste);
									
									fread(&num1,sizeof(char),1,out2); //le dados para iserir no registro
									fread(&num2,sizeof(char),1,out2); //le dados para iserir no registro
									fread(&num3,sizeof(char),1,out2); //le dados para iserir no registro
									fread(&num4,sizeof(char),1,out2); //le dados para iserir no registro
									fread(&num5,sizeof(char),1,out2); //le dados para iserir no registro
									numero=(num1-'0')*10000+(num2-'0')*1000+(num3-'0')*100+(num4-'0')*10+(num5-'0');
									fread(&num5,sizeof(char),1,out);//mudar isso no primaria pois pode dar errado
							
									fseek(out,numero+4,0);
									fread(&num5,sizeof(char),1,out);
									
									char filme[(int)num5];//att isso
									fread(&filme,sizeof(filme),1,out);
									pch = identifica_campo(filme,1);
									printf("Nome:%s\n",pch);
									pch = identifica_campo(NULL,1);
									printf("Nome Filme:%s\n",pch);
									pch = identifica_campo(NULL,1);
									printf("Genero:%s\n\n",pch);
									pch = identifica_campo(NULL,1);
									cond=1;
								   }
								fseek(out2,5,SEEK_CUR);
								
							} while(ftell(out2)<tam3 && cond!=1);//fim arquivo
							
							cond=0;
							fseek(out4,1,SEEK_CUR);
							fread(&num,sizeof(char),1,out4);
							teste=(int)num;
							if(teste==1)
								teste=0;
							fseek(out4,teste,0);
						}while(num!='-'); //fazer enquanto o num n tiver a flag de fim (n aponta para mais ngm)
						cond=1;
					}
	   				fseek(out3,2,SEEK_CUR);
			} while(ftell(out3)<tam2 && cond!=1);//dim arquivo
   		fseek(out3,0,0);	
   		// /*
			  if (cond!=1)
			  {
			  	sprintf(reg,"%d",cont);
				fwrite(reg, sizeof(char), strlen(reg), consultei2);
				cond=1;
			  }	 //se tirar isso fica como antes, se deixar ele vai um por um
			  // */
		}
		else
			cond=0;
	
	} while(ftell(pesquisaS)<tam && cond==0); //fim arquivo
	fclose(out); fclose(out2); fclose(out3);fclose(out4);fclose(consultei2);
}
int main()
{
	int op=1;
	FILE *insere,*remove,*out,*out2,*out3,*out4,*consultei,*consultei2;

	while(op!=4)
	{
		printf("\n1- Insercao\n2-Procurar por chave primaria\n3-Procurar por chave secundaria\n4-Sair\nEscolha uma opcao:");
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
						if ((out = fopen("principal.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nprincipal.bin carregado");
						if ((out2 = fopen("primaria.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nprimaria.bin carregado");
						if ((out3 = fopen("secundaria1.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						 printf("\nsecundaria1.bin carregado");
						 if ((out4 = fopen("secundaria2.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nsecundaria2.bin carregado\n");
						insercao(out,insere,out2,out3,out4);	
						break;
				case 2:
						if ((out = fopen("principal.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nprincipal.bin carregado");
						if ((out2 = fopen("primaria.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nprimaria.bin carregado");
						if ((consultei = fopen("consultei.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nconsultei.bin carregado\n");
						pesquisaPrimaria(out,out2,consultei);
						break;
				case 3:
						if ((out = fopen("principal.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nprincipal.bin carregado");
						if ((out2 = fopen("primaria.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nprimaria.bin carregado");
						if ((out3 = fopen("secundaria1.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						 printf("\nsecundaria1.bin carregado");
						 if ((out4 = fopen("secundaria2.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						 printf("\nsecundaria2.bin carregado");
						 if ((consultei2 = fopen("consultei2.bin","a+b")) == NULL)
						 {
							printf("Nao foi possivel abrir o arquivo");
							getche();
							return 0;
						 }
						printf("\nconsultei2.bin carregado\n");
						pesquisaSecundaria(out,out2,out3,out4,consultei2);
						break;
				case 4:
						printf("\nSaindo...\n");break;
				default: printf("\nErro\n");
				}
	}
	fclose(out); fclose(out2); fclose(out3); fclose(out);fclose(consultei); fclose(consultei2); fclose(insere); 
}
