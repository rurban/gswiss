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
 * Vergleichsoperator
 */
typedef struct playercardcomparator
{
  int (*compareFunction) (PLAYERCARD *, PLAYERCARD *);
  struct playercardcomparator *next;
} PLAYERCARDCOMPARATOR;

/**
 * Standardcomparators
 */
#define COMPARATOR_OUTPUT 0
#define COMPARATOR_TABLE 1
#define COMPARATOR_GROUPSPLIT 2
#define COMPARATOR_GROUP 3
#define COMPARATOR_SUBGROUPSPLIT 4
#define COMPARATOR_TARGETQUALCHECK 5

/**
 * Vergleichsfunktion fuer zwei spieler nach punkten,
 * Spieler mit mehr Punkten sind "kleiner"
 */
int comparePlayercardForPointsDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach punkten,
 * Spieler mit mehr Punkten sind "groesser"
 */
int comparePlayercardForPointsAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach spielerid
 */
int comparePlayercardForIdDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach spielerid
 */
int comparePlayercardForIdAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzwertung,
 * Spieler mit groesserer Wertung sind "kleiner"
 */
int comparePlayercardForBuchholzDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzwertung,
 * Spieler mit groesserer Wertung sind "groesser"
 */
int comparePlayercardForBuchholzAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzsummenwertung,
 * Spieler mit groesserer Wertung sind "kleiner"
 */
int comparePlayercardForBuchholzsumDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach buchholzsummenwertung,
 * Spieler mit groesserer Wertung sind "groesser"
 */
int comparePlayercardForBuchholzsumAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach farbe,
 * Spieler mit weiss sind "kleiner"
 */
int comparePlayercardForColorAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach farbe,
 * Spieler mit weiss sind "groesser"
 */
int comparePlayercardForColorDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach Sollfarbe,
 * Spieler mit weiss sind "kleiner"
 */
int comparePlayercardForPreferedColorAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach Sollfarbe,
 * Spieler mit weiss sind "groesser"
 */
int comparePlayercardForPreferedColorDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach Gewichtung der Sollfarbe
 */
int comparePlayercardForPreferedColorEmphasisAsc (PLAYERCARD *pc1,
                                                  PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach Gewichtung der Sollfarbe ungekehrt
 */
int comparePlayercardForPreferedColorEmphasisDesc (PLAYERCARD *pc1,
                                                   PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler alphabetsch
 */
int comparePlayercardForNameAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler ungekehrt alphabetsch
 */
int comparePlayercardForNameDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach rating,
 * Spieler mit groesserem Rating sind "groesser"
 */
int comparePlayercardForRatingAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach rating,
 * Spieler mit groesserem Rating sind "kleiner"
 */
int comparePlayercardForRatingDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach titel,
 * Spieler mit hoeherem Titel sind "groesser"
 */
int comparePlayercardForTitleAsc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Vergleichsfunktion fuer zwei spieler nach tite,
 * Spieler mit hoeherem Titel sind "kleiner"
 */
int comparePlayercardForTitleDesc (PLAYERCARD *pc1, PLAYERCARD *pc2);

/**
 * Sortiert eine Liste
 * benoetigt PlayerListe und id der Standard-Vergleichsfunktion
 */
void playerlist_sortWithStandardComparator (PLAYERLIST *aPlayerList,
                                            int comparator);

/**
 * Sortierung einer spielerliste
 */
void playerlist_sort (PLAYERLIST *aPlayerList,
                      PLAYERCARDCOMPARATOR *comparator);

/**
 * Vergleicht 2 Spielerkarten mit einer Standard-Vergleichsfunktion
 */
int comparePlayerCardsWithStandardComparator (PLAYERCARD *aPlayerCard,
                                              PLAYERCARD *anotherPlayerCard,
                                              int comparator);

/**
 * Sortierung einer Paarungsliste nach festem Muster
 */
void pairinglist_sort (PAIRINGLIST *aPairingList);
