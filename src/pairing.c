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

#include "include/structdef.h"
#include "include/basiclistoperations.h"
#include "include/sorting.h"
#include "include/pairing.h"
#include "include/gswiss.h"
#include "include/language.h"

int local_playedRounds = 0;
FILE *local_debugTarget = NULL;
int pairing_optimizeFactor;

/**
 * Zwei Spieler dürfen nicht mehr als einmal gegeneinander gepaart werden
 */
int
pair_checkrule_havenotyetplayed (PLAYERCARD *pCard1, PLAYERCARD *pCard2)
{
  int i;

  if (pCard1->pId == pCard2->pId)
    return 0;

  for (i = 0; i < local_playedRounds; i++)
    {
      if (pCard1->opponents[i] == pCard2->pId)
        return 0;
    }
  return 1;
}

/**
 * - Die Farbdifferenz keines Spielers darf groesser als +2 oder kleiner als -2
 * werden.
 * - Kein Spieler darf dreimal hintereinander mit der gleichen Farbe spielen.
 */
int
pair_checkrule_cangetnextcolor (PLAYERCARD *pCard1, int nextColor)
{
  if (pCard1->preferedColorEmphasis > 2 && nextColor != pCard1->preferedColor)
    return 0;
  else
    return 1;
}

/**
 * Spieler haben noch nicht gespielt und es ist eine Farbzuteilung dieser
 * Paarung möglich die keine absoluten Farb-Regeln verletzt
 */
int
pair_checkrule_canplay (PLAYERCARD *pCard1, PLAYERCARD *pCard2)
{
  return pair_checkrule_havenotyetplayed (pCard1, pCard2)
         && (pCard1->preferedColor != pCard2->preferedColor
             || pair_checkrule_cangetnextcolor (pCard1,
                                                pCard1->preferedColor ? 0 : 1)
             || pair_checkrule_cangetnextcolor (
                 pCard2, pCard2->preferedColor ? 0 : 1));
}

/**
 * Ein Spieler darf immer heruntergelost werden
 */
int
pair_checkrule_candownfloat (PLAYERCARD *pCard1)
{
  /* players can always downfloat, but this does low the quality */
  return 1;
}

/**
 * Ein Spieler, der einen kampflosen Punkt erhalten hat, entweder durch
 * Freilos, oder weil sein Gegner nicht rechtzeitig erschien, darf nicht
 * nochmals das Freilos (kampflos) erhalten.
 */
int
pair_checkrule_canrecievebye (PLAYERCARD *pCard1)
{
  if (pCard1->hasRecievedFreePoint)
    return 0;
  else
    return 1;
}

/**
 * Prüft die "Qualität" dieser Paarung nach Farbzuteilungsgesichtspunkten
 *  0 : optimale Qualität -> beide Spieler erhalten ihre Sollfarbe
 * >0 : ein Spieler erhält nicht seine Sollfarbe -> die Gewichtung der
 * Sollfarbe entspricht die Qualitätseinbuße
 */
int
pair_checkquality_color (PLAYERCARD *pCard1, PLAYERCARD *pCard2)
{
  return pCard1->preferedColor != pCard2->preferedColor
             ? 0
             : (pCard1->preferedColorEmphasis > pCard2->preferedColorEmphasis
                    ? pCard2->preferedColorEmphasis
                    : pCard1->preferedColorEmphasis);
}

/**
 * Prüft die "Qualität" einer Auslosung nach dem Gesichtspunkt wie oft ein
 * Spieler "Upfloater" war 0 : optimale Qualität -> Spieler war in den letzte
 * beiden Runden kein Upfloater 1 : Spieler war in der vorletzten Runde
 * Upfloater 2 : Spieler war in der letzten Runde Upfloater 3 : Spieler war in
 * den letzte beiden Runden Upfloater
 */
int
pair_checkquality_upfloat (PLAYERCARD *pCard)
{
  /* check if it is good to upfloat */
  return pCard->floaterstatus < 0 ? 0 - pCard->floaterstatus : 0;
}

/**
 * Prüft die "Qualität" einer Auslosung nach dem Gesichtspunkt wie oft ein
 * Spieler "Downfloater" war 0 : optimale Qualität -> Spieler war in den letzte
 * beiden Runden kein Downfloater 1 : Spieler war in der vorletzten Runde
 * Downfloater 2 : Spieler war in der letzten Runde Downfloater 3 : Spieler war
 * in den letzte beiden Runden Downfloater
 */
int
pair_checkquality_downfloat (PLAYERCARD *pCard)
{
  /* check if it is good to downfloat */
  return pCard->floaterstatus > 0 ? pCard->floaterstatus : 0;
}

/**
 * Prüft die "Qualität" einer Paarung unter dem Gesichtspunkt des
 * "Floaterstatus"-wie oft war welcher Spieler Up-/Downfolater
 */
int
pair_checkquality_float (PLAYERCARD *pCard1, PLAYERCARD *pCard2)
{
  if (pCard2->points == pCard1->points)
    return 0;
  else if (pCard2->points > pCard1->points)
    return pair_checkquality_downfloat (pCard2)
           + pair_checkquality_upfloat (pCard1);
  else
    return pair_checkquality_downfloat (pCard1)
           + pair_checkquality_upfloat (pCard2);
}

/**
 * Prüft die Qualität einer Paarung
 */
int
pair_checkquality_pair (PLAYERCARD *pCard1, PLAYERCARD *pCard2)
{
  return pair_checkquality_color (pCard1, pCard2)
         + pair_checkquality_float (pCard1, pCard2);
}

/**
 * Hat ein Spieler in einer Liste einen möglichen Gegener, gegen den
 * er nach absoluten Kriterien gepaart werden kann
 */
int
pair_playerHasPossibleOpponentInList (PLAYERCARD *aPlayerCard,
                                      PLAYERLIST *aPlayerList)
{
  PLAYERLISTENTRY *strapEntry;
  for (strapEntry = aPlayerList->first; strapEntry != NULL;
       strapEntry = strapEntry->next)
    {
      if (strapEntry->aPlayerCard != aPlayerCard
          && pair_checkrule_canplay (strapEntry->aPlayerCard, aPlayerCard))
        {
          return 1;
        }
    }
  return 0;
}

/**
 * Ueberpruefung ob jeder Spieler einen erlaubten gegner hat
 * das ergebnis ist NULL wenn alles ok ist oder der PLAYELISTENTRY der nicht
 * gepaart werden konnte
 */
PLAYERCARD *
pair_extractPlayerWithoutPossibleOpponent (PLAYERGROUP *aPlayerGroup)
{
  PLAYERLISTENTRY *checkEntry;
  PLAYERCARD *resultCard;
  for (checkEntry = aPlayerGroup->subgroup1->first; checkEntry != NULL;
       checkEntry = checkEntry->next)
    {
      if ((!pair_playerHasPossibleOpponentInList (checkEntry->aPlayerCard,
                                                  aPlayerGroup->subgroup1))
          && (!pair_playerHasPossibleOpponentInList (checkEntry->aPlayerCard,
                                                     aPlayerGroup->subgroup2)))
        {
          resultCard = checkEntry->aPlayerCard;
          playerlist_delete (aPlayerGroup->subgroup1, checkEntry);
          return resultCard;
        }
    }
  return NULL;
}

/**
 * Generiert die Gruppeneinteilung aus der Spielerliste,
 * gibt die erste Gruppe einer Gruppen-Kette zurück
 */
PLAYERGROUP *
pair_createpointgroups (PLAYERLIST *aPlayerList)
{
  PLAYERGROUP *aGroup, *anotherGroup, *firstGroup;
  PLAYERLISTENTRY *aEntry;

  int currentPoints = -1;
  aGroup = playergroup_create ();
  firstGroup = aGroup;
  /* einfach nach punkten sortieren um gruppen zu splitten */
  playerlist_sortWithStandardComparator (aPlayerList, COMPARATOR_GROUPSPLIT);

  for (aEntry = aPlayerList->first; aEntry != NULL; aEntry = aEntry->next)
    {
      /** initialisieren mit maximaler Punktanzahl*/
      if (currentPoints == -1)
        {
          currentPoints = aEntry->aPlayerCard->points;
        }
      /** wenn weniger Punkte als vorher dann neue Gruppe */
      if (currentPoints > aEntry->aPlayerCard->points)
        {
          anotherGroup = aGroup;
          aGroup = playergroup_create ();
          anotherGroup->next = aGroup;
          aGroup->prev = anotherGroup;
          currentPoints = aEntry->aPlayerCard->points;
        }
      playerlist_append (aGroup->subgroup1, aEntry->aPlayerCard);
    }
  return firstGroup;
}

