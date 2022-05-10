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
#include "include/listing.h"
#include "include/gswiss.h"
#include "include/language.h"

/**
 * copy the default string value of a title constant
 * to givem string-address
 */
void
getTitleStr (int titleId, char *title)
{

  switch (titleId)
    {
    case TITLE_IGM:
      strcpy (title, "IGM");
      break;
    case TITLE_WGM:
      strcpy (title, "WGM");
      break;
    case TITLE_IM:
      strcpy (title, "IM");
      break;
    case TITLE_WIM:
      strcpy (title, "WIM");
      break;
    case TITLE_FM:
      strcpy (title, "FM");
      break;
    case TITLE_WFM:
      strcpy (title, "WFM");
      break;
    case TITLE_NO:
      strcpy (title, "");
      break;
    }
}

/**
 *  List the players of a playerlist
 */
int
listPlayerList (PLAYERLIST *aPlayerList, FILE *file)
{
  PLAYERLISTENTRY *aPlayerListEntry;
  int j = 0;
  char playertitle[4];
  for (aPlayerListEntry = aPlayerList->first; aPlayerListEntry != NULL;
       aPlayerListEntry = aPlayerListEntry->next)
    {
      getTitleStr (aPlayerListEntry->aPlayerCard->title, playertitle);
      fprintf (file, L_TABLEFORMAT, ++j, aPlayerListEntry->aPlayerCard->pId,
               aPlayerListEntry->aPlayerCard->name,
               aPlayerListEntry->aPlayerCard->rating, playertitle,
               ((double)aPlayerListEntry->aPlayerCard->points) / 2,
               ((double)aPlayerListEntry->aPlayerCard->buchholz) / 2,
               ((double)aPlayerListEntry->aPlayerCard->buchholzsum) / 2);
    }
  return j;
}

/**
 * List the table of the given round
 */
void
listTable (PLAYERLIST *aPlayerList, FILE *file, int playedRounds)
{
  fprintf (file, L_TABLE, playedRounds);
  fprintf (file, L_TABLEHEADER);
  listPlayerList (aPlayerList, file);
}

/**
 * List the pairings of a pairinglist
 */
int
listPairingList (PAIRINGLIST *aPairngList, FILE *file)
{
  PAIRINGLISTENTRY *aPairingListEntry;
  int number = 0;
  for (aPairingListEntry = aPairngList->first; aPairingListEntry != NULL;
       aPairingListEntry = aPairingListEntry->next)
    {
      fprintf (
          file, L_PAIRINGSFORMAT, ++number, aPairingListEntry->player1->pId,
          aPairingListEntry->player1->name,
          ((double)aPairingListEntry->player1->points) / 2,
          L_COLORSHORT_WHITE
              - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                 * aPairingListEntry->player1->preferedColor),
          aPairingListEntry->player1->preferedColorEmphasis,
          aPairingListEntry->player2->pId, aPairingListEntry->player2->name,
          ((double)aPairingListEntry->player2->points) / 2,
          L_COLORSHORT_WHITE
              - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                 * aPairingListEntry->player2->preferedColor),
          aPairingListEntry->player2->preferedColorEmphasis,
          aPairingListEntry->result == '+'
              ? "+/-"
              : (aPairingListEntry->result == '-'
                     ? "-/+"
                     : (aPairingListEntry->result == '='
                            ? "1/2"
                            : (aPairingListEntry->result == '1'
                                   ? "1:0"
                                   : (aPairingListEntry->result == '0'
                                          ? "0:1"
                                          : "   ")))));
    }
  if (aPairngList->byeplayer != NULL)
    {
      fprintf (file, L_PAIRINGSBYE, aPairngList->byeplayer->pId,
               aPairngList->byeplayer->name,
               ((double)aPairngList->byeplayer->points) / 2,
               L_COLORSHORT_WHITE
                   - ((L_COLORSHORT_WHITE - L_COLORSHORT_BLACK)
                      * aPairngList->byeplayer->preferedColor),
               aPairngList->byeplayer->preferedColorEmphasis, L_BYENAME);
      number++;
    }
  return number;
}

/**
 * List the pairings of the given round
 */
int
listPairings (PAIRINGLIST *aPairngList, FILE *file, int playedRounds)
{
  fprintf (file, L_PAIRINGS, playedRounds + 1);
  fprintf (file, L_PAIRINGSHEADER);
  return listPairingList (aPairngList, file);
}

/**
 * List the results of the given round
 */
int
listResults (PAIRINGLIST *aPairngList, FILE *file, int playedRounds)
{
  fprintf (file, L_RESULTS, playedRounds);
  fprintf (file, L_PAIRINGSHEADER);
  return listPairingList (aPairngList, file);
}

/**
 * List a playergroup
 */
void
listGroup (PLAYERGROUP *aPlayerGroup, FILE *file)
{
  fprintf (file, "Subgroup1:\n");
  if (!listPlayerList (aPlayerGroup->subgroup1, file))
    fprintf (file, "  ---\n");
  fprintf (file, "Subgroup2:\n");
  if (!listPlayerList (aPlayerGroup->subgroup2, file))
    fprintf (file, "  ---\n");
  fprintf (file, "Pairings:\n");
  if (!listPairingList (aPlayerGroup->pairedplayers, file))
    fprintf (file, "  ---\n");
  fprintf (file, "\n");
}

/**
 * List all playergroups, starting at first group
 */
void
listAllGroups (PLAYERGROUP *firstPlayerGroup, FILE *file)
{
  PLAYERGROUP *aPlayerGroup;
  int i = 0;
  for (aPlayerGroup = firstPlayerGroup; aPlayerGroup != NULL;
       aPlayerGroup = aPlayerGroup->next)
    {
      fprintf (file, "Gruppe %d:\n", ++i);
      listGroup (aPlayerGroup, file);
    }
}
