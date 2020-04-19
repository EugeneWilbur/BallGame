#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

template<typename t>

class myPQueue{
private:
    t *nums;
    int curSize{}, capacity{};
    bool isRusty = false;

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

    bool empty(){
        return this->size() == 0;
    }

    void markRusty(){
        this->isRusty = true;
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

    t digitSum(t n){
        t sum = 0;

        while(n > 0){
            sum += n % 10;
            n /= 10;
        }

        return sum;
    }

    void remove(t value){
        int index = -1;

        for(int i = 0; i < this->size(); i++){
            if(this->nums[i] == value){
                index = i;
                break;
            }
        }

        if(index < 0){
            std::cerr << "Cannot remove value that is not in heap." << std::endl;
            return;
        }

        //replace index with last number;
        nums[index] = nums[curSize-1];
        curSize--;

        //fix heap
        myHeapify(index);
    }

    void push(t num){
        if(curSize == capacity){
            std::cerr << "Error: Not enough space." << std::endl;
            return;
        }

        int i = this->curSize;
        this->curSize++;

        nums[i] = num;


        if(this->isRusty){
            while((i && digitSum(nums[getParent(i)]) <= digitSum(nums[i]))){


                if(digitSum(nums[getParent(i)]) == digitSum(nums[i])){
                    if(nums[getParent(i)] < nums[i]){
                        t temp = nums[i];
                        nums[i] = nums[getParent(i)];
                        nums[getParent(i)] = temp;
                    }
                } else {
                    t temp = nums[i];
                    nums[i] = nums[getParent(i)];
                    nums[getParent(i)] = temp;
                }
                i = getParent(i);
            }
        } else {
            while((i && nums[getParent(i)] < nums[i])){
                t temp = nums[i];
                nums[i] = nums[getParent(i)];
                nums[getParent(i)] = temp;
                i = getParent(i);
            }
        }


    }

    t top(){
        return nums[0];
    }

    t pop(){
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

        t lSum = digitSum(nums[getLeft(i)]);
        t rSum = digitSum(nums[right]);
        t iSum = digitSum(nums[i]);

        if(this->isRusty){
            if(left < curSize && lSum >= iSum){
                if(lSum == iSum){
                    if(nums[left] > nums[i]){
                        largest = left;
                    }
                } else {
                    largest = left;
                }
            }
            t largestSum = digitSum(nums[largest]);
            if(right < curSize && rSum >= largestSum){
                if(rSum == largestSum){
                    if(nums[right] > nums[largest]){
                        largest = right;
                    }
                } else {
                    largest = right;
                }
            }
            if(largest != i){
                t temp = nums[i];
                nums[i] = nums[largest];
                nums[largest] = temp;
                myHeapify(largest);
            }
        } else {
            if(left < curSize && nums[left] > nums[i]){
                largest = left;
            }
            if(right < curSize && nums[right] > nums[largest]){
                largest = right;
            }
            if(largest != i){
                t temp = nums[i];
                nums[i] = nums[largest];
                nums[largest] = temp;
                myHeapify(largest);
            }
        }
    }
};


class BallGame{
private:
    int numBalls{}, maxTurns{};
    myPQueue<long> scott{};
    myPQueue<long> rusty{};
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
                scott = myPQueue<long>(numBalls);
                rusty = myPQueue<long>(numBalls);
                rusty.markRusty();
                mark = 0;
                //set the marker to the front of the string
                for(int k = 0; k < this->numBalls; k++){
                    scott.push(stol(s));
                    rusty.push(stol(s, &mark));
                    s = s.substr(mark);
                }
            }else {
                //if heads, isHeads == true;
                this->isHeads = s == "HEADS";
            }
        }
    }

    std::vector<long> findWinner(){
        long scottsScore = 0, rustysScore = 0;

        while(!scott.empty()){
            for(int i = 0; i < maxTurns; i++){
                if(isHeads){
                    rusty.remove(scott.top());
                    scottsScore += scott.pop();
                } else {
                    scott.remove(rusty.top());
                    rustysScore += rusty.pop();
                }
                if(scott.empty() || rusty.empty()){
                    break;
                }
            }
            isHeads = !isHeads;
        }
        std::vector<long> results;
        results.push_back(scottsScore);
        results.push_back(rustysScore);

        return results;
    }
};





int main(int argc, char *argv[]) {
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    std::string s;
    std::getline(inputFile, s);
    int testCases = stoi(s);
    std::ofstream output;
    output.open("Output.txt");
    std::vector<long> result;
    double cpuTime;
    clock_t start, end;



    //run for loop for every test case.
    for(int i = 0; i < testCases; i++) {
        start = clock();
        BallGame currentGame(inputFile);
        result = currentGame.findWinner();
        end = clock();
        output << result[0] << " " << result[1] << std::endl;

        cpuTime = ((double) (end - start)) / CLOCKS_PER_SEC;
        std::cout << i + 1 << ": " << cpuTime << std::endl;
    }

    output.close();
    inputFile.close();
    return 0;
}