/**
 * Runterlosen aller restlichen Spieler
 */
void
downfloatAllRemainingPlayers (PLAYERGROUP *aPlayerGroup)
{
  if (aPlayerGroup->next == NULL)
    {
      printf ("cant downfloat players\n");
      return;
    }
  playerlist_moveall (aPlayerGroup->subgroup2, aPlayerGroup->next->subgroup1);
}

/**
 * gepaarte heterogene Gruppe splitten und Restspieler in
 * neuer Gruppe dahinter anhängen
 */
void
splitHeterogenGroup (PLAYERGROUP *aPlayerGroup)
{
  PLAYERGROUP *aGroup;

  if (aPlayerGroup->subgroup2->size > 0)
    {
      aGroup = aPlayerGroup->next;
      aPlayerGroup->next = pair_createpointgroups (aPlayerGroup->subgroup2);
      aPlayerGroup->next->prev = aPlayerGroup;
      playerlist_clear (aPlayerGroup->subgroup2);

      if (aGroup != NULL)
        {
          while (aPlayerGroup->next != NULL)
            aPlayerGroup = aPlayerGroup->next;

          aPlayerGroup->next = aGroup;
          aGroup->prev = aPlayerGroup;
        }
    }
}

/**
 * Die Subgroups einer Gruppe abgleichen, so dass
 * Gruppe2 = Gruppe1 (+1 falls ungerade)
 * Falls dadurch Spieler in Subgruppen geraten, deren Farben sie nicht annehmen
 * können werden gleich mehr Spieler in Gruppe 2 verschoben da sowieso
 * runtergelost werden muss
 */
void
balanceSubgroups (PLAYERGROUP *aPlayerGroup)
{
  PLAYERLISTENTRY *aEntry;
  /* zuviele weisse */
  if (aPlayerGroup->subgroup1->size > aPlayerGroup->subgroup2->size)
    {
      int toMove
          = (aPlayerGroup->subgroup1->size - aPlayerGroup->subgroup2->size)
            / 2;
      int modResult
          = (aPlayerGroup->subgroup1->size - aPlayerGroup->subgroup2->size)
            % 2;
      toMove += modResult;
      /* fuer jeden spieler der transferiert wird aber mit schwarz nicht
         spielen darf, kann gleich ein 2ter weisser spieler mit verschoben
         werden, da eine paarung mit dem verschobenen nicht möglich ist */
      aEntry = playerlist_get (aPlayerGroup->subgroup1,
                               aPlayerGroup->subgroup1->size - toMove
                                   + modResult);
      for (; aEntry != NULL; aEntry = aEntry->next)
        {
          if (aEntry->aPlayerCard->preferedColorEmphasis > 2)
            {
              toMove++;
            }
        }
      playerlist_split (aPlayerGroup->subgroup1, aPlayerGroup->subgroup2,
                        aPlayerGroup->subgroup1->size - toMove);
    }
  /* zuviele schwarze */
  else if (aPlayerGroup->subgroup1->size < aPlayerGroup->subgroup2->size - 1)
    {
      int toMove
          = (aPlayerGroup->subgroup2->size - aPlayerGroup->subgroup1->size)
            / 2;
      /* jeder spieler der transferiert werden soll aber mit weiss nicht
         spielen darf, braucht nicht transferiert werden, sondern bleibt bei
         schwarz und wird herabgestuft*/
      aEntry = playerlist_get (aPlayerGroup->subgroup2,
                               aPlayerGroup->subgroup2->size - toMove);
      for (; aEntry != NULL; aEntry = aEntry->next)
        {
          if (aEntry->aPlayerCard->preferedColorEmphasis > 2)
            {
              toMove--;
            }
        }
      playerlist_split (aPlayerGroup->subgroup2, aPlayerGroup->subgroup1,
                        aPlayerGroup->subgroup2->size - toMove);
    }
}

/**
 * Gruppe in 2 Subgruppen splitten
 */
void
splitGroup (PLAYERGROUP *aPlayerGroup)
{
  PLAYERLISTENTRY *aEntry;
  int splitPos = 0;
  int maxPointsInGroup;

  /* Leere Gruppe -> nix zu tun*/
  if (aPlayerGroup->subgroup1->size == 0)
    return;

  /* Die Splitposition ermitteln (=^ Position an der ein Spieler weniger Punkte
   * hat als sein Vorgaenger)*/
  maxPointsInGroup = aPlayerGroup->subgroup1->first->aPlayerCard->points;

  for (aEntry = aPlayerGroup->subgroup1->first; aEntry != NULL;
       aEntry = aEntry->next)
    {
      if (aEntry->aPlayerCard->points < maxPointsInGroup)
        {
          break;
        }
      else
        splitPos++;
    }

  /*
   * wenn der Schalter isHeterogen auf -1 gestellt ist (dh diese Gruppe darf
   * nicht heterogen sein, da wir schon wieder in der Rückrunde der Auslosung
   * sind) oder wenn splitPos = 0 ist (alle spieler gleiche punktzahl) oder
   * splitpos > haelfte aller spieler dieser Gruppe dann ist das eine Homogene
   * gruppe -> subgruppe 1 enthaelt die haelfte der spieler (abgerundet)
   */
  if (aPlayerGroup->isHeterogen < 0 || splitPos == 0
      || splitPos > (aPlayerGroup->subgroup1->size / 2))
    {
      /* homogene gruppe wird nach Sollfarben und Farbgewichtung geordnet
         weiss oben swarz unten (an der nahtstelle wird gesplitted)
         und innerhalb der farben die hohen gewichtungen oben (ab da gibt es
         gruppen standardsortierung) */
      playerlist_sortWithStandardComparator (aPlayerGroup->subgroup1,
                                             COMPARATOR_SUBGROUPSPLIT);

      splitPos = 0;

      for (aEntry = aPlayerGroup->subgroup1->first; aEntry != NULL;
           aEntry = aEntry->next)
        {
          if (aEntry->aPlayerCard->preferedColor)
            {
              break;
            }
          else
            splitPos++;
        }
      /* Gruppe splitten */
      playerlist_split (aPlayerGroup->subgroup1, aPlayerGroup->subgroup2,
                        splitPos);
      /* Subgruppen abgleichen */
      balanceSubgroups (aPlayerGroup);
    }
  else
    {
      aPlayerGroup->isHeterogen = 1;

      /* Gruppe splitten */
      playerlist_split (aPlayerGroup->subgroup1, aPlayerGroup->subgroup2,
                        splitPos);
    }
}

/**
 * Notice the current pairing in the pairinglist of this group.
 * Current pairing means that all players in subgroup1 are paired with the
 * player in subgroup2 at the same pos
 */
void
noticeCurrentPairing (PLAYERGROUP *aPlayerGroup)
{
  PLAYERLISTENTRY *aEntry;
  PLAYERLISTENTRY *anotherEntry = NULL;

  pairinglist_clear (aPlayerGroup->pairedplayers);

  for (aEntry = aPlayerGroup->subgroup1->first; aEntry != NULL;
       aEntry = aEntry->next)
    {
      if (anotherEntry == NULL)
        anotherEntry = aPlayerGroup->subgroup2->first;
      else
        anotherEntry = anotherEntry->next;

      pairinglist_append (aPlayerGroup->pairedplayers, aEntry->aPlayerCard,
                          anotherEntry->aPlayerCard);
    }
}

/**
 * Die Gegner und Farben an den Spielerkarten setzen
 */
