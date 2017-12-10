#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct knot {
	char exp[200];
	struct knot* left;
	struct knot* right;
} knot;

int e (int a, int b);
int ou (int a, int b);
int imp (int a, int b);
int neg (int a);
int val (char exp[]);
int meio (char exp[]);

int valsalva (char exp[], knot* root);
knot* insert (knot* root, char exp[]);
void emordem(knot *q, FILE* fp);
void emordemval(knot *q, FILE* fp);
void preord(knot *root);
int* vetxyzw (int* vet, char exp[]);
void emordemln (knot *q, FILE* fp);

int x=0, y=0, z=0, t=0;
int tau=1;
int sat=0;

int main () {
	FILE *fp;
	FILE *fi;
	fi = fopen("Expressoes.in", "r");
	fp = fopen("Expressoes.out", "w");
	int vet[4] = {0,0,0,0};
	char exp[200];
	int casos=0;
	fscanf(fi, "%d", &casos);
	//scanf("%d", &casos);
	int c=0;
	for (c=1; c<=casos; c++){
	vet[0]=0;
	vet[1]=0;
	vet[2]=0;
	vet[3]=0;
	fprintf (fp, "Tabela #%d\n", c);
	fflush(stdin);
	fscanf(fi, "%s", exp);
	//gets(exp);
	vetxyzw (vet, exp);
	knot *n = (knot*) malloc(sizeof(knot));
	strcpy (n->exp,exp);
	n->right = n->left = NULL;
	valsalva (exp, n);
	fprintf(fp, "-");
	emordemln(n, fp);
	fprintf(fp, "\n");
	if (vet[0]) fprintf (fp, "|x");
	if (vet[1]) fprintf (fp, "|y");
	if (vet[2]) fprintf (fp, "|z");
	if (vet[3]) fprintf (fp, "|t");
	emordem (n, fp);
	fprintf(fp, "|");
	fprintf(fp, "\n");
	for (x=0; x<=vet[0]; ++x){
		for (y=0; y<=vet[1]; ++y){
			for (z=0; z<=vet[2]; ++z){
				for (t=0; t<=vet[3]; ++t){
					fprintf(fp, "-");
					emordemln(n, fp);
					fprintf(fp, "\n");
					emordemval(n, fp);
					fprintf(fp, "|");
					fprintf(fp, "\n");
					if (val(exp)==1) sat=1;
					else tau=0;
				}
			}
		}
	}
	fprintf(fp, "-");
	emordemln(n, fp);
	fprintf(fp, "\n");
	if (sat==1) fprintf (fp, "satísfativel e ");
	else fprintf (fp, "insatisfatível e ");
	if (tau==1) fprintf (fp, "tautologia");
	else fprintf (fp, "refutável");
	fprintf (fp, "\n");
	fprintf (fp, "\n");

	x=0, y=0, z=0, t=0;
	tau=1;
	sat=0;
	
	//emordemval (n);
	//fprintf ("final: %d", val (exp));
	//fprintf("meio: %d", meio(exp));
	free(n);
	}
	fclose(fp);
	fclose(fi);
	return 0;
}
int* vetxyzw (int* vet, char exp[]){
	int i=0;
	for (i=0; i<strlen(exp); i++){
		if (exp[i]=='x') vet[0]=1;
		if (exp[i]=='y') vet[1]=1;
		if (exp[i]=='z') vet[2]=1;
		if (exp[i]=='t') vet[3]=1;	
	}
}
void emordemln (knot *q, FILE* fp){
int s;
	if(q != NULL) {
		emordemln(q->left, fp);
		for (s=0; s<(strlen(q->exp)+1); s++){
			fprintf(fp, "-");
		}
		emordemln(q->right, fp);
	}
}
void emordem(knot *q, FILE* fp) {
	if(q != NULL) {
		emordem(q->left, fp);
		if (strlen(q->exp)!=1){
			fprintf(fp, "|%s",q->exp);
		}
		emordem(q->right, fp);
	}
}
void emordemval(knot *q, FILE* fp) {
	int s;
	if(q != NULL) {
		emordemval(q->left, fp);
		fprintf(fp, "|");
		for (s=0; s<strlen(q->exp)-1; s++){
			fprintf(fp, " ");
		}
		fprintf(fp, "%d",val(q->exp));
		emordemval(q->right, fp);
	}
}


knot* insert (knot* root, char exp[]){
	if (root==NULL) {
		knot *novo = (knot*) malloc(sizeof(knot));
			if (novo==NULL) exit(1);
		strcpy(novo->exp, exp);
		novo->right = novo->left = NULL;
		return novo;
	}//((x+y).(y+z))
	else if (strlen(exp)<strlen(root->exp)){
		root->left = insert (root->left, exp);
	}
	else if (strlen(exp)>strlen(root->exp)){
		root->right = insert (root->right, exp);
	}
	else if (strcmp(exp,root->exp)<0) {
			root->left = insert (root->left, exp);
		}
	else if (strcmp(exp,root->exp)>0){
			root->right = insert (root->right, exp);
	}
	return (root);
	//emordem (root);//
}

