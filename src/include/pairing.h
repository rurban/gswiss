/**
 *  Copyright (C) 2004 Stefan Kn�fel
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

PAIRINGLIST *
playeradministration_alotpairing (PLAYERADMINISTRATION *aPlayerAdministration,
                                  int playedRounds, FILE *debugTarget);

PAIRINGLIST *playeradministration_createResultList (
    PLAYERADMINISTRATION *aPlayerAdministration, int targetRound);

int playeradministration_evaluate (PLAYERADMINISTRATION *aPlayerAdministration,
                                   int playedRounds, int reportErrors);

extern int pairing_optimizeFactor;
