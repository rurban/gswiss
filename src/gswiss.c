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
#include <stdlib.h>

/** Globale Rundennummer, nach jeder aenderung mussen eventuell die
    speicher in den PLAYERCARD 's erweitert werden*/
int playedRounds = 0;
int expectedRounds = 7;

#include "include/gswiss.h"
#include "include/language.h"
#include "include/structdef.h"
#include "include/basiclistoperations.h"
#include "include/sorting.h"
#include "include/pairing.h"
#include "include/listing.h"

char
getopt (char *argument)
{
  if (strlen (argument) == 2 && argument[0] == '-')
    return argument[1];
  /* long arguments are only provided for help and version */
  if (strlen (argument) > 2 && argument[0] == '-' && argument[1] == '-')
    {
      if (strcmp (&argument[2], "help") == 0)
        return 'h';
      else if (strcmp (&argument[2], "version") == 0)
        return 'v';
    }
  return 0;
}

/**
 *  version print
 */
void
printVersionText ()
{
  printf (L_VERSIONTEXT_VERSION, LANGUAGE, GSWISS_VERSION);
  printf (L_VERSIONTEXT_COPYRIGHT);
  printf (L_VERSIONTEXT_LICENSE);
}

/**
 *  help print
 */
void
printHelpText ()
{
  printf (L_HELPTEXT_USAGE);
  printf (L_HELPTEXT_PARAMS);
  printf (L_HELPTEXT_OPTIONS);
  printf (L_HELPTEXT_EXAMPLES);
  printf (L_HELPTEXT_BUGREPORT);
}

void
printOutput (PLAYERADMINISTRATION *aPlayerAdministration, FILE *file,
             int playedRounds)
{
  PLAYERLISTENTRY *aPlayerListEntry;
  int i;
  char playertitle[4];
  for (aPlayerListEntry = aPlayerAdministration->player_list->first;
       aPlayerListEntry != NULL; aPlayerListEntry = aPlayerListEntry->next)
    {
      getTitleStr (aPlayerListEntry->aPlayerCard->title, playertitle);
      fprintf (file, "%d,%s,%d,%s", aPlayerListEntry->aPlayerCard->pId,
               aPlayerListEntry->aPlayerCard->name,
               aPlayerListEntry->aPlayerCard->rating, playertitle);
      for (i = 0; i < playedRounds; i++)
        {
          fprintf (file, ",%d,%c,%c",
                   aPlayerListEntry->aPlayerCard->opponents[i],
                   aPlayerListEntry->aPlayerCard->colors[i] == 0
                       ? L_COLORSHORT_WHITE
                       : L_COLORSHORT_BLACK,
                   aPlayerListEntry->aPlayerCard->results[i]);
        }
      fprintf (file, ",%d,%c\n",
               aPlayerListEntry->aPlayerCard->opponents[playedRounds],
               aPlayerListEntry->aPlayerCard->colors[playedRounds] == 0
                   ? L_COLORSHORT_WHITE
                   : L_COLORSHORT_BLACK);
    }
}

