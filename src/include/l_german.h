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

#define LANGUAGE "deutsch"

#define L_MEMERROR_PLAYERLISTENTRY                                            \
  "Kein Speicherplatz fuer neuen Spielerlisteneintrag.\n"
#define L_MEMERROR_PLAYERLIST "Kein Speicherplatz fuer neue Spielerliste.\n"
#define L_MEMERROR_PLAYERADMINISTRATION                                       \
  "Kein Speicherplatz fuer neue Spielerverwaltung.\n"
#define L_MEMERROR_PLAYERADMINISTRATIONREALLOC                                \
  "Kein Speicherplatz fuer Erweiterung der Spielerverwaltung.\n"
#define L_MEMERROR_PLAYERCARD "Kein Speicherplatz fuer neue Spielerkarte.\n"
#define L_MEMERROR_PLAYERGROUP "Kein Speicherplatz fuer neue Spielergruppe.\n"
#define L_MEMERROR_PAIRINGLISTENTRY                                           \
  "Kein Speicherplatz fuer neuen Paarungslisteneintrag.\n"
#define L_MEMERROR_PAIRINGLIST "Kein Speicherplatz fuer neue Paarungsliste.\n"

#define L_EVALERROR_OPPONENTNOTFOUND                                          \
  "Der Gegner von Spieler %d %s in Runde %d mit Id %d wurde nicht "           \
  "gefunden!\n"
#define L_EVALERROR_OPPONENTNOTMATCH                                          \
  "Spieler %d %s hatte in Runde %d als Gegner %d %s, doch dessen Gegener "    \
  "hat Nr. %d!\n"
#define L_EVALERROR_RESULTSNOTMATCH                                           \
  "Ergebnisse stimmen nicht ueberein in Runde %d in der Paarung %d %s (%c) "  \
  "- %d %s (%c)!\n"
#define L_EVALERROR_COLORSNOTMATCH                                            \
  "Farben stimmen nicht ueberein in Runde %d in der Paarung %d %s (%c) - %d " \
  "%s (%c)!\n"
#define L_EVALERROR_COLORTHREETIMIESINSERIES                                  \
  "Spieler %d %s hatte Farbe %c drei mal in Folge (Runden %d-%d).\n"
#define L_EVALERROR_COLORTHREETIMIESMORE                                      \
  "Spieler %d %s hatte Farbe %c drei mal oefter als Farbe %c in Runde %d.\n"
#define L_EVALERROR_OPPONENTTWICE                                             \
  "Spieler %d %s hatte den Gegner %d %s zwei mal, in den Runden %d und %d!\n"
#define L_EVALERROR_BYEBUTNOPOINT                                             \
  "Spieler %d %s hatte Freilos in Runde %d bekam aber keinen Punkt!\n"
#define L_EVALERROR_BYETWICE                                                  \
  "Spieler %d %s hatte Freilos zwei mal, in den Runden %d und %d!\n"
#define L_EVALERROR_TWOPLAYERSWITHBYE                                         \
  "In Runde %d hatten 2 Spieler ein Freilos %d %s und %d %s!\n"

#define L_INPUTERROR_IGNORINGCHARS                                            \
  "Ignoriere Zeichen %d - %d in Sektion %s auf Zeile %d.\n"
#define L_INPUTERROR_IGNORINGCHARS_ROUND                                      \
  "Ignoriere Zeichen %d - %d in Sektion %s der Runde %d auf Zeile %d.\n"

#define L_INPUTERROR_IGNORINGLINE_WRONGNUMBERFORMAT                           \
  "Überspringe Zeile %d da %s nicht als Zahl erkannt wurde \"%s\".\n"
#define L_INPUTERROR_IGNORINGLINE_WRONGNUMBERFORMAT_ROUND                     \
  "Überspringe Zeile %d da %s in Runde %d nicht als Zahl erkannt wurde "      \
  "\"%s\" .\n"
