// drzewo czerwone-czarne
// (C) Damian Dziaduch

#include "naglowek.h"

void dodajElement(tree *a, int dana) {
	if ((*a) == NULL) {
		(*a) = (tree) malloc(sizeof(struct node));
		(*a)->w = dana;
		(*a)->c = 0;
		(*a)->o = NULL;
		(*a)->l = NULL;
		(*a)->p = NULL;
	} else {
		while (1) {
			if ((((*a)->l == NULL) && (dana < (*a)->w)) || (((*a)->p == NULL)
					&& (dana > (*a)->w))) {
				tree nowyElement; //dodanie nowego elementu do drzewa
				nowyElement = (tree) malloc(sizeof(struct node));
				nowyElement->w = dana;
				nowyElement->c = 1; //dodajemy nowy czerwony obiekt
				nowyElement->p = NULL;
				nowyElement->l = NULL;
				nowyElement->o = (*a);
				if (dana < (*a)->w)
					(*a)->l = nowyElement;
				else if (dana > (*a)->w)
					(*a)->p = nowyElement;
				if ((*a)->c == 1)
					przywrocKolory(&nowyElement); //gdy ojciec jest czerwony to napraw wlasciwosi drzewa
				powrotDoKorzenia(a);
				break;
			} else if (dana < (*a)->w) {
				(*a) = (*a)->l;
			} else if (dana > (*a)->w) {
				(*a) = (*a)->p;
			}
		}
	}
	printf("Dodano element: %d\n", dana);
	return;
}

void przywrocKolory(tree *a) {
	tree b, c, d;

	b = (*a)->o; //ojciec
	if (b == NULL)
		return;
	c = b->o; //dziadek
	if (c == NULL)
		return; //gdy b-ojciec jest korzeniem to drzewo jest ok wiec zakoncz
	if (b == c->l)
		d = c->p; //wujek, przypadek gdy jest prawy
	else
		d = c->l; //wujek, przypadek gdy jest lewy
	if (d != NULL && d->c == 1) // czerwony wujek
	{
		b->c = 0; //ustaw ojca czarnego

		if (c->o != NULL)
			c->c = 1; //ustaw dziadka czerwonego ale tylko gdy nie jest korzeniem

		d->c = 0; //ustaw wujka na czarno
		przywrocKolory(&c);
	} else if ((*a) == b->p) //czarny wujek prawy syn
	{
		obrocLewo(a); //pierwsza rotacja
		b = (*a)->o;
		c = b->o;
		if (c->l == b)
			obrocPrawo(&b); //druga rotacja
		else
			obrocLewo(&b);
		c = b->o;

		c->c = 0; //zmien kolor elementu na czarny
		b->c = 1; //zmien kolor starego dziadka na czerwony

		przywrocKolory(&c);
	} else if ((*a) == b->l) //czarny wujek lewy syn
	{
		if (b == c->l)
			obrocPrawo(&b);
		else
			obrocLewo(&b); //wykonujemy rotacje ojca
		b->c = 1; //zmieniamy kolor starego dziadka na czerwony

		c = b->o;
		c->c = 0; //zmieniamy kolor ojca na czarny

		przywrocKolory(&c);
	}
}

void wyswietlDrzewo(tree *a) {
	printf("[");
	if ((*a) != NULL) {
		if ((*a)->l == NULL && (*a)->p == NULL)
			printf("(%d,%d)", (*a)->w, (*a)->c);
		else {
			printf("(%d,%d)", (*a)->w, (*a)->c);
			tree l = (*a)->l, p = (*a)->p;
			wyswietlDrzewo(&l);
			wyswietlDrzewo(&p);
		}
		printf("]");
	}
}

void czyWystapilElement(tree *a, int dana) {
	if ((*a) != NULL) {
		if ((*a)->w == dana)
			czyWystepuje = 1;
		tree l = (*a)->l, p = (*a)->p;
		czyWystapilElement(&l, dana);
		czyWystapilElement(&p, dana);
	}
}

void obrocPrawo(tree *a) {
	tree x, y, ojciec, syn;
	x = (*a);
	y = x->o;
	ojciec = y->o;
	syn = x->p;
	(*a) = y;
	(*a)->l = syn;
	(*a)->o = x;
	x->o = ojciec;
	if (ojciec != NULL) {
		if (ojciec->p == y)
			ojciec->p = x;
		else
			ojciec->l = x;
	}
	x->p = (*a);
	return;
}

void obrocLewo(tree *a) {
	tree x, y, ojciec, syn;
	y = (*a);
	x = y->o;
	ojciec = x->o;
	syn = y->l;
	(*a) = x;
	(*a)->p = syn;
	(*a)->o = y;
	y->o = ojciec;
	if (ojciec != NULL) {
		if (ojciec->p == x)
			ojciec->p = y;
		else
			ojciec->l = y;
	}
	y->l = (*a);
	return;
}