PLAYERADMINISTRATION *
readInput (FILE *file, int reportErrors)
{
  PLAYERADMINISTRATION *newplayeradministration;
  PLAYERCARD *newplayercard;

  int c;
  int linecount = 1;
  int max_sections = -1;
  char buffer[31];

  int current_number;
  char current_name[31];
  int current_rating;
  int current_title;
  int current_opponent;
  int current_color;
  int section = 0;
  int section_char_count = 0;
  int ignorecurrentline = 0;

  expectedRounds = 1;
  newplayeradministration = playeradministration_create ();
  newplayercard = NULL;
  do
    {
      /* zeichen lesen */
      c = fgetc (file);

      /* seperatorchars behandeln */
      if ((!ignorecurrentline)
          && (c == (int)',' || c == EOF || c == (int)'\n' || c == (int)'\r'))
        {
          /* Diese Sektion abschliessen (komplette leeerzeilen ignorieren) */
          if (c == (int)',' || section > 0 || section_char_count > 0)
            {
              section++;
              buffer[section_char_count < 30 ? section_char_count : 30] = 0;
              switch (section)
                {
                case 1:
                  if (section_char_count == 0)
                    {
                      if (reportErrors)
                        fprintf (stderr, L_INPUTERROR_IGNORINGLINE_VALUENOTSET,
                                 linecount, L_INPUTSECTION_PID);
                      ignorecurrentline = 1;
                    }
                  else
                    {
                      if (section_char_count > 6)
                        {
                          if (reportErrors)
                            fprintf (stderr, L_INPUTERROR_IGNORINGCHARS, 7,
                                     section_char_count, L_INPUTSECTION_PID,
                                     linecount);
                          buffer[6] = 0;
                        }
                      current_number = -1;
                      if (sscanf (buffer, "%d", &current_number) == EOF
                          || current_number <= 0)
                        {
                          if (reportErrors)
                            fprintf (
                                stderr,
                                L_INPUTERROR_IGNORINGLINE_WRONGNUMBERFORMAT,
                                linecount, L_INPUTSECTION_PID, buffer);
                          ignorecurrentline = 1;
                        }
                      else if (playeradministration_ht_get (
                                   newplayeradministration, current_number)
                               != NULL)
                        {
                          if (reportErrors)
                            fprintf (
                                stderr,
                                L_INPUTERROR_IGNORINGLINE_DUPLICATEPLAYERID,
                                linecount, L_INPUTSECTION_PID, current_number);
                          ignorecurrentline = 1;
                        }
                    }
                  break;
                case 2:
                  if (section_char_count > 30)
                    if (reportErrors)
                      fprintf (stderr, L_INPUTERROR_IGNORINGCHARS, 31,
                               section_char_count, L_INPUTSECTION_NAME,
                               linecount);
                  strcpy (current_name, buffer);
                  break;
                case 3:
                  if (section_char_count == 0)
                    {
                      if (reportErrors)
                        fprintf (stderr,
                                 L_INPUTERROR_USINGDEFAULTINT_VALUENOTSET, 0,
                                 L_INPUTSECTION_RATING, linecount);
                      current_rating = 0;
                    }
                  else
                    {
                      if (section_char_count > 4)
                        {
                          if (reportErrors)
                            fprintf (stderr, L_INPUTERROR_IGNORINGCHARS, 5,
                                     section_char_count, L_INPUTSECTION_RATING,
                                     linecount);
                          buffer[4] = 0;
                        }
                      current_rating = -1;
                      if (sscanf (buffer, "%d", &current_rating) == EOF
                          || current_rating < 0)
                        {
                          if (reportErrors)
                            fprintf (
                                stderr,
                                L_INPUTERROR_USINGDEFAULTINT_WRONGNUMBERFORMAT,
                                0, L_INPUTSECTION_RATING, linecount, buffer);
                          current_rating = 0;
                        }
                    }
                  break;
                case 4:
                  if (section_char_count > 3)
                    {
                      if (reportErrors)
                        fprintf (stderr, L_INPUTERROR_IGNORINGCHARS, 4,
                                 section_char_count, L_INPUTSECTION_TITLE,
                                 linecount);
                      buffer[3] = 0;
                    }
                  current_title = TITLE_NO;
                  /* check title */
                  if (section_char_count >= 3)
                    {
                      if (buffer[0] == 'I' && buffer[1] == 'G'
                          && buffer[2] == 'M')
                        current_title = TITLE_IGM;
                      else if (buffer[0] == 'W' && buffer[1] == 'G'
                               && buffer[2] == 'M')
                        current_title = TITLE_WGM;
                      else if (buffer[0] == 'I' && buffer[1] == 'M'
                               && buffer[2] == ' ')
                        current_title = TITLE_IM;
                      else if (buffer[0] == 'W' && buffer[1] == 'I'
                               && buffer[2] == 'M')
                        current_title = TITLE_WIM;
                      else if (buffer[0] == 'F' && buffer[1] == 'M'
                               && buffer[2] == ' ')
                        current_title = TITLE_FM;
                      else if (buffer[0] == 'W' && buffer[1] == 'F'
                               && buffer[2] == 'M')
                        current_title = TITLE_WFM;
                      else if (!(buffer[0] == ' ' && buffer[1] == ' '
                                 && buffer[2] == ' '))
                        if (reportErrors)
                          fprintf (stderr,
                                   L_INPUTERROR_USINGDEFAULT_UNKNOWNCONSTSTR,
                                   "", L_INPUTSECTION_TITLE, linecount, buffer,
                                   L_INPUTSECTION_TITLEVALUES);
                    }
                  else if (section_char_count == 2)
                    {
                      if (buffer[0] == 'I' && buffer[1] == 'M')
                        current_title = TITLE_IM;
                      else if (buffer[0] == 'F' && buffer[1] == 'M')
                        current_title = TITLE_FM;
                      else if (!(buffer[0] == ' ' && buffer[1] == ' '))
                        if (reportErrors)
                          fprintf (stderr,
                                   L_INPUTERROR_USINGDEFAULT_UNKNOWNCONSTSTR,
                                   "", L_INPUTSECTION_TITLE, linecount, buffer,
                                   L_INPUTSECTION_TITLEVALUES);
                    }
                  else if (section_char_count == 1)
                    if (reportErrors)
                      fprintf (stderr,
                               L_INPUTERROR_USINGDEFAULT_UNKNOWNCONSTSTR, "",
                               L_INPUTSECTION_TITLE, linecount, buffer,
                               L_INPUTSECTION_TITLEVALUES);

                  /* save (temp) player */
                  newplayercard = playercard_create (
                      current_number, current_name, current_rating,
                      current_title, expectedRounds);
                  break;
                default:
                  switch ((section - 4) % 3)
                    {
                    case 1:
                      if (section_char_count == 0)
                        {
                          if (reportErrors)
                            fprintf (
                                stderr,
                                L_INPUTERROR_IGNORINGLINE_VALUENOTSET_ROUND,
                                linecount, L_INPUTSECTION_OPPONENT,
                                (section - 4) / 3 + 1);
                          ignorecurrentline = 1;
                        }
                      else
                        {
                          if (section_char_count > 6)
                            {
                              if (reportErrors)
                                fprintf (stderr,
                                         L_INPUTERROR_IGNORINGCHARS_ROUND, 7,
                                         section_char_count,
                                         L_INPUTSECTION_OPPONENT,
                                         (section - 4) / 3 + 1, linecount);
                              buffer[6] = 0;
                            }
                          current_opponent = -1;
                          if (sscanf (buffer, "%d", &current_opponent) == EOF
                              || current_opponent < 0)
                            {
                              if (reportErrors)
                                fprintf (
                                    stderr,
                                    L_INPUTERROR_IGNORINGLINE_WRONGNUMBERFORMAT_ROUND,
                                    linecount, L_INPUTSECTION_OPPONENT,
                                    (section - 4) / 3 + 1, buffer);
                              ignorecurrentline = 1;
                            }
                        }
                      break;
                    case 2:
                      if (section_char_count == 0)
                        {
                          if (reportErrors)
                            fprintf (
                                stderr,
                                L_INPUTERROR_IGNORINGLINE_VALUENOTSET_ROUND,
                                linecount, L_INPUTSECTION_COLOR,
                                (section - 4) / 3 + 1);
                          ignorecurrentline = 1;
                        }
                      else
                        {
                          if (section_char_count > 1)
                            if (reportErrors)
                              fprintf (
                                  stderr, L_INPUTERROR_IGNORINGCHARS_ROUND, 2,
                                  section_char_count, L_INPUTSECTION_COLOR,
                                  (section - 4) / 3 + 1, linecount);

                          if (buffer[0] == L_COLORSHORT_BLACK
                              || buffer[0] == L_COLORSHORT_WHITE)
                            current_color
                                = buffer[0] == L_COLORSHORT_BLACK ? 1 : 0;
                          else
                            {
                              if (reportErrors)
                                fprintf (
                                    stderr,
                                    L_INPUTERROR_IGNORINGLINE_UNKNOWNCONSTCHAR_ROUND,
                                    linecount, L_INPUTSECTION_COLOR, buffer[0],
                                    (section - 4) / 3 + 1,
                                    L_INPUTSECTION_COLORVALUES);
                              ignorecurrentline = 1;
                            }
                        }
                      break;
                    case 0:
                      if (section_char_count == 0)
                        {
                          if (reportErrors)
                            fprintf (
                                stderr,
                                L_INPUTERROR_IGNORINGLINE_VALUENOTSET_ROUND,
                                linecount, L_INPUTSECTION_RESULT,
                                (section - 4) / 3);
                          ignorecurrentline = 1;
                        }
                      else
                        {
                          if (section_char_count > 1)
                            if (reportErrors)
                              fprintf (
                                  stderr, L_INPUTERROR_IGNORINGCHARS_ROUND, 2,
                                  section_char_count, L_INPUTSECTION_RESULT,
                                  (section - 4) / 3, linecount);

                          /* parse result */
                          if (buffer[0] == '0' || buffer[0] == '1'
                              || buffer[0] == '-' || buffer[0] == '+'
                              || buffer[0] == '=')
                            {
                              playercard_setRound (
                                  newplayercard, (section - 4) / 3,
                                  current_opponent, current_color, buffer[0]);
                            }
                          else
                            {
                              if (reportErrors)
                                fprintf (
                                    stderr,
                                    L_INPUTERROR_IGNORINGLINE_UNKNOWNCONSTCHAR_ROUND,
                                    linecount, L_INPUTSECTION_RESULT,
                                    buffer[0], (section - 4) / 3,
                                    L_INPUTSECTION_RESULTVALUES);
                              ignorecurrentline = 1;
                            }
                        }
                      break;
                    };
                }
              section_char_count = 0;
            }
        }
      /* linebreaks und dateiende behandeln */
      if (c == EOF || c == (int)'\n' || c == (int)'\r')
        {
          /* Store max sections */
          if (!ignorecurrentline && section > 0)
            {
              if (section < 4)
                {
                  if (reportErrors)
                    fprintf (stderr, L_INPUTERROR_IGNORINGLINE_COLUMNSTOFEW,
                             linecount, section);
                  ignorecurrentline = 1;
                }
              else if ((section - 4) % 3 != 0)
                {
                  if ((section - 4) % 3 == 1)
                    {
                      if (reportErrors)
                        fprintf (stderr,
                                 L_INPUTERROR_IGNORINGLINE_VALUENOTSET_ROUND,
                                 linecount, L_INPUTSECTION_COLOR,
                                 (section - 4) / 3 + 1);
                    }
                  else
                    {
                      if (reportErrors)
                        fprintf (stderr,
                                 L_INPUTERROR_IGNORINGLINE_VALUENOTSET_ROUND,
                                 linecount, L_INPUTSECTION_RESULT,
                                 (section - 4) / 3 + 1);
                    }
                  ignorecurrentline = 1;
                }
              else if (max_sections <= 0)
                {
                  max_sections = section;
                  /* store expected rounds */
                  playedRounds = (section - 4) / 3;
                  expectedRounds = playedRounds + 1;
                }
              else if (section != max_sections)
                {
                  if (reportErrors)
                    fprintf (stderr, L_INPUTERROR_IGNORINGLINE_COLUMNSNOTEQUAL,
                             linecount, section, max_sections);
                  ignorecurrentline = 1;
                }
            }
          if (!ignorecurrentline && section > 0)
            {
              playeradministration_add (newplayeradministration,
                                        newplayercard);
            }
          if (ignorecurrentline && newplayercard != NULL)
            playercard_finalize (newplayercard);

          /* diese zeile abschliessen */
          linecount++;
          newplayercard = NULL;
          ignorecurrentline = 0;
          section = 0;
          section_char_count = 0;
        }
      /* alle sonstigen zeichen beachten */
      else if (c != (int)',' && !ignorecurrentline)
        {
          if (section_char_count < 30)
            buffer[section_char_count] = c;
          section_char_count++;
        }
    }
  while (c != EOF);

  return newplayeradministration;
}

