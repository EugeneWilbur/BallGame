#include <iostream>
#include <fstream>
#include <vector>


class BallGame{
private:
    int numBalls{}, maxTurns{};
    std::vector<int> nums;
    bool isHeads{};

public:
    explicit BallGame(std::ifstream &input){this->setupGame(input);}

    void setupGame(std::ifstream &inputFile){
        std::string s;
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
                //if heads, isHeads == true;
                this->isHeads = s == "HEADS";
            }
        }
        std::cout << this->numBalls << " ";
        std::cout << this->maxTurns << std::endl;
        for(auto a : nums){
            std::cout << a << " ";
        }
        std::cout << std::endl;
        std::cout << this->isHeads << std::endl;
    }

};





int main(int argc, char *argv[]) {
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    std::string s;
    std::getline(inputFile, s);
    int testCases = stoi(s);

    //run for loop for every test case.
    for(int i = 0; i < testCases; i++) {
        //run while loop for each input step
        //TODO remove this
        std::cout << "Test: " << i + 1 << std::endl;

        BallGame currentGame(inputFile);
    }

    inputFile.close();
    return 0;
}
