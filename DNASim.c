#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char T;
typedef struct nod
{
	T valoare; // Numele/valoarea elementului (A,C,T sau G)
	T snod;	// Nod suprapus (T sau F)
	struct nod *leg;
} Nod;
typedef Nod *List;

// Declarare stiva
typedef int ElType;
typedef struct cell
{
	ElType linie, bucla, index;
	T comanda[3], nume, snod;
	struct cell *next;
} Stack;

// Initializare stiva vida
Stack *initstack(Stack *s)
{
	s = NULL;
	return s;
}

// Pune un element pe stiva
Stack *push(char comanda[], int linie, int bucla, int index, char nume, char s_nod, Stack *s)
{
	Stack *nou;
	nou = (Stack *)malloc(sizeof(Stack));
	if (nou == NULL)
		return NULL; // Nu mai exista spatiu disponibil
	strcpy(nou->comanda, comanda);
	nou->linie = linie;
	nou->bucla = bucla;
	nou->index = index;
	nou->nume = nume;
	nou->snod = s_nod;
	if (s == NULL)
	{
		nou->next = NULL;
		s = nou;
	}
	else
	{
		nou->next = s;
		s = nou;
	}
	return s;
}

// Extrage (sterge) elemntul din varful stivei
Stack *pop(Stack *s)
{
	Stack *aux;
	aux = s;
	if (s == NULL) // stiva vida
		return 0;
	s = aux->next;
	free(aux);
	return s;
}

/* Retine argumentele comenzii din varful stivei
	(linie, bucla, index, nume, s_nod) */
int top(int *linie, int *bucla, int *index, char *nume, char *s_nod, Stack *s)
{
	Stack *aux;
	aux = s;
	if (s == NULL) // Stiva vida
		return -1;
	*linie = aux->linie;
	*bucla = aux->bucla;
	*index = aux->index;
	*nume = aux->nume;
	*s_nod = aux->snod;
	if (strcmp(aux->comanda, "ie") == 0)
		return 1; // trebuie sa sterg elementul
	if (strcmp(aux->comanda, "de") == 0)
		return 2; // trebuie sa inserez elementul
	if (strcmp(aux->comanda, "ee") == 0)
		return 3; // trebuie sa editez elementul
	if (strcmp(aux->comanda, "rs") == 0)
		return 4; // trebuie sa inversez ambele linii
	else
		return 0; /* comanda a fost de printare
					=> nu face numic */
}

// Afisare stiva
void printstack(Stack *s, FILE *f_out)
{
	Stack *aux;
	aux = s;
	while (aux != NULL)
	{
		fprintf(f_out, "%s", aux->comanda);
		if (aux->next != NULL)
			fprintf(f_out, " ");
		aux = aux->next;
	}
}

// Adaugare elemente in lista
void creare_lista(List *prim1, List *ultim1, char ch)
{
	if (*prim1 == NULL)
	{
		// pentru primul nod
		*prim1 = (List)malloc(sizeof(Nod));
		(*prim1)->valoare = ch;
		(*prim1)->snod = 'F';
		(*prim1)->leg = NULL;
		*ultim1 = *prim1;
	}
	else
	{
		// adaugare la sfarsitul listei
		List nou = (List)malloc(sizeof(Nod));
		nou->valoare = ch;
		nou->snod = 'F';
		(*ultim1)->leg = nou;
		(*ultim1) = nou;
		(*ultim1)->leg = NULL;
	}
}

// Identifica comanda citita din fisier
int comanda(char ch2[])
{
	if (strcmp(ch2, "ie") == 0)
		return 1;
	else if (strcmp(ch2, "de") == 0)
		return 2;
	else if (strcmp(ch2, "ee") == 0)
		return 3;
	else if (strcmp(ch2, "pl") == 0)
		return 4;
	else if (strcmp(ch2, "pr") == 0)
		return 5;
	else if (strcmp(ch2, "rs") == 0)
		return 6;
	else if (strcmp(ch2, "un") == 0)
		return 7;
	else if (strcmp(ch2, "ps") == 0)
		return 8;
	else if (strcmp(ch2, "pv") == 0)
		return 9;
	else if (strcmp(ch2, "ex") == 0)
		return 10;
	else
		return 0; // default
}

