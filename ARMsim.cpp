#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
int Cycle=1;
bool stop = false;
char flip(char c) {return (c == '0')? '1': '0';}
int AddBit(int n1, int n2)
{
    while (n2 != 0)
    {
        int carry = n1 & n2;

        n1 = n1 ^ n2;

        n2 = carry << 1;
    }
    return n1;
}

int SubBit(int n1, int n2)
{
    while (n2 != 0)
    {
        int borrow = (~n1) & n2;

        n1 = n1 ^ n2;

        n2 = borrow << 1;
    }
    return n1;
}
string TwosComplement(string bin)
{
    int n = bin.length();
    int i;

    string ones, twos;
    ones = twos = "";
    for (i = 0; i < n; i++)
        ones += flip(bin[i]);
    twos = ones;
    for (i = n - 1; i >= 0; i--)
    {
        if (ones[i] == '1')
            twos[i] = '0';
        else
        {
            twos[i] = '1';
            break;
        }
    }

    if (i == -1)
        twos = '1' + twos;


    return twos;
}

int getNegNum(string givenNum){
    string OGivenNum;
    if(givenNum.at(0)=='1'){
        OGivenNum= TwosComplement(givenNum);
        return -1*stoi(OGivenNum,0,2);
    }
    int k=stoi(givenNum, 0, 2);
    return k;
}

void printCycle(std::map<string,int>&mp,std::map<int,int>&Gkey,ofstream &output_fstream){
    output_fstream<<endl;
    output_fstream<<"Registers"<<endl;
    output_fstream<<"X00:";
    output_fstream<<char(9)<<mp.at("00000");
    output_fstream<<char(9)<<mp.at("00001");
    output_fstream<<char(9)<<mp.at("00010");
    output_fstream<<char(9)<<mp.at("00011");
    output_fstream<<char(9)<<mp.at("00100");
    output_fstream<<char(9)<<mp.at("00101");
    output_fstream<<char(9)<<mp.at("00110");
    output_fstream<<char(9)<<mp.at("00111");
    output_fstream<<endl;

    //-------------
    output_fstream<<"X08:";
    output_fstream<<char(9)<<mp.at("01000");
    output_fstream<<char(9)<<mp.at("01001");
    output_fstream<<char(9)<<mp.at("01010");
    output_fstream<<char(9)<<mp.at("01011");
    output_fstream<<char(9)<<mp.at("01100");
    output_fstream<<char(9)<<mp.at("01101");
    output_fstream<<char(9)<<mp.at("01110");
    output_fstream<<char(9)<<mp.at("01111");
    output_fstream<<endl;

    //-------------
    output_fstream<<"X16:";
    output_fstream<<char(9)<<mp.at("10000");
    output_fstream<<char(9)<<mp.at("10001");
    output_fstream<<char(9)<<mp.at("10010");
    output_fstream<<char(9)<<mp.at("10011");
    output_fstream<<char(9)<<mp.at("10100");
    output_fstream<<char(9)<<mp.at("10101");
    output_fstream<<char(9)<<mp.at("10110");
    output_fstream<<char(9)<<mp.at("10111");
    output_fstream<<endl;
    //-------------

    output_fstream<<"X24:";
    output_fstream<<char(9)<<mp.at("11000");
    output_fstream<<char(9)<<mp.at("11001");
    output_fstream<<char(9)<<mp.at("11010");
    output_fstream<<char(9)<<mp.at("11011");
    output_fstream<<char(9)<<mp.at("11100");
    output_fstream<<char(9)<<mp.at("11101");
    output_fstream<<char(9)<<mp.at("11110");
    output_fstream<<char(9)<<mp.at("11111");
    output_fstream<<endl;
    output_fstream<<endl;
    output_fstream<<"Data"<<endl;
    std::map<int,int>::iterator it=Gkey.begin();
    int c=0;
    for(it;it!=Gkey.end();++it)
    {

        c++;
        if(c%9==0)
        {
            output_fstream<<"\n";
            output_fstream<<it->first<<":"<<char(9)<<it->second;
        }
        else
        {
            if(it==Gkey.begin()){
                output_fstream<<it->first<<":";
            }
            output_fstream<<char(9)<<it->second;
        }

    }
    output_fstream<<endl;

}

