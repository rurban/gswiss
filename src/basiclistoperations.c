/**
 *  Copyright (C) 2004 Stefan Knöfel
 * 
 *  This file is part of GSwiss.
 *
 *  GSwiss is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  GSwiss is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Foobar; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/language.h"
#include "include/structdef.h"
#include "include/basiclistoperations.h"


/* defaults for player hashtable extension */
int initialPlayerAnz = 4;
int extendPlayerAnzStepSize = 4;

/**
 *
 * Creation
 *
 *****************************************************************/

/**
 * Neue Spielerkarte anlegen
 * Create new playercard
 */
PLAYERCARD* playercard_create(int pId, char *name, int rating, int title, int expectedRounds)
{
    PLAYERCARD *newPlayerCard;
    
    if ( (newPlayerCard =(PLAYERCARD *) malloc(sizeof(PLAYERCARD))) == NULL)
       fprintf(stderr,L_MEMERROR_PLAYERCARD);
       
    newPlayerCard->pId = pId;
    strcpy(newPlayerCard->name, name);
    newPlayerCard->rating = rating;
    newPlayerCard->points = 0;
    newPlayerCard->buchholz = 0;
    newPlayerCard->buchholzsum = 0;
    newPlayerCard->nextColor = 0;
    newPlayerCard->title = title;
    newPlayerCard->colorDifference = 0;
    newPlayerCard->preferedColor = 0;
    newPlayerCard->preferedColorEmphasis = 0;
    newPlayerCard->hasRecievedFreePoint = 0;
    newPlayerCard->floaterstatus = 0;
	newPlayerCard->reservedRounds = expectedRounds;

    if ( (newPlayerCard->opponents = (int *) malloc(sizeof(int)*expectedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(newPlayerCard->opponents,-1,sizeof(*(newPlayerCard->opponents))*expectedRounds);
        
    if ( (newPlayerCard->results = (char *) malloc(sizeof(char)*expectedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(newPlayerCard->results,' ',sizeof(*(newPlayerCard->results))*expectedRounds);
        
    if ( (newPlayerCard->colors = (int *) malloc(sizeof(int)*expectedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(newPlayerCard->colors,0,sizeof(*(newPlayerCard->colors))*expectedRounds);

    if ( (newPlayerCard->floaterhistory = (int *) malloc(sizeof(int)*expectedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(newPlayerCard->floaterhistory,0,sizeof(*(newPlayerCard->floaterhistory))*expectedRounds);
    
    return newPlayerCard;
}


/**
 * Neuen Spielerlisteneintrag anlegen
 * Create new playerlistentry
 */
PLAYERLISTENTRY* playerlistentry_create(PLAYERCARD *aPlayerCard)
{
   PLAYERLISTENTRY *newPlayerListEntry;
   
   if( (newPlayerListEntry =(PLAYERLISTENTRY *) malloc(sizeof(PLAYERLISTENTRY))) == NULL)
       fprintf(stderr,L_MEMERROR_PLAYERLISTENTRY);   
   else
   {
       newPlayerListEntry->aPlayerCard = aPlayerCard;
       newPlayerListEntry->next = NULL;
       newPlayerListEntry->prev = NULL;
   }
   
   return newPlayerListEntry;
}

/**
 * Neue Spielerliste anlegen
 * Create new playerlist
 */
PLAYERLIST* playerlist_create()
{
    PLAYERLIST *newPlayerList;
    
    if( (newPlayerList =(PLAYERLIST *) malloc(sizeof(PLAYERLIST))) == NULL)
        fprintf(stderr,L_MEMERROR_PLAYERLIST);
    else
    {
        newPlayerList->first = NULL;
        newPlayerList->last = NULL;
        newPlayerList->size = 0;
    }
    
    return newPlayerList;
} 

/**
 * Neuen Paarungslisteneintrag anlegen
 * Create new pairinglistentry
 */
PAIRINGLISTENTRY* pairinglistentry_create(PLAYERCARD *player1, PLAYERCARD *player2)
{
   PAIRINGLISTENTRY *newPairingListEntry;
   
   if( (newPairingListEntry =(PAIRINGLISTENTRY *) malloc(sizeof(PAIRINGLISTENTRY))) == NULL)
       fprintf(stderr,L_MEMERROR_PAIRINGLISTENTRY);   
   else
   {
       newPairingListEntry->player1 = player1;
       newPairingListEntry->player2 = player2;
       newPairingListEntry->result = ' ';
       newPairingListEntry->next = NULL;
       newPairingListEntry->prev = NULL;
   }
   
   return newPairingListEntry;
}

/**
 * Neue Paarungsliste anlegen
 * Create new pairinglist
 */
PAIRINGLIST* pairinglist_create()
{
    PAIRINGLIST *newPairinglist;
    
    if( (newPairinglist =(PAIRINGLIST *) malloc(sizeof(PAIRINGLIST))) == NULL)
        fprintf(stderr,L_MEMERROR_PAIRINGLIST);
    else
    {
        newPairinglist->first = NULL;
        newPairinglist->last = NULL;
        newPairinglist->byeplayer = NULL;
    }
    
    return newPairinglist;
} 

/**
 * Neue Spielergruppe anlegen
 * Create new playergroup
 */
PLAYERGROUP* playergroup_create()
{
    PLAYERGROUP *newPlayerGroup;
    
    if( (newPlayerGroup =(PLAYERGROUP *) malloc(sizeof(PLAYERGROUP))) == NULL)
        fprintf(stderr,L_MEMERROR_PLAYERGROUP);
    else
    {
        newPlayerGroup->subgroup1 = playerlist_create();
        newPlayerGroup->subgroup2 = playerlist_create();
        newPlayerGroup->pairedplayers = pairinglist_create();
        
        newPlayerGroup->next = NULL;
        newPlayerGroup->prev = NULL;
    
        newPlayerGroup->isHeterogen = 0;
    }
    return newPlayerGroup;
}

/**
 * Neue Spielerverwaltung anlegen
 * Create new playeradministration
 */
PLAYERADMINISTRATION* playeradministration_create()
{
    PLAYERADMINISTRATION *newPlayerAdministration;
    
    if( (newPlayerAdministration =(PLAYERADMINISTRATION *) malloc(sizeof(PLAYERADMINISTRATION))) == NULL)
        fprintf(stderr,L_MEMERROR_PLAYERADMINISTRATION);
    else
    { 
        if ( (newPlayerAdministration->player_list = playerlist_create()) == NULL )
             fprintf(stderr,L_MEMERROR_PLAYERADMINISTRATION);
        else
        {
            if( (newPlayerAdministration->player_ht =(PLAYERCARD**) malloc(sizeof(PLAYERCARD*) * initialPlayerAnz)) == NULL)
                fprintf(stderr,L_MEMERROR_PLAYERADMINISTRATION);    
            else
            {
                newPlayerAdministration->size_ht = initialPlayerAnz;
                memset(newPlayerAdministration->player_ht,(int) NULL, sizeof(PLAYERCARD*) * initialPlayerAnz);
            }
        }
    }
    
    return newPlayerAdministration;
}

/**
 *
 * Finalization and clearing of lists
 *
 **************************************************************/
 
/**
 * Spielerkarte freigeben
 * finalize playercard
 */
void playercard_finalize(PLAYERCARD *aPlayerCard)
{
	free(aPlayerCard->opponents);
	free(aPlayerCard->results);
	free(aPlayerCard->colors);
	free(aPlayerCard->floaterhistory);
	free(aPlayerCard);
	aPlayerCard = NULL;
}

/**
 * Spielerlisteneintrag freigeben (enthaltene Spielerkarte wird nicht gelöscht !!)
 * finalize playerlistentry (does not finalize the contained playercard !!)
 */
void playerlistentry_finalize(PLAYERLISTENTRY *aEntry)
{
    free(aEntry);
    aEntry = NULL;
}

/**
 * Spielerliste leeren, alle Einträge werden gelöscht
 * clears a playerlist, all entrys will be deleted
 */
void playerlist_clear(PLAYERLIST *aPlayerList)
{
	PLAYERLISTENTRY *aEntry;
    
    for ( aEntry = aPlayerList->first; aEntry != NULL;)
    {
        if (aEntry->next != NULL)
        {
            aEntry = aEntry->next;
            playerlistentry_finalize(aEntry->prev);
        }
        else
        {
             playerlistentry_finalize(aEntry);
             aEntry = NULL;
        }
    }
    aPlayerList->first = NULL;
    aPlayerList->last = NULL;
    aPlayerList->size = 0;
}

/**
 * Spielerliste freigeben
 * finalize playerlist
 */
void playerlist_finalize(PLAYERLIST *aPlayerList)
{
    playerlist_clear(aPlayerList);
    free(aPlayerList);
    aPlayerList = NULL;
}

/**
 * Paarungslisteneintrag freigeben
 * finalize pairinglistentry
 */
void pairinglistentry_finalize(PAIRINGLISTENTRY *aEntry)
{
    free(aEntry);
    aEntry = NULL;
}


/**
 * Paarungsliste leeren, alle Einträge werden gelöscht
 * clears a pairinglist, all entrys will be deleted
 */
void pairinglist_clear(PAIRINGLIST *aPairingList)
{
	PAIRINGLISTENTRY *aEntry;
    for ( aEntry = aPairingList->first; aEntry != NULL;)
    {
        if (aEntry->next != NULL)
        {
            aEntry = aEntry->next;
            pairinglistentry_finalize(aEntry->prev);
        }
        else
        {
             pairinglistentry_finalize(aEntry);
             aEntry = NULL;
        }
    }
    aPairingList->first = NULL;
    aPairingList->last = NULL;
    aPairingList->byeplayer = NULL;
}

/**
 * Paarungsliste freigeben
 * finalize pairinglist
 */
void pairinglist_finalize(PAIRINGLIST *aPairingList)
{
    pairinglist_clear(aPairingList);
    free(aPairingList);
    aPairingList = NULL;
}

/**
 * Spielergruppe freigeben
 * finalize playergroup
 */
void playergroup_finalize(PLAYERGROUP *aPlayerGroup)
{
    playerlist_finalize(aPlayerGroup->subgroup1);
    playerlist_finalize(aPlayerGroup->subgroup2);
    pairinglist_finalize(aPlayerGroup->pairedplayers);
    free(aPlayerGroup);
    aPlayerGroup = NULL;
}

/**
 * alle Spielergruppe freigeben die durch verkettung an dieser (ersten) Gruppe hängen
 * finalize all playergroup, which are attached to this (first) playergroup
 */
void playergroup_finalizeall(PLAYERGROUP *firstPlayerGroup)
{
	while (firstPlayerGroup != NULL)
	{
		if (firstPlayerGroup->next == NULL)
		{
			playergroup_finalize(firstPlayerGroup);
			firstPlayerGroup = NULL;
		}
		else
		{
			firstPlayerGroup = firstPlayerGroup->next;
			playergroup_finalize(firstPlayerGroup->prev);
		}
	}
}

/**
 *
 * Manipulation
 *
 **************************************************************/

/**
 * Spielerkarte in díe Hashtable der Spieleradministration einfügen
 * insert playercard in hashtable of playeradministration
 */
void playeradministration_ht_insert(PLAYERADMINISTRATION *aPlayerAdministration, PLAYERCARD *aPlayerCard)
{
    if (aPlayerCard->pId <= 0)
    {
        return;
    }
    
    /* Erweiterung der Kapazitaet der Hashtable notwendig  
       we need to extend capacity of hashtable */
    if (aPlayerCard->pId >= aPlayerAdministration->size_ht)
    {
        int nextSize;
        
        /* neue groesse berechnen / calculate new size */
        nextSize = (aPlayerCard->pId+1) / extendPlayerAnzStepSize;
        if (nextSize * extendPlayerAnzStepSize <= aPlayerCard->pId) 
            nextSize++;
        nextSize *= extendPlayerAnzStepSize;
        
        /* speicher reservieren / allocate memory */
        if ( (aPlayerAdministration->player_ht =(PLAYERCARD**) realloc(aPlayerAdministration->player_ht,sizeof(PLAYERCARD*) * nextSize)) == NULL)
        {    
            fprintf(stderr,L_MEMERROR_PLAYERADMINISTRATIONREALLOC);
            return;
        }
        
        /* mit NULL vorbelegen / fill new fields with NULL */
        memset(&(aPlayerAdministration->player_ht[aPlayerAdministration->size_ht]),(int) NULL, sizeof(PLAYERCARD*) * (nextSize-aPlayerAdministration->size_ht));
        
        aPlayerAdministration->size_ht = nextSize;
    }
    aPlayerAdministration->player_ht[aPlayerCard->pId] = aPlayerCard;
}

/**
 * Zugriff auf ein Element in der HashTable per index
 * get element of the hashtable via index
 */
PLAYERCARD* playeradministration_ht_get(PLAYERADMINISTRATION *aPlayerAdministration, int index)
{
    if (index >= aPlayerAdministration->size_ht || index<0)
        return NULL;
    return aPlayerAdministration->player_ht[index];
}

/**
 * Spielerkarte an Liste anhaengen
 * appand playercard to playerlist
 */
void playerlist_append(PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard)
{
    PLAYERLISTENTRY *aPlayerListEntry;
    
    aPlayerListEntry = playerlistentry_create(aPlayerCard);
    
    if (aPlayerList->first == NULL)
    {
        aPlayerList->first = aPlayerListEntry;
        aPlayerList->last  = aPlayerListEntry;
    }
    else
    {
        aPlayerList->last->next = aPlayerListEntry;
        aPlayerListEntry->prev = aPlayerList->last;
        aPlayerList->last = aPlayerListEntry;
    }
   
    aPlayerList->size++;
}

/** 
 * Loescht einen eintrag aus einer spielerliste (keine Ueberpruefung ob der Eintrag auch zu dieser Liste gehoert!)
 * deletes a entry form playerlist (no check if the entry is part  of this list !!)
 */
void playerlist_delete(PLAYERLIST *aPlayerList, PLAYERLISTENTRY *aPlayerListEntry)
{
    if (aPlayerList->first == aPlayerListEntry)
        aPlayerList->first = aPlayerListEntry->next;
    else 
        aPlayerListEntry->prev->next = aPlayerListEntry->next;
        
    
    if (aPlayerList->last == aPlayerListEntry)
        aPlayerList->last = aPlayerListEntry->prev;
    else 
        aPlayerListEntry->next->prev = aPlayerListEntry->prev;
        
    playerlistentry_finalize(aPlayerListEntry);
    
    aPlayerList->size--;
}

/** 
 * Entfernt eine Spielerkarte aus einer Spielerliste wenn vorhanden
 * removes a playercard from the list if present
 * returns
 *  1: war vorhanden / was in list
 *  0: war nicht vorhanden / was not in list
 */
int playerlist_remove(PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard)
{
    PLAYERLISTENTRY *aPlayerListEntry;
    
    for (aPlayerListEntry = aPlayerList->first;aPlayerListEntry != NULL;  aPlayerListEntry = aPlayerListEntry->next)
    {
        if (aPlayerListEntry->aPlayerCard == aPlayerCard)
        {
            playerlist_delete(aPlayerList,aPlayerListEntry);
            return 1;
        }
    }
    return 0;
}

/**
 * neue Paarung an Paarungsliste anhaengen
 * appand new pairing to pairinglist
 */
void pairinglist_append(PAIRINGLIST *aPairingList, PLAYERCARD *player1, PLAYERCARD *player2)
{
	PAIRINGLISTENTRY *aEntry;
    
    aEntry = pairinglistentry_create(player1,player2);
    
    if (aPairingList->first == NULL)
    {
        aPairingList->first = aEntry;
        aPairingList->last  = aEntry;
    }
    else
    {
        aPairingList->last->next = aEntry;
        aEntry->prev = aPairingList->last;
        aPairingList->last = aEntry;
    }
}

/** 
 * Entfernt einen Spieler aus einer spielergruppe wenn vorhanden
 * removes a playercard from the group if present
 * returns
 *  1: war vorhanden / was present
 *  0: war nicht vorhanden / was not present
 */
int playergroup_remove(PLAYERGROUP *aPlayerGroup, PLAYERCARD *aPlayerCard)
{
    return ( playerlist_remove(aPlayerGroup->subgroup1,aPlayerCard) || 
             playerlist_remove(aPlayerGroup->subgroup2,aPlayerCard) );
}

/**
 * SpielerKarte in der Verwaltung einfuegen
 * adds new playercard to a playeradministration
 */
void playeradministration_add(PLAYERADMINISTRATION *aPlayerAdministration, PLAYERCARD *aPlayerCard)
{
    /* An die Liste anhaengen / append to list */
    playerlist_append(aPlayerAdministration->player_list,aPlayerCard);

    /* Auch in der hashtable einfuegen / insert in hashtable too */
    playeradministration_ht_insert(aPlayerAdministration,aPlayerCard);
}

/**
 * SpielerKarte an Gruppe anhaengen
 * add a playercard to playergroup
 */
void playergroup_add(PLAYERGROUP *aPlayerGroup, PLAYERCARD *aPlayerCard)
{
    /* Immer in der ersten liste hinzufuegen */
    playerlist_append(aPlayerGroup->subgroup1, aPlayerCard);
}

/**
 * Die neue Gruppe hinter der aktuellen einfuegen
 * insert a new group in a group-list behind the geven group
 */
void playergroup_insertbehind(PLAYERGROUP *aPlayerGroup, PLAYERGROUP *anotherPlayerGroup)
{
	anotherPlayerGroup->next = aPlayerGroup->next;
	if (anotherPlayerGroup->next != NULL)
		anotherPlayerGroup->next->prev = anotherPlayerGroup;
	aPlayerGroup->next = anotherPlayerGroup;
	anotherPlayerGroup->prev = aPlayerGroup;
}

/**
 * gibt den Eintrag der Liste an der Stelle pos
 * returns the playerlistentry which is in the playerlist at pos
 */
PLAYERLISTENTRY* playerlist_get(PLAYERLIST *aPlayerList, int pos)
{
    PLAYERLISTENTRY *aEntry;
    int counter;
    
    if (pos < 0 || pos >= aPlayerList->size)
    {
        return NULL;
    }
    
    /* von vorn zaehlen wenn pos <= die halbe groesse /
       start counting form begin if pos <= the half size */
    if (pos * 2 <= aPlayerList->size)
    {
        counter = 0;
        for (aEntry = aPlayerList->first; counter < pos; counter++)
             aEntry = aEntry->next;
        return aEntry;
    }
    /* ansonsten von hinten suchen / else start counting from end */
    else
    {
        counter = aPlayerList->size - 1;
        for (aEntry = aPlayerList->last; counter > pos; counter--)
             aEntry = aEntry->prev;
        return aEntry;
    }
} 

/**
 * gibt den eintrag der Liste in dem der gesuchte spieler steht
 * returns the pos in list of a playercard
 */
PLAYERLISTENTRY* playerlist_getentry(PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard)
{
    PLAYERLISTENTRY *aEntry;

    for (aEntry = aPlayerList->first; aEntry != NULL; aEntry = aEntry->next)
    {
        if (aEntry->aPlayerCard == aPlayerCard)
            return aEntry;
    }
    
    return NULL;
} 


/**
 * gibt den Eintrag der Liste zurueck der den Spieler mit der gesuchten id enthält
 * returns the entry which contains playercard with given id
 */
PLAYERLISTENTRY* playerlist_getwithid(PLAYERLIST *aPlayerList, int pId)
{
    PLAYERLISTENTRY *aEntry;

    for (aEntry = aPlayerList->first; aEntry != NULL; aEntry = aEntry->next)
    {
        if (aEntry->aPlayerCard->pId == pId)
            return aEntry;
    }

    return NULL;
}

/**
 * Groesse einer Spielerkarte erweitern (erweitert die Größe der Arrays deren Größe auf der Rundenanzahl basiert)
 * extend the size of a playercard (extend all arrays which size is based on rounds)
 */
void playercard_setReservedRounds(PLAYERCARD *aPlayerCard, int reservedRounds)
{
	//no need for reallocate
	if (reservedRounds <= aPlayerCard->reservedRounds)
		return;
	
	if ( (aPlayerCard->opponents = (int *) realloc(aPlayerCard->opponents,sizeof(int)*reservedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(&(aPlayerCard->opponents[aPlayerCard->reservedRounds]),-1,sizeof(*(aPlayerCard->opponents))*reservedRounds-aPlayerCard->reservedRounds);
        
    if ( (aPlayerCard->results = (char *) realloc(aPlayerCard->results,sizeof(char)*reservedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(&(aPlayerCard->results[aPlayerCard->reservedRounds]),' ',sizeof(*(aPlayerCard->results))*reservedRounds-aPlayerCard->reservedRounds);
        
    if ( (aPlayerCard->colors = (int *) realloc(aPlayerCard->colors,sizeof(int)*reservedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(&(aPlayerCard->colors[aPlayerCard->reservedRounds]),0,sizeof(*(aPlayerCard->colors))*reservedRounds-aPlayerCard->reservedRounds);

    if ( (aPlayerCard->floaterhistory = (int *) realloc(aPlayerCard->floaterhistory,sizeof(int)*reservedRounds) ) == NULL) 
        fprintf(stderr,L_MEMERROR_PLAYERCARD);
    else
        memset(&(aPlayerCard->floaterhistory[aPlayerCard->reservedRounds]),0,sizeof(*(aPlayerCard->floaterhistory))*(reservedRounds-aPlayerCard->reservedRounds));
    
	aPlayerCard->reservedRounds = reservedRounds;
}

/**
 * Spieler einen Gegner fuer eine Runde zuweisen
 * set the opponent of a player in a given round
 */
void playercard_setOpponent(PLAYERCARD *aPlayerCard, int aRound, int opponentId)
{
    playercard_setReservedRounds(aPlayerCard,aRound);
    aPlayerCard->opponents[aRound-1] = opponentId;
}

/**
 * Spieler ein Ergebnis fuer eine Runde zuweisen
 * set the result of a player in a given round
 */
void playercard_setResult(PLAYERCARD *aPlayerCard, int aRound, char result)
{
   	playercard_setReservedRounds(aPlayerCard,aRound);
    aPlayerCard->results[aRound-1] = result;
}

/**
 * Spieler eine Farbe fuer eine Runde zuweisen
 * set the color of a player in a given round
 */
void playercard_setColor(PLAYERCARD *aPlayerCard, int aRound, int color)
{
   	playercard_setReservedRounds(aPlayerCard,aRound);
    aPlayerCard->colors[aRound-1] = color;
}

/**
 * Spieler einen Gegener, die Farbe und ein Ergebnis fuer eine Runde zuweisen
 * set the opponent, color and result of a player in a given round
 */
void playercard_setRound(PLAYERCARD *aPlayerCard, int aRound, int opponentId, int color, char result)
{
    playercard_setOpponent(aPlayerCard,aRound,opponentId);
    playercard_setColor(aPlayerCard,aRound,color);
    playercard_setResult(aPlayerCard,aRound,result);
}

/**
 * Vertauscht die Spielerkarten die an den uebergebenen Adressen gespeichert sind
 * swaps th playercards which are stored on given addresses
 */
void playercard_swap(PLAYERCARD **aPlayerCard, PLAYERCARD **anotherPlayerCard)
{
	// we do use a temp var
    PLAYERCARD *tempPlayerCard;
	tempPlayerCard = *anotherPlayerCard;
	*anotherPlayerCard = *aPlayerCard;
	*aPlayerCard = tempPlayerCard;
	
	//if you want to swap via xor you need to inclide <stddef.h> 
	//a = (PLAYERCARD*)  ( ((ptrdiff_t ) a) ^ ((ptrdiff_t ) b)); 
	//b = (PLAYERCARD*)  ( ((ptrdiff_t ) a) ^ ((ptrdiff_t ) b)); 
	//a = (PLAYERCARD*)  ( ((ptrdiff_t ) a) ^ ((ptrdiff_t ) b)); 
}

/**
 * verschiebe alle Eintraege einer Spielerliste in eine andere
 * move all entries of one playerlist into another
 */
void playerlist_moveall(PLAYERLIST *sourcePlayerList,PLAYERLIST *targetPlayerList)
{
   	if (sourcePlayerList->first == NULL)
		return;
	
	/* anhaengen / append */
	if (targetPlayerList->first == NULL)
		targetPlayerList->first = sourcePlayerList->first;
	else
	{
		targetPlayerList->last->next = sourcePlayerList->first;
		sourcePlayerList->first->prev = targetPlayerList->last;
	}

	targetPlayerList->last = sourcePlayerList->last;
	
	sourcePlayerList->first = NULL;
	sourcePlayerList->last = NULL;
	targetPlayerList->size += sourcePlayerList->size;
	sourcePlayerList->size = 0;
}

/**
 * verschiebe alle Eintraege einer Paarungsliste in eine andere
 * move all entries of one pairinglist into another
 */
void pairinglist_moveall(PAIRINGLIST *sourcePairingList, PAIRINGLIST *targetPairingList)
{
	if (sourcePairingList->first == NULL)
		return;
	
	/* anhaengen / append */
	if (targetPairingList->first == NULL)
		targetPairingList->first = sourcePairingList->first;
	else
	{
		targetPairingList->last->next = sourcePairingList->first;
		sourcePairingList->first->prev = targetPairingList->last;
	}

	targetPairingList->last = sourcePairingList->last;
	
	sourcePairingList->first = NULL;
	sourcePairingList->last = NULL;
}

/***************************************************************
 * 
 * some dirty playerlist manipulation method be careful if using
 * 
 */
 
/**
 * Verschiebt einen eintrag in einer SpielerListe um eine pos nach hinten
 * Ueberprüft NICHT! ob der Eintrag wirklich aus dieser Liste stammt -> das kann in einem segmentation fault resultieren
 */
void playerlist_movedown(PLAYERLIST *aPlayerList,PLAYERLISTENTRY *aEntry)
{
    if (aEntry == NULL || aEntry->next == NULL)
        return;
   	//eventuell hier statt dessen vertauschfunktion playercard_swap verwenden

    //Zur doku (x=aEntry): 6 Aenderungen muessen vorgenommen werden
    //
    //alt:      a->x->b->c
    //          a<-x<-b<-c
    //neu:      a->b->x->c
    //          a<-b<-x<-c    
        
    // x<-b wird zu a<-b
    aEntry->next->prev = aEntry->prev;
    
    // a->x wird zu a->b
    if (aPlayerList->first == aEntry)
        aPlayerList->first = aEntry->next;
    else    
        aEntry->prev->next = aEntry->next;

    // a<-x wird zu b<-x
    aEntry->prev = aEntry->next;
    
    // b<-c wird zu x<-c
    if (aPlayerList->last == aEntry->next)
        aPlayerList->last = aEntry;
    else
        aEntry->next->next->prev = aEntry;
    
    // x->b wird zu x->c
    aEntry->next = aEntry->prev->next;  
    
    // b->c wird zu b->x
    aEntry->prev->next = aEntry;
}


/**
 * Verschiebt einen eintrag in einer SpielerListe vor einen anderen
 * Ueberprüft NICHT! ob der Eintrag wirklich aus dieser Liste stammt -> das kann in einem segmentation fault resultieren
 */
void playerlist_movebefore(PLAYERLIST *aPlayerList,PLAYERLISTENTRY *aEntry,PLAYERLISTENTRY *anotherEntry)
{
    if (aEntry == NULL || anotherEntry == NULL || aEntry == anotherEntry || aEntry->next == anotherEntry)
        return;

   	//eventuell hier statt dessen vertauschfunktion playercard_swap verwenden

    /* Zuerst den eintrag an seiner aktuellen position ausklinken (ohne ihn zu loeschen) */
    if (aPlayerList->first == aEntry)
        aPlayerList->first = aEntry->next;
    else 
        aEntry->prev->next = aEntry->next;
        
    
    if (aPlayerList->last == aEntry)
        aPlayerList->last = aEntry->prev;
    else 
        aEntry->next->prev = aEntry->prev;
    
    /* Dann den Eintrag an neuer stelle einfuegen */
    if (aPlayerList->first == anotherEntry)
        aPlayerList->first = aEntry;
    else
        anotherEntry->prev->next = aEntry;
    
    aEntry->prev = anotherEntry->prev;
    anotherEntry->prev = aEntry;
    aEntry->next = anotherEntry;
}

/**
 * Verschiebt einen eintrag in einer SpielerListe nach hinten
 * Ueberprüft NICHT! ob der Eintrag wirklich aus dieser Liste stammt -> das kann in einem segmentation fault resultieren
 */
void playerlist_movelast(PLAYERLIST *aPlayerList,PLAYERLISTENTRY *aEntry)
{
    if (aEntry == NULL || aEntry->next == NULL)
        return;
        
   	//eventuell hier statt dessen vertauschfunktion playercard_swap verwenden

    /* Zuerst den eintrag an seiner aktuellen position ausklinken (ohne ihn zu loeschen) */
    if (aPlayerList->first == aEntry)
        aPlayerList->first = aEntry->next;
    else 
        aEntry->prev->next = aEntry->next;
        
    aEntry->next->prev = aEntry->prev;
    
    /* Dann den Eintrag hinten anhaengen */
    
    aPlayerList->last->next = aEntry;
    aEntry->prev = aPlayerList->last;
    aEntry->next = NULL;
    aPlayerList->last = aEntry;
}

/**
 * Teilt eine Spielerliste ab einem eintrag und kopiert die ueberzaehligen spieler ans ende einer
 * anderen (leeren) Liste
 */
void playerlist_split(PLAYERLIST *aPlayerList,PLAYERLIST *anotherPlayerList,int pos)
{
    PLAYERLISTENTRY *aEntry,*anotherEntry;

    if (pos < 0 || aPlayerList->size <= pos)
        return;
    
    aEntry = playerlist_get(aPlayerList, pos);
    
    /* vorgaenger vom eintrag*/
    anotherEntry = aEntry->prev;
    /* neuer vorgaenger ist jetzt letzter der zweiten liste */
    aEntry->prev = anotherPlayerList->last;
    /* vorgaenger oder anfang der zweiten liste auf eintrag linken */
    if ( aEntry->prev == NULL )
        anotherPlayerList->first = aEntry;
    else
        aEntry->prev->next = aEntry;
    /* ende der zweiten liste auf neues Ende linken*/
    anotherPlayerList->last  = aPlayerList->last;
    /* ende der ersten liste wird alter vorgaenger */
    aPlayerList->last = anotherEntry;
    /* vorgaenger als ende markieren, falls kein vorgaenger dann erste liste = leere liste */
    if (anotherEntry == NULL)
        aPlayerList->first = NULL;
    else
        anotherEntry->next = NULL;
    
    anotherPlayerList->size += aPlayerList->size - pos;
    aPlayerList->size = pos;
}

/**
 * Verschiebt einen eintrag in einer PaarungsListe um eine pos nach hinten
 * Ueberprüft NICHT! ob der Eintrag wirklich aus dieser Liste stammt -> das kann in einem segmentation fault resultieren
 */
void pairinglist_movedown(PAIRINGLIST *aPairingList,PAIRINGLISTENTRY *aEntry)
{
    if (aEntry == NULL || aEntry->next == NULL)
        return;
   	//eventuell hier statt dessen vertauschfunktion playercard_swap verwenden

    //Zur doku (x=aEntry): 6 Aenderungen muessen vorgenommen werden
    //
    //alt:      a->x->b->c
    //          a<-x<-b<-c
    //neu:      a->b->x->c
    //          a<-b<-x<-c    
        
    // x<-b wird zu a<-b
    aEntry->next->prev = aEntry->prev;
    
    // a->x wird zu a->b
    if (aPairingList->first == aEntry)
        aPairingList->first = aEntry->next;
    else    
        aEntry->prev->next = aEntry->next;

    // a<-x wird zu b<-x
    aEntry->prev = aEntry->next;
    
    // b<-c wird zu x<-c
    if (aPairingList->last == aEntry->next)
        aPairingList->last = aEntry;
    else
        aEntry->next->next->prev = aEntry;
    
    // x->b wird zu x->c
    aEntry->next = aEntry->prev->next;  
    
    // b->c wird zu b->x
    aEntry->prev->next = aEntry;
}