int
main (int argc, char **argv)
{
  PLAYERADMINISTRATION *aPlayerAdministration = NULL;
  PAIRINGLIST *resultList = NULL;
  int param_printResults = 0;
  int param_printTable = 0;
  int param_printPairngs = 0;
  int param_printVerbose = 0;
  int param_dopair = 1;
  int param_printerrors = 1;
  char *param_inputFile = NULL;
  char *param_outputFile = NULL;
  FILE *inputFile = NULL;
  FILE *outputFile = NULL;
  int i, evalResult;

  /* Auslosung zeitoptipieren nach wert x */
  pairing_optimizeFactor = 10000;

  for (i = 1; i < argc; i++)
    {
      switch (getopt (argv[i]))
        {
        case 0:
          printf (L_PARAMERROR_UNKNOWNPARAM, argv[i]);
          exit (1);
          break;
        case 'h':
          printHelpText ();
          exit (0);
          break;
        case 'v':
          printVersionText ();
          exit (0);
          break;
        case 'r':
          param_printResults = 1;
          break;
        case 'R':
          param_printResults = 2;
          break;
        case 't':
          param_printTable = 1;
          break;
        case 'T':
          param_printTable = 2;
          break;
        case 'p':
          param_printPairngs = 1;
          break;
        case 'P':
          param_printPairngs = 2;
          break;
        case 'd':
          param_printVerbose = 1;
          break;
        case 'D':
          param_printVerbose = 2;
          break;
        case 'i':
          if (++i < argc)
            param_inputFile = argv[i];
          else
            {
              if (param_printerrors)
                fprintf (stderr, L_PARAMERROR_MISSINGFILE, 'i');
              exit (3);
            }
          break;
        case 'o':
          if (++i < argc)
            param_outputFile = argv[i];
          else
            {
              if (param_printerrors)
                fprintf (stderr, L_PARAMERROR_MISSINGFILE, 'o');
              exit (4);
            }
          break;
        case 'e':
          param_printerrors = 0;
          break;
        case 'n':
          param_dopair = 0;
          break;
        default:
          printf (L_PARAMERROR_UNKNOWNOPT, getopt (argv[i]));
          exit (2);
        }
    }

  /*Streams oeffnen*/
  if (param_inputFile != NULL)
    {
      if (strlen (param_inputFile) > FILENAME_MAX)
        {
          if (param_printerrors)
            fprintf (stderr, L_PARAMERROR_FILETOLONG, (int)strlen (param_inputFile),
                     'i', FILENAME_MAX);
          exit (3);
        }
      else
        {
          inputFile = fopen (param_inputFile, "rt");
          if (inputFile == NULL)
            {
              if (param_printerrors)
                fprintf (stderr, L_PARAMERROR_FILECANTREAD, param_inputFile);
              exit (3);
            }
        }
    }
  else
    inputFile = stdin;

  if (param_outputFile != NULL
      && param_dopair) /* wenn nicht gepaart werden soll gibts keine ausgabe -
                          wir koennen uns das Stream öffnen sparen*/
    {
      if (strlen (param_outputFile) > FILENAME_MAX)
        {
          if (param_printerrors)
            fprintf (stderr, L_PARAMERROR_FILETOLONG,
                     (int)strlen (param_outputFile), 'o', FILENAME_MAX);
          exit (4);
        }
      else
        {
          outputFile = fopen (param_outputFile, "wt");
          if (outputFile == NULL)
            {
              if (param_printerrors)
                fprintf (stderr, L_PARAMERROR_FILECANTWRITE, param_outputFile);
              exit (4);
            }
        }
    }
  else
    outputFile = stdout;

  /* daten einlesen */
  aPlayerAdministration = readInput (inputFile, param_printerrors);
  /* Stream schliessen */
  if (param_inputFile != NULL)
    fclose (inputFile);

  /* immer evaluieren */
  evalResult = playeradministration_evaluate (aPlayerAdministration,
                                              playedRounds, param_printerrors);

  /* wenn evaluieren fehler brachte, dann ende */
  if (evalResult)
    exit (evalResult + 10);

  /* wenn letzte ergebnisse ausgeben werden sollen und runde > 0 */
  if (param_printResults && playedRounds)
    {
      /* neu evaluieren mit vorheriger runde (eventuelle fehler ignorieren, da
       * schon evaluiert) */
      playeradministration_evaluate (aPlayerAdministration, playedRounds - 1,
                                     0);
      /* alte paarungsliste incl resultate erstellen */
      resultList = playeradministration_createResultList (
          aPlayerAdministration, playedRounds - 1);
      listResults (resultList, param_printResults == 1 ? stdout : stderr,
                   playedRounds);
      pairinglist_finalize (resultList);
      resultList = NULL;
      /* neu evaluieren mit aktueller runde für die auslosung (eventuelle
       * fehler ignorieren, da schon evaluiert) */
      playeradministration_evaluate (aPlayerAdministration, playedRounds, 0);
    }

  /* wenn tabelle ausgegeben werden soll dann das jetzt tun */
  if (param_printTable)
    {
      /* vor der ausgabe tabelle sortieren */
      playerlist_sortWithStandardComparator (
          aPlayerAdministration->player_list, COMPARATOR_TABLE);
      listTable (aPlayerAdministration->player_list,
                 param_printTable == 1 ? stdout : stderr, playedRounds);
    }

  /* wenn nicht gepaart werden soll, dann hier abbruch */
  if (param_dopair == 0)
    exit (0);

  /* paarung durchfuehren */
  resultList = playeradministration_alotpairing (
      aPlayerAdministration, playedRounds,
      param_printVerbose == 0 ? NULL
                              : (param_printVerbose == 1 ? stdout : stderr));

  /* wenn nicht erfolgreich ausgelost wurde -> exit */
  if (resultList == 0)
    exit (3);

  /* wenn paarungen ausgeprintet werden sollen, dann das jetzt tun	*/
  if (param_printPairngs)
    {
      listPairings (resultList, param_printPairngs == 1 ? stdout : stderr,
                    playedRounds);
    }

  /* Ausgeben des resultats */
  playerlist_sortWithStandardComparator (aPlayerAdministration->player_list,
                                         COMPARATOR_OUTPUT);
  printOutput (aPlayerAdministration, outputFile, playedRounds);
  /* Stream schliessen */
  if (param_outputFile != NULL)
    fclose (outputFile);

  return 0;
}