void readReg(std::map<int, string> &InstCom,std::map<int, int>&Gkey,map<string,int>&mp,int key,vector<int>&regValue,ofstream &output_fstream){

    if(!stop) {
        int Gvalue;

        int index = distance(InstCom.begin(), InstCom.find(key));
        std::map<int, string>::iterator it = InstCom.begin();
        // advance(it, k);
        for (advance(it, index); it != InstCom.end(); ++it) {
            string catNum = it->second.substr(0, 3);
            string OtherLine = it->second.substr(0, it->second.size());
            string firstWord = it->second.substr(0, 1);
            string two;
            if (catNum == "001") {
                //001 10001 01011 1111111111111110110
                string OpCode1 = it->second.substr(3, 5);
                string Src1 = it->second.substr(8, 5);
                string offSet = it->second.substr(13, it->second.size());

                output_fstream<<"--------------------"<<endl;
                output_fstream<<"Cycle "<<Cycle<<":";
                Cycle++;
                output_fstream << char(9)<< it->first <<char(9);
                if (OpCode1 == "10000") {
                    //output_fstream<<"This is OpCode for CBZ: "<<OpCode1<<endl;

                    output_fstream << "CBZ ";
                    output_fstream << "X" << stoi(Src1, 0, 2) << ",";
                    output_fstream << " #" << getNegNum(offSet) << endl;
                    printCycle(mp,Gkey,output_fstream);
                    if (mp.at(Src1) == 0) {
                        int off = getNegNum(offSet);
                        off = 4 * off + it->first;
                        //InstCom.begin()+4;
                        //std::map<int,string>::iterator itr = InstCom.begin()+InstCom.find(off);
                        if(!stop) {

                            readReg(InstCom, Gkey, mp, off,regValue,output_fstream);
                            stop=true;
                            break;
                        }
                    }



                } else if (OpCode1 == "10001") {
                    //output_fstream<<"This is OpCode for CBNZ: "<<OpCode1<<endl;
                    output_fstream << "CBNZ ";
                    output_fstream << "X" << stoi(Src1, 0, 2) << ",";
                    output_fstream << " #" << getNegNum(offSet) << endl;
                    printCycle(mp,Gkey,output_fstream);
                    if (mp.at(Src1) != 0) {
                        int off = getNegNum(offSet);
                        off = 4 * off + it->first;
                        //InstCom.begin()+4;
                        //std::map<int,string>::iterator itr = InstCom.begin()+InstCom.find(off);
                        if(!stop) {
                            readReg(InstCom, Gkey, mp, off,regValue,output_fstream);
                            stop=true;
                            break;
                        }

                    }


                }

            } else if (catNum == "010") {
                //cat 2
                string OpCode2 = it->second.substr(3, 7);
                string Dest2 = it->second.substr(10, 5);
                string Src2 = it->second.substr(15, 5);
                string offSet2 = it->second.substr(20, it->second.size());
                int Src2int = mp.at(Src2);
                int Dest2s;
                output_fstream<<"--------------------"<<endl;
                output_fstream<<"Cycle "<<Cycle<<":";
                Cycle++;
                output_fstream << char(9)<< it->first <<char(9);
                int imm = stoi(offSet2, 0, 2);

                if (OpCode2 == "1000000") {
                    Dest2s = (Src2int | imm);
                    std::map<string, int>::iterator it = mp.find(Dest2);
                    if (it != mp.end()) {
                        it->second = Dest2s;
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    } else {
                        mp.insert({Dest2, Dest2s});
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    }

                    output_fstream << "ORRI ";
                } else if (OpCode2 == "1000001") {

                    Dest2s = (Src2int ^ imm);
                    std::map<string, int>::iterator it = mp.find(Dest2);
                    if (it != mp.end()) {
                        it->second = Dest2s;
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    } else {
                        mp.insert({Dest2, Dest2s});
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    }

                    output_fstream << "EORI ";

                } else if (OpCode2 == "1000010") {

                    Dest2s = AddBit(Src2int, imm);
                    std::map<string, int>::iterator it = mp.find(Dest2);
                    if (it != mp.end()) {
                        it->second = Dest2s;
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    } else {
                        mp.insert({Dest2, Dest2s});
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    }
                    output_fstream << "ADDI ";

                } else if (OpCode2 == "1000011") {
                    Dest2s = SubBit(Src2int, imm);
                    std::map<string, int>::iterator it = mp.find(Dest2);
                    if (it != mp.end()) {
                        it->second = Dest2s;
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    } else {
                        mp.insert({Dest2, Dest2s});
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    }

                    output_fstream << "SUBI ";
                } else if (OpCode2 == "1000100") {
                    Dest2s = (Src2int & imm);
                    std::map<string, int>::iterator it = mp.find(Dest2);
                    if (it != mp.end()) {
                        it->second = Dest2s;
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    } else {
                        mp.insert({Dest2, Dest2s});
                        regValue.insert(regValue.begin()+getNegNum(Dest2),Dest2s);
                    }
                    output_fstream << "ANDI ";

                }

                output_fstream << "X" << stoi(Dest2, 0, 2) << ",";
                output_fstream << " X" << stoi(Src2, 0, 2) << ",";
                output_fstream << " #" << stoi(offSet2, 0, 2) << endl;
                printCycle(mp,Gkey,output_fstream);

            } else if (catNum == "011") {
                //cat 3

                string OpCode3 = it->second.substr(3, 8);
                string Dest3 = it->second.substr(11, 5);
                string Src3a = it->second.substr(16, 5);
                string Src3b = it->second.substr(21, 5);
                output_fstream<<"--------------------"<<endl;
                output_fstream<<"Cycle "<<Cycle<<":";
                Cycle++;
                output_fstream << char(9)<< it->first <<char(9);
                if (OpCode3 == "10100000") {
                    int Dest3s, Src3A, Src3B;
                    Src3A = mp.at(Src3a);
                    Src3B = mp.at(Src3b);
                    Dest3s = (Src3A ^ Src3B);
                    std::map<string, int>::iterator it = mp.find(Dest3);
                    if (it != mp.end()) {
                        it->second = Dest3s;
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    } else {
                        mp.insert({Dest3, Dest3s});
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    }

                    output_fstream << "EOR ";
                } else if (OpCode3 == "10100010") {
                    int Dest3s, Src3A, Src3B;
                    Src3A = mp.at(Src3a);
                    Src3B = mp.at(Src3b);
                    Dest3s = AddBit(Src3A, Src3B);
                    std::map<string, int>::iterator it = mp.find(Dest3);
                    if (it != mp.end()) {
                        it->second = Dest3s;
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    } else {
                        mp.insert({Dest3, Dest3s});
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    }

                    output_fstream << "ADD ";
                } else if (OpCode3 == "10100011") {
                    int Dest3s, Src3A, Src3B;
                    Src3A = mp.at(Src3a);
                    Src3B = mp.at(Src3b);
                    Dest3s = SubBit(Src3A, Src3B);
                    std::map<string, int>::iterator it = mp.find(Dest3);
                    if (it != mp.end()) {
                        it->second = Dest3s;
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    } else {
                        mp.insert({Dest3, Dest3s});
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    }
                    output_fstream << "SUB ";
                } else if (OpCode3 == "10100100") {
                    int Dest3s, Src3A, Src3B;
                    Src3A = mp.at(Src3a);
                    Src3B = mp.at(Src3b);
                    Dest3s = (Src3A & Src3B);
                    std::map<string, int>::iterator it = mp.find(Dest3);
                    if (it != mp.end()) {
                        it->second = Dest3s;
                       regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    } else {
                        mp.insert({Dest3, Dest3s});
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    }
                    output_fstream << "AND ";
                } else if (OpCode3 == "10100101") {

                    int Dest3s, Src3A, Src3B;
                    Src3A = mp.at(Src3a);
                    Src3B = mp.at(Src3b);
                    Dest3s = (Src3A | Src3B);
                    std::map<string, int>::iterator it = mp.find(Dest3);
                    if (it != mp.end()) {
                        it->second = Dest3s;
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    } else {
                        mp.insert({Dest3, Dest3s});
                        regValue.insert(regValue.begin()+getNegNum(Dest3),Dest3s);
                    }

                    output_fstream << "ORR ";
                } else if (OpCode3 == "10100110") {
                    //output_fstream<<"This is OpCode for CBZ: "<<OpCode1<<endl;

                    int valToShift2 = mp.at(Src3a);
                    int bitshift = mp.at(Src3b);


                    valToShift2 = valToShift2 >> bitshift;
                    std::map<string, int>::iterator it = mp.find(Dest3);
                    if (it != mp.end()) {
                        it->second = valToShift2;
                        regValue.insert(regValue.begin()+getNegNum(Dest3),valToShift2);
                    } else {
                        mp.insert({Dest3, valToShift2});
                        regValue.insert(regValue.begin()+getNegNum(Dest3),valToShift2);
                    }

                    output_fstream << "LSR ";

                } else if (OpCode3 == "10100111") {
                    //output_fstream<<"This is OpCode for CBZ: "<<OpCode1<<endl;
                    int valToShift2 = mp.at(Src3a);
                    int bitshift = mp.at(Src3b);
                    valToShift2 = valToShift2 << bitshift;
                    std::map<string, int>::iterator it = mp.find(Dest3);
                    if (it != mp.end()) {
                        it->second = valToShift2;
                         regValue.insert(regValue.begin()+getNegNum(Dest3),valToShift2);
                    } else {
                        mp.insert({Dest3, valToShift2});
                         regValue.insert(regValue.begin()+getNegNum(Dest3),valToShift2);
                    }
                    output_fstream << "LSL ";
                }
                output_fstream << "X" << stoi(Dest3, 0, 2) << ",";
                output_fstream << " X" << stoi(Src3a, 0, 2) << ",";
                output_fstream << " X" << stoi(Src3b, 0, 2) << endl;
                printCycle(mp,Gkey,output_fstream);


            } else if (catNum == "100") {

                string OpCode4 = it->second.substr(3, 8);
                string Src4a = it->second.substr(11, 5);
                string Src4b = it->second.substr(16, 5);
                string offSet4 = it->second.substr(21, it->second.size());


                //output_fstream << it->second << "  ";
                output_fstream<<"--------------------"<<endl;
                output_fstream<<"Cycle "<<Cycle<<":";
                Cycle++;
                output_fstream << char(9)<< it->first <<char(9);

                int loadCount = 0;
                if (OpCode4 == "10101010") {
                    if (stoi(Src4b, 0, 2) >= 31) {
                        // 10010101010010011111100001111100	64	LDUR X9, [XZR, #124]



                        std::map<string, int>::iterator it = mp.find(Src4a);
                        if (it != mp.end()) {

                            it->second = Gkey.at(getNegNum(offSet4));
                            //vector_name.insert(vector_name.begin()+(x-1), element_to_be_inserted);
                             regValue.insert(regValue.begin()+getNegNum(Src4a),Gkey.at(getNegNum(offSet4)));

                        } else {
                            int oSome = getNegNum(offSet4);
                            int rSome = Gkey.at(oSome);
                            mp.insert({Src4a, rSome});
                            regValue.insert(regValue.begin()+getNegNum(Src4a),rSome);

                        }

                    } else {
                        std::map<string, int>::iterator it = mp.find(Src4a);
                        if (it != mp.end()) {
                            loadCount = mp.at(Src4b) + getNegNum(offSet4);
                            it->second = Gkey.at(loadCount);
                            regValue.insert(regValue.begin()+getNegNum(Src4a),Gkey.at(loadCount));
                        } else {
                            loadCount = mp.at(Src4b) + getNegNum(offSet4);
                            mp.insert({Src4a, Gkey.at(loadCount)});
                            regValue.insert(regValue.begin()+getNegNum(Src4a),Gkey.at(loadCount));
                        }
                    }

                    output_fstream << "LDUR ";
//-------------------------------------------------------------------------------------------------------------------
                } else if (OpCode4 == "10101011") {


                    if (stoi(Src4b, 0, 2) >= 31) {
                        // 10010101011011110100100000000000	100	STUR X15, [X9, #0]
                        int offSetLoad = getNegNum(offSet4);
                        std::map<int, int>::iterator it = Gkey.find(offSetLoad);
                        if (it != Gkey.end()) {
                            it->second = mp.at(Src4a);
                        } else {
                            Gkey.insert({offSetLoad, mp.at(Src4a)});
                            //regValue.insert(regValue.begin()+getNegNum(Src4a),Gkey.at(loadCount));
                        }

                    } else {
                        int offSetLoad = mp.at(Src4b) + getNegNum(offSet4);
                        std::map<int, int>::iterator itr = Gkey.find(offSetLoad);
                        if (itr != Gkey.end()) {
                            itr->second = mp.at(Src4a);

                        } else {
                            Gkey.insert({offSetLoad, mp.at(Src4a)});

                        }
                    }

                    output_fstream << "STUR ";
                }


                output_fstream << "X" << stoi(Src4a, 0, 2) << ",";
                if (stoi(Src4b, 0, 2) >= 31) {
                    output_fstream << " [X" << "ZR" << ",";
                } else {
                    output_fstream << " [X" << stoi(Src4b, 0, 2) << ",";
                }

                output_fstream << " #" << getNegNum(offSet4) << "]" << endl;
                printCycle(mp,Gkey,output_fstream);
//        output_fstream << " #" << stoi(offSet4, 0, 2) << "]" << endl;


            } else {

                if (firstWord == "1") {
                    stop=true;
                    if (OtherLine == "10100000000000000000000000000000") {
                        break;

                    } else {
                    }
                } else {
                    break;
                }

            }
        }
        stop=true;
    }
}
int main(int argc, char** argv) {
    map<string, int> mp;
    map<int,string>InstCom;
    map<int, int> Gkey;
    int Gvalue;
    vector<int> regValue;
    for(int i=0;i<32;i++){
        regValue.push_back(0);
    }
    vector<int> regValue2;
    string line;
    ifstream myFile;
    myFile.open(argv[1]);
    mp.insert({"00000",0}); //0
    mp.insert({"00001",0}); //1
    mp.insert({"00010",0}); //2
    mp.insert({"00011",0}); //3
    mp.insert({"00100",0}); //4
    mp.insert({"00101",0}); //5
    mp.insert({"00110",0}); //6
    mp.insert({"00111",0});
    mp.insert({"01000",0});
    mp.insert({"01001",0});
    mp.insert({"01010",0});
    mp.insert({"01011",0});
    mp.insert({"01100",0});
    mp.insert({"01101",0});
    mp.insert({"01110",0});
    mp.insert({"01111",0});
    mp.insert({"10000",0});
    mp.insert({"10001",0});
    mp.insert({"10010",0});
    mp.insert({"10011",0});
    mp.insert({"10100",0});
    mp.insert({"10101",0});
    mp.insert({"10110",0});
    mp.insert({"10111",0});
    mp.insert({"11000",0});
    mp.insert({"11001",0});
    mp.insert({"11010",0});
    mp.insert({"11011",0});
    mp.insert({"11100",0});
    mp.insert({"11101",0});
    mp.insert({"11110",0});
    mp.insert({"11111",0});






    if (myFile.is_open()) {
        int counter2 = 64;
        while (getline(myFile, line)) {
            //output_fstream << line << '\n';
            string catNum = line.substr(0, 3);
            string OtherLine = line.substr(0, line.size());
            string firstWord = line.substr(0, 1);
            string two;
            if (catNum == "001") {
                InstCom.insert({counter2, line});
                counter2 += 4;
            } else if (catNum == "010") {
                InstCom.insert({counter2, line});
                counter2 += 4;
            } else if (catNum == "011") {
                InstCom.insert({counter2, line});
                counter2 += 4;
            } else if (catNum == "100") {
                InstCom.insert({counter2, line});
                counter2 += 4;
            } else {
                if (firstWord == "1") {
                    if (OtherLine == "10100000000000000000000000000000") {

                    } else {
                        two = TwosComplement(OtherLine);
                        Gvalue = -1 * stoi(two, 0, 2);
                        Gkey.insert({counter2, Gvalue});
                        regValue2.push_back(Gvalue);
                    }
                } else {
                    Gvalue = stoi(OtherLine, 0, 2);
                    Gkey.insert({counter2, Gvalue});
                    regValue2.push_back(Gvalue);
                }

                counter2 += 4;
            }
        }
        myFile.close();
    }
    int someNum=64;
    //----------------------------------------
    //now make a new file
    myFile.open(argv[1]);

    string filename("sample_simulation.txt");
    ofstream output_fstream;
    if (myFile.is_open()) {

        output_fstream.open(filename, std::ios_base::out);
        readReg(InstCom,Gkey,mp,someNum,regValue,output_fstream);

        output_fstream<<"--------------------"<<endl;
        output_fstream<<"Cycle "<<Cycle<<":";
        std::map<int, string>::iterator it = InstCom.end();
        --it;
        output_fstream << char(9)<<it->first+4<<char(9);
        output_fstream<<"DUMMY";
        printCycle(mp,Gkey,output_fstream);
myFile.close();


    }


    return 0;
}