void
confirmPairing (PAIRINGLISTENTRY *aEntry)
{
  aEntry->player1->nextColor = aEntry->player1->preferedColor;
  aEntry->player2->nextColor = aEntry->player2->preferedColor;
  if (aEntry->player1->nextColor == aEntry->player2->nextColor)
    {
      /* Spieler 1 hat höhere Gewichtung */
      if (aEntry->player1->preferedColorEmphasis
          > aEntry->player2->preferedColorEmphasis)
        {
          aEntry->player2->nextColor = aEntry->player2->nextColor == 1 ? 0 : 1;
        }
      /* Spieler 2 hat höhere Gewichtung */
      else if (aEntry->player1->preferedColorEmphasis
               < aEntry->player2->preferedColorEmphasis)
        {
          aEntry->player1->nextColor = aEntry->player1->nextColor == 1 ? 0 : 1;
        }
      else
        {
          /* Regel:
                 - Die Farben werden gegenüber der am weitesten zurückliegenden
             Runde, in der die Spieler mit unterschiedlichen Farben spielten,
             gewechselt.
                 - Die Farbpräferenz des höherrangigen Spielers wird gewährt.*/
          int i = local_playedRounds - 1;
          int j = i;
          int colorsSet = 0;
          while (i >= 0 && j >= 0)
            {
              /* vor dem vergleich die kampflosen Runden herausfiltern */
              while (aEntry->player1->results[i] == '-'
                     || aEntry->player1->results[i] == '+')
                {
                  i--;
                  if (i < 0)
                    break;
                }
              /* vor dem vergleich die kampflosen Runden herausfiltern */
              while (aEntry->player2->results[j] == '-'
                     || aEntry->player2->results[j] == '+')
                {
                  j--;
                  if (j < 0)
                    break;
                }

              if (i < 0 || j < 0)
                break;
              /* runde mit unterschiedlichen Farben */
              if (aEntry->player1->colors[i] != aEntry->player2->colors[j])
                {
                  aEntry->player1->nextColor = aEntry->player2->colors[j];
                  aEntry->player2->nextColor
                      = aEntry->player1->nextColor == 1 ? 0 : 1;
                  colorsSet = 1;
                  break;
                }
              j--;
              i--;
            }
          /* nie unterschiedliche farben
             hoeherrangiger spieler  erhält Sollfarbe */
          if (colorsSet == 0)
            {
              if (comparePlayerCardsWithStandardComparator (
                      aEntry->player1, aEntry->player2, COMPARATOR_GROUP)
                  <= 0)
                aEntry->player2->nextColor
                    = aEntry->player2->nextColor == 1 ? 0 : 1;
              else
                aEntry->player1->nextColor
                    = aEntry->player1->nextColor == 1 ? 0 : 1;
            }
        }
    }
  /* wenn spieler 1 schwarz dann tauschen */
  if (aEntry->player1->nextColor)
    {
      playercard_swap (&(aEntry->player1), &(aEntry->player2));
    }
}

/**
 * Stores the found pairing as (temp) final pairing,
 * which means all paired players are removed form this group and
 * all unpaired players are moved to subgroup 2 to downfloat or get bye
 */
void
storeCurrentPairing (PLAYERGROUP *aPlayerGroup)
{
  PAIRINGLISTENTRY *aEntry;

  /* remove all paired players from the 2 subgroups */
  for (aEntry = aPlayerGroup->pairedplayers->first; aEntry != NULL;
       aEntry = aEntry->next)
    {
      confirmPairing (aEntry);
      playergroup_remove (aPlayerGroup, aEntry->player1);
      playergroup_remove (aPlayerGroup, aEntry->player2);
    }
  /* move all unpoaired players which remain in subgroup 1 to subgroup 2 */
  playerlist_moveall (aPlayerGroup->subgroup1, aPlayerGroup->subgroup2);
}

/**
 * cancels the current pairing in a group which means that all entrys in
 * pairinglist were deleted and the players were moved to subgroup1
 */
void
cancelCurrentPairing (PLAYERGROUP *aPlayerGroup)
{
  PAIRINGLISTENTRY *aEntry;

  /* remove all paired players from the 2 subgroups */
  for (aEntry = aPlayerGroup->pairedplayers->first; aEntry != NULL;
       aEntry = aEntry->next)
    {
      playerlist_append (aPlayerGroup->subgroup1, aEntry->player1);
      playerlist_append (aPlayerGroup->subgroup1, aEntry->player2);
    }
  pairinglist_clear (aPlayerGroup->pairedplayers);
}

void
checkPairingRek (PLAYERGROUP *aPlayerGroup, PLAYERLISTENTRY *entry1,
                 PLAYERLISTENTRY *entry2, int *targetQuality,
                 int *currentQuality, int quality, int *count)
{
  /* Auslosung zeitoptimieren, nach jeweils x schritten die Targetquality
   * anheben */
  if (pairing_optimizeFactor)
    {
      *count = *count + 1;
      if (*count / pairing_optimizeFactor > 0)
        {
          *count = 0;
          *targetQuality = *targetQuality + 1;
          if (local_debugTarget != NULL)
            fprintf (local_debugTarget, L_VERBOSEMSG_PUSHMAXQUAL,
                     *targetQuality, pairing_optimizeFactor);
        }
    }

  /* wenn wir bei 0 sind oder in einer heterogenen gruppe alle sub1-eintraege
     abgearbeitet haben muessen wir nur noch die aktuelle quali speichern -
     ansonsten wären wir nicht hier */
  if (entry2 == NULL || (aPlayerGroup->isHeterogen == 1 && entry1 == NULL))
    {
      *currentQuality = quality;
      noticeCurrentPairing (aPlayerGroup);
    }
  else
    {
      int thisquality;
      PLAYERLISTENTRY *strapEntry;

      for (strapEntry = entry2; strapEntry != NULL;
           strapEntry = strapEntry->next)
        {
          /* tauschen wenn noetig */
          if (strapEntry != entry2)
            playercard_swap (&(entry2->aPlayerCard),
                             &(strapEntry->aPlayerCard));

          /* check at this pos  */
          if (entry1 != NULL)
            {
              if (!(pair_checkrule_canplay (entry1->aPlayerCard,
                                            entry2->aPlayerCard)))
                thisquality = -1;
              else
                thisquality = pair_checkquality_pair (entry1->aPlayerCard,
                                                      entry2->aPlayerCard);
            }
          else
            {
              /* wenn wir letzte gruppe sind dann muss dieser Spieler freilos
                 bekommen koennen
                 ansonsten muss der Spieler downfloater werden koennen */
              if ((aPlayerGroup->next == NULL
                   && !(pair_checkrule_canrecievebye (entry2->aPlayerCard)))
                  || !(pair_checkrule_candownfloat (entry2->aPlayerCard)))
                thisquality = -1;
              else
                thisquality
                    = pair_checkquality_downfloat (entry2->aPlayerCard);
            }

          /* wenn wir noch die chance haben ein speicherbares ergebnis zu
           * liefern dann rekursion */
          if (thisquality >= 0
              && (*currentQuality < 0
                  || thisquality + quality < *currentQuality))
            {
              checkPairingRek (aPlayerGroup,
                               entry1 == NULL ? entry1 : entry1->next,
                               entry2->next, targetQuality, currentQuality,
                               thisquality + quality, count);
            }

          /* zuruecktauschen ist pflicht */
          if (strapEntry != entry2)
            playercard_swap (&(entry2->aPlayerCard),
                             &(strapEntry->aPlayerCard));

          /* wenn inzwischen ein brauchbares ergebniss geliefert wird dann
           * abbruch */
          if (0 <= *currentQuality && *currentQuality <= *targetQuality)
            return;
        }
    }
}

/**
 * In homogenen gruppen koennen spieler zwischen den Gruppen beliebig wechseln
 * in heterogene Gruppen nur austauschen wenn beide spieler die gleiche
 * punktzahl haben (das ist der Fall wenn wegen 'Unpaarbarkeit' ein spieler aus
 * untergruppe 1 in 2 wandern musste)
 */
int
pair_canexchange (int heterogenGroup, PLAYERCARD *card1, PLAYERCARD *card2)
{
  if (heterogenGroup > 0 && card1->points != card2->points)
    return 0;
  else
    return 1;
}

/**
 * find the max reachable quality, called target quality
 */
