/**************************************************************
 ____  ____    __    _  _  ___  __      __   ____  _____  ____ 
(_  _)(  _ \  /__\  ( \( )/ __)(  )    /__\ (_  _)(  _  )(  _ \
  )(   )   / /(__)\  )  ( \__ \ )(__  /(__)\  )(   )(_)(  )   /
 (__) (_)\_)(__)(__)(_)\_)(___/(____)(__)(__)(__) (_____)(_)\_)

 **************************************************************/

#include <pcre.h>               /* PCRE lib        NONE  */
#include <stdio.h>              /* I/O lib         C89   */
#include <stdlib.h>             /* Standard Lib    C89   */
#include <string.h>             /* Strings         C89   */
#include <stdbool.h>            /* Booleans        C99   */

//const char * languages[] = {"en", "pt", "pt-br"};
//const char * commands[] = {"from", "to"};

const char * command;
const char * value;

_Bool arguments_validation();

int main(int argc, char ** argv) {

    // comparing two strings
    // if (strcmp(argv[0], "translator"))

    // loop arguments
    for (int i = 0; i < sizeof (argv); i++) {
        // verify if arguments is present
        if (arguments_validation(argv[i])) {
            printf("'%s'\n", command);
            printf("'%s'\n", value);
        }
    }

    return (EXIT_SUCCESS);
}

_Bool arguments_validation(char * strings) {

    pcre * reCompiled;
    pcre_extra * pcreExtra;

    int pcreExecRet;
    int subStrVec[30];

    int pcreErrorOffset;
    int j;

    char * aStrRegex;
    char ** aLineToMatch;
    char * testStrings[] = {strings};

    const char * psubStrMatchStr;
    const char * pcreErrorStr;

    /* PCRE */
    aStrRegex = ".*?((?:[a-z][a-z0-9_]*)).*?((?:[a-z][a-z0-9_]*))";

    // compile
    reCompiled = pcre_compile(aStrRegex, 0, &pcreErrorStr, &pcreErrorOffset, NULL);

    // compile: validate
    if (reCompiled == NULL) return false;

    // study
    pcreExtra = pcre_study(reCompiled, 0, &pcreErrorStr);

    // study:validate
    if (pcreErrorStr != NULL) return false;

    for (aLineToMatch = testStrings; * aLineToMatch != NULL; aLineToMatch++) {

        // execute regular expression
        pcreExecRet = pcre_exec(reCompiled, pcreExtra, * aLineToMatch, strlen(* aLineToMatch), 0, 0, subStrVec, 30);

        // error
        if (pcreExecRet < 0) {
            // find out what error occurred
            switch (pcreExecRet) {
                case PCRE_ERROR_NOMATCH: printf("String did not match the pattern\n");
                    break;
                case PCRE_ERROR_NULL: printf("Something was null\n");
                    break;
                case PCRE_ERROR_BADOPTION: printf("A bad option was passed\n");
                    break;
                case PCRE_ERROR_BADMAGIC: printf("Magic number bad (compiled re corrupt?)\n");
                    break;
                case PCRE_ERROR_UNKNOWN_NODE: printf("Something kooky in the compiled re\n");
                    break;
                case PCRE_ERROR_NOMEMORY: printf("Ran out of memory\n");
                    break;
                default: printf("Unknown error\n");
                    break;
            }

            return false;
        } else {
            // loop de found results
            for (j = 0; j < pcreExecRet; j++) {
                switch (j) {
                    case 1: pcre_get_substring(* aLineToMatch, subStrVec, pcreExecRet, j, &(command));
                        break;
                    case 2: pcre_get_substring(* aLineToMatch, subStrVec, pcreExecRet, j, &(value));
                        break;
                }
            }
            // free
            pcre_free_substring(psubStrMatchStr);
        }
    }
    // free me
    pcre_free(reCompiled);

    //
    if (pcreExtra != NULL) {
        // 
        pcre_free(pcreExtra);
    }

    // everything went okay
    return true;

}