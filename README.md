# PollDataParsing
This program parsed polldata strings such as "D32CAr54TXG21CT" and given an input of a party can tell you how many votes there were for that party. In this example, if inputed D or d for party the result would be 32. The two letters after the digits are a statecode.

Testdata:
Using cassert header file:

	    int votes;

        assert(computeVotes("r30CAd30all22ri", 'l',votes)==0);                              //add test for no digit
        assert(computeVotes("r6CAd3kSl2ri",'r',votes)==0 && votes==6);                      //checking is 1 digit for r is added to votecount (6)
        assert(computeVotes("p40CAd3kSp40ri", 'p',votes)==0 && votes==80);                  //checking correct poll data string
        assert(computeVotes("p40CAd3KSp4ri", 'p', votes)==0);                               //checking if the statecode is uppercase if it registers properly
        assert(computeVotes("D54CA", '@', votes)==3);                                       //checking return statement for if party is not alphabetical
        assert(computeVotes("@54CA", 'R', votes)==1);                                       //checking hasRightSyntax for not alphanumerical characters
        assert(computeVotes("", 'R', votes)==0);                                            //checking is empty string returns 0
        assert(computeVotes("R40txr54cAr64MSd28NyL06UT", 'r', votes) == 0 && votes==158);   //checking lowercase and a mix of non-lowercase statecodes
        votes = -999;
        assert(computeVotes("R40TXD54CA", '%', votes) == 3 && votes==-999);                 //checking if value of votes is not changed when we return 3
        assert(hasRightSyntax("R40MSD54CA"));                                               //checking hasRightSyntax on a correct string
        assert(!hasRightSyntax("R40MXD54CA"));                                              //checking hasRightSyntax on a wrong statecode ("MX")
        assert(computeVotes("L0CAd3kSr22ri", 'l',votes)==2 );                               //checking return 2 for 0 without another digit afterwards
        assert(computeVotes("r40CAd3kSL22ri", 'l',votes)==0);                               //checking case-sentivity of party character
        assert(computeVotes("r66CAd3kSl22ri",'p',votes)==0);                                //checking what happens if party is not in string
        assert(computeVotes("p40CAd3KSp04ri", 'p', votes)==0);                              //checking that code adds two digit numbers with single digit numbers correctly
        assert(computeVotes("30CAd3kSs22ri", 'l',votes)==1);                                //checking condition of having no party to start
        assert(computeVotes("r30CAdks22ri", 'l',votes)==1);                                 //checking for extra stuff with no digits
        assert(computeVotes("r30CAdkl22ri", 'l',votes)==1);                                 //checking for what happens if missing digits
        assert(computeVotes("", '\n', votes)==3);                                           //checking what happens if we have no party character and no string
        assert(computeVotes("30CAs22ri", 'l',votes)==1);                                    //checking what happens if first letter not there
        assert(computeVotes("p40CAd3rip40", 'p', votes)==1);                                //checking to make sure the last statecode is checked
        assert(computeVotes("p40CAd3CAp40Cx", 'p', votes)==1);                              //checking last statecode is correct
        assert(computeVotes("p40CAd3p40Ca", 'p', votes)==1);                                //checking for missing statecode in middle of string
        assert(computeVotes("pCAdpCa", 'p', votes)==1);