int
findTargetQuality (PLAYERGROUP *aPlayerGroup)
{
  int targetQual = 0;

  PLAYERLISTENTRY *entry = NULL;
  PLAYERLISTENTRY *entry2 = NULL;

  /* in heterogenen Gruppen ist die qualität vermindert um die
    downfloatQualitaet aller in der gruppe vorhandenen downfloater*/
  if (aPlayerGroup->isHeterogen == 1)
    {
      for (entry = aPlayerGroup->subgroup1->first; entry != NULL;
           entry = entry->next)
        {
          targetQual += pair_checkquality_downfloat (entry->aPlayerCard);
        }
    }
  /* in homogenen gruppen ist die qualität vermindert um alle
     zielfarbe schwarz in subgroup1 und zielfarbe weiss in subgroup2 (bis zur
     groesse von sub1) downfloater werden hier nicht eingerechnet, da in
     gruppen in denen es auf möglichst exakte targetquality ankommt (sehr
     grosse gruppen) das verhältnis von floatern zu nicht floatern sehr groß
     ist */
  else
    {
      /* Sortierungseihenfolge in sub2 temporär ändern, alle schwarzen muessen
        oben sein
        und von den weissen muessen die mit schwacher sollfarbe oben sein */
      playerlist_sortWithStandardComparator (aPlayerGroup->subgroup2,
                                             COMPARATOR_TARGETQUALCHECK);

      entry2 = aPlayerGroup->subgroup2->first;
      for (entry = aPlayerGroup->subgroup1->first; entry != NULL;
           entry = entry->next)
        {
          if (entry->aPlayerCard->preferedColor == 1)
            targetQual += entry->aPlayerCard->preferedColorEmphasis;
          if (entry2->aPlayerCard->preferedColor == 0)
            targetQual += entry2->aPlayerCard->preferedColorEmphasis;
          entry2 = entry2->next;
        }
      /* zuruecksortieren nach standard guppensortierung */
      playerlist_sortWithStandardComparator (aPlayerGroup->subgroup2,
                                             COMPARATOR_GROUP);
    }
  return targetQual;
}

/**
 * Find a pairing of this group, use the methods
 * of resorting group 2 (implicit) and of exchanging one
 * player between group 1 and 2
 */
int
findPairingTroughExchangingPlayers (PLAYERGROUP *aPlayerGroup)
{
  int combNr = aPlayerGroup->subgroup2->size * aPlayerGroup->subgroup1->size;

  int i, posG1, posG2;
  PLAYERLISTENTRY *entry1, *entry2;

  int quality = -1;
  int laufCount = 0;

  int targetQuality = findTargetQuality (aPlayerGroup);
  if (local_debugTarget != NULL)
    fprintf (local_debugTarget, L_VERBOSEMSG_DETMAXQUAL, targetQuality);

  /* zuerst in orginalaufteilung testen */
  checkPairingRek (aPlayerGroup, aPlayerGroup->subgroup1->first,
                   aPlayerGroup->subgroup2->first, &targetQuality, &quality, 0,
                   &laufCount);

  for (i = 0; i < combNr; i++)
    {
      /* wenn zielquality ist erreicht dann abbruch */
      if (0 <= quality && quality <= targetQuality)
        break;

      posG1 = i / aPlayerGroup->subgroup2->size;
      posG2 = i % aPlayerGroup->subgroup2->size;
      entry1 = playerlist_get (aPlayerGroup->subgroup1, posG1);
      entry2 = playerlist_get (aPlayerGroup->subgroup2, posG2);
      if (pair_canexchange (aPlayerGroup->isHeterogen, entry1->aPlayerCard,
                            entry2->aPlayerCard))
        {
          playercard_swap (&(entry1->aPlayerCard), &(entry2->aPlayerCard));
          /* test pairing */
          checkPairingRek (aPlayerGroup, aPlayerGroup->subgroup1->first,
                           aPlayerGroup->subgroup2->first, &targetQuality,
                           &quality, 0, &laufCount);
          playercard_swap (&(entry1->aPlayerCard), &(entry2->aPlayerCard));
        }
    }
  /* TODO: Maybe we can try here to exchange 2 players
           if no possible pairing is found yet*/
  return quality;
}

/**
 * move last player of subgrou1 to subgroup2
 */
void
moveLastOfGroup1 (PLAYERGROUP *aPlayerGroup)
{
  playerlist_split (aPlayerGroup->subgroup1, aPlayerGroup->subgroup2,
                    aPlayerGroup->subgroup1->size - 1);
}

/**
 * move all found pairings to a new resultlist,
 * after that you can delete the groups
 */
PAIRINGLIST *
pair_movepairings2resultlist (PLAYERGROUP *firstPlayerGroup)
{
  PAIRINGLIST *resultPairingList;
  PLAYERGROUP *aPlayerGroup;

  resultPairingList = pairinglist_create ();

  for (aPlayerGroup = firstPlayerGroup; aPlayerGroup != NULL;
       aPlayerGroup = aPlayerGroup->next)
    {
      /* umlinken der paarungen */
      pairinglist_moveall (aPlayerGroup->pairedplayers, resultPairingList);
      /* letzte gruppe auf freilosspieler ueberpruefen */
      if (aPlayerGroup->next == NULL && aPlayerGroup->subgroup2->size > 0)
        {
          resultPairingList->byeplayer
              = aPlayerGroup->subgroup2->first->aPlayerCard;
          playerlist_remove (aPlayerGroup->subgroup2,
                             aPlayerGroup->subgroup2->first->aPlayerCard);
        }
    }
  return resultPairingList;
}

int
pair_canChangePairingReplace1 (PLAYERCARD *aPlayerCard,
                               PAIRINGLISTENTRY *aPairingListEntry)
{
  return pair_checkrule_canplay (aPlayerCard, aPairingListEntry->player2)
         && aPlayerCard->points == aPairingListEntry->player1->points
         && pair_checkquality_pair (aPlayerCard, aPairingListEntry->player2)
                <= pair_checkquality_pair (aPairingListEntry->player1,
                                           aPairingListEntry->player2);
}

int
pair_canChangePairingReplace2 (PLAYERCARD *aPlayerCard,
                               PAIRINGLISTENTRY *aPairingListEntry)
{
  return pair_checkrule_canplay (aPlayerCard, aPairingListEntry->player1)
         && aPlayerCard->points == aPairingListEntry->player2->points
         && pair_checkquality_pair (aPlayerCard, aPairingListEntry->player1)
                <= pair_checkquality_pair (aPairingListEntry->player1,
                                           aPairingListEntry->player2);
}

PLAYERCARD *
pair_findAndExchangeNonByePlayer (PLAYERCARD *nonByePlayer,
                                  PLAYERGROUP *lastGroup)
{
  PAIRINGLISTENTRY *pairingListEntry;
  for (lastGroup = lastGroup->prev; lastGroup != NULL;
       lastGroup = lastGroup->prev)
    {
      for (pairingListEntry = lastGroup->pairedplayers->first;
           pairingListEntry != NULL; pairingListEntry = pairingListEntry->next)
        {
          if (pair_canChangePairingReplace1 (nonByePlayer, pairingListEntry)
              && pair_checkrule_canrecievebye (pairingListEntry->player1))
            {
              playercard_swap (&nonByePlayer, &(pairingListEntry->player1));
              confirmPairing (pairingListEntry);
              return nonByePlayer;
            }
          if (pair_canChangePairingReplace2 (nonByePlayer, pairingListEntry)
              && pair_checkrule_canrecievebye (pairingListEntry->player2))
            {
              playercard_swap (&nonByePlayer, &(pairingListEntry->player2));
              confirmPairing (pairingListEntry);
              return nonByePlayer;
            }
        }
    }
  return NULL;
}

PLAYERCARD *
pair_findAndExchangeNonPairablePlayer (PLAYERCARD *nonPairablePlayer,
                                       PLAYERGROUP *currentGroup)
{
  PLAYERGROUP *aPlayerGroup;
  PAIRINGLISTENTRY *pairingListEntry;

  for (aPlayerGroup = currentGroup->prev; aPlayerGroup != NULL;
       aPlayerGroup = aPlayerGroup->prev)
    {
      for (pairingListEntry = aPlayerGroup->pairedplayers->first;
           pairingListEntry != NULL; pairingListEntry = pairingListEntry->next)
        {
          if (pair_canChangePairingReplace1 (nonPairablePlayer,
                                             pairingListEntry)
              && (pair_playerHasPossibleOpponentInList (
                      pairingListEntry->player1, currentGroup->subgroup1)
                  || pair_playerHasPossibleOpponentInList (
                      pairingListEntry->player1, currentGroup->subgroup2)))
            {
              playercard_swap (&nonPairablePlayer,
                               &(pairingListEntry->player1));
              confirmPairing (pairingListEntry);
              return nonPairablePlayer;
            }
          if (pair_canChangePairingReplace2 (nonPairablePlayer,
                                             pairingListEntry)
              && (pair_playerHasPossibleOpponentInList (
                      pairingListEntry->player2, currentGroup->subgroup1)
                  || pair_playerHasPossibleOpponentInList (
                      pairingListEntry->player2, currentGroup->subgroup2)))
            {
              playercard_swap (&nonPairablePlayer,
                               &(pairingListEntry->player2));
              confirmPairing (pairingListEntry);
              return nonPairablePlayer;
            }
        }
    }
  return NULL;
}

