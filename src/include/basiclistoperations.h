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

extern PLAYERCARD* playercard_create(int pId, char *name, int rating, int title, int expectedRounds);

extern PLAYERLISTENTRY* playerlistentry_create(PLAYERCARD *aPlayerCard);

extern PLAYERLIST* playerlist_create();

extern PAIRINGLISTENTRY* pairinglistentry_create(PLAYERCARD *player1, PLAYERCARD *player2);

extern PAIRINGLIST* pairinglist_create();

extern PLAYERGROUP* playergroup_create();

extern PLAYERADMINISTRATION* playeradministration_create();

extern void pairinglist_append(PAIRINGLIST *aPairingList, PLAYERCARD *player1, PLAYERCARD *player2);

extern void playercard_finalize(PLAYERCARD *aPlayerCard);

extern void playerlistentry_finalize(PLAYERLISTENTRY *aEntry);

extern void playerlist_clear(PLAYERLIST *aPlayerList);

extern void playerlist_finalize(PLAYERLIST *aPlayerList);

extern void pairinglistentry_finalize(PAIRINGLISTENTRY *aEntry);

extern void pairinglist_clear(PAIRINGLIST *aPairingList);

extern void pairinglist_finalize(PAIRINGLIST *aPairingList);

extern void playergroup_finalize(PLAYERGROUP *aPlayerGroup);

extern void playergroup_finalizeall(PLAYERGROUP *firstPlayerGroup);

extern void playeradministration_ht_insert(PLAYERADMINISTRATION *aPlayerAdministration, PLAYERCARD *aPlayerCard);

extern PLAYERCARD* playeradministration_ht_get(PLAYERADMINISTRATION *aPlayerAdministration, int index);

extern void playerlist_append(PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard);

extern void playerlist_delete(PLAYERLIST *aPlayerList, PLAYERLISTENTRY *aPlayerListEntry);

extern int playerlist_remove(PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard);

extern int playergroup_remove(PLAYERGROUP *aPlayerGroup, PLAYERCARD *aPlayerCard);

extern void playeradministration_add(PLAYERADMINISTRATION *aPlayerAdministration, PLAYERCARD *aPlayerCard);

extern void playergroup_add(PLAYERGROUP *aPlayerGroup, PLAYERCARD *aPlayerCard);

extern void playergroup_insertbehind(PLAYERGROUP *aPlayerGroup, PLAYERGROUP *anotherPlayerGroup);

extern PLAYERLISTENTRY* playerlist_get(PLAYERLIST *aPlayerList, int pos);

extern PLAYERLISTENTRY* playerlist_getentry(PLAYERLIST *aPlayerList, PLAYERCARD *aPlayerCard);

extern PLAYERLISTENTRY* playerlist_getwithid(PLAYERLIST *aPlayerList, int pId);

extern void playercard_setReservedRounds(PLAYERCARD *aPlayerCard, int reservedRounds);

extern void playercard_setOpponent(PLAYERCARD *aPlayerCard, int aRound, int opponentId);

extern void playercard_setResult(PLAYERCARD *aPlayerCard, int aRound, char result);

extern void playercard_setColor(PLAYERCARD *aPlayerCard, int aRound, int color);

extern void playercard_setRound(PLAYERCARD *aPlayerCard, int aRound, int opponentId, int color, char result);

extern void playercard_swap(PLAYERCARD **aPlayerCard, PLAYERCARD **anotherPlayerCard);

extern void playerlist_movedown(PLAYERLIST *aPlayerList,PLAYERLISTENTRY *aEntry);

extern void playerlist_movebefore(PLAYERLIST *aPlayerList,PLAYERLISTENTRY *aEntry,PLAYERLISTENTRY *anotherEntry);

extern void playerlist_movelast(PLAYERLIST *aPlayerList,PLAYERLISTENTRY *aEntry);

extern void playerlist_split(PLAYERLIST *aPlayerList,PLAYERLIST *anotherPlayerList,int pos);

extern void playerlist_moveall(PLAYERLIST *sourcePlayerList,PLAYERLIST *targetPlayerList);

extern void pairinglist_moveall(PAIRINGLIST *sourcePairingList, PAIRINGLIST *targetPairingList);

extern void pairinglist_movedown(PAIRINGLIST *aPairingList,PAIRINGLISTENTRY *aEntry);