void usunElement(tree *a, int dana) {
	tree element = (*a);
	//przesuniecie do danego elementu
	while (element->w != dana) {
		if (dana < element->w)
			element = element->l;
		else if (dana > element->w)
			element = element->p;
	}
	tree ojciec = element->o, synPrawy = element->p, synLewy = element->l, brat = NULL, bratanekPrawy = NULL, bratanekLewy = NULL;
	if (ojciec->l == element) brat = ojciec->p;
	else brat = ojciec->l;
	if (brat != NULL) {
		bratanekLewy = brat->l;
		bratanekPrawy = brat->p;
	}
	//gdy element jest korzeniem
	if (synLewy == NULL && synPrawy == NULL && ojciec == NULL) {
		(*a) = NULL;
		free (element);
	}
	//gdy element jest lisciem
	if (synLewy == NULL && synPrawy == NULL) {
		if (ojciec != NULL) {
			if (ojciec->l == element)
				ojciec->l = NULL;
			else
				ojciec->p = NULL;
		}
		free (element);
	}
	//gdy element jest czerwony
	else usunDalej(&element);
	powrotDoKorzenia(a);
	printf("Usunieto element %d", dana);
}

void powrotDoKorzenia (tree *a) {
	tree tmp;
	for (tmp = (*a); tmp != NULL; tmp = tmp->o) {
		(*a) = tmp;
	}
}

tree brat(tree *a)
{
	if ((*a) == (*a)->o->l)
		return (*a)->o->p;
	else
		return (*a)->o->l;
}

int czyLisc(tree *a)
{
	if (a == NULL) return 1;
	else return 0;
}

void zastapElement(tree *a, tree *b)
{
	tree stary = (*a), nowy = (*b);
	tree ojciec = stary->o;
	tree synPrawy = stary->p;
	tree synLewy = stary->l;
	if (ojciec->p == stary) ojciec->p = nowy;
	else ojciec->l = nowy;
	nowy->o = ojciec;
	if (synPrawy == nowy)
	{
		nowy->l = synLewy;
		if (synLewy != NULL) synLewy->o = nowy;
	}
	else
	{
		nowy->p = synPrawy;
		if (synPrawy != NULL) synPrawy->o = nowy;
	}
}

void usunDalej(tree *a)
{
	tree n = (*a);
	/*
	 * Precondition: n has at most one non-null child.
	 */
	tree syn = czyLisc(&(n->p)) ? n->l : n->p;

	zastapElement(&n, &syn);
	if (n->c == 0) {
		if (syn->c == 1)
			syn->c = 0;
		else
			usunPrzypadek1(&syn);
	}
	free(n);
}

void usunPrzypadek1(tree *a)
{
	tree n = (*a);
	if (n->o != NULL) usunPrzypadek2(&n);
}

void usunPrzypadek2(tree *a)
{
	tree n = (*a), s = brat(&n);

	if (s->c == 1) {
		n->o->c = 1;
		s->c = 0;
		if (n == n->o->l)
		{
			obrocLewo(&n);
			n = n->o;
		}
		else
		{
			obrocPrawo(&n);
			n = n->o;
		}
	}
	usunPrzypadek3(&n);
}

void usunPrzypadek3(tree *a)
{
	tree n = (*a), s = brat(&n);

	if ((n->o->c == 0) &&
	    (s->c == 0) &&
	    (s->l->c == 0) &&
	    (s->p->c == 0)) {
		s->c = 1;
		usunPrzypadek1((&n->o));
	} else
		usunPrzypadek4(&n);
}

void usunPrzypadek4(tree *a)
{
	tree n = (*a), s = brat(&n);

	if ((n->o->c == 1) &&
	    (s->c == 0) &&
	    (s->l->c == 0) &&
	    (s->p->c == 0)) {
		s->c = 1;
		n->o->c = 0;
	} else
		usunPrzypadek5(&n);
}

void usunPrzypadek5(tree *a)
{
	tree n = (*a), s = brat(&n);

	if  (s->c == 0) { /* this if statement is trivial,
due to Case 2 (even though Case two changed the sibling to a sibling's child,
the sibling's child can't be red, since no red parent can have a red child). */
// the following statements just force the red to be on the left of the left of the parent,
// or right of the right, so case six will rotate correctly.
		if ((n == n->o->l) &&
		    (s->p->c == 0) &&
		    (s->l->c == 1)) { // this last test is trivial too due to cases 2-4.
			s->c = 1;
			s->l->c = 0;
			obrocPrawo(&(s->p));
		} else if ((n == n->o->p) &&
		           (s->l->c == 0) &&
		           (s->p->c == 1)) {// this last test is trivial too due to cases 2-4.
			s->c = 1;
			s->p->c = 0;
			obrocLewo((&s->l));
		}
	}
	usunPrzypadek6(&n);
}

void usunPrzypadek6(tree *a)
{
	tree n = (*a), s = brat(&n);

	s->c = n->o->c;
        n->o->c = 0;

	if (n == n->o->l) {
                s->p->c = 0;
                obrocLewo(&n);
	} else {
		s->o->c = 0;
		obrocPrawo(&n);
	}
}