// [ie] = insereaza un element
void insert(List *linie, int bucla, int index, char nume, char s_nod, int *BUCLA, int *INDEX)
{
	int i = 0, j = 0, ok = 0;
	List p;
	p = *linie;
	*BUCLA = bucla;
	*INDEX = index;

	// creare nod nou
	List nou = (List)malloc(sizeof(Nod));
	nou->valoare = nume;
	nou->snod = s_nod;

	// inserare la inceputul listei
	if ((bucla == 0) && (index == 1))
	{
		nou->leg = *linie;
		*linie = nou;
		ok = 1;
	}
	else
		while (p != NULL)
		{
			j++;
			// inserare pe [index] = 0 al unei bucle
			if (((p->leg)->snod == 'T') && (index == 0) && (i == bucla - 1))
			{
				nou->leg = p->leg;
				p->leg = nou;
				*BUCLA = bucla - 1;
				*INDEX = j + 1;
				ok = 1;
				break;
			}
			if (p->snod == 'T')
			{
				i++;
				j = 0;
			}
			if ((i == bucla) && (j == index - 1))
			{
				// inserare pe pozitia [index] a elementului [nou]
				nou->leg = p->leg;
				p->leg = nou;
				ok = 1;
				break;
			}
			p = p->leg;
		}
	// inserare la sfarsitul listei
	if (ok == 0)
	{
		while (p->leg != NULL)
			p = p->leg;
		p->leg = nou;
		p = nou;
		p->leg = NULL;
	}
}

// [de] = sterge un element
void delete (List *linie, int bucla, int index, char *nume, char *snod, int *POZ)
{
	int i = 0, j = 0, ok = 0;
	List p, rm;
	p = *linie;
	*POZ = 0;

	// stergere de la inceputul listei
	if ((bucla == 0) && (index == 1))
	{
		rm = p;
		*nume = p->valoare;
		*snod = p->snod;
		p = p->leg;
		*linie = p->leg;
		*linie = p;
		free(rm);
		ok = 1;
	}
	else
		while (p != NULL)
		{
			j++;
			(*POZ)++;
			// stergere element cu [index] = 0
			if (((p->leg)->snod == 'T') && (index == 0) && (i == bucla - 1))
			{
				rm = p->leg;
				*nume = (p->leg)->valoare;
				*snod = (p->leg)->snod;
				p->leg = (p->leg)->leg;
				free(rm);
				ok = 1;
				break;
			}
			else if ((p->snod == 'T') && ((p->leg)->snod == 'T') && (index == 0) && (i == bucla - 2))
			{
				rm = p->leg;
				*nume = (p->leg)->valoare;
				*snod = (p->leg)->snod;
				p->leg = (p->leg)->leg;
				free(rm);
				ok = 1;
				break;
			}
			if (p->snod == 'T')
			{
				i++;
				j = 0;
			}
			if ((i == bucla) && (j == index - 1))
			{
				// stergere din interiorul listei
				rm = p->leg;
				*nume = (p->leg)->valoare;
				*snod = (p->leg)->snod;
				p->leg = (p->leg)->leg;
				free(rm);
				ok = 1;
				break;
			}
			p = p->leg;
		}
	// stergere de la sfarsitul listei
	if (ok == 0)
	{
		while ((p->leg)->leg != NULL)
			p = p->leg;
		rm = p->leg;
		*nume = p->valoare;
		*snod = p->snod;
		p->leg = NULL;
		free(rm);
	}
}

// [ee] = editeaza un element
void edit(List *linie, int bucla, int index, char nume, char s_nod, char *nume_st, char *snod_st, int *POZ)
{
	int i = 0, j = 0;
	List p;
	p = *linie;
	*POZ = 0;
	while (p != NULL)
	{
		j++;
		(*POZ)++;
		if (p->snod == 'T')
		{
			i++;
			j = 0;
		}
		if ((i == bucla) && (j == index))
		{
			*nume_st = p->valoare;
			*snod_st = p->snod;
			p->valoare = nume;
			p->snod = s_nod;
			break;
		}
		p = p->leg;
	}
}

// Editeaza un element la comanda [un]
void edit_undo(List *linie, int poz, char nume, char s_nod)
{
	int j = 0;
	List p;
	p = *linie;
	while (p != NULL)
	{
		j++;
		if (j == poz)
		{
			p->valoare = nume;
			p->snod = s_nod;
			break;
		}
		p = p->leg;
	}
}

