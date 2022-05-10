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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/structdef.h"
#include "include/basiclistoperations.h"
#include "include/language.h"
#include "include/sorting.h"

/**
 * Vergleichsfunktion fuer zwei spieler nach punkten,
 * Spieler mit mehr Punkten sind "kleiner"
 */
int
comparePlayercardForPointsDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->points - pc1->points;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach punkten,
 * Spieler mit mehr Punkten sind "groesser"
 */
int
comparePlayercardForPointsAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->points - pc2->points;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach spielerid
 */
int
comparePlayercardForIdDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->pId - pc1->pId;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach spielerid
 */
int
comparePlayercardForIdAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->pId - pc2->pId;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzwertung,
 * Spieler mit groesserer Wertung sind "kleiner"
 */
int
comparePlayercardForBuchholzDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->buchholz - pc1->buchholz;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzwertung,
 * Spieler mit groesserer Wertung sind "groesser"
 */
int
comparePlayercardForBuchholzAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->buchholz - pc2->buchholz;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzsummenwertung,
 * Spieler mit groesserer Wertung sind "kleiner"
 */
int
comparePlayercardForBuchholzsumDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->buchholzsum - pc1->buchholzsum;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzsummenwertung,
 * Spieler mit groesserer Wertung sind "groesser"
 */
int
comparePlayercardForBuchholzsumAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->buchholzsum - pc2->buchholzsum;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach farbe,
 * Spieler mit weiss sind "kleiner"
 */
int
comparePlayercardForColorAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->nextColor - pc2->nextColor;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach farbe,
 * Spieler mit weiss sind "groesser"
 */
int
comparePlayercardForColorDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->nextColor - pc1->nextColor;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach Sollfarbe,
 * Spieler mit weiss sind "kleiner"
 */
int
comparePlayercardForPreferedColorAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->preferedColor - pc2->preferedColor;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach Sollfarbe,
 * Spieler mit weiss sind "groesser"
 */
int
comparePlayercardForPreferedColorDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->preferedColor - pc1->preferedColor;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach Gewichtung der Sollfarbe
 */
int
comparePlayercardForPreferedColorEmphasisAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->preferedColorEmphasis - pc2->preferedColorEmphasis;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach Gewichtung der Sollfarbe ungekehrt
 */
int
comparePlayercardForPreferedColorEmphasisDesc (PLAYERCARD *pc1,
                                               PLAYERCARD *pc2)
{
  return pc2->preferedColorEmphasis - pc1->preferedColorEmphasis;
}

/**
 * Vergleichsfunktion fuer zwei spieler alphabetsch
 */
int
comparePlayercardForNameAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return strcmp (pc1->name, pc2->name);
}

/**
 * Vergleichsfunktion fuer zwei spieler ungekehrt alphabetsch
 */
int
comparePlayercardForNameDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return strcmp (pc2->name, pc1->name);
}

/**
 * Vergleichsfunktion fuer zwei spieler nach rating,
 * Spieler mit groesserem Rating sind "groesser"
 */
int
comparePlayercardForRatingAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->rating - pc2->rating;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach rating,
 * Spieler mit groesserem Rating sind "kleiner"
 */
int
comparePlayercardForRatingDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->rating - pc1->rating;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach titel,
 * Spieler mit hoeherem Titel sind "groesser"
 */
int
comparePlayercardForTitleAsc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc1->title - pc2->title;
}

/**
 * Vergleichsfunktion fuer zwei spieler nach tite,
 * Spieler mit hoeherem Titel sind "kleiner"
 */
int
comparePlayercardForTitleDesc (PLAYERCARD *pc1, PLAYERCARD *pc2)
{
  return pc2->title - pc1->title;
}

/**
 * Neues "Vergleicher"-Objekt erzeugen
 */
