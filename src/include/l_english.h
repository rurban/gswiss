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

#define LANGUAGE "english"

#define L_MEMERROR_PLAYERLISTENTRY              "Not enough memory for new entry in player list.\n"
#define L_MEMERROR_PLAYERLIST                   "Not enough memory for new player list.\n"
#define L_MEMERROR_PLAYERADMINISTRATION         "Not enough memory for new player administration.\n"
#define L_MEMERROR_PLAYERADMINISTRATIONREALLOC  "Not enough memory for extending player administration.\n"
#define L_MEMERROR_PLAYERCARD                   "Not enough memory for new player card.\n"
#define L_MEMERROR_PLAYERGROUP                  "Not enough memory for new player group.\n"
#define L_MEMERROR_PAIRINGLISTENTRY             "Not enough memory for new entry in pairing list.\n"
#define L_MEMERROR_PAIRINGLIST                  "Not enough memory for new pairing list.\n"

#define L_EVALERROR_OPPONENTNOTFOUND          "Opponent of player %d %s in round %d with id %d is not found!\n"
#define L_EVALERROR_OPPONENTNOTMATCH          "Player %d %s in round %d has opponent %d %s, but his opponent has id %d!\n"
#define L_EVALERROR_RESULTSNOTMATCH           "Results do not match in round %d for players %d %s (%c) - %d %s (%c)!\n"
#define L_EVALERROR_COLORSNOTMATCH            "Colors do not match in round %d for players %d %s (%c) - %d %s (%c)!\n"
#define L_EVALERROR_COLORTHREETIMIESINSERIES  "Player %d %s has color %c three times in succession (rounds %d-%d).\n"
#define L_EVALERROR_COLORTHREETIMIESMORE      "Player %d %s has color %c three times often than color %c in round %d.\n"
#define L_EVALERROR_OPPONENTTWICE             "Player %d %s has opponent %d %s twice in round %d and %d!\n"
#define L_EVALERROR_BYEBUTNOPOINT             "Player %d %s has bye in round %d but got no point!\n"
#define L_EVALERROR_BYETWICE                  "Player %d %s has received a point without playing two times in round %d and %d!\n"
#define L_EVALERROR_TWOPLAYERSWITHBYE         "There are 2 players with bye in round %d, %d %s and %d %s!\n"

#define L_INPUTERROR_IGNORINGCHARS        "Ignoring chars %d - %d in section %s in line %d.\n"
#define L_INPUTERROR_IGNORINGCHARS_ROUND  "Ignoring chars %d - %d in section %s in round %d in line %d.\n"

#define L_INPUTERROR_IGNORINGLINE_WRONGNUMBERFORMAT       "Ignoring line %d because %s is not in right format \"%s\".\n"
#define L_INPUTERROR_IGNORINGLINE_WRONGNUMBERFORMAT_ROUND "Ignoring line %d because %s in round %d is not in right format \"%s\" .\n"
#define L_INPUTERROR_IGNORINGLINE_VALUENOTSET             "Ignoring line %d because no %s is set.\n"
#define L_INPUTERROR_IGNORINGLINE_VALUENOTSET_ROUND       "Ignoring line %d because no %s is set for round %d.\n"
#define L_INPUTERROR_IGNORINGLINE_UNKNOWNCONSTCHAR_ROUND  "Ignoring line %d because unknown %s '%c' is set for round %d (values %s).\n"
#define L_INPUTERROR_IGNORINGLINE_COLUMNSTOFEW            "Ignoring line %d because there are to few columns (%d) minimum is (4).\n"
#define L_INPUTERROR_IGNORINGLINE_COLUMNSNOTEQUAL         "Ignoring line %d because the number of columns (%d) is not equal to column number in first line (%d).\n"
#define L_INPUTERROR_IGNORINGLINE_DUPLICATEPLAYERID       "Ignoring line %d because the %s (%d) is already asserted to another player.\n"

#define L_INPUTERROR_USINGDEFAULTINT_VALUENOTSET          "Using default value (%d) for %s in line %d because value is not set.\n"
#define L_INPUTERROR_USINGDEFAULTINT_WRONGNUMBERFORMAT    "Using default value (%d) for %s in line %d because value is nor in right format \"%s\".\n"
#define L_INPUTERROR_USINGDEFAULT_UNKNOWNCONSTSTR         "Using default value \"%s\" for %s in line %d because value \"%s\" is unknown (values %s).\n"

#define L_INPUTSECTION_PID          "player number"
#define L_INPUTSECTION_NAME         "player name"
#define L_INPUTSECTION_RATING       "player rating"
#define L_INPUTSECTION_TITLE        "player title"
#define L_INPUTSECTION_TITLEVALUES  "[\"\"|\"WFM\"|\"FM\"|\"WIM\"|\"IM\"|\"WGM\"|\"IGM\"]"
#define L_INPUTSECTION_OPPONENT     "opponent"
#define L_INPUTSECTION_COLOR        "color"
#define L_INPUTSECTION_COLORVALUES  "['W'|'B']"
#define L_INPUTSECTION_RESULT       "result"
#define L_INPUTSECTION_RESULTVALUES "['0'|'1'|'='|'+'|'-']"

#define L_PARAMERROR_UNKNOWNPARAM  "No valid param %s, use option -h for help.\n"
#define L_PARAMERROR_UNKNOWNOPT    "Unknown option -%c, use option -h for help.\n"
#define L_PARAMERROR_MISSINGFILE   "Missing param <filename> for option -%c.\n"
#define L_PARAMERROR_FILETOLONG    "To long value (%d) of <filename> for option -%c (max %d)."
#define L_PARAMERROR_FILECANTREAD  "Cant open file \"%s\" for reading.\n"
#define L_PARAMERROR_FILECANTWRITE "Cant open file \"%s\" for writing.\n"