/**
 * Auslosungsverfahren erste Runde
 */
PAIRINGLIST *
pairFirstRound (PLAYERGROUP *firstPlayerGroup)
{
  PLAYERGROUP *aPlayerGroup;
  PLAYERLISTENTRY *aEntry, *anotherEntry;
  PAIRINGLIST *resultPairingList;

  int firstColor = 0; /* TODO need Random ? */
  int splitPos;

  resultPairingList = pairinglist_create ();
  /* jede gruppe paaren*/
  for (aPlayerGroup = firstPlayerGroup; aPlayerGroup != NULL;
       aPlayerGroup = aPlayerGroup->next)
    {
      /* Gruppe wird nach standard vergleich sortiert */
      playerlist_sortWithStandardComparator (aPlayerGroup->subgroup1,
                                             COMPARATOR_GROUP);
      splitPos = aPlayerGroup->subgroup1->size / 2;
      playerlist_split (aPlayerGroup->subgroup1, aPlayerGroup->subgroup2,
                        splitPos);

      aEntry = aPlayerGroup->subgroup1->first;
      anotherEntry = aPlayerGroup->subgroup2->first;
      while (aEntry != NULL)
        {
          if (firstColor == 0)
            pairinglist_append (resultPairingList, aEntry->aPlayerCard,
                                anotherEntry->aPlayerCard);
          else
            pairinglist_append (resultPairingList, anotherEntry->aPlayerCard,
                                aEntry->aPlayerCard);

          firstColor = firstColor == 0 ? 1 : 0;
          aEntry = aEntry->next;
          anotherEntry = anotherEntry->next;
        }
      if (anotherEntry != NULL)
        {
          if (aPlayerGroup->next != NULL)
            {
              playerlist_append (aPlayerGroup->next->subgroup1,
                                 anotherEntry->aPlayerCard);
            }
          else
            {
              resultPairingList->byeplayer = anotherEntry->aPlayerCard;
            }
        }
      playerlist_clear (aPlayerGroup->subgroup1);
      playerlist_clear (aPlayerGroup->subgroup2);
    }
  return resultPairingList;
}

/**
 * Auslosungsverfahren
 */
PAIRINGLIST *
pair (PLAYERGROUP *firstPlayerGroup)
{
  PLAYERGROUP *aPlayerGroup;
  PLAYERCARD *aPlayerCard, *anotherPlayerCard;
  int grpCounter = 1;
  int quality = -1;
  int pairingsingroup = 0;

  if (local_debugTarget != NULL)
    fprintf (local_debugTarget, L_VERBOSEMSG_START);

  /* jede gruppe paaren*/
  for (aPlayerGroup = firstPlayerGroup; aPlayerGroup != NULL;)
    {
      if (local_debugTarget != NULL)
        fprintf (local_debugTarget, L_VERBOSEMSG_WORKGRP, grpCounter,
                 aPlayerGroup->subgroup1->size);

      /**
       * Nicht paarbare spieler nach unten verschieben / Freilos zuweisen
       */
      while ((aPlayerCard
              = pair_extractPlayerWithoutPossibleOpponent (aPlayerGroup))
             != NULL)
        {
          if (local_debugTarget != NULL)
            fprintf (local_debugTarget, L_VERBOSEMSG_FOUNDNPP,
                     aPlayerCard->pId);

          /* ersetzung falls:
                es ist die letzte Gruppe und die gruppe ist leer (nachdem
             dieser Spieler entfernt wurde) und dieser Spieler ist nicht
             Freilosfähig und ersetungsspieler ist freilosfähig*/
          if (aPlayerGroup->next == NULL && aPlayerGroup->subgroup1->size == 0
              && aPlayerGroup->subgroup2->size == 0)
            {
              if (!pair_checkrule_canrecievebye (aPlayerCard))
                {
                  anotherPlayerCard = pair_findAndExchangeNonByePlayer (
                      aPlayerCard, aPlayerGroup);
                  if (anotherPlayerCard != NULL)
                    {
                      aPlayerCard = anotherPlayerCard;
                      if (local_debugTarget != NULL)
                        fprintf (local_debugTarget, L_VERBOSEMSG_SWAPNPPBYE,
                                 aPlayerCard->pId);
                    }
                  else
                    {
                      if (local_debugTarget != NULL)
                        fprintf (local_debugTarget, L_VERBOSEMSG_HOLDNPPERR);
                    }
                }
              else
                {
                  if (local_debugTarget != NULL)
                    fprintf (local_debugTarget, L_VERBOSEMSG_HOLDNPPOK);
                }
              playerlist_append (aPlayerGroup->subgroup2, aPlayerCard);
            }
          else
            {
              /* Hier suche Nach ersetzung durchführen */
              anotherPlayerCard = pair_findAndExchangeNonPairablePlayer (
                  aPlayerCard, aPlayerGroup);
              if (anotherPlayerCard != NULL)
                {
                  playerlist_append (aPlayerGroup->subgroup1,
                                     anotherPlayerCard);
                  /* Alle aus Gruppe 2 zurueckschieben */
                  playerlist_moveall (aPlayerGroup->subgroup2,
                                      aPlayerGroup->subgroup1);
                  if (local_debugTarget != NULL)
                    fprintf (local_debugTarget, L_VERBOSEMSG_SWAPNPPPAIR,
                             anotherPlayerCard->pId);
                }
              else
                {
                  playerlist_append (aPlayerGroup->subgroup2, aPlayerCard);
                  if (local_debugTarget != NULL)
                    fprintf (local_debugTarget, L_VERBOSEMSG_DOWNFLTNPP);
                }
            }
        }
      /* wenn in der letzten Gruppe Spieler gefunden werden die nicht gepaart
         werden koennen
         werden sie nicht runtergleost sondern sie muessen jetzt wieder in
         subgroup 1 wandern */
      if (aPlayerGroup->subgroup2->size > 0)
        {
          if (aPlayerGroup->next == NULL)
            playerlist_moveall (aPlayerGroup->subgroup2,
                                aPlayerGroup->subgroup1);
          /* nichtpaarbare Spieler runterlosen */
          else
            {
              if (local_debugTarget != NULL)
                fprintf (local_debugTarget, L_VERBOSEMSG_DOWNFLTALLNPP,
                         aPlayerGroup->subgroup2->size);
              downfloatAllRemainingPlayers (aPlayerGroup);
            }
        }

      /* Sortieren nach standard gruppen vergleich */
      playerlist_sortWithStandardComparator (aPlayerGroup->subgroup1,
                                             COMPARATOR_GROUP);

      if (local_debugTarget != NULL)
        fprintf (local_debugTarget, L_VERBOSEMSG_SPLITGRP,
                 aPlayerGroup->subgroup1->size);
      /* gruppen einteilen */
      splitGroup (aPlayerGroup);

      /* subgruppen auch sortieren */
      playerlist_sortWithStandardComparator (aPlayerGroup->subgroup1,
                                             COMPARATOR_GROUP);
      playerlist_sortWithStandardComparator (aPlayerGroup->subgroup2,
                                             COMPARATOR_GROUP);

      if (local_debugTarget != NULL)
        fprintf (local_debugTarget, L_VERBOSEMSG_PAIRGRP,
                 aPlayerGroup->isHeterogen == 1 ? L_VERBOSEMSG_GRPTYPE_HET
                                                : L_VERBOSEMSG_GRPTYPE_HOM,
                 aPlayerGroup->subgroup1->size, aPlayerGroup->subgroup2->size);

      /* schleife solange subgroup1 spieler enthaelt ist keine paarung gefunden
       */
      quality = -1;
      pairingsingroup = 0;
      while (aPlayerGroup->subgroup1->size > 0)
        {
          quality = findPairingTroughExchangingPlayers (aPlayerGroup);
          /* eine moeglichePaarung wurde gefunden */
          if (0 <= quality)
            {
              pairingsingroup = aPlayerGroup->subgroup1->size;
              storeCurrentPairing (aPlayerGroup);
            }
          /* sonderfall letzte Gruppe, da nur ein frielos möglich ist können
           * wir gleich alle in gruppe2 verschieben */
          else if (aPlayerGroup->next == NULL
                   && aPlayerGroup->isHeterogen != 1)
            {
              playerlist_moveall (aPlayerGroup->subgroup1,
                                  aPlayerGroup->subgroup2);
            }
          /* keine moegliche Paarung -> die anzahl der Spieler in SubGroup 1
           * verkleinern */
          else
            {
              moveLastOfGroup1 (aPlayerGroup);
              if (local_debugTarget != NULL)
                fprintf (local_debugTarget, L_VERBOSEMSG_MOVEPLAYER);
            }
        }
      if (local_debugTarget != NULL)
        fprintf (local_debugTarget, L_VERBOSEMSG_PAIRCOMPL, pairingsingroup,
                 quality, aPlayerGroup->subgroup2->size);
      /* heterogeneGruppe splitten und alle Restspielr dahin verschieben
             nur splitten wenn wir wenigstens eine paarung erhalten haben und
         noch ungepaart spieler uebrig sind, */
      if (aPlayerGroup->isHeterogen > 0
          && aPlayerGroup->pairedplayers->first != NULL
          && aPlayerGroup->subgroup2->size > 0)
        {
          if (local_debugTarget != NULL)
            fprintf (local_debugTarget, L_VERBOSEMSG_NEWHOMGRP);
          splitHeterogenGroup (aPlayerGroup);
          /* weiter mit splitgruppe */
          aPlayerGroup = aPlayerGroup->next;
          grpCounter++;
        }
      /* normalfall, es gibt eine naechste geruppe -> alle Restspieler nach
       * unten verschieben  */
      else if (aPlayerGroup->next != NULL)
        {
          if (aPlayerGroup->subgroup2->size > 0 && local_debugTarget != NULL)
            fprintf (local_debugTarget, L_VERBOSEMSG_DOWNFLTTAIL);
          downfloatAllRemainingPlayers (aPlayerGroup);

          /* weiter mit naechster gruppe */
          aPlayerGroup = aPlayerGroup->next;
          grpCounter++;
        }
      /* letzte Gruppe, kein oder nur ein Spieler in SubGroup2, dieser darf
         freilos erhalten - Paarung beendet */
      else if (aPlayerGroup->subgroup2->size == 0
               || (aPlayerGroup->subgroup2->size == 1
                   && pair_checkrule_canrecievebye (
                       aPlayerGroup->subgroup2->first->aPlayerCard)))
        {
          if (local_debugTarget != NULL)
            fprintf (local_debugTarget, L_VERBOSEMSG_LASTCOMPL);
          /* Naechste Gruppe ist dann NULL -> es wird mit Sicherheit
           * abgebrochen */
          aPlayerGroup = aPlayerGroup->next;
          grpCounter++;
        }
      else
        /* letzte gruppe nicht paar-bar, uebergeordnete paarung auflösen (dabei
           leere gruppen ignorieren -> ist noch einzubauen) zusammengeführte
           gruppen als nichtheterogen bezeichene !! wenn kein zusammenführen
           mehr möglich ist also auch die paarung der ersten gruppe aufgelöst
           wurde dann ist der Algorithmus definitiv am ende ! */
        {
          /* Es gibt keine vorhergehende Gruppe Mehr -> keine Paarung findbar
           */
          if (aPlayerGroup->prev == NULL)
            {
              if (local_debugTarget != NULL)
                {
                  fprintf (local_debugTarget, L_VERBOSEMSG_CANTCANCELPREV);
                  fprintf (local_debugTarget, L_VERBOSEMSG_BREAK);
                }
              return NULL;
            }
          /* diese gruppe alle eventuellvorgenommenen paarungen auflösen
             Vorhergehende Gruppe alle Paarungen auflösen
             alle Spieler dieser Gruppe in verhergehende hochschieben
             diese Gruppe löschen
             vorherhegende gruppe als niemals Heterogen definieren */
          else
            {
              if (local_debugTarget != NULL)
                fprintf (local_debugTarget, L_VERBOSEMSG_CANCELPREV);
              cancelCurrentPairing (aPlayerGroup);
              aPlayerGroup = aPlayerGroup->prev;
              cancelCurrentPairing (aPlayerGroup);
              playerlist_moveall (aPlayerGroup->next->subgroup2,
                                  aPlayerGroup->subgroup1);
              playerlist_moveall (aPlayerGroup->next->subgroup1,
                                  aPlayerGroup->subgroup1);
              aPlayerGroup->isHeterogen = -1;
              playergroup_finalize (aPlayerGroup->next);
              aPlayerGroup->next = NULL;
              grpCounter--;
            }
        }
    }
  if (local_debugTarget != NULL)
    fprintf (local_debugTarget, L_VERBOSEMSG_FINISH);
  return pair_movepairings2resultlist (firstPlayerGroup);
}

