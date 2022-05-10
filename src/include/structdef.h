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

/**
 * Eine Spielerkarte / a playercard
 */
typedef struct playercard
{
  /** Id des Spielers im Turnier / playerid in tournament */
  int pId;

  /**
   * Speilerdaten / playerdata
   */

  /** Name */
  char name[31];
  /** Wertzahl (Turnierwertzahl) / rating (tournamentrating) */
  int rating;
  /** Titel (umgekehrte Reihenfolge) / title (in reverse order)
   *  0 = kein Titel / no title
   *  1 = WFM
   *  2 = FM
   *  3 = WIM
   *  4 = IM
   *  5 = WGM
   *  6 = IGM
   */
  int title;

  /**
   * gegnerIds und resultate der letzten runden / opponentids and results of
   * the previous rounds
   */

  /** punkte (mal 2) / points (mult. 2)*/
  int points;
  /** buchholzwertung (mal 2) */
  int buchholz;
  /** buchholzsummenwertung (mal 2) */
  int buchholzsum;
  /** gegner / opponents */
  int *opponents;
  /** farben / colors */
  int *colors;
  /** Resultate (0,1,=,+,-) / results */
  char *results;
  /** Schwimmerstatus in bisherigen runden / floaterstatus in previous rounds*/
  int *floaterhistory;
  /** counter fuer die groesse der arrays */
  int reservedRounds;

  /**
   *  Attribute zur Farbermittlung / attributes for determining next color
   *  Definition: 0 = weiss / white
   *              1 = schwarz / black
   */

  /** farbdifferenz / color difference */
  int colorDifference;
  /** ermittelte naechste farbe / determined next color*/
  int nextColor;
  /** bevorzugte farbe / perfered color */
  int preferedColor;
  /** Gewichtung der beforzugten Farbe / emphasis of next color   0-3*/
  int preferedColorEmphasis;

  /**
   *  Attribute zur Auslosung / attributes for drawing
   */

  /** spieler erhielt kampflosen Punkt / player got point without playing */
  int hasRecievedFreePoint;
  /** Schwimmerstatus / floaterstatus:
   * 0 kein floater
   * 3 war letzten 2 runden downfloater
   * 2 war letzte Runde downfloater
   * 1 war vorletzte Runde downfloater
   * -3 war letzten 2 runden upfloater
   * -2 war letzte Runde upfloater
   * -1 war vorletzte Runde upfloater
   */
  int floaterstatus;
} PLAYERCARD;

/**
 * Eintrag in einer Spielerliste / entry in playerlist
 */
typedef struct playerlistentry
{
  struct playercard *aPlayerCard;
  struct playerlistentry *prev;
  struct playerlistentry *next;
} PLAYERLISTENTRY;

/**
 * Spielerliste / playerlist
 */
typedef struct playerlist
{
  struct playerlistentry *first;
  struct playerlistentry *last;
  int size;
} PLAYERLIST;

/**
 * Eintrag in der resultierenden Paarungsliste / entry in pairinglist
 */
typedef struct pairinglistentry
{
  struct playercard *player1;
  struct playercard *player2;
  struct pairinglistentry *next;
  struct pairinglistentry *prev;
  char result;
} PAIRINGLISTENTRY;

/**
 * Spielerliste / playerlist
 */
typedef struct pairinglist
{
  struct pairinglistentry *first;
  struct pairinglistentry *last;
  struct playercard *byeplayer;
} PAIRINGLIST;

/**
 * eine gruppe / a group
 */
typedef struct playergroup
{
  /** untergruppe 1 / subgroup 1 */
  struct playerlist *subgroup1;
  /** untergruppe 2 / subgroup 2 */
  struct playerlist *subgroup2;
  /** fertige paarungen / paired players */
  struct pairinglist *pairedplayers;
  struct playergroup *next;
  struct playergroup *prev;
  int isHeterogen;
} PLAYERGROUP;

/**
 * Spielerverwaltung
 */
typedef struct playeradministration
{
  /* Eine vekettete Liste mit den Spielern */
  struct playerlist *player_list;
  /* eine HashTable mit den Spielern für den Direktzugriff über pID
     =^ Array mit pId als index; toDo: Eventuell durch einen Tree ersetzen */
  struct playercard **player_ht;
  int size_ht;
} PLAYERADMINISTRATION;