#define L_VERBOSEMSG_START          "start\n"
#define L_VERBOSEMSG_FINISH         "finish\n"
#define L_VERBOSEMSG_BREAK          "break\n"
#define L_VERBOSEMSG_WORKGRP        "working with group %d (size=%d)...\n"
#define L_VERBOSEMSG_FOUNDNPP       "found non pairable player (Id=%d)...\n"
#define L_VERBOSEMSG_HOLDNPPOK      "...holding player, he may receive bye and is the only player in the last group.\n"
#define L_VERBOSEMSG_HOLDNPPERR     "...holding player (this may result in canceling pairings of previous group)\n"
#define L_VERBOSEMSG_SWAPNPPBYE     "...swaping player with already paired player %d who can recieve bye\n"
#define L_VERBOSEMSG_SWAPNPPPAIR    "...swaping player with already paired player %d who can be paired in this group\n"
#define L_VERBOSEMSG_DOWNFLTNPP     "...marking player for downfloat\n"
#define L_VERBOSEMSG_DOWNFLTALLNPP  "downfloat %d nonpairable players...\n"
#define L_VERBOSEMSG_SPLITGRP       "spliting group (size %d) in 2 subgroups...\n"
#define L_VERBOSEMSG_PAIRGRP        "start pairing of %s group (size1=%d, size2=%d)...\n"
#define L_VERBOSEMSG_GRPTYPE_HET    "heterogen"
#define L_VERBOSEMSG_GRPTYPE_HOM    "homogen"
#define L_VERBOSEMSG_MOVEPLAYER     "moving 1 player from sub1 to sub2 because no pairing was found...\n"
#define L_VERBOSEMSG_PAIRCOMPL      "finished pairing of group (pairings=%d, quality=%d, remain=%d)...\n"
#define L_VERBOSEMSG_DOWNFLTTAIL    "downfloat remaining players to next group...\n"
#define L_VERBOSEMSG_NEWHOMGRP      "moving remaining players of heterogen group to new group...\n"
#define L_VERBOSEMSG_LASTCOMPL      "last group is completely paired.\n"
#define L_VERBOSEMSG_CANCELPREV     "cancel pairings of prevoius group and moving all players to this group, starting pairnig of this group again...\n"
#define L_VERBOSEMSG_CANTCANCELPREV "cant cancel previous group because this is already the first group.\n"
#define L_VERBOSEMSG_DETMAXQUAL     "max reachable quality of this group is %d\n"
#define L_VERBOSEMSG_PUSHMAXQUAL    "push max reachable quality to %d after %d trials to speed up\n"

#define L_TABLE            "Table round %d:\n"
#define L_PAIRINGS         "Pairings round %d:\n"
#define L_RESULTS          "Results round %d:\n"
#define L_TABLEHEADER      "Place  No               Name                Rat Title Pnt   BH    BHS \n"
#define L_PAIRINGSHEADER   "Board  No         Name          Pnt P E -  No          Name         Pnt P E  R\n"

#define L_BYENAME ">> no opp. <<"

#define L_COLORSHORT_WHITE 'W'
#define L_COLORSHORT_BLACK 'B'

#define L_VERSIONTEXT_LICENSE   "This program comes with NO WARRANTY, to the extent permitted by law.\nYou may redistribute copies under the terms of the GNU General Public License.\nFor more information about these matters, see the file named COPYING.\nWritten by Stefan Knöfel.\n"

#define L_HELPTEXT_USAGE     "\nUsage: gswiss -[options|params]\n"
#define L_HELPTEXT_PARAMS    "Params:\n -i <file>  uses <file> as input instead of <stdin>\n -o <file>  uses <file> as output instead of <stdout>\n"
#define L_HELPTEXT_OPTIONS   "Options:\n -r/R  print the results of previous round to <stdout>/<stderr> (round>1)\n -t/T  print the current table to <stdout>/<stderr>\n -p/P  print the allotted pairings to <stdout>/<stderr>\n -d/D  detailed printout of the pairing process to <stdout>/<stderr>\n -e    do not print any errors\n -n    do not allot pairing (-o/p/d were ignored)\n -v    print version and license information\n -h    print this help text\n"
#define L_HELPTEXT_EXAMPLES  "Examples:\n gswiss -T -P           read input from <stdin>, write output to <stdout> and\n                        print table and pairings to <stderr>.\n gswiss -t -n -i a.csv  read input from a.csv and print table to <stdout>\n                        without allot the pairings.\n"
#define L_HELPTEXT_BUGREPORT "Report bugs to <sknoefel@users.sourceforge.net>.\n\n"
/*
\nUsage: gswiss -[options|params]\n
Params:\n
 -i <file>  uses <file> as input instead of <stdin>\n
 -o <file>  uses <file> as output instead of <stdout>\n
Options:\n
 -r/R  print the results of previous round to <stdout>/<stderr> (round>1)\n
 -t/T  print the current table to <stdout>/<stderr>\n
 -p/P  print the allotted pairings to <stdout>/<stderr>\n
 -d/D  detailed printout of the pairing process to <stdout>/<stderr>\n
 -e    do not print any errors\n
 -n    do not allot pairing (-o/p/d were ignored)\n
 -v    print version and license information\n
 -h    print this help text\n
Examples:\n
 gswiss -T -P           read input from <stdin>, write output to <stdout> and\n
                        print table and pairings to <stderr>.\n
 gswiss -t -n -i a.csv  read input from a.csv and print table to <stdout>\n
                        without allot the pairings.\n
Report bugs to <sknoefel@users.sourceforge.net>.\n\n
*/