/**
 * In einer kompletten SpielerVerwaltung Punkte ausrechnen, gegner ueberpreufen
 * ... errorcodes: 0 - alles ok 1 - gegner fehlt 2 - gegner hat anderen gegner
 * in dieser runde 3 - ergebnisse 2er gegner in einer runde passen nicht 4 -
 * farben 2er gegner in einer runde passen nicht
 */
int
playeradministration_evaluate (PLAYERADMINISTRATION *aPlayerAdministration,
                               int playedRounds, int reportErrors)
{
  PLAYERLISTENTRY *aPlayerListEntry;
  PLAYERCARD *opponent;
  /* Zaehlvariablen */
  int i, j;
  /* Farbberechnung/Auswerung */
  int col, lastcol, lastcol2;
  /* freiloszaehler */
  int hasRecievedBye;
  /* zaehler der floaterhistpry*/
  int lastfloater, lastfloater2;
  /* Zahler fuer die Freilosspieler jeder Runde (nur ein Freilosspieler pro
   * Runde erlaubt)*/
  int byeRounds[playedRounds];
  for (i = 0; i < playedRounds; i++)
    byeRounds[i] = 0;

  for (aPlayerListEntry = aPlayerAdministration->player_list->first;
       aPlayerListEntry != NULL; aPlayerListEntry = aPlayerListEntry->next)
    {
      /* punkte aufsummieren, vorher auf 0 setzen */
      aPlayerListEntry->aPlayerCard->points = 0;

      /* farbe berechen */
      aPlayerListEntry->aPlayerCard->colorDifference = 0;
      lastcol = 0;
      lastcol2 = 0;

      /* floaterhistory berechnen*/
      lastfloater = 0;
      lastfloater2 = 0;

      /* freilos bzw kampflosenPunkt erhalten (in runde x)*/
      hasRecievedBye = -1;

      for (i = 0; i < playedRounds; i++)
        {
          /* testen ob gegner dieser runde freilos */
          if (aPlayerListEntry->aPlayerCard->opponents[i] == BYE_ID)
            {
              /* Testen ob bei freilos auch punkt */
              if (aPlayerListEntry->aPlayerCard->results[i] != '+')
                {
                  if (reportErrors)
                    fprintf (stderr, L_EVALERROR_BYEBUTNOPOINT,
                             aPlayerListEntry->aPlayerCard->pId,
                             aPlayerListEntry->aPlayerCard->name, i + 1);
                  return 9;
                }
              /* gegen Freilos bekommt er einen Punkt */
              else
                aPlayerListEntry->aPlayerCard->points += 2;

              /* testen ob nur einmal freilos */
              if (hasRecievedBye > -1)
                {
                  if (reportErrors)
                    fprintf (stderr, L_EVALERROR_BYETWICE,
                             aPlayerListEntry->aPlayerCard->pId,
                             aPlayerListEntry->aPlayerCard->name,
                             hasRecievedBye, i + 1);
                  return 8;
                }

              /* testen ob nur ein Spieler in einer Runde ein Freilos erhielt
               */
              if (byeRounds[i])
                {
                  opponent = (PLAYERCARD *)playeradministration_ht_get (
                      aPlayerAdministration, byeRounds[i]);
                  if (reportErrors)
                    fprintf (stderr, L_EVALERROR_TWOPLAYERSWITHBYE, i + 1,
                             aPlayerListEntry->aPlayerCard->pId,
                             aPlayerListEntry->aPlayerCard->name,
                             opponent->pId, opponent->name);
                  return 10;
                }
              else
                byeRounds[i] = aPlayerListEntry->aPlayerCard->pId;

              hasRecievedBye = i + 1;
              /* freilos merken */
              aPlayerListEntry->aPlayerCard->hasRecievedFreePoint = 1;
              /* freilos als downfloater behandeln */
              lastfloater2 = lastfloater;
              lastfloater = 1;
              aPlayerListEntry->aPlayerCard->floaterhistory[i] = 1;
              continue;
            }

          /* testen ob gegner dieser runde nicht NULL*/
          if ((opponent = (PLAYERCARD *)playeradministration_ht_get (
                   aPlayerAdministration,
                   aPlayerListEntry->aPlayerCard->opponents[i]))
              == NULL)
            {
              if (reportErrors)
                fprintf (stderr, L_EVALERROR_OPPONENTNOTFOUND,
                         aPlayerListEntry->aPlayerCard->pId,
                         aPlayerListEntry->aPlayerCard->name, i + 1,
                         aPlayerListEntry->aPlayerCard->opponents[i]);
              return 1;
            }

          /* testen ob gegner dieser runde hat als gegnerid dieser runde eigene
           * id */
          if (opponent->opponents[i] != aPlayerListEntry->aPlayerCard->pId)
            {
              if (reportErrors)
                fprintf (stderr, L_EVALERROR_OPPONENTNOTMATCH,
                         aPlayerListEntry->aPlayerCard->pId,
                         aPlayerListEntry->aPlayerCard->name, i + 1,
                         opponent->pId, opponent->name,
                         opponent->opponents[i]);
              return 2;
            }

          /* testen ob ergebnis diese runde passt zu gegnerergebnis diese runde
           */
          if ((opponent->results[i] == '+'
               && aPlayerListEntry->aPlayerCard->results[i] != '-')
              || (opponent->results[i] == '-'
                  && aPlayerListEntry->aPlayerCard->results[i] != '+')
              || (opponent->results[i] == '='
                  && aPlayerListEntry->aPlayerCard->results[i] != '=')
              || (opponent->results[i] == '1'
                  && aPlayerListEntry->aPlayerCard->results[i] != '0')
              || (opponent->results[i] == '0'
                  && aPlayerListEntry->aPlayerCard->results[i] != '1'))
            {
              if (reportErrors)
                fprintf (stderr, L_EVALERROR_RESULTSNOTMATCH, i + 1,
                         aPlayerListEntry->aPlayerCard->pId,
                         aPlayerListEntry->aPlayerCard->name,
                         aPlayerListEntry->aPlayerCard->results[i],
                         opponent->pId, opponent->name, opponent->results[i]);
              return 3;
            }

          /* floaterstatus errechnen */
          lastfloater2 = lastfloater;
          /* lastfloater als tempvariable der punktesumme des gegners der
           * vorrunde verwenden */
          lastfloater = 0;
          for (j = 0; j < i; j++)
            {
              switch (opponent->results[j])
                {
                case '0':
                  break;
                case '-':
                  break;
                case '=':
                  lastfloater += 1;
                  break;
                case '1':
                  lastfloater += 2;
                  break;
                case '+':
                  lastfloater += 2;
                  break;
                }
            }
          /* downfloater wenn punktunterschied > */
          if (aPlayerListEntry->aPlayerCard->points > lastfloater)
            lastfloater = 1;
          else if (aPlayerListEntry->aPlayerCard->points < lastfloater)
            lastfloater = -1;
          else
            lastfloater = 0;
          aPlayerListEntry->aPlayerCard->floaterhistory[i] = lastfloater;

          /* testen ob farbe diese runde + gegnerfarbe diese runde = 1 */
          if (opponent->colors[i] + aPlayerListEntry->aPlayerCard->colors[i]
              != 1)
            {
              if (reportErrors)
                fprintf (stderr, L_EVALERROR_COLORSNOTMATCH, i + 1,
                         aPlayerListEntry->aPlayerCard->pId,
                         aPlayerListEntry->aPlayerCard->name,
                         L_COLORSHORT_WHITE
                             - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                                * aPlayerListEntry->aPlayerCard->colors[i]),
                         opponent->pId, opponent->name,
                         L_COLORSHORT_WHITE
                             - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                                * opponent->colors[i]));
              return 4;
            }

          /* Farbevaluierung (was wird als naechste, wurde farbausgleich
             eingehalten) aktuelle farbe (1) schwarz, (-1) weiss */
          col = aPlayerListEntry->aPlayerCard->colors[i];
          if (col == 0)
            col = -1;
          /* auf gleichheit testen =^ farbe 3 mal hintereinander */
          if (col == lastcol && col == lastcol2)
            {
              if (reportErrors)
                fprintf (stderr, L_EVALERROR_COLORTHREETIMIESINSERIES,
                         aPlayerListEntry->aPlayerCard->pId,
                         aPlayerListEntry->aPlayerCard->name,
                         L_COLORSHORT_WHITE
                             - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                                * aPlayerListEntry->aPlayerCard->colors[i]),
                         i - 1, i + 1);
              return 5;
            }
          lastcol2 = lastcol;
          lastcol = col;
          aPlayerListEntry->aPlayerCard->colorDifference += col;

          /* auf maxunterschied 2 testen */
          if (aPlayerListEntry->aPlayerCard->colorDifference < -2
              || aPlayerListEntry->aPlayerCard->colorDifference > 2)
            {
              if (reportErrors)
                fprintf (stderr, L_EVALERROR_COLORTHREETIMIESMORE,
                         aPlayerListEntry->aPlayerCard->pId,
                         aPlayerListEntry->aPlayerCard->name,
                         L_COLORSHORT_WHITE
                             - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                                * aPlayerListEntry->aPlayerCard->colors[i]),
                         L_COLORSHORT_WHITE
                             - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                                * opponent->colors[i]),
                         i + 1);
              return 6;
            }

          /* gegnerevaluierung kein gegner 2 mal */
          for (j = i + 1; j < playedRounds; j++)
            {
              if (aPlayerListEntry->aPlayerCard->opponents[i]
                  == aPlayerListEntry->aPlayerCard->opponents[j])
                {
                  if (reportErrors)
                    fprintf (stderr, L_EVALERROR_OPPONENTTWICE,
                             aPlayerListEntry->aPlayerCard->pId,
                             aPlayerListEntry->aPlayerCard->name,
                             opponent->pId, opponent->name, i + 1, j + 1);
                  return 7;
                }
            }

          /* punkte aufsummieren */
          switch (aPlayerListEntry->aPlayerCard->results[i])
            {
            case '0':
              break;
            case '-':
              break;
            case '=':
              aPlayerListEntry->aPlayerCard->points += 1;
              break;
            case '1':
              aPlayerListEntry->aPlayerCard->points += 2;
              break;
            case '+':
              aPlayerListEntry->aPlayerCard->points += 2;
              /* kampfloser punkt wird gespeichert */
              aPlayerListEntry->aPlayerCard->hasRecievedFreePoint = 1;
              break;
            }
        }

      /* sollfarbe und gewichtung berechnen */

      /* Fall 1: Die letzten beiden Runden gleiche farbe -> zwingend eine
       * andere nehmen*/
      if (lastcol == lastcol2 && lastcol != 0)
        {
          if (lastcol == 1)
            aPlayerListEntry->aPlayerCard->preferedColor = 0;
          else
            aPlayerListEntry->aPlayerCard->preferedColor = 1;

          aPlayerListEntry->aPlayerCard->preferedColorEmphasis = 3;
        }
      /* Fall 2: Farbdifferenz = 2 -> zwingend eine andere nehmen*/
      else if (aPlayerListEntry->aPlayerCard->colorDifference == -2
               || aPlayerListEntry->aPlayerCard->colorDifference == 2)
        {
          if (aPlayerListEntry->aPlayerCard->colorDifference == 2)
            aPlayerListEntry->aPlayerCard->preferedColor = 0;
          else
            aPlayerListEntry->aPlayerCard->preferedColor = 1;
          aPlayerListEntry->aPlayerCard->preferedColorEmphasis = 3;
        }
      /* Fall 3: Farbdifferenz = 1 -> andere farbe wird stark empfohlen */
      else if (aPlayerListEntry->aPlayerCard->colorDifference != 0)
        {
          if (aPlayerListEntry->aPlayerCard->colorDifference == 1)
            aPlayerListEntry->aPlayerCard->preferedColor = 0;
          else
            aPlayerListEntry->aPlayerCard->preferedColor = 1;
          aPlayerListEntry->aPlayerCard->preferedColorEmphasis = 2;
        }
      /* Fall 4: Nicht erste runde -> andere farbe als letzte wird  empfohlen
       */
      else if (lastcol != 0)
        {
          /* Zuletzt schwarz -> weiss */
          if (lastcol == 1)
            aPlayerListEntry->aPlayerCard->preferedColor = 0;
          /* Zuletzt weiss -> schwarz */
          else
            aPlayerListEntry->aPlayerCard->preferedColor = 1;

          aPlayerListEntry->aPlayerCard->preferedColorEmphasis = 1;
        }
      /* Fall 5: defaultfarbe ist weiss (eigentlich egal, da für die erste
       * runde eine Sonderauslosung durchgeführt wird)*/
      else
        {
          aPlayerListEntry->aPlayerCard->preferedColorEmphasis = 0;
          /* Default weiss */
          aPlayerListEntry->aPlayerCard->preferedColor = 0;
        }

      /* aktuellen floaterstatus berechnen */
      if (lastfloater > 0)
        if (lastfloater2 > 0)
          aPlayerListEntry->aPlayerCard->floaterstatus = 3;
        else
          aPlayerListEntry->aPlayerCard->floaterstatus = 2;
      else if (lastfloater < 0)
        if (lastfloater2 < 0)
          aPlayerListEntry->aPlayerCard->floaterstatus = -3;
        else
          aPlayerListEntry->aPlayerCard->floaterstatus = -2;
      else
        aPlayerListEntry->aPlayerCard->floaterstatus = lastfloater2;
    }

  /** Bucholzwertung errechnen */
  for (aPlayerListEntry = aPlayerAdministration->player_list->first;
       aPlayerListEntry != NULL; aPlayerListEntry = aPlayerListEntry->next)
    {
      /* aufsummieren, vorher auf 0 setzen */
      aPlayerListEntry->aPlayerCard->buchholz = 0;

      for (i = 0; i < playedRounds; i++)
        {
          /* testen ob gegner dieser runde freilos */
          if (!aPlayerListEntry->aPlayerCard->opponents[i] == BYE_ID)
            {
              aPlayerListEntry->aPlayerCard->buchholz
                  += ((PLAYERCARD *)playeradministration_ht_get (
                          aPlayerAdministration,
                          aPlayerListEntry->aPlayerCard->opponents[i]))
                         ->points;
            }
        }
    }
  /** Bucholzsummenwertung errechnen */
  for (aPlayerListEntry = aPlayerAdministration->player_list->first;
       aPlayerListEntry != NULL; aPlayerListEntry = aPlayerListEntry->next)
    {
      /* aufsummieren, vorher auf 0 setzen */
      aPlayerListEntry->aPlayerCard->buchholzsum = 0;

      for (i = 0; i < playedRounds; i++)
        {
          /* testen ob gegner dieser runde freilos */
          if (!aPlayerListEntry->aPlayerCard->opponents[i] == BYE_ID)
            {
              aPlayerListEntry->aPlayerCard->buchholzsum
                  += ((PLAYERCARD *)playeradministration_ht_get (
                          aPlayerAdministration,
                          aPlayerListEntry->aPlayerCard->opponents[i]))
                         ->buchholz;
            }
        }
    }
  return 0;
}