#define L_INPUTERROR_IGNORINGLINE_VALUENOTSET                                 \
  "Überspringe Zeile %d da %s nicht angegeben wurde.\n"
#define L_INPUTERROR_IGNORINGLINE_VALUENOTSET_ROUND                           \
  "Überspringe Zeile %d da %s in Runde %d nicht angegeben wurde.\n"
#define L_INPUTERROR_IGNORINGLINE_UNKNOWNCONSTCHAR_ROUND                      \
  "Überspringe Zeile %d da %s einen unbekannten Wert '%c' in Runde %d hat "   \
  "(Wertemenge %s).\n"
#define L_INPUTERROR_IGNORINGLINE_COLUMNSTOFEW                                \
  "Überspringe Zeile %d da die Anzahl der Spalten (%d) zu klein ist. "        \
  "Minimum ist (4).\n"
#define L_INPUTERROR_IGNORINGLINE_COLUMNSNOTEQUAL                             \
  "Überspringe Zeile %d da die Anzahl der Spalten (%d) nicht mit der Anzahl " \
  "in den vorhergehenden Zeilen (%d) übereinstimmt.\n"
#define L_INPUTERROR_IGNORINGLINE_DUPLICATEPLAYERID                           \
  "Überspringe Zeile %d da die %s (%d) schon belegt ist.\n"

#define L_INPUTERROR_USINGDEFAULTINT_VALUENOTSET                              \
  "Verwende voreingestellten Wert (%d) für %s auf Zeile %d da kein Wert "     \
  "angegeben wurde.\n"
#define L_INPUTERROR_USINGDEFAULTINT_WRONGNUMBERFORMAT                        \
  "Verwende voreingestellten Wert (%d) für %s auf Zeile %d da der Wert "      \
  "\"%s\" nicht als Zahl erkannt wurde.\n"
#define L_INPUTERROR_USINGDEFAULT_UNKNOWNCONSTSTR                             \
  "Verwende voreingestellten Wert \"%s\" für %s auf Zeile %d da der Wert "    \
  "\"%s\" unbekannt ist (Wertemenge %s).\n"

#define L_INPUTSECTION_PID "Spielernummer"
#define L_INPUTSECTION_NAME "Spielername"
#define L_INPUTSECTION_RATING "Spielerwertzahl"
#define L_INPUTSECTION_TITLE "Spielertitel"
#define L_INPUTSECTION_TITLEVALUES                                            \
  "[\"\"|\"WFM\"|\"FM\"|\"WIM\"|\"IM\"|\"WGM\"|\"IGM\"]"
#define L_INPUTSECTION_OPPONENT "Gegner"
#define L_INPUTSECTION_COLOR "Farbe"
#define L_INPUTSECTION_COLORVALUES "['W'|'S']"
#define L_INPUTSECTION_RESULT "Ergebnis"
#define L_INPUTSECTION_RESULTVALUES "['0'|'1'|'='|'+'|'-']"

#define L_PARAMERROR_UNKNOWNPARAM                                             \
  "Unbekannter Parameter %s, benutzen Sie die Option -h für Hilfe.\n"
#define L_PARAMERROR_UNKNOWNOPT                                               \
  "Unbekannte Option -%c, benutzen Sie die Option -h für Hilfe.\n"
#define L_PARAMERROR_MISSINGFILE                                              \
  "Fehlender Parameter <Dateiname> für die Option -%c.\n"
#define L_PARAMERROR_FILETOLONG                                               \
  "Zu langer Wert (%d) für <Dateiname> für die Option -%c (max %d)."
#define L_PARAMERROR_FILECANTREAD                                             \
  "Datei \"%s\" konnte nicht lesend geöffnet werden.\n"
#define L_PARAMERROR_FILECANTWRITE                                            \
  "Datei \"%s\" konnte nicht schreibend geöffnet werden.\n"