// Insereaza un element la comanda [un]
void insert_undo(List *linie, int POZ, char nume, char s_nod)
{
	int j = 0, ok = 0;
	List p;
	p = *linie;

	// creare nod nou
	List nou = (List)malloc(sizeof(Nod));
	nou->valoare = nume;
	nou->snod = s_nod;

	// pozitionare pe linie (gasire bucla si index)
	// inserare la inceputul listei
	if (POZ == 0)
	{
		nou->leg = *linie;
		*linie = nou;
		ok = 1;
	}
	else
		while (p != NULL)
		{
			j++;
			if (POZ == j)
			{
				//inserare pe pozitia [poz] a elementului [nou]
				nou->leg = p->leg;
				p->leg = nou;
				ok = 1;
				break;
			}
			p = p->leg;
		}
	//inserare la sfarsitul listei
	if (ok == 0)
	{
		while (p->leg != NULL)
			p = p->leg;
		p->leg = nou;
		p = nou;
		p->leg = NULL;
	}
}

// [pl] = afiseaza lista specificata
void print_line(List *prim1, FILE *f_out)
{
	List p;
	p = *prim1;
	while (p != NULL)
	{
		fprintf(f_out, "%c", p->valoare);
		if (p->snod == 'T')
			fprintf(f_out, "*");
		if (p->leg != NULL)
			fprintf(f_out, " ");
		p = p->leg;
	}
	fprintf(f_out, "\n");
}

// [pr] = afiseaza linia specificata in ordine inversa
void print_reversed(List *prim1, FILE *f_out)
{
	List p;
	p = *prim1;
	if (p == NULL)
		return;
	print_reversed(&(p->leg), f_out);
	fprintf(f_out, "%c", p->valoare);
	if (p->snod == 'T')
		fprintf(f_out, "*");
	if (p != NULL)
		fprintf(f_out, " ");
}

// Afiseaza primul element al listei
void print_first(List *prim1, FILE *f_out)
{
	List p;
	p = *prim1;
	fprintf(f_out, "%c", p->valoare);
	if (p->snod == 'T')
		fprintf(f_out, "*");
	fprintf(f_out, "\n");
}

// [rs] = inverseaza elementele liniei date
List reverse(List prim)
{
	if (prim->leg == NULL)
		return prim;
	else
	{
		List p = reverse(prim->leg);
		prim->leg->leg = prim;
		prim->leg = NULL;
		return p;
	}
}

// [pv] = print validation
int print_valid(List *prim1, List *prim2)
{
	List p1, p2;
	p1 = *prim1;
	p2 = *prim2;

	// pana cand se ajune la sfarsitul unei liste
	while ((p1 != NULL) && (p2 != NULL))
		if ((p1->snod == p2->snod) && (((p1->valoare == 'A') && (p2->valoare == 'T')) ||
									   ((p1->valoare == 'T') && (p2->valoare == 'A')) ||
									   ((p1->valoare == 'C') && (p2->valoare == 'G')) ||
									   ((p1->valoare == 'G') && (p2->valoare == 'C'))))
		{
			p1 = p1->leg;
			p2 = p2->leg;
		}
		else
			return 0;
	if ((p1 == NULL) && (p2 == NULL))
		return 1; // listele corespund
	else
		return 0; // listele nu au acelasi nr de elemente
}