void
playeradministration_storepairings (PAIRINGLIST *aResultList)
{
  PAIRINGLISTENTRY *aPairingListEntry;
  for (aPairingListEntry = aResultList->first; aPairingListEntry != NULL;
       aPairingListEntry = aPairingListEntry->next)
    {
      playercard_setRound (aPairingListEntry->player1, local_playedRounds + 1,
                           aPairingListEntry->player2->pId, 0, ' ');
      playercard_setRound (aPairingListEntry->player2, local_playedRounds + 1,
                           aPairingListEntry->player1->pId, 1, ' ');
    }
  if (aResultList->byeplayer != NULL)
    {
      playercard_setRound (aResultList->byeplayer, local_playedRounds + 1,
                           BYE_ID, 0, '+');
    }
}

void
playeradministration_storeresult (PAIRINGLIST *aResultList)
{
  PAIRINGLISTENTRY *aPairingListEntry;

  for (aPairingListEntry = aResultList->first; aPairingListEntry != NULL;
       aPairingListEntry = aPairingListEntry->next)
    {
      playercard_setRound (aPairingListEntry->player1, local_playedRounds + 1,
                           aPairingListEntry->player2->pId, 0,
                           aPairingListEntry->result);
      playercard_setRound (aPairingListEntry->player2, local_playedRounds + 1,
                           aPairingListEntry->player1->pId, 1,
                           aPairingListEntry->result == '0'
                               ? '1'
                               : (aPairingListEntry->result == '1'
                                      ? '0'
                                      : aPairingListEntry->result));
    }
  if (aResultList->byeplayer != NULL)
    {
      playercard_setRound (aResultList->byeplayer, local_playedRounds + 1,
                           BYE_ID, 0, '+');
    }
}

