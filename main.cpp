#include <iostream>
#include <fstream>
#include <vector>


class BallGame{
private:
    int testCases, numBalls, maxTurns;
    std::vector<int> nums;
    bool isHeads;


public:
    explicit BallGame(const std::string &fileName) {
        setupGame(fileName);
    }

    void setupGame(const std::string &fileName){
        std::ifstream inputFile;
        inputFile.open(fileName);
        std::string s;

        std::getline(inputFile, s);
        this->testCases = stoi(s);

        //run for loop for every test case.
        for(int i = 0; i < this->testCases; i++){
            //run while loop for each input step
            //TODO remove this
            std::cout << "Test: " << i + 1 << std::endl;
            for(int j = 0; j < 3; j++){
                std::getline(inputFile, s);
                //set a marker to keep track of where we are up to for the string input.
                std::string::size_type mark;
                if(j == 0){
                    //step 1 init
                    //input first number in the current string
                    this->numBalls = stoi(s, &mark);
                    //input the second number in the current string
                    this->maxTurns = stoi(s.substr(mark));
                }else if(j == 1){
                    //step 2 init.
                    mark = 0;
                    //set the marker to the front of the string
                    for(int k = 0; k < this->numBalls; k++){
                        nums.push_back(stoi(s, &mark));
                        s = s.substr(mark);
                    }
                }else{
                    if(s == "HEADS"){
                        this->isHeads = true;
                    } else {
                        this->isHeads = false;
                    }
                }
            }
            std::cout << this->numBalls << std::endl;
            std::cout << this->maxTurns << std::endl;
            for(auto a : nums){
                std::cout << a << " ";
            }
            std::cout << std::endl;
            std::cout << this->isHeads << std::endl;

        }


    }

};





int main(int argc, char *argv[]) {
    BallGame game(argv[1]);



    return 0;
}
