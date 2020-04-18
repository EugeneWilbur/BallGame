#include <iostream>
#include <fstream>
#include <vector>


template<typename t>

class myPQueue{
private:
    t *nums;
    int curSize{}, capacity{};

public:
    explicit myPQueue(int capacity): nums{new t[capacity]}, capacity{capacity} {}
    myPQueue(): myPQueue(10) {}
    myPQueue(myPQueue const & that): capacity{that.capacity}, nums{new t[that.capacity]} {}
    myPQueue(myPQueue && that) noexcept: capacity{std::move(that.capacity)}, nums{std::move(that.nums)} {}
    myPQueue & operator = (myPQueue const & that) noexcept {
        if(this == &that){
            return *this;
        }
        capacity = that.capacity;
        nums = that.nums;
        return *this;
    }
    myPQueue & operator = (myPQueue && that) noexcept {
        capacity = std::move(that.capacity);
        nums = std::move(that.nums);
        that.nums = nullptr;
        return *this;
    }
    ~myPQueue(){delete [] nums;}

    t &operator [] (int i){
        if(i > curSize || i < 0){
            std::cerr << "Index out of queue." << std::endl;
            return nums[0];
        } else {
            return nums[i];
        }
    }

    void print(){
        for(int i = 0; i < curSize; i++){
            std::cout << nums[i] << " ";
        }
        std::cout << std::endl;
    }

    int size(){
        return curSize;
    }

    int getParent(int i){
        return (i-1)/2;
    }

    int getLeft(int i){
        return (i*2)+1;
    }

    int getRight(int i){
        return (i*2)+2;
    }

    void push(int num){
        if(curSize == capacity){
            std::cerr << "Error: Not enough space." << std::endl;
            return;
        }

        int i = this->curSize;
        this->curSize++;

        nums[i] = num;

        while((i && nums[getParent(i)] < nums[i])){
            int temp = nums[i];
            nums[i] = nums[getParent(i)];
            nums[getParent(i)] = temp;
            i = getParent(i);
        }
    }

    int pop(){
        if(curSize <= 0){
            std::cerr << "No items left in queue." << std::endl;
            return 0;
        }
        if(curSize == 1){
            curSize--;
            return nums[0];
        }

        //keep top number;
        int temp = nums[0];
        //replace root with last number;
        nums[0] = nums[curSize-1];
        curSize--;

        //fix heap
        myHeapify(0);

        return temp;
    }

    void myHeapify(int i){
        int left = getLeft(i);
        int right = getRight(i);
        int largest = i;

        if(left < curSize && nums[getLeft(i)] > nums[i]){
            largest = left;
        }
        if(right < curSize && nums[right] > nums[largest]){
            largest = right;
        }
        if(largest != i){
            int temp = nums[i];
            nums[i] = nums[largest];
            nums[largest] = temp;
            myHeapify(largest);
        }
    }
};


class BallGame{
private:
    int numBalls{}, maxTurns{};
    myPQueue<int> *nums{};
    bool isHeads{};

public:
    explicit BallGame(std::ifstream &input){this->setupGame(input);}

    void setupGame(std::ifstream &inputFile){
        std::string s;
        //run for loop for each set up step.
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
                nums = new myPQueue<int>[numBalls];
                mark = 0;
                //set the marker to the front of the string
                for(int k = 0; k < this->numBalls; k++){
                    nums->push(stoi(s, &mark));
                    s = s.substr(mark);
                }
            }else {
                //if heads, isHeads == true;
                this->isHeads = s == "HEADS";
            }
        }
    }

    void findWinner(){
        //TODO complete this
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
        //TODO remove this
        std::cout << "Test: " << i + 1 << std::endl;

        BallGame currentGame(inputFile);
        //currentGame.findWinner();
    }

    inputFile.close();
    return 0;
}