/**
 * Die Auslosung der nächsten Runde durchfuehren
 */
PAIRINGLIST *
playeradministration_alotpairing (PLAYERADMINISTRATION *aPlayerAdministration,
                                  int playedRounds, FILE *debugTarget)
{
  PLAYERGROUP *firstGroup;
  PAIRINGLIST *resultList;

  /* store playedRounds in common var */
  local_playedRounds = playedRounds;
  local_debugTarget = debugTarget;

  if (aPlayerAdministration->player_list->first == NULL)
    return NULL;

  /** Nach Punkten und Wertzahl und Titel und Name sortieren */
  playerlist_sortWithStandardComparator (aPlayerAdministration->player_list,
                                         COMPARATOR_GROUP);

  /* In Gruppen einteilen */
  firstGroup = pair_createpointgroups (aPlayerAdministration->player_list);

  /* Gruppen paaren */
  if (playedRounds == 0)
    resultList = pairFirstRound (firstGroup);
  else
    resultList = pair (firstGroup);

  playergroup_finalizeall (firstGroup);

  if (resultList != NULL)
    {
      pairinglist_sort (resultList);
      playeradministration_storepairings (resultList);
    }

  return resultList;
}

/**
 * Die Resultate einer früheren Runde als Paarungsliste ermitteln
 */
PAIRINGLIST *
playeradministration_createResultList (
    PLAYERADMINISTRATION *aPlayerAdministration, int targetRound)
{
  PAIRINGLIST *resultPairingList;
  PLAYERLISTENTRY *aPlayerListEntry;
  PLAYERCARD *aPlayerCard;

  resultPairingList = pairinglist_create ();

  for (aPlayerListEntry = aPlayerAdministration->player_list->first;
       aPlayerListEntry != NULL; aPlayerListEntry = aPlayerListEntry->next)
    {
      if (aPlayerListEntry->aPlayerCard->opponents[targetRound] == BYE_ID)
        {
          resultPairingList->byeplayer = aPlayerListEntry->aPlayerCard;
        }
      else if (aPlayerListEntry->aPlayerCard->colors[targetRound] == 0)
        {
          aPlayerCard = playeradministration_ht_get (
              aPlayerAdministration,
              aPlayerListEntry->aPlayerCard->opponents[targetRound]);
          pairinglist_append (resultPairingList, aPlayerListEntry->aPlayerCard,
                              aPlayerCard);
          resultPairingList->last->result
              = aPlayerListEntry->aPlayerCard->results[targetRound];
        }
    }
  pairinglist_sort (resultPairingList);
  return resultPairingList;
}
