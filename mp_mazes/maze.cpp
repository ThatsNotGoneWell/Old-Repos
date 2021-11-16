/* Your code here! */
#include "maze.h"
#include <vector>
#include <iostream>
#include <queue>
#include <map>

using namespace std;

SquareMaze::SquareMaze() {
	
}

void SquareMaze::makeMaze(int width, int height) {
    width_ = width;
    height_ = height;
    area = width * height;
    walls.resize(area, 3); 
    disjoint_set.addelements(area);
    int dir=0;
    int second=0;
    int rInt = random_int();
    
    while (disjoint_set.size(0) != area) {
        
        makeMazeH(rInt,dir,second,width);
        setWall(rInt % width, rInt / width, dir, false);
        disjoint_set.setunion(rInt, second);
    }
}

bool SquareMaze::makeMazeH(int rInt,int dir,int second,int width){
    
    bool descende = random_bool();
        
        if(descende) {
            if(rInt + width >= area) return true;
            if (disjoint_set.find(rInt) == disjoint_set.find(rInt + width)) return true;
            dir = 1;
            second = rInt + width;
           
        } else {
            if((rInt + 1) % width == 0) return true;
            if (disjoint_set.find(rInt) == disjoint_set.find(rInt + 1)) return true;
            dir = 0;
            second = rInt + 1;
          
        }
        return false;
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
    if(height_ != 0 && width_ != 0) {
        bool condition_1, condition_2, condition_3;
        if(dir == 0) {
            if(x < width_ - 1){
                if(walls.at(y * width_ + x) != 1){
                    if(walls.at(y * width_ + x) != 3){
                        return true;
                    }
                }
            }
        } else if(dir == 1) {
            if(y < height_ - 1){
                if(walls.at(y * width_ + x) != 2){
                    if(walls.at(y * width_ + x) != 3){
                        return true;
                    }
                }
            }
        } else if(dir == 2) {
            if(x != 0){
                if(walls.at(y * width_ + x - 1) != 1){
                    if(walls.at(y * width_ + x - 1) != 3){
                        return true;
                    }
                }
            }
        } else {
            if(y != 0){
                if(walls.at((y - 1) * width_ + x) != 2){
                    if(walls.at((y - 1) * width_ + x) != 3){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
    int &wall = walls.at(y * width_ + x);
    
    if((dir==0 && !exists && wall ==1)||(dir==1 && !exists && wall ==2)){
        wall=0;
    }
    if((dir==0 && exists && wall ==0)||(dir==1 && !exists && wall ==3)){
        wall=1;
    }
    if((dir==0 && !exists && wall ==3)||(dir==1 && exists && wall ==0)){
        wall=2;
    }
    if((dir==0 && exists && wall ==2)||(dir==1 && exists && wall ==1)){
        wall=3;
    }
}



std::vector<int> SquareMaze::solveMaze() {
    
    std::vector<int> solved = solveMazeH();
    std::vector<int> output;
    int cur_longest_index = INT_MIN;
    int cur_longest_dist = INT_MIN;
    int m=(height_ - 1) * width_;

    while(m<area){
        if(solved.at(m) > cur_longest_dist) {
            cur_longest_index = m;
            cur_longest_dist = solved.at(m);
        }
        m++;
    }
    
    return findLongest(output,solved,cur_longest_dist);
}

std::vector<int> SquareMaze::solveMazeH() {
    
    std::vector<int> path(3 * area, -1); 
    
    q.push(0); 
    while(!q.empty()) {
        auto v = q.front();
        int x = v % width_;
        int y = v / width_;
        int temp=0;

        for(int i=0;i<3;i++){
            if(canTravel(x, y, i)) {
                if(i==0) temp=1;
                else if(i==1) temp=width_;
                else if(i==2) temp=-1;
                else if(i==3) temp= -1*width_;
                if(path[v + temp] == -1) {
                    path[v + temp] = path[v] + 1;
                    path[v + temp + area] = i;
                    path[v + temp + 2 * area] = v;
                    q.push(v + temp);
                }
            }
        }
        q.pop();
    }
    return path;
}

std::vector<int> SquareMaze::findLongest(std::vector<int> output,std::vector<int> solved,int cur_longest_index){
    while(cur_longest_index) {
        output.push_back(solved.at(cur_longest_index + area));
        cur_longest_index = solved.at(cur_longest_index + 2 * area);
    }
    std::reverse(output.begin(), output.end());
    return output;
}


PNG * SquareMaze::drawMaze()  {
    PNG * result = new PNG(width_ * 10 + 1, height_ * 10 + 1);
    HSLAPixel & top_pixel = result->getPixel(0, 0);
    top_pixel.l = 0;

    
    return drawMazeH(result);
}

PNG * SquareMaze::drawMazeH(PNG * result_)  {
    for(int i = 10; i < width_ * 10 + 1; i++) {
        HSLAPixel & pixel = result_->getPixel(i, 0);
        pixel.l = 0;
    }
    for(int j = 0; j < height_ * 10 + 1; j++) {
        HSLAPixel & pixel = result_->getPixel(0, j);
        pixel.l = 0;
    }
    for(int x = 0; x < width_; x++) {
        for(int y = 0; y < height_; y++) {
            if(!canTravel(x, y, 0)) {
                for(unsigned i = 0; i <= 10; i++) {
                    HSLAPixel & pixel = result_->getPixel((x + 1) * 10, y * 10 + i);
                    pixel.l = 0;
                }
            }
            if(!canTravel(x, y, 1)) {
                for(unsigned j = 0; j <= 10; j++) {
                    HSLAPixel & pixel = result_->getPixel(x * 10 + j, (y + 1) * 10);
                    pixel.l = 0;
                }
            }
        }
    }
    return result_;
}

PNG * SquareMaze::drawMazeWithSolution() {
    PNG * maze_ = drawMaze();
    std::vector<int> path = solveMaze();
    std::pair<int, int> point(5, 5);
    HSLAPixel & pixel = maze_->getPixel(5, 5);
    
    setPixel(pixel);
    for(int index : path) {
        for(int i = 0; i < 10; i++) {
            if(index == 0) point.first ++;
            else if(index == 1) point.second++;
            else if(index == 2) point.first--;
            else if(index == 3) point.second--;
            HSLAPixel & pixel = maze_->getPixel(point.first, point.second);
            
            setPixel(pixel);
        }
        
    }

    point.first = point.first-4;
    for(int i = point.first; i < point.first + 9; i++) {
        HSLAPixel & pixel = maze_->getPixel(i, height_ * 10);
        pixel.l = 1;
    }
    return maze_;
}

void SquareMaze::setPixel(HSLAPixel & pixel_){
    pixel_.h = 0;
    pixel_.s = 1.0;
    pixel_.l = 0.5;
    pixel_.a = 1;
}

bool SquareMaze::random_bool() {
    
    return rand()%2;
}

int SquareMaze::random_int() {
    return rand() % area;
}