PLAYERCARDCOMPARATOR *
pccomparator_create (int (*compareFunction) (PLAYERCARD *, PLAYERCARD *))
{
  PLAYERCARDCOMPARATOR *newPlayerCardComparator;

  if ((newPlayerCardComparator
       = (PLAYERCARDCOMPARATOR *)malloc (sizeof (PLAYERCARDCOMPARATOR)))
      == NULL)
    fprintf (stderr, L_MEMERROR_PLAYERCARD);

  newPlayerCardComparator->compareFunction = compareFunction;
  newPlayerCardComparator->next = NULL;

  return newPlayerCardComparator;
}

/**
 * Neues "Vergleicher"-Objekt erzeugen und an bestehendes anhaengen
 */
PLAYERCARDCOMPARATOR *
pccomparator_createAndAppend (PLAYERCARDCOMPARATOR *prevComparator,
                              int (*compareFunction) (PLAYERCARD *,
                                                      PLAYERCARD *))
{
  PLAYERCARDCOMPARATOR *newPlayerCardComparator;

  if ((newPlayerCardComparator
       = (PLAYERCARDCOMPARATOR *)malloc (sizeof (PLAYERCARDCOMPARATOR)))
      == NULL)
    fprintf (stderr, L_MEMERROR_PLAYERCARD);

  newPlayerCardComparator->compareFunction = compareFunction;
  newPlayerCardComparator->next = NULL;

  prevComparator->next = newPlayerCardComparator;

  return newPlayerCardComparator;
}

/**
 * "Vergleicher"-Objekt löschen
 */
void
pccomparator_finalize (PLAYERCARDCOMPARATOR *aComparator)
{
  PLAYERCARDCOMPARATOR *anotherComparator = NULL;

  while (aComparator != NULL)
    {
      anotherComparator = aComparator->next;
      free (aComparator);
      aComparator = anotherComparator;
    }
}

/**
 * compare 2 playercards throug a comparator
 */
int
pccomparator_compare (PLAYERCARDCOMPARATOR *aComparator,
                      PLAYERCARD *aPlayerCard, PLAYERCARD *anotherPlayerCard)
{
  int result = aComparator->compareFunction (aPlayerCard, anotherPlayerCard);

  while (result == 0 && aComparator->next != NULL)
    {
      aComparator = aComparator->next;
      result = aComparator->compareFunction (aPlayerCard, anotherPlayerCard);
    }
  return result;
}

/**
 * he default comparators were cached in
 * memory to speed up equal calls from different sources
 */
PLAYERCARDCOMPARATOR *standardComparator_table = NULL;
PLAYERCARDCOMPARATOR *standardComparator_output = NULL;
PLAYERCARDCOMPARATOR *standardComparator_groupsplit = NULL;
PLAYERCARDCOMPARATOR *standardComparator_group = NULL;
PLAYERCARDCOMPARATOR *standardComparator_subgroupsplit = NULL;
PLAYERCARDCOMPARATOR *standardComparator_targetqualcheck = NULL;

