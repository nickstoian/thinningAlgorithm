//Nicolas Stoian
//CS780 Image Processing
//Project 5 - Thinning Algorithm

//This program needs 3 command line arguments
//argv[1] "input" for text file representing the binary image
//argv[2] "output1" for a skeleton image from the final result of the thinning algorithm with image header for future processing
//argv[3] "output2" for a pretty print of the results of each thinning cycle

#include <fstream>
using namespace std;

class ImageProcessing{
private:
    int** firstAry;
    int** secondAry;
    bool changeFlag;
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

public:
    ImageProcessing();
    ImageProcessing(ifstream& inFile);
    ~ImageProcessing();
    void loadImage(ifstream& inFile);
    void zeroFramed();
    static string mapInt2Char(int theInt);
    void prettyPrint(ofstream& outFile);
    void printImage(ofstream& outFile);
    bool getChangeFlag();
    void setChangeFlag(bool value);
    void copyAry();
    void northThinning();
    void southThinning();
    void westThinning();
    void eastThinning();
    void oneCycleThinning();
};

int main(int argc, char* argv[]){
    ifstream inFile1;
    inFile1.open(argv[1]);
    ImageProcessing* image = new ImageProcessing(inFile1);
    inFile1.close();
    ofstream outFile2;
    outFile2.open(argv[3]);
    image->northThinning();
    outFile2 << "Pretty print of first cycle - northThinning" << endl;
    image->prettyPrint(outFile2);
    image->southThinning();
    outFile2 << "Pretty print of first cycle - southThinning" << endl;
    image->prettyPrint(outFile2);
    image->westThinning();
    outFile2 << "Pretty print of first cycle - westThinning" << endl;
    image->prettyPrint(outFile2);
    image->eastThinning();
    outFile2 << "Pretty print of first cycle - eastThinning" << endl;
    image->prettyPrint(outFile2);
    int cycleNumber = 2;
    while(image->getChangeFlag()){
        image->setChangeFlag(false);
        image->copyAry();
        image->oneCycleThinning();
        if(cycleNumber % 3 == 0){
            outFile2 << "Pretty print of complete cycle - cycle #" << cycleNumber << endl;
            image->prettyPrint(outFile2);
        }
        cycleNumber++;
    }
    outFile2 << "Pretty print of skeleton image after final cycle" << endl;
    image->prettyPrint(outFile2);
    outFile2.close();
    ofstream outFile1;
    outFile1.open(argv[2]);
    image->printImage(outFile1);
    outFile1.close();
}

ImageProcessing::ImageProcessing(): firstAry(NULL), secondAry(NULL), changeFlag(false), numRows(0), numCols(0), minVal(0), maxVal(0){
}

ImageProcessing::ImageProcessing(ifstream& inFile): changeFlag(false){
    loadImage(inFile);
}

ImageProcessing::~ImageProcessing(){
    if(firstAry != NULL){
        for(int i = 0; i < numRows; i++){
            delete [] firstAry[i];
        }
    }
    if(secondAry != NULL){
        for(int i = 0; i < numRows; i++){
            delete [] secondAry[i];
        }
    }
    delete [] firstAry;
    delete [] secondAry;
}

void ImageProcessing::loadImage(ifstream& inFile){
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    zeroFramed();
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            int value;
            inFile >> value;
            firstAry[row][col] = value;
            secondAry[row][col] = value;
        }
    }
}

void ImageProcessing::zeroFramed(){
    firstAry = new int* [numRows + 2];
    secondAry = new int* [numRows + 2];
    for(int i = 0; i < numRows + 2; i++){
        firstAry[i] = new int [numCols + 2];
        secondAry[i] = new int [numCols + 2];
    }
    for(int row = 0; row < numRows + 2; row++){
        for(int col = 0; col < numCols + 2; col++){
            firstAry[row][col] = 0;
            secondAry[row][col] = 0;
        }
    }
}

string ImageProcessing::mapInt2Char(int theInt){
    char toReturn [33];
    sprintf(toReturn, "%d", theInt);
    //itoa(theInt, toReturn, 10);
    return toReturn;
}