#define L_VERBOSEMSG_START "Start\n"
#define L_VERBOSEMSG_FINISH "Ende\n"
#define L_VERBOSEMSG_BREAK "Abbruch\n"
#define L_VERBOSEMSG_WORKGRP "beginne mit Gruppe %d (Größe=%d)...\n"
#define L_VERBOSEMSG_FOUNDNPP                                                 \
  "Spieler gefunden der in dieser Gruppe nicht gepaart werden kann "          \
  "(Id=%d)...\n"
#define L_VERBOSEMSG_HOLDNPPOK                                                \
  "...erhält Freilos da er der einzige Spieler in der letzten Gruppe ist\n"
#define L_VERBOSEMSG_HOLDNPPERR                                               \
  "...kann kein Freilos erhalten und auch nicht ausgetauscht werden.\n"
#define L_VERBOSEMSG_SWAPNPPBYE                                               \
  "...wird gegen schon gepaarten Spieler %d ausgetauscht der ein Freilos "    \
  "erhält.\n"
#define L_VERBOSEMSG_SWAPNPPPAIR                                              \
  "...wird gegen schon gepaarten Spieler %d ausgetauscht der gepaart werden " \
  "kann.\n"
#define L_VERBOSEMSG_DOWNFLTNPP "...wird zum Herunterlosen vorgemerkt.\n"
#define L_VERBOSEMSG_DOWNFLTALLNPP                                            \
  "Herunterlosen von %d nicht paarbaren Spielern...\n"
#define L_VERBOSEMSG_SPLITGRP                                                 \
  "Gruppe (Größe=%d) in 2 Untergruppen aufteilen...\n"
#define L_VERBOSEMSG_PAIRGRP                                                  \
  "beginne Auslosung von %s Gruppe (Größe1=%d, Größe2=%d)...\n"
#define L_VERBOSEMSG_GRPTYPE_HET "heterogener"
#define L_VERBOSEMSG_GRPTYPE_HOM "homogener"
#define L_VERBOSEMSG_MOVEPLAYER                                               \
  "verschiebe 1 Spieler aus Untergruppe1 in Untergruppe2 da keine passende "  \
  "Auslosung gefunden wurde...\n"
#define L_VERBOSEMSG_PAIRCOMPL                                                \
  "fertig mit Auslosung der Gruppe (Paarungen=%d, Qualität=%d, "              \
  "übrige=%d)...\n"
#define L_VERBOSEMSG_DOWNFLTTAIL                                              \
  "Herunterlosen der übrigen Spieler in nächste Gruppe...\n"
#define L_VERBOSEMSG_NEWHOMGRP                                                \
  "Herunterlosen der übrigen Spieler der heterogenen gruppe in neue "         \
  "Gruppe...\n"
#define L_VERBOSEMSG_LASTCOMPL "letzte Gruppe wurde erfolgreich ausgelost.\n"
#define L_VERBOSEMSG_CANCELPREV                                               \
  "Auflösen der Paarungen der vorherigen Gruppe, verschieben aller Spieler "  \
  "in Herunterlosen der übrigen Spieler in nächste Gruppe Gruppe und neuer "  \
  "Auslosungsversuch dieser Gruppe...\n"
#define L_VERBOSEMSG_CANTCANCELPREV                                           \
  "Kann vorherigen Gruppe nicht auflösen, da diese Gruppe bereits die erste " \
  "ist.\n"
#define L_VERBOSEMSG_DETMAXQUAL                                               \
  "maximal erreichbare Qualität dieser Gruppe ist %d\n"
#define L_VERBOSEMSG_PUSHMAXQUAL                                              \
  "erhöhe maximale Qualität auf %d nach %d Versuchen (Laufzeitoptimierung)\n"

#define L_TABLE "Tabelle Runde %d:\n"
#define L_PAIRINGS "Ansetzungen Runde %d:\n"
#define L_RESULTS "Ergebnisse Runde %d:\n"
#define L_TABLEHEADER                                                         \
  "Platz StartNr               Name             WZ Titel Pkt   BH    BHS \n"
#define L_PAIRINGSHEADER                                                      \
  "Brett StartNr       Name        Pkt S G - StartNr       Name        Pkt "  \
  "S G  E\n"

