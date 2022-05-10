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

PLAYERCARD *playercard_create (int pId, char *name, int rating, int title,
                               int expectedRounds);

PLAYERLISTENTRY *playerlistentry_create (PLAYERCARD *aPlayerCard);

PLAYERLIST *playerlist_create ();

PAIRINGLISTENTRY *pairinglistentry_create (PLAYERCARD *player1,
                                           PLAYERCARD *player2);

PAIRINGLIST *pairinglist_create ();

PLAYERGROUP *playergroup_create ();

PLAYERADMINISTRATION *playeradministration_create ();

void pairinglist_append (PAIRINGLIST *aPairingList, PLAYERCARD *player1,
                         PLAYERCARD *player2);

void playercard_finalize (PLAYERCARD *aPlayerCard);

void playerlistentry_finalize (PLAYERLISTENTRY *aEntry);

void playerlist_clear (PLAYERLIST *aPlayerList);

void playerlist_finalize (PLAYERLIST *aPlayerList);

void pairinglistentry_finalize (PAIRINGLISTENTRY *aEntry);

void pairinglist_clear (PAIRINGLIST *aPairingList);

void pairinglist_finalize (PAIRINGLIST *aPairingList);

void playergroup_finalize (PLAYERGROUP *aPlayerGroup);

void playergroup_finalizeall (PLAYERGROUP *firstPlayerGroup);

void
playeradministration_ht_insert (PLAYERADMINISTRATION *aPlayerAdministration,
                                PLAYERCARD *aPlayerCard);

PLAYERCARD *
playeradministration_ht_get (PLAYERADMINISTRATION *aPlayerAdministration,
                             int index);

void playerlist_append (PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard);

void playerlist_delete (PLAYERLIST *aPlayerList,
                        PLAYERLISTENTRY *aPlayerListEntry);

int playerlist_remove (PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard);

int playergroup_remove (PLAYERGROUP *aPlayerGroup, PLAYERCARD *aPlayerCard);

void playeradministration_add (PLAYERADMINISTRATION *aPlayerAdministration,
                               PLAYERCARD *aPlayerCard);

void playergroup_add (PLAYERGROUP *aPlayerGroup, PLAYERCARD *aPlayerCard);

void playergroup_insertbehind (PLAYERGROUP *aPlayerGroup,
                               PLAYERGROUP *anotherPlayerGroup);

PLAYERLISTENTRY *playerlist_get (PLAYERLIST *aPlayerList, int pos);

PLAYERLISTENTRY *playerlist_getentry (PLAYERLIST *aPlayerList,
                                      PLAYERCARD *aPlayerCard);

PLAYERLISTENTRY *playerlist_getwithid (PLAYERLIST *aPlayerList, int pId);

void playercard_setReservedRounds (PLAYERCARD *aPlayerCard,
                                   int reservedRounds);

void playercard_setOpponent (PLAYERCARD *aPlayerCard, int aRound,
                             int opponentId);

void playercard_setResult (PLAYERCARD *aPlayerCard, int aRound, char result);

void playercard_setColor (PLAYERCARD *aPlayerCard, int aRound, int color);

void playercard_setRound (PLAYERCARD *aPlayerCard, int aRound, int opponentId,
                          int color, char result);

void playercard_swap (PLAYERCARD **aPlayerCard,
                      PLAYERCARD **anotherPlayerCard);

void playerlist_movedown (PLAYERLIST *aPlayerList, PLAYERLISTENTRY *aEntry);

void playerlist_movebefore (PLAYERLIST *aPlayerList, PLAYERLISTENTRY *aEntry,
                            PLAYERLISTENTRY *anotherEntry);

void playerlist_movelast (PLAYERLIST *aPlayerList, PLAYERLISTENTRY *aEntry);

void playerlist_split (PLAYERLIST *aPlayerList, PLAYERLIST *anotherPlayerList,
                       int pos);

void playerlist_moveall (PLAYERLIST *sourcePlayerList,
                         PLAYERLIST *targetPlayerList);

void pairinglist_moveall (PAIRINGLIST *sourcePairingList,
                          PAIRINGLIST *targetPairingList);

void pairinglist_movedown (PAIRINGLIST *aPairingList,
                           PAIRINGLISTENTRY *aEntry);