int main(int argc, char **argv)
{
	List prim1, prim2, ultim1, ultim2;
	int ok = 0, test, linie, bucla, index, BUCLA = 0, INDEX = 0, POZ = 0;
	char ch, ch2[4], nume, s_nod, nume_st = '\0', snod_st = '\0';

	// initializare stiva
	Stack *s = NULL;
	s = initstack(s);

	// declarare fisiere
	FILE *f_in, *f_out, *f_com;
	f_in = fopen(argv[1], "r");
	f_com = fopen(argv[2], "r");
	f_out = fopen(argv[3], "w");

	// citire din fisierul dataX.txt si completare liste
	prim1 = prim2 = ultim1 = ultim2 = NULL;
	while (!feof(f_in))
	{
		fscanf(f_in, "%c", &ch);
		if (ch == '\n')
			ok = 1;
		if ((ch != ' ') && (ch != '*') && (ch != '\n'))
			switch (ok)
			{
			case 0:
				creare_lista(&prim1, &ultim1, ch);
				break;
			case 1:
				creare_lista(&prim2, &ultim2, ch);
				break;
			}
		if (ch == '*')
		{
			if (ok == 0)
				ultim1->snod = 'T';
			else
				ultim2->snod = 'T';
		}
	}

	// citire din fisierul commandsX.txt  si executare comenzi
	while (!feof(f_com))
	{
		fgets(ch2, 3, f_com);
		switch (comanda(ch2))
		{
		case 1: // [ie]
			fscanf(f_com, "%d", &linie);
			fscanf(f_com, "%d", &bucla);
			fscanf(f_com, "%d", &index);
			fscanf(f_com, "%c", &nume);
			fscanf(f_com, "%c", &nume);
			fscanf(f_com, "%c", &s_nod);
			fscanf(f_com, "%c", &s_nod);
			if (linie == 1)
			{
				insert(&prim1, bucla, index, nume, s_nod, &BUCLA, &INDEX);
				s = push(ch2, 1, BUCLA, INDEX, nume, s_nod, s);
			}
			else
			{
				insert(&prim2, bucla, index, nume, s_nod, &BUCLA, &INDEX);
				s = push(ch2, 2, BUCLA, INDEX, nume, s_nod, s);
			}
			break;

		case 2: // [de]
			fscanf(f_com, "%d", &linie);
			fscanf(f_com, "%d", &bucla);
			fscanf(f_com, "%d", &index);

			if (linie == 1)
			{
				delete (&prim1, bucla, index, &nume, &s_nod, &POZ);
				s = push(ch2, 1, POZ, index, nume, s_nod, s);
			}
			else
			{
				delete (&prim2, bucla, index, &nume, &s_nod, &POZ);
				s = push(ch2, 2, POZ, index, nume, s_nod, s);
			}
			break;

		case 3: // [ee]
			fscanf(f_com, "%d", &linie);
			fscanf(f_com, "%d", &bucla);
			fscanf(f_com, "%d", &index);
			fscanf(f_com, "%c", &nume);
			fscanf(f_com, "%c", &nume);
			fscanf(f_com, "%c", &s_nod);
			fscanf(f_com, "%c", &s_nod);
			if (linie == 1)
			{
				edit(&prim1, bucla, index, nume, s_nod, &nume_st, &snod_st, &POZ);
				s = push(ch2, 1, POZ, index, nume_st, snod_st, s);
			}
			else
			{
				edit(&prim2, bucla, index, nume, s_nod, &nume_st, &snod_st, &POZ);
				s = push(ch2, 2, POZ, index, nume_st, snod_st, s);
			}
			break;

		case 4: // [pl]
			fscanf(f_com, "%d", &linie);
			if (linie == 1)
				print_line(&prim1, f_out);
			else
				print_line(&prim2, f_out);
			s = push(ch2, 0, 0, 0, 0, 0, s);
			break;

		case 5: // [pr]
			fscanf(f_com, "%d", &linie);
			if (linie == 1)
			{
				print_reversed(&(prim1->leg), f_out);
				print_first(&prim1, f_out);
			}
			else
			{
				print_reversed(&(prim2->leg), f_out);
				print_first(&prim2, f_out);
			}
			s = push(ch2, 0, 0, 0, 0, 0, s);
			break;

		case 6: // [rs]
			prim1 = reverse(prim1);
			prim2 = reverse(prim2);
			s = push(ch2, 0, 0, 0, 0, 0, s);
			break;

		case 7: // [un]
			test = top(&linie, &bucla, &index, &nume, &s_nod, s);
			switch (test)
			{
			case 1:
				if (linie == 1)
					delete (&prim1, bucla, index, &nume, &s_nod, &POZ);
				else
					delete (&prim2, bucla, index, &nume, &s_nod, &POZ);
				s = pop(s);
				break;

			case 2:
				if (linie == 1)
					insert_undo(&prim1, bucla, nume, s_nod);
				else
					insert_undo(&prim2, bucla, nume, s_nod);
				s = pop(s);
				break;

			case 3:
				if (linie == 1)
					edit_undo(&prim1, bucla, nume, s_nod);
				else
					edit_undo(&prim2, bucla, nume, s_nod);
				s = pop(s);
				break;

			case 4:
				prim1 = reverse(prim1);
				prim2 = reverse(prim2);
				s = pop(s);
				break;

			case 0:
				s = pop(s);
				break;
			}
			break;

		case 8: // [ps]
			printstack(s, f_out);
			fprintf(f_out, "\n");
			break;

		case 9: // [pv]
			if (print_valid(&prim1, &prim2) == 0)
				fprintf(f_out, "false\n");
			else
				fprintf(f_out, "true\n");
			s = push(ch2, 0, 0, 0, 0, 0, s);
			break;

		case 10: // [ex]
			free(ultim1);
			free(ultim2);
			break;
		}
	}

	fclose(f_in);
	fclose(f_com);
	fclose(f_out);
	return 0;
}
