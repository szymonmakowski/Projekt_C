#define ILOSC_ELEMENTOW 10

typedef struct lw{
	struct lw *pierwszy;
	struct lw *nastepny;
	struct lw *poprzedni;
	char dane[];
};

void dodaj_elementy();
void wyswietl();
void usun_element(int);

struct lw *nowy=NULL;
struct lw *pierwszy=NULL;
struct lw *poprzedni=NULL;


void dodaj_elementy()
{
	for(int i=0;i<ILOSC_ELEMENTOW;i++)
	{
		if(!pierwszy)
		{

			nowy=(struct lw *) malloc(sizeof(struct lw));
			pierwszy=nowy;
			nowy->pierwszy=nowy;
			nowy->poprzedni=NULL;
			nowy->nastepny=NULL;
			nowy->liczba=i;
			poprzedni=nowy;
		}
		else
		{
			nowy=(struct lw *) malloc(sizeof(struct lw));
			poprzedni->nastepny=nowy;
			nowy->pierwszy=pierwszy;
			nowy->nastepny=NULL;
			nowy->poprzedni=poprzedni;
			nowy->liczba=i;
			poprzedni=nowy;
		}
	}
}
void wyswietl()
{
	for(struct lw* tmp=pierwszy;tmp!=NULL;tmp=tmp->nastepny)
		printf("Element numer:%d\n",tmp->liczba);
}
void usun_element(int ktory){

	if(!(ktory>ILOSC_ELEMENTOW))
	{
		struct lw * wsk=pierwszy;
		struct lw * pom;

		for(int i=0;i<ktory-1;i++)
			wsk=wsk->nastepny;

		if((pom=wsk->poprzedni))
		{
			pom->nastepny=wsk->nastepny;
			if((pom=wsk->nastepny))
			{
				pom->poprzedni=wsk->poprzedni;
				free(wsk);
			}
			else
				free(wsk);
		}
		else
		{
			pom=wsk->nastepny;
			pom->poprzedni=NULL;
			pierwszy=pom;
			free(wsk);
			for(struct lw* tmp=pierwszy;tmp!=NULL;tmp=tmp->nastepny)
				tmp->pierwszy=pierwszy;
		}
	}
	else
		printf("Nie ma takiego elementu na liscie");
}