#define L_BYENAME ">> spielfrei <<"

#define L_COLORSHORT_WHITE 'W'
#define L_COLORSHORT_BLACK 'S'

#define L_VERSIONTEXT_LICENSE                                                 \
  "Dieses Programm wird ohne Gewährleistung geliefert, soweit dies "          \
  "gesetzlich zulässig ist.\nSie können es unter den Bedingungen der GNU "    \
  "General Public License weitergeben.\nDetails dazu enthält die Datei "      \
  "COPYING.\nGeschrieben von Stefan Knöfel.\n"

#define L_HELPTEXT_USAGE "\nProgrammaufruf: gswiss -[Optionen|Parameter]\n"
#define L_HELPTEXT_PARAMS                                                     \
  "Folgende Parameter werden ausgewertet:\n -i <datei>  benutzt <datei> als " \
  "input anstelle von <stdin>\n -o <datei>  benutzt <datei> als output "      \
  "anstelle von <stdout>\n"
#define L_HELPTEXT_OPTIONS                                                    \
  "Folgende Optionen stehen zur Verfügung:\n -r/R  Ergebnisse der letzten "   \
  "Runde nach <stdout>/<stderr> schreiben (Runde>1)\n -t/T  Tabelle vor der " \
  "Paarung nach <stdout>/<stderr> schreiben\n -p/P  Paarungsliste nach "      \
  "<stdout>/<stderr> schreiben\n -d/D  detailierte Ausgabe des "              \
  "Auslosungsverlaufs nach <stdout>/<stderr> schreiben\n -e    "              \
  "Fehlerausgabe unterdrücken\n -n    keine Auslosung durchführen (-o/p/d "   \
  "werden ignoriert)\n -v    Versions- und Lizenzinformationen ausgeben\n "   \
  "-h    diesen Hilfetext ausgeben\n"
#define L_HELPTEXT_EXAMPLES                                                   \
  "Beispiele:\n gswiss -T -P           liest von <stdin>, schreibt nach "     \
  "<stdout> und gibt\n                        die Tabelle und die "           \
  "Paarungsliste nach <stderr> aus.\n gswiss -t -n -i a.csv  liest die "      \
  "Eingabe aus a.csv und schreibt die Tabelle\n                        nach " \
  "<stdout> ohne eine Auslosung durchzuführen.\n"
#define L_HELPTEXT_BUGREPORT                                                  \
  "Fehlerberichte an <sknoefel@users.sourceforge.net>.\n\n"
/*
\nProgrammaufruf: gswiss -[Optionen|Parameter]\n
Folgende Parameter werden ausgewertet:\n
 -i <datei>  benutzt <datei> als input anstelle von <stdin>\n
 -o <datei>  benutzt <datei> als output anstelle von <stdout>\n
Folgende Optionen stehen zur Verfügung:\n
 -r/R  Ergebnisse der letzten Runde nach <stdout>/<stderr> schreiben
(Runde>1)\n -t/T  Tabelle vor der Paarung nach <stdout>/<stderr> schreiben\n
 -p/P  Paarungsliste nach <stdout>/<stderr> schreiben\n
 -d/D  detailierte Ausgabe des Auslosungsverlaufs nach <stdout>/<stderr>
schreiben\n -e    Fehlerausgabe unterdrücken\n -n    keine Auslosung
durchführen (-o/p/d werden ignoriert)\n -v    Versions- und Lizenzinformationen
ausgeben\n -h    diesen Hilfetext ausgeben\n Beispiele:\n gswiss -T -P liest
von <stdin>, schreibt nach <stdout> und gibt\n die Tabelle und die
Paarungsliste nach <stderr> aus.\n gswiss -t -n -i a.csv  liest die Eingabe aus
a.csv und schreibt die Tabelle\n nach <stdout> ohne eine Auslosung
durchzuführen.\n Fehlerberichte an <sknoefel@users.sourceforge.net>.\n\n
*/