PLAYERCARDCOMPARATOR *
getStandardComparator (int id)
{
  switch (id)
    {
    case COMPARATOR_TABLE:
      if (standardComparator_table == NULL)
        {
          standardComparator_table
              = pccomparator_create (comparePlayercardForPointsDesc);
          pccomparator_createAndAppend (
              pccomparator_createAndAppend (
                  pccomparator_createAndAppend (
                      pccomparator_createAndAppend (
                          standardComparator_table,
                          comparePlayercardForBuchholzDesc),
                      comparePlayercardForBuchholzsumDesc),
                  comparePlayercardForRatingDesc),
              comparePlayercardForTitleDesc);
        }
      return standardComparator_table;
    case COMPARATOR_OUTPUT:
      if (standardComparator_output == NULL)
        standardComparator_output
            = pccomparator_create (comparePlayercardForIdAsc);
      return standardComparator_output;
    case COMPARATOR_GROUPSPLIT:
      if (standardComparator_groupsplit == NULL)
        standardComparator_groupsplit
            = pccomparator_create (comparePlayercardForPointsDesc);
      return standardComparator_groupsplit;
    case COMPARATOR_GROUP:
      if (standardComparator_group == NULL)
        {
          standardComparator_group
              = pccomparator_create (comparePlayercardForPointsDesc);
          pccomparator_createAndAppend (
              pccomparator_createAndAppend (
                  pccomparator_createAndAppend (
                      standardComparator_group,
                      comparePlayercardForRatingDesc),
                  comparePlayercardForTitleDesc),
              comparePlayercardForNameAsc);
        }
      return standardComparator_group;
    case COMPARATOR_SUBGROUPSPLIT:
      if (standardComparator_subgroupsplit == NULL)
        {
          standardComparator_subgroupsplit
              = pccomparator_create (comparePlayercardForPreferedColorAsc);
          pccomparator_createAndAppend (
              pccomparator_createAndAppend (
                  pccomparator_createAndAppend (
                      pccomparator_createAndAppend (
                          pccomparator_createAndAppend (
                              standardComparator_subgroupsplit,
                              comparePlayercardForPreferedColorEmphasisDesc),
                          comparePlayercardForPointsDesc),
                      comparePlayercardForRatingDesc),
                  comparePlayercardForTitleDesc),
              comparePlayercardForNameAsc);
        }
      return standardComparator_subgroupsplit;
    case COMPARATOR_TARGETQUALCHECK:
      if (standardComparator_targetqualcheck == NULL)
        {
          standardComparator_targetqualcheck
              = pccomparator_create (comparePlayercardForPreferedColorDesc);
          pccomparator_createAndAppend (
              pccomparator_createAndAppend (
                  pccomparator_createAndAppend (
                      pccomparator_createAndAppend (
                          pccomparator_createAndAppend (
                              standardComparator_targetqualcheck,
                              comparePlayercardForPreferedColorEmphasisAsc),
                          comparePlayercardForPointsDesc),
                      comparePlayercardForRatingDesc),
                  comparePlayercardForTitleDesc),
              comparePlayercardForNameAsc);
        }
      return standardComparator_targetqualcheck;
    default:
      return NULL;
    }
}

/**
 * Sortiert eine Liste (Bubblesort)
 * benoetigt PlayerListe und sortierfunktion
 */
void
playerlist_bubblesort (PLAYERLIST *aPlayerList,
                       PLAYERCARDCOMPARATOR *comparator)
{
  PLAYERLISTENTRY *currEntry, *stopEntry, *lastMoved;
  int cmp_result;

  /* Der aktuelle Eintrag der immer mit Nachfolger verglichen wird */
  currEntry = aPlayerList->first;
  /* Der Ausschlusseintrag  */
  stopEntry = NULL;
  /* Der letzte bewegte Eintrag */
  lastMoved = NULL;

  /* leere Listen werden nicht sortiert */
  if (currEntry == NULL)
    return;

  while (1)
    {
      /* Durchlauf zuende */
      if (currEntry->next == stopEntry)
        {
          /* keine Aenderung, abbruch */
          if (lastMoved == NULL)
            break;

          /* neuanfangen, ab letztem bewegtem Element ist sortiert */
          stopEntry = lastMoved;
          lastMoved = NULL;
          currEntry = aPlayerList->first;
          continue;
        }

      /* TODO pccomparator_compare() könnte hier nocheinmal nachgebildet werden
         wir müssen so oft vergelichen, das es viel performater
         ist die lokalen variablen hier zu halten, als sie immer neu
         beim aufruf der methode zu reservieren */
      cmp_result = pccomparator_compare (comparator, currEntry->aPlayerCard,
                                         currEntry->next->aPlayerCard);

      /* testen ob wir tauschen */
      if (cmp_result > 0)
        {
          /* TODO: playercard_swap sollte hier nachgebildet werden, wir
             müssen so oft tauschen, das es viel performater
             ist die tauschvariable hier zu halten, als sie immer neu
             beim aufruf von swap zu reservieren*/
          playercard_swap (&(currEntry->aPlayerCard),
                           &(currEntry->next->aPlayerCard));
          /* wir haben getauscht -> weiterrücken*/
          currEntry = currEntry->next;
          lastMoved = currEntry;
        }
      /* bei nicht tausch aufs naechste element weiterrücken */
      else
        currEntry = currEntry->next;
    }
}

