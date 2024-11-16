#include <iostream>
#include <string>
#include <cassert>

using namespace std;

bool isValidStateCode(string stateCode){
    const string codes =
        "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
        "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NJ.NH.NM.NY.NC."
        "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY"
        "aL.aK.aZ.aR.cA.cO.cT.dE.dC.fL.gA.hI.iD.iL.iN.iA.kS"
        "kY.lA.mE.mD.mA.mI.mN.mS.mO.mT.nE.nV.nJ.nH.nM.nY.nC."
        "nD.oH.oK.oR.pA.rI.sC.sD.tN.tX.uT.vT.vA.vA.wV.wI.wY"
        "Al.Ak.Az.Ar.Ca.Co.Ct.De.Dc.Fl.Ga.Hi.Id.Il.In.Ia.Ks."
        "Ky.La.Me.Md.Ma.Mi.Mn.Ms.Mo.Mt.Ne.Nv.Nj.Nh.Nm.Ny.Nc."
        "Nd.Oh.Ok.Or.Pa.Ri.Sc.Sd.Tn.Tx.Ut.Vt.Va.Wa.Wv.Wi.Wy"
        "al.ak.az.ar.ca.co.ct.de.dc.fl.ga.hi.id.il.in.ia.ks."
        "ky.la.me.md.ma.mi.mn.ms.mo.mt.ne.nv.nj.nh.nm.ny.nc."
        "nd.oh.ok.or.pa.ri.sc.sd.tn.tx.ut.vt.va.wa.wv.wi.wy";
    return (stateCode.size() == 2  &&
            stateCode.find('.') == string::npos  &&  
            codes.find(stateCode) != string::npos);  
}

bool hasRightSyntax(string pollData) {
    int stringdigitcounter=0;//used later for checking if string has no digits
    if(pollData=="")
        return true;//according to spec empty string returns as true
    int digitcounter = 0; // Keeps track of consecutive digits
    if(!(isalpha(pollData[0]) && isdigit(pollData[1])))//if no p to start string followed by a digit
        return false;
    for (size_t i = 0; i < pollData.size(); i++) {
        char j = pollData.at(i);

        // Check for valid characters (digits or letters)
        if (!(isdigit(j) || isalpha(j))) {
            return false;
        }

        if (isdigit(j)) {
            stringdigitcounter++;//if any digits in string we will satisfy the req at end (line 87)
            size_t numEnd = i;//set i equal to a new number to limit the testing to the size of the string
            if(i>4){//bounding limit (already checked first iteration)
                if(isalpha(pollData.at(i-1)) && isalpha(pollData.at(i-2)) && !isalpha(pollData.at(i-3)))//statecode followed by party
                return false;
            }
            while (numEnd+1 < pollData.size() && isdigit(pollData.at(numEnd))) {//if num is a number and the value is less than the polldatasize then skip to the next number (num++)
                numEnd++;
            }
            if(numEnd + 1 <= pollData.size() && isalpha(pollData.at(numEnd)) && isalpha(pollData.at(numEnd+1))){ // Ensure enough characters left for state code (keep from going out of scope of polldata)
                char firstStateCodeLetter=pollData.at(numEnd);//numend is now at first statecode letter
                char secondStateCodeLetter=pollData.at(numEnd+1);
                if (!isalpha(firstStateCodeLetter)||!isalpha(secondStateCodeLetter))//checking for non alphabetical chars
                    return false;
                string stateCode;
                stateCode+=firstStateCodeLetter;
                stateCode+=secondStateCodeLetter;
                 // Extract state code (start at numend then take 2)
                if (isValidStateCode(stateCode)) {//check if it is a valid statecode
                    if(numEnd+2<pollData.size()&& !isalpha(pollData.at(numEnd+2))){//check if party after statecode
                        return false;
                    }
                    
                    else if(numEnd+3<pollData.size()&& !isdigit(pollData.at(numEnd+3))){
                        return false;
                    }
                    } 
                else {
                    return false;
                }
            }
            else{
                return false;
            }
             // Increment or reset digit counter
                if(digitcounter<2){
                    digitcounter++;
                }
                else if (digitcounter > 2) {//if more than 2 in a row digits return false
                    return false;
                }
                else {
                digitcounter = 0; // Reset counter when non-digit is found
                }
        if(stringdigitcounter==0)
            return false;
        }
    }
    return true;
}

int computeVotes(string pollData, char party, int& voteCount){
    if (!hasRightSyntax(pollData)) {
        return 1;  // If not a valid poll data string, return 1
    }
    if (!isalpha(party)) {
        return 3;  // If party is not a letter, return 3
    }
    voteCount = 0;  // Initialize vote count
    size_t x = 0;
    while(x < pollData.size()) {
        char j = pollData.at(x);
        if (toupper(j) == toupper(party)) {
            size_t numStart = x + 1;  // Look at number after party
            size_t lastNum = numStart;  // Placeholder
            while (lastNum < pollData.size() && isdigit(pollData.at(lastNum))) {
                lastNum++;
            }
            // Check if there is a zero without subsequent digit
            if (pollData.at(numStart)=='0') {
                if (pollData.at(numStart+1) == '0' || !isdigit(pollData.at(numStart+1))) {
                    return 2;
                }
            }
            // Extract the number and validate the state code
            if (numStart <= lastNum && lastNum + 2 <= pollData.size() && isValidStateCode(pollData.substr(lastNum, 2))){
                int num = stoi(pollData.substr(numStart, lastNum - numStart));//take index starting at start end at lastnum
                voteCount += num;
                x = lastNum + 2;  // Skip past the number and state code
                continue;
            }
        }
        x++;
    }
    return 0;
}

int main(){
	    int votes;

        //used for testing: Ignore if using for other applications
        /*
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
        */

}