void ImageProcessing::prettyPrint(ofstream& outFile){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(secondAry[row][col] <= 0){
                outFile << " " << " ";
            }
            else{
                outFile << mapInt2Char(secondAry[row][col]) << " ";
            }
        }
        outFile << endl;
    }
}

void ImageProcessing::printImage(ofstream& outFile){
    int minVal = 0;
    int maxVal = 0;
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(secondAry[row][col] < minVal){
                minVal = secondAry[row][col];
            }
            if(secondAry[row][col] > maxVal){
                maxVal = secondAry[row][col];
            }
        }
    }
    outFile << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            outFile << mapInt2Char(secondAry[row][col]) << " ";
        }
        outFile << endl;
    }
}

bool ImageProcessing::getChangeFlag(){
    return changeFlag;
}

void ImageProcessing::setChangeFlag(bool value){
    changeFlag = value;
}

void ImageProcessing::copyAry(){
    for(int row = 0; row < numRows + 2; row++){
        for(int col = 0; col < numCols + 2; col++){
            firstAry[row][col] = secondAry[row][col];
        }
    }
}

void ImageProcessing::northThinning(){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(firstAry[row][col] > 0 && firstAry[row - 1][col] == 0){
                if(8 - (firstAry[row - 1][col - 1]
                      + firstAry[row - 1][col]
                      + firstAry[row - 1][col + 1]
                      + firstAry[row][col - 1]
                      + firstAry[row][col + 1]
                      + firstAry[row + 1][col - 1]
                      + firstAry[row + 1][col]
                      + firstAry[row + 1][col + 1])
                   <= 3){
                     secondAry[row][col] = 0;
                     if(!changeFlag){
                        changeFlag = true;
                     }
                }
            }
        }
    }
}

void ImageProcessing::southThinning(){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(firstAry[row][col] > 0 && firstAry[row + 1][col] == 0){
                if(8 - (firstAry[row - 1][col - 1]
                      + firstAry[row - 1][col]
                      + firstAry[row - 1][col + 1]
                      + firstAry[row][col - 1]
                      + firstAry[row][col + 1]
                      + firstAry[row + 1][col - 1]
                      + firstAry[row + 1][col]
                      + firstAry[row + 1][col + 1])
                   <= 3){
                     secondAry[row][col] = 0;
                     if(!changeFlag){
                        changeFlag = true;
                     }
                }
            }
        }
    }
}

void ImageProcessing::westThinning(){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(firstAry[row][col] > 0 && firstAry[row][col - 1] == 0){
                if(8 - (firstAry[row - 1][col - 1]
                      + firstAry[row - 1][col]
                      + firstAry[row - 1][col + 1]
                      + firstAry[row][col - 1]
                      + firstAry[row][col + 1]
                      + firstAry[row + 1][col - 1]
                      + firstAry[row + 1][col]
                      + firstAry[row + 1][col + 1])
                   <= 3){
                     secondAry[row][col] = 0;
                     if(!changeFlag){
                        changeFlag = true;
                     }
                }
            }
        }
    }
}

void ImageProcessing::eastThinning(){
    for(int row = 1; row < numRows + 1; row++){
        for(int col = 1; col < numCols + 1; col++){
            if(firstAry[row][col] > 0 && firstAry[row][col + 1] == 0){
                if(8 - (firstAry[row - 1][col - 1]
                      + firstAry[row - 1][col]
                      + firstAry[row - 1][col + 1]
                      + firstAry[row][col - 1]
                      + firstAry[row][col + 1]
                      + firstAry[row + 1][col - 1]
                      + firstAry[row + 1][col]
                      + firstAry[row + 1][col + 1])
                   <= 3){
                     secondAry[row][col] = 0;
                     if(!changeFlag){
                        changeFlag = true;
                     }
                }
            }
        }
    }
}

void ImageProcessing::oneCycleThinning(){
    northThinning();
    southThinning();
    westThinning();
    eastThinning();
}