/**
 * Sortierung einer spielerliste mit einer Standard-Vergleichsfunktion
 */
void
playerlist_sortWithStandardComparator (PLAYERLIST *aPlayerList, int comparator)
{
  playerlist_bubblesort (aPlayerList, getStandardComparator (comparator));
}

/**
 * Sortierung einer spielerliste
 */
void
playerlist_sort (PLAYERLIST *aPlayerList, PLAYERCARDCOMPARATOR *comparator)
{
  playerlist_bubblesort (aPlayerList, comparator);
}

/**
 * Vergleicht 2 Spielerkarten mit einer Standard-Vergleichsfunktion
 */
int
comparePlayerCardsWithStandardComparator (PLAYERCARD *aPlayerCard,
                                          PLAYERCARD *anotherPlayerCard,
                                          int comparator)
{
  return pccomparator_compare (getStandardComparator (comparator), aPlayerCard,
                               anotherPlayerCard);
}

/**
 * Sortierung einer Paarungsliste nach festem Muster (Bubblesort)
 */
void
pairinglist_sort (PAIRINGLIST *aPairingList)
{
  PAIRINGLISTENTRY *currEntry, *stopEntry, *lastMoved;
  int cmp_result;

  /* Der aktuelle Eintrag der immer mit Nachfolger verglichen wird */
  currEntry = aPairingList->first;
  /* Der Ausschlusseintrag  */
  stopEntry = NULL;
  /* Der letzte bewegte Eintrag */
  lastMoved = NULL;

  /* leere Listen werden nicht sortiert */
  if (currEntry == NULL)
    return;

  while (1)
    {
      /* Durchlauf zuende */
      if (currEntry->next == stopEntry)
        {
          /* keine Aenderung, abbruch */
          if (lastMoved == NULL)
            break;

          /* neuanfangen, ab letztem bewegtem Element ist sortiert */
          stopEntry = lastMoved;
          lastMoved = NULL;
          currEntry = aPairingList->first;
          continue;
        }

      /* ein festgelegter ablauf von gestaffelten vergleichen, da ein
         comparator hier nicht anwendbar ist (die zu vergleichenden objekte
         sind keine playerListEntries)*/
      cmp_result = comparePlayercardForPointsDesc (
          currEntry->player1->points > currEntry->player2->points
              ? currEntry->player1
              : currEntry->player2,
          currEntry->next->player1->points > currEntry->next->player2->points
              ? currEntry->next->player1
              : currEntry->next->player2);
      if (cmp_result == 0)
        {
          cmp_result = comparePlayercardForPointsDesc (
              currEntry->player1->points < currEntry->player2->points
                  ? currEntry->player1
                  : currEntry->player2,
              currEntry->next->player1->points
                      < currEntry->next->player2->points
                  ? currEntry->next->player1
                  : currEntry->next->player2);
        }
      if (cmp_result == 0)
        {
          cmp_result = comparePlayerCardsWithStandardComparator (
              comparePlayerCardsWithStandardComparator (
                  currEntry->player1, currEntry->player2, COMPARATOR_GROUP)
                      > 0
                  ? currEntry->player2
                  : currEntry->player1,
              comparePlayerCardsWithStandardComparator (
                  currEntry->next->player1, currEntry->next->player2,
                  COMPARATOR_GROUP)
                      > 0
                  ? currEntry->next->player2
                  : currEntry->next->player1,
              COMPARATOR_GROUP);
        }

      /* testen ob wir tauschen */
      if (cmp_result > 0)
        {
          /* tauschen */
          playercard_swap (&(currEntry->player1), &(currEntry->next->player1));
          playercard_swap (&(currEntry->player2), &(currEntry->next->player2));
          currEntry->result ^= currEntry->next->result;
          currEntry->next->result ^= currEntry->result;
          currEntry->result ^= currEntry->next->result;
          /* wir haben getauscht -> weiterrücken */
          currEntry = currEntry->next;
          lastMoved = currEntry;
        }
      /* bei nicht tausch aufs naechste element weiterrücken */
      else
        currEntry = currEntry->next;
    }
}