int valsalva (char exp[], knot* root){
	int tam = strlen(exp);
	char exp2[200], exp3[200]; int i=0;
	//bases recursivas
	//int x=0, y=0, z=0, t=0;//funcao para variar valores
	if (strcmp (exp, "x")==0) return x;
	if (strcmp (exp, "y")==0) return y;
	if (strcmp (exp, "z")==0) return z;
	if (strcmp (exp, "t")==0) return t;
	//casos recursivos
	char op = exp[meio(exp)];
	switch(op){
		//negacao
		case '-':
			for (i=0; i<tam-1 ;i++) exp2[i]=exp[i+2];
			exp2[i-2]='\0';
			//salva exp2
			insert (root, exp2);
			//fprintf ("%s\n", exp2);
			//fprintf("%d\n", neg(val(exp2)));	
			return neg(valsalva(exp2, root));
			break;
		//e
		case '.':
			for (i=0;i<meio(exp);i++) exp2[i]=exp[i+1];
			exp2[i-1]='\0';
			//salva exp2
			insert (root, exp2);
			//fprintf("%s\n", exp2);
			for (i=0;i<(tam-meio(exp));i++) exp3[i]=exp[i+meio(exp)+1];
			exp3[i-2]='\0';
			//salva exp3
			insert (root, exp3);
			//fprintf("%s\n", exp3);
			return e(valsalva(exp2, root),valsalva(exp3, root));
			break;
		//ou
		case '+':
			for (i=0;i<meio(exp);i++) exp2[i]=exp[i+1];
			exp2[i-1]='\0';
			//salva exp2
			insert (root, exp2);
			//fprintf("%s\n", exp2);
			for (i=0;i<(tam-meio(exp));i++) exp3[i]=exp[i+meio(exp)+1];
			exp3[i-2]='\0';
			//salva exp3
			insert (root, exp3);
			//fprintf("%s\n", exp3);
			return ou(valsalva(exp2, root),valsalva(exp3, root));
			break;	
		//imp
		case '>':
			for (i=0;i<meio(exp);i++) exp2[i]=exp[i+1];
			exp2[i-1]='\0';
			//salva exp2
			insert (root, exp2);
			//fprintf("%s\n", exp2);
			for (i=0;i<(tam-meio(exp));i++) exp3[i]=exp[i+meio(exp)+1];
			exp3[i-2]='\0';
			//salva exp3
			insert (root, exp3);
			//fprintf("%s\n", exp3);
			return imp(valsalva(exp2, root),valsalva(exp3, root));
			break;
		//extra parenteses
	}
}

int val (char exp[]){
	int tam = strlen(exp);
	char exp2[200], exp3[200]; int i=0;
	//bases recursivas
	//int x=0, y=0, z=0, t=0;//funcao para variar valores
	if (strcmp (exp, "x")==0) return x;
	if (strcmp (exp, "y")==0) return y;
	if (strcmp (exp, "z")==0) return z;
	if (strcmp (exp, "t")==0) return t;
	//casos recursivos
	char op = exp[meio(exp)];
	switch(op){
		//negacao
		case '-':
			for (i=0; i<tam-1 ;i++) exp2[i]=exp[i+2];
			exp2[i-2]='\0';
			//salva exp2
			//fprintf ("%s", exp2);
			//fprintf("%d\n", neg(val(exp2)));	
			return neg(val(exp2));
			break;
		//e
		case '.':
			for (i=0;i<meio(exp);i++) exp2[i]=exp[i+1];
			exp2[i-1]='\0';
			//salva exp2
			//fprintf("%s", exp2);
			for (i=0;i<(tam-meio(exp));i++) exp3[i]=exp[i+meio(exp)+1];
			exp3[i-2]='\0';
			//salva exp3
			//fprintf("%s", exp3);
			return e(val(exp2),val(exp3));
			break;
		//ou
		case '+':
			for (i=0;i<meio(exp);i++) exp2[i]=exp[i+1];
			exp2[i-1]='\0';
			//salva exp2
			//fprintf("%s", exp2);
			for (i=0;i<(tam-meio(exp));i++) exp3[i]=exp[i+meio(exp)+1];
			exp3[i-2]='\0';
			//salva exp3
			//fprintf("%s", exp3);
			return ou(val(exp2),val(exp3));
			break;	
		//imp
		case '>':
			for (i=0;i<meio(exp);i++) exp2[i]=exp[i+1];
			exp2[i-1]='\0';
			//salva exp2
			//fprintf("%s", exp2);
			for (i=0;i<(tam-meio(exp));i++) exp3[i]=exp[i+meio(exp)+1];
			exp3[i-2]='\0';
			//salva exp3
			//fprintf("%s", exp3);
			return imp(val(exp2),val(exp3));
			break;
		//extra parenteses
	}
	
}

int e (int a, int b){
	return (a&&b);
}
int ou (int a, int b){
	return (a||b);
}
int imp (int a, int b){
	if (a==0 || (a!=0 && b!=0)) return 1;
	else return 0;
}
int neg (int a){
	return !a;
}
int meio (char exp[]){
	int l=0, i=0;
	int tam = strlen(exp);
	
	while (i<tam){
		if (exp[i]=='(') l++;
		if (exp[i]==')') l--;
		if (l==1){
			if (exp[i+1]=='-'||exp[i+1]=='+'||exp[i+1]=='.'||exp[i+1]=='>'){
				return i+1;
			}
			if (exp[i+2]=='+'||exp[i+2]=='.'||exp[i+2]=='>'){
				return i+2;
			}
		}
		i++;
	}
}



