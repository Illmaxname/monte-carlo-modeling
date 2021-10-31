#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

class Block{
    double prob;
    bool used;
    vector<Block*> block;
public:
    Block(const double probability):prob(probability), used(false){}
    vector<Block*> get(){
        return this->block;
    }
    void addBlock(Block* nBlock){
        this->block.push_back(nBlock);
    }
    void useBlock(bool used){
        this->used = used;
    }
    bool isUsed(){
        return this->used;
    }
    double getProb(){
        return this->prob;
    }
};

class Connection{
public:
    int from;
    int to;
};

static mt19937_64 gen { static_cast<unsigned long long>(std::chrono::system_clock::now().time_since_epoch().count()) };

bool isWork(Block* startBlock) {
    if (!startBlock->isUsed()) {
        return false;
    }
    if (!startBlock->get().size()) {
        return true;
    }
    for (Block* block : startBlock->get()) {
        if (isWork(block)) {
            return true;
        }
    }
    return false;
}

int main() {
    Connection connections[] = {
            {0,1},{1,2},{1,3},{1,4},
            {2,5},{3,5},{4,5},{5,6},
            {5,7},{0,8},{0,9},{8,10},
            {9,10},{10,6},{10,7}
    };
    int connectionsCount = sizeof(connections) / sizeof(Connection);

    double probs[] = {1.0, 0.85, 0.80, 0.75, 0.70, 0.65, 0.60, 0.55,
                      0.50, 0.45, 0.40};
    int probCount = sizeof(probs) / sizeof(double);

    Block* blocks[probCount];
    uniform_real_distribution<double> distribution(0.0, 1.0); // равномерное распределение от 0.0 до 1.0

    for (int i = 0; i < probCount; i++) {
        blocks[i] = new Block(probs[i]);
    }

    for (int i = 0; i < connectionsCount; i++) {
        blocks[connections[i].from]->addBlock(blocks[connections[i].to]);
    }
    int probability = 0;

    int modeling = 100;
    for (int i = 0; i < modeling; i++) {
        for (auto block : blocks) {
            block->useBlock(distribution(gen) < block->getProb());
        }

        if (isWork(blocks[0])) {
            probability++;
        }
    }

    cout << "Probability: " << (double) probability / modeling << endl;

    for (int i = 0; i < probCount; i++) {
        delete blocks[i];
    }
    return 0;
}